/*
 * MemoryManager.cpp
 *
 *  Created on: 27 марта 2017 г.
 *      Author: hudienko_a
 */

#include "MemoryManager.h"
#include<string.h>


MemoryManager* MemoryManager::MemoryManObj=0;

MemoryManager::MemoryManager(unsigned long StartAddr, unsigned int length)
{
	MemoryManObj = this;
	_baseAddr = StartAddr;
	_sramLength = length;
}


ErrorStatus MemoryManager::Init()
{
	ErrorStatus ret_wert=ERROR;

	RCC->APB2ENR |= RCC_APB2Periph_GPIOB;
	RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

	RCC->APB2ENR |= RCC_APB2Periph_GPIOD;
	RCC->APB2ENR |= RCC_APB2Periph_GPIOE;

	RCC->AHBENR |= RCC_AHBPeriph_FSMC;

	GPIO_InitTypeDef GPIO_InitStructure;


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//8bit mode
//	GPIO_SetBits(GPIOE,GPIO_Pin_1);//NBH =1
//	GPIO_ResetBits(GPIOE,GPIO_Pin_0);//NBL=0


	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//init fsmc

	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMDeInit(FSMC_Bank1_NORSRAM1);

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 15;
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 15;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 255;
	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 15;
	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 16;
	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_PSRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;

	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/* - BANK 1 (of NOR/SRAM Bank 0~3) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);


	uint8_t* tmp = (uint8_t*)_baseAddr;

	uint32_t length = _sramLength;

	uint16_t oldwert,istwert;

	for(short i=0;i<(length);i++)
	{

		SRAM_Write(i,i);
		istwert=SRAM_Read(i);
//		tmp[i] = i;
//		istwert = tmp[i];

		if(istwert==i)
		{
			ret_wert=SUCCESS;
		}
		else
		{
			int a=0;
			ret_wert = ERROR;
			//break;
		}
	}

	if(ret_wert==SUCCESS)
	{
		_blocks = (DataBlock*)_baseAddr;
		_lastAddr = (_baseAddr+sizeof(DataBlock));
		((DataBlock*)_lastAddr)->size =0;
		((DataBlock*)_lastAddr)->Next = 0;
		((DataBlock*)_lastAddr)->alloc = false;

//		uint16_t next = (uint16_t)((uint32_t*)&_blocks->Next);
//		uint32_t addr = _baseAddr+next;

		((DataBlock*)_baseAddr)->Next = _lastAddr;
		((DataBlock*)_baseAddr)->size =0;
		((DataBlock*)_baseAddr)->alloc = false;

	}
	return ret_wert;
}

void* MemoryManager::Malloc(unsigned int size)
{
	DataBlock* _curBlock = _blocks;
	while(_curBlock->alloc)
	{
		uint32_t cur_block = (uint32_t)_curBlock;
		_curBlock = (DataBlock*)_curBlock->Next;
		_lastAddr = cur_block+sizeof(DataBlock)+_curBlock->size;
	}
	_curBlock-> size = size;
	_curBlock->alloc = true;
	uint32_t addr = _baseAddr+((uint16_t)(_curBlock->Next))<<1;
	*(uint32_t*)(addr) = _lastAddr;
	//*(uint32_t*)(_baseAddr+((uint16_t)(*_curBlock->Next))<<1) = _lastAddr;//(DataBlock*)(_lastAddr+sizeof(DataBlock)+size);
	return _curBlock->Data;
}


void MemoryManager::Free(void* obj)
{

}


void MemoryManager::ShowMemory()
{

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
