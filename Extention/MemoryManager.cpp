/*
 * MemoryManager.cpp
 *
 *  Created on: 27 марта 2017 г.
 *      Author: hudienko_a
 */

#include "MemoryManager.h"
#include<string.h>


MemoryManager::MemoryManager(unsigned int StartAddr, unsigned int length)
{
	_baseAddr = StartAddr;
	_sramLength = length;
	MemHeapHasBeenInitialised = false;

		_memInfoTbl[0].block_size = 32;
		_memInfoTbl[0].num_max = LENGTH_36_NUM;
		_memInfoTbl[0].wm_head =0;
		_memInfoTbl[0].calc[0] =0;
		_memInfoTbl[0].calc[1] =0;

		_memInfoTbl[1].block_size = 52;
		_memInfoTbl[1].num_max = LENGTH_52_NUM;
		_memInfoTbl[1].wm_head =0;
		_memInfoTbl[1].calc[0] =0;
		_memInfoTbl[1].calc[1] =0;

		_memInfoTbl[2].block_size = 64;
		_memInfoTbl[2].num_max = LENGTH_64_NUM;
		_memInfoTbl[2].wm_head =0;
		_memInfoTbl[2].calc[0] =0;
		_memInfoTbl[2].calc[1] =0;

		_memInfoTbl[3].block_size = 128;
		_memInfoTbl[3].num_max = LENGTH_128_NUM;
		_memInfoTbl[3].wm_head =0;
		_memInfoTbl[3].calc[0] =0;
		_memInfoTbl[3].calc[1] =0;

		_memInfoTbl[4].block_size = 256;
		_memInfoTbl[4].num_max = LENGTH_256_NUM;
		_memInfoTbl[4].wm_head =0;
		_memInfoTbl[4].calc[0] =0;
		_memInfoTbl[4].calc[1] =0;

		_memInfoTbl[5].block_size = 512;
		_memInfoTbl[5].num_max = LENGTH_512_NUM;
		_memInfoTbl[5].wm_head =0;
		_memInfoTbl[5].calc[0] =0;
		_memInfoTbl[5].calc[1] =0;

		_memInfoTbl[6].block_size = 640;
		_memInfoTbl[6].num_max = LENGTH_640_NUM;
		_memInfoTbl[6].wm_head =0;
		_memInfoTbl[6].calc[0] =0;
		_memInfoTbl[6].calc[1] =0;

		_memInfoTbl[7].block_size = 1536;
		_memInfoTbl[7].num_max = LENGTH_1536_NUM;
		_memInfoTbl[7].wm_head =0;
		_memInfoTbl[7].calc[0] =0;
		_memInfoTbl[7].calc[1] =0;

//	= {
//
//	 {36,  LENGTH_36_NUM  , 0, {0,0} },
//	 {52,  LENGTH_52_NUM  , 0, {0,0} },
//	 {64,  LENGTH_64_NUM  , 0, {0,0} },
//	 {128, LENGTH_128_NUM , 0, {0,0} },
//	 {132, LENGTH_132_NUM , 0, {0,0} },
//	 {256, LENGTH_256_NUM , 0, {0,0} },
//	 {512, LENGTH_512_NUM , 0, {0,0} },
//	 {640, LENGTH_640_NUM , 0, {0,0} },
//	 {1536,LENGTH_1536_NUM, 0, {0,0} },
//	};
}


ErrorStatus MemoryManager::Init()
{
	  ErrorStatus ret_wert=ERROR;
	  uint16_t oldwert,istwert;
	//init gpio fsmc

	 RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
			 RCC->APB2ENR |= RCC_APB2Periph_GPIOB;
			 RCC->APB2ENR |= RCC_APB2Periph_GPIOC;
			 RCC->APB2ENR |= RCC_APB2Periph_GPIOD;
			 RCC->APB2ENR |= RCC_APB2Periph_GPIOE;
			 RCC->APB2ENR |= RCC_APB2Periph_GPIOG;
		 GPIO_InitTypeDef GPIO_InitStructure;


		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
		                               GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
		                               GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		 GPIO_Init(GPIOD, &GPIO_InitStructure);

		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 |
		          GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
		          GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
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
				 FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 15;
				 FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 15;
				 FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 255;
				 FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 15;
				 FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
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

			 oldwert=SRAM_Read(0x00);
			  SRAM_Write(0x00,0x5A3C);
			  istwert=SRAM_Read(0x00);
			  SRAM_Write(0x00,oldwert);
			  if(istwert==0x5A3C) ret_wert=SUCCESS; // RAM vorhanden

			if(ret_wert==SUCCESS)
			{
				int i,j;
				    mem_block_s *head=0;
				    unsigned int addr;

				    addr = _baseAddr;

				    for(i=0; i<MEM_TBL_MAX; i++)
				    {
				        head = (mem_block_s *)addr;
				        _memInfoTbl[i].wm_head = head;
				        for(j=0;j<_memInfoTbl[i].num_max; j++)
				        {
				            head->used =MEM_NO_USED;
				            head->block_size = _memInfoTbl[i].block_size;
				            head->ptr = (char *)(addr + sizeof(mem_block_s));
				            addr += (_memInfoTbl[i].block_size + sizeof(mem_block_s));
				            head->next =(char *)addr;
				            head = (mem_block_s *)head->next;
				            if(head >  (mem_block_s *)((_baseAddr+_sramLength)-0x10))
				            {
				            	ret_wert=ERROR;
				                return ret_wert;
				            }
				        }
				    }
				    head->ptr = 0;
				    head->block_size = 0;
				    head->next = (char*)_baseAddr;

				    MemHeapHasBeenInitialised=true;
			}
			return ret_wert;
}

void* MemoryManager::Malloc(unsigned int size)
{
	 void *pwtReturn = 0;
	    int i;
	    mem_block_s *head;

	    if(!MemHeapHasBeenInitialised )
	            goto done_exit;

	    for(i=0; i<MEM_TBL_MAX; i++)
	    {
	        if(size <= _memInfoTbl[i].block_size)
	        {
	            head = _memInfoTbl[i].wm_head;
	            uint32_t ptr = (uint32_t)(&head->ptr);
	            head = _memInfoTbl[i].wm_head;
					while(ptr)
					{
						if(head->used == MEM_NO_USED)
						{
							head->used = MEM_USED;
							pwtReturn = head->ptr;
							goto done;
						}
						head = (  mem_block_s *)head->next;
					}
	            goto done;

	        }
	    }
	 done:


	    if(pwtReturn)
	    {
	        for(i=0; i<MEM_TBL_MAX; i++)
	        {
	            if(_memInfoTbl[i].block_size == head->block_size)
	            {

	                _memInfoTbl[i].calc[CALC_CNT]++;
	                if(_memInfoTbl[i].calc[CALC_CNT] > _memInfoTbl[i].calc[CALC_MAX] )
	                    _memInfoTbl[i].calc[CALC_MAX]=_memInfoTbl[i].calc[CALC_CNT];
	                break;
	            }
	        }
	    }
	  done_exit:
	    return pwtReturn;
}


void MemoryManager::Free(void* obj)
{
	 mem_block_s *head;
	    char fault=0;


	    if( ( obj == 0 ) || (!MemHeapHasBeenInitialised) )
	        goto done;

	    if( (obj < (void*)_baseAddr) && (obj > (void*)(_baseAddr+_sramLength)) )
	    {
	        goto done;
	    }

	    head = (mem_block_s*)obj-sizeof(mem_block_s);


	    if(head->used)
	        head->used = MEM_NO_USED;
	    else
	    {
	        fault=1;
	    }


	    if(fault)
	        goto done;
	    int i;
	    for(i=0;i<MEM_TBL_MAX;i++)
	    {
	        if(_memInfoTbl[i].block_size == head->block_size)
	        {
	            _memInfoTbl[i].calc[CALC_CNT]--;
	            goto done;
	        }
	    }
	 done:;
}


void MemoryManager::ShowMemory()
{

}


void MemoryManager::SRAM_Write(uint32_t adr, uint16_t wert)
	{
	  // adresse muss um 1bit nach links verschoben werden
	  adr=adr<<1;

	  *(uint16_t *) (_baseAddr + adr) = wert;
	}

uint16_t MemoryManager::SRAM_Read(uint32_t adr)
	{
	  uint16_t ret_wert=0;

	  // adresse muss um 1bit nach links verschoben werden
	  adr=adr<<1;

	  ret_wert=*(__IO uint16_t*) (_baseAddr + adr);

	  return(ret_wert);
	}
