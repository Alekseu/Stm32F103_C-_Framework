/*
 * ShareStorage.cpp
 *
 *  Created on: 30 марта 2017 г.
 *      Author: hudienko_a
 */

#include "shareStorage.h"

ShareStorage* ShareStorage::SSObj =0;

ShareStorage::ShareStorage()
{
	SSObj = this;
	for(int i=0;i<STORAGE_COUNT;i++)
	{
		_ptrs[i].obj =0;
		_ptrs[i].counter=0;
	}
}


int ShareStorage::Add(void* obj)
{
	int index = -1;

	if(obj==0) return index;

	for(int i=0;i<200;i++)
	{
		if(obj==_ptrs[i].obj)
		{
			index=i;
			_ptrs[index].counter++;
			break;
		}

		if(_ptrs[i].obj==0 && _ptrs[i].counter==0)
		{
			index=i;
			_ptrs[index].obj = obj;
			_ptrs[index].counter =1;
			break;
		}
	}

	return index;
}

void ShareStorage::Modify(void* obj,int index)
{
	_ptrs[index].obj = obj;
}

void ShareStorage::Remove(void* obj, int index)
{
	if(obj==_ptrs[index].obj && _ptrs)
	{
		_ptrs[index].counter--;
	}
}

bool ShareStorage::Check(void* obj, int index)
{
	if(obj==_ptrs[index].obj && _ptrs[index].counter<=0)
	{
		return true;
	}
	return false;
}

int ShareStorage::getIndex(void* obj)
{
	int index = -1;

	if(obj==0) return index;

	for(int i=0;i<200;i++)
	{
		if(obj==_ptrs[i].obj)
		{
			index=i;
			break;
		}
	}
	return index;
}

void* ShareStorage::getObj(int index)
{
	return _ptrs[index].obj;
}
