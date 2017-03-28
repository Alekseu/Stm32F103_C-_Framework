/*
 * MemoryPool.cpp
 *
 *  Created on: 28 марта 2017 г.
 *      Author: Alekseu
 */
#include "MemoryPool.h"

extern "C"
{
	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_fsmc.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
}

 MemPool::MemPool(void *buffer, SIZE_T bufferLength) : m_blocks((MemPoolBlock *)buffer)
 {

	 _baseAddr = (unsigned int)buffer;
	 //init fsmc
	 ErrorStatus ret_wert=ERROR;
	 uint16_t oldwert,istwert;

	 RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

	 RCC->APB2ENR |= RCC_APB2Periph_GPIOD;
	 RCC->APB2ENR |= RCC_APB2Periph_GPIOE;

	 GPIO_InitTypeDef GPIO_InitStructure;


	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_Init(GPIOD, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
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

	 RCC->AHBENR |= RCC_AHBPeriph_FSMC;
	 FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 25;
	 FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 25;
	 FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 512;
	 FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 25;
	 FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 2;
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

			 for(int i=0;i<bufferLength;i++)
			 {
				 // oldwert=SRAM_Read(0x00);
				 SRAM_Write(0x00,0x5A3C);
				 istwert=SRAM_Read(0x00);
				 //SRAM_Write(0x01,oldwert);
				 if(istwert==0x5A3C)
				 {
					 ret_wert=SUCCESS;

				 }
				 else
				 {
					 break;
				 }
			 }


  bufferLength -= bufferLength % sizeof(MemPoolBlock);

  m_blocks->next = 0;
  m_blocks->alloc = 0;
  m_blocks->size = bufferLength - sizeof(MemPoolBlock);
}

MemPoolBlock * MemPool::findFreeBlock(SIZE_T minsize) {
  for (MemPoolBlock *curblk = m_blocks; curblk; curblk = curblk->next)
    if ((curblk->alloc == 0) && curblk->size >= minsize)
      return curblk;
  return 0;
}

MemPoolBlock* MemPool::findBlockFromPtr(void const *ptr) {
  for (MemPoolBlock *curblk = m_blocks; curblk; curblk = curblk->next)
    if (ptr == curblk + 1)
      return curblk;
  return 0;
}

void * MemPool::malloc(SIZE_T size) {

  if (size % sizeof(MemPoolBlock) != 0)
    size += sizeof(MemPoolBlock) - size % sizeof(MemPoolBlock);

  MemPoolBlock *freeblk = findFreeBlock(size);
  if (freeblk) {

	  unsigned char *ptr = (unsigned char *)(freeblk + 1);
    if (freeblk->size > size + sizeof(MemPoolBlock))
    {
      MemPoolBlock *newblk = freeblk + 1 + size / sizeof(MemPoolBlock);
      newblk->next = freeblk->next;
      newblk->alloc = 0;
      newblk->size = freeblk->size - size - sizeof(MemPoolBlock);
      freeblk->next = newblk;
      freeblk->size = size;
    }
    freeblk->alloc = 1;

    return ptr;
  } else
    return 0;
}

void  MemPool::free(void const *ptr) {
  if (ptr) {
    MemPoolBlock *blk = findBlockFromPtr(ptr);
    if (blk) {
      blk->alloc = 0;
      mergeFreeBlocks();
    }
  }
}

void  MemPool::mergeFreeBlocks() {
  MemPoolBlock *curblk = m_blocks;
  while (curblk->next) {
    if (curblk->alloc == 0 && curblk->next->alloc == 0) {

      curblk->size += curblk->next->size + sizeof(MemPoolBlock);
      curblk->next = curblk->next->next;
    } else {
      curblk = curblk->next;
    }
  }
}

void  MemPool::getStats(SIZE_T *largestFreeBlock, SIZE_T *totalFreeSize) {
  *largestFreeBlock = 0;
  *totalFreeSize = 0;
  for (MemPoolBlock *blk = m_blocks; blk; blk = blk->next) {
    if (blk->alloc == 0) {
      *totalFreeSize += blk->size;
      if (blk->size > *largestFreeBlock)
        *largestFreeBlock = blk->size;
    }
  }
}

SIZE_T  MemPool::getLargestFreeBlock() {
  SIZE_T largestFreeBlock, totalFreeSize;
  getStats(&largestFreeBlock, &totalFreeSize);
  return largestFreeBlock;
}


void MemPool::SRAM_Write(unsigned int adr, unsigned int wert)
	{
	  // adresse muss um 1bit nach links verschoben werden
	  adr=adr<<1;

	  *(unsigned int *) (_baseAddr + adr) = wert;
	}

unsigned int MemPool::SRAM_Read(unsigned int adr)
	{
	  uint16_t ret_wert=0;

	  // adresse muss um 1bit nach links verschoben werden
	  adr=adr<<1;

	  ret_wert=*(__IO unsigned int*) (_baseAddr + adr);

	  return(ret_wert);
	}
