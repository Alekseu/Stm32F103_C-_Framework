/*
 * MemoryManager.h
 *
 *  Created on: 27 ����� 2017 �.
 *      Author: hudienko_a
 */

#ifndef EXTENTION_MEMORYMANAGER_H_
#define EXTENTION_MEMORYMANAGER_H_


/*
 * MemoryManagment example
 *
 *
 * MemoryManager mem((uint32_t)0x60000000,0x80000);
 *
 * 	ErrorStatus t = mem.Init();
 *
 * 	char* array = new (&mem) char[130];
 * 	if(array!=0)
 * 	{
 * 	  memcpy(array,"Hello world  this is playcement new!!!!",strlen("Hello world  this is playcement new!!!!"));
 * 	  mem.ShowMemory();
 * 	  char* array = (char*)mem.Malloc(150);
 * 	  memcpy(array,"Hello world !!!!",strlen("Hello world !!!!"));
 * 	  mem.ShowMemory();
 * 	  mem.Free(array);
 * 	  mem.ShowMemory();
 * 	}
 *
 */

extern "C"
{
	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_fsmc.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
}


 struct DataBlock
{
	DataBlock* Next;
	uint32_t size;
	bool alloc;
	bool deleted;
	unsigned char Data[];
};


class MemoryManager
{
public:
	static MemoryManager* MemoryManObj;
	unsigned int RemovedElementsAutoCollectCount;
	bool UsingShareStorage;

	//todo �������� ����������� � ���������� ����
	MemoryManager(void* StartAddr, unsigned int length);

	bool Init();

	void* Malloc(unsigned int size);

	void Free(void* obj);

	void Collect();

	void ShowMemory();

private:
	DataBlock* _blocks;

	uint32_t _baseAddr;
	DataBlock* _lastAddr;

	unsigned int _sramLength;
	unsigned int _totalElements;
	unsigned int _removedElements;
	unsigned int _remainder;
	unsigned int _allocatedReminder;

	void SRAM_Write(uint32_t adr, uint32_t wert);

	uint16_t SRAM_Read(uint32_t adr);

};



#endif /* EXTENTION_MEMORYMANAGER_H_ */
