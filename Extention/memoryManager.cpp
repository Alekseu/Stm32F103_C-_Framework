/*
 * MemoryManager.cpp
 *
 *  Created on: 27 марта 2017 г.
 *      Author: hudienko_a
 */

#include "memoryManager.h"
#include<string.h>

#include "shareStorage/shareStorage.h"

MemoryManager* MemoryManager::MemoryManObj=0;

MemoryManager::MemoryManager(void* StartAddr, unsigned int length)
{
	MemoryManObj = this;
	_baseAddr = (uint32_t)StartAddr;
	_sramLength = length;
	_blocks =0;
	_lastAddr=0;
	_totalElements=0;
	_remainder = length;
	RemovedElementsAutoCollectCount=0;
	_removedElements=0;
	_allocatedReminder=0;
	UsingShareStorage = false;
}

bool MemoryManager::Init()
{
		_blocks = (DataBlock*)_baseAddr;
		_lastAddr = (DataBlock*)(_baseAddr+sizeof(DataBlock));
		_lastAddr->size =_sramLength-sizeof(DataBlock);
		_lastAddr->Next = 0;
		_lastAddr->alloc = false;
		_lastAddr->deleted = false;

		((DataBlock*)_baseAddr)->Next = _lastAddr;
		((DataBlock*)_baseAddr)->size =_sramLength-sizeof(DataBlock);
		((DataBlock*)_baseAddr)->alloc = false;
		((DataBlock*)_baseAddr)->deleted = false;
		_remainder = _sramLength-sizeof(DataBlock);
		return true;
}

void* MemoryManager::Malloc(unsigned int size)
{
	// если запрос на выделение памяти больше чем у нас есть впринцепи
	if(size > _remainder && size >_allocatedReminder )
	{
		return 0;
	}

	bool _firstMalloc = true;
	DataBlock* _curBlock = _blocks;

	while(_curBlock->alloc )
	{
		_firstMalloc = false;

		// пробуем разместить в существующем удаленном блоке
		if(_curBlock->alloc && _curBlock->deleted && _curBlock->size >=size )
		{
			DataBlock* _nextBlock = _curBlock->Next;
			DataBlock* _newFreeBlock= (DataBlock*)((uint32_t)_curBlock+sizeof(DataBlock)+size);
			int _curBlockOldSize = _curBlock->size;

			_newFreeBlock->Next = _nextBlock;
			_newFreeBlock->size = _curBlockOldSize - size;
			_newFreeBlock->alloc = true;
			_newFreeBlock->deleted = true;

			_curBlock->Next = _newFreeBlock;
			_curBlock->alloc = true;
			_curBlock->deleted = false;
			_curBlock->size = size;
			_allocatedReminder-=size;

			_totalElements++;

			return _curBlock->Data;
		}

		_curBlock = _curBlock->Next;
		_firstMalloc = true;
	}

	if((uint32_t)_curBlock>(_baseAddr+_sramLength)) return 0;

	if(_firstMalloc)
	{
		// размещаем новый блок
		_curBlock-> size = size;
		_curBlock->alloc = true;
		_curBlock->deleted = false;

		DataBlock* _nextBlock = (DataBlock*)(((uint32_t)_curBlock)+sizeof(DataBlock)+size);
		_nextBlock->Next =0;
		_nextBlock->alloc = false;
		_nextBlock->deleted = false;
		_nextBlock->size =_remainder - (size+ sizeof(DataBlock));

		_curBlock->Next = _nextBlock;

		_remainder = _remainder - (size+ sizeof(DataBlock));
		_totalElements++;

		return _curBlock->Data;
	}
}


void MemoryManager::Free(void* obj)
{
	DataBlock* _curBlock = _blocks;

	while(_curBlock->Next!=0)
	{
		void* cur_obj = _curBlock->Data;
			if(cur_obj==obj)
			{
				memset(_curBlock->Data,0, _curBlock->size);
				_curBlock->deleted = true;
				_allocatedReminder+=_curBlock->size;
				_totalElements--;
				_removedElements++;
				break;
			}
			_curBlock = _curBlock->Next;
	}
	//автоматический дефрагметатор (при использовании умных указателей также ужимает кучу)
	if(RemovedElementsAutoCollectCount>0 && _removedElements>=RemovedElementsAutoCollectCount)
	{
		Collect();
	}
}

void MemoryManager::Collect()
{
	//проcтой способ дефрагментации
	DataBlock* _curBlock = _blocks;
	while(_curBlock->Next!=0 && _curBlock->Next->Next!=0)
	{
		if((_curBlock->alloc && _curBlock->deleted)&& (_curBlock->Next->alloc && _curBlock->Next->deleted))
		{
			DataBlock* _nextBlock = _curBlock->Next;
			_curBlock->size = _curBlock->size+_nextBlock->size;
			_curBlock->Next = _nextBlock->Next;
		}
		else
		{
		   _curBlock = _curBlock->Next;
		}
	}
	_removedElements =0;

	if(UsingShareStorage)
	{
		//ужимаем кучу (только при условии использования умных указателей, и глобального хранилища указателей)
		_curBlock = _blocks;
		DataBlock* _emptyBlock =0;
		DataBlock* _notEmptyBlock =0;
		bool foundNotEmptyBlock = false;
		bool foundEmptyBlock = false;
		while(_curBlock->Next!=0 && _curBlock->Next->Next!=0)
		{
			if((_curBlock->alloc && _curBlock->deleted )&&(_curBlock->Next->alloc && !_curBlock->Next->deleted))
			{
				_emptyBlock = _curBlock;
				foundEmptyBlock = true;
				_notEmptyBlock = _curBlock->Next;
				foundNotEmptyBlock = true;
			}

			if(foundEmptyBlock && foundNotEmptyBlock)
			{
				if(_emptyBlock->size>=_notEmptyBlock->size)
				{
					DataBlock* _newEmptyBlock = (DataBlock*)(((uint32_t)_emptyBlock)+sizeof(DataBlock)+_notEmptyBlock->size);
					DataBlock* _nextBlock = _notEmptyBlock->Next;

					// получаем индекс обьекта в глобальном хранилище
					int _index = ShareStorage::SSObj->getIndex(_notEmptyBlock->Data);
					if(_index>=0)
					{
						int storedemptyBlockSize = _emptyBlock->size;

						_emptyBlock->size  = _notEmptyBlock->size;
						_emptyBlock->alloc = _notEmptyBlock->alloc;
						_emptyBlock->deleted = _notEmptyBlock->deleted;

						//копируем обьект в новое место
						for(int i=0;i<_notEmptyBlock->size;i++)
						{
							_emptyBlock->Data[i] = _notEmptyBlock->Data[i];
						}
						memset(_notEmptyBlock->Data,0,_notEmptyBlock->size);

						// записываем адрес положения обьекта в глобальное хранилище
						ShareStorage::SSObj->Modify(_emptyBlock->Data,_index);

						_emptyBlock->Next = _newEmptyBlock;

						_newEmptyBlock->size = storedemptyBlockSize;
						_newEmptyBlock->alloc = true;
						_newEmptyBlock->deleted = true;
						_newEmptyBlock->Next = _nextBlock;
					}
				}
				foundEmptyBlock =false;
				foundNotEmptyBlock = false;
				_curBlock = _curBlock->Next;
			}
			else
			{
				_curBlock = _curBlock->Next;
			}
		}
	}//end if(UsingShareStorage)
	_removedElements =0;


}

void MemoryManager::ShowMemory()
{
	DataBlock* _curBlock = _blocks;
	for(int i=0;i< _totalElements;i++)
	{

	}
}


void MemoryManager::SRAM_Write(uint32_t adr, uint32_t wert)
{
	*(volatile uint16_t*)(_baseAddr |(adr <<1) )=wert;
}

uint16_t MemoryManager::SRAM_Read(uint32_t adr)
	{
	  uint32_t ret_wert=0;

	  ret_wert=*(__IO uint16_t*) (_baseAddr|(adr<<1));

	  return(ret_wert);
	}
