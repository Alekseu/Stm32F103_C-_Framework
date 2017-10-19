/*
 * fsmc.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "fsmc.h"

namespace Driver
{
	Fsmc* Fsmc::FsmcObj=0;
}


//void init_sram()
//{
//	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
//	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
//
//	    GPIO_InitTypeDef GPIO_InitStructure;
//
//	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
//	   	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	   	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	   	    GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
//	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	    GPIO_Init(GPIOD, &GPIO_InitStructure);
//
//	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
//	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	  	GPIO_Init(GPIOE, &GPIO_InitStructure);
//
//	  	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
//		FSMC_NORSRAMTimingInitTypeDef fsmcTiming;
//		FSMC_NORSRAMTimingInitTypeDef wfsmcTiming;
//	  	    //-- FSMC Configuration ---------------------------------------------------
//		FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &fsmcTiming;
//		    	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &fsmcTiming;
//
//		 FSMC_NORSRAMStructInit(&FSMC_NORSRAMInitStructure);
//	    	fsmcTiming.FSMC_AddressSetupTime = 0;
//	    	fsmcTiming.FSMC_AddressHoldTime = 1;
//	    	fsmcTiming.FSMC_DataSetupTime = 1;
//	    	fsmcTiming.FSMC_BusTurnAroundDuration = 0;
//	    	fsmcTiming.FSMC_CLKDivision = 4;
//	    	fsmcTiming.FSMC_DataLatency = 0;
//	    	fsmcTiming.FSMC_AccessMode = FSMC_AccessMode_A;
//
//
//
//
//	    	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
//	    	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
//	    	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
//	    	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
//	    	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
//	    	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
//	    	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
//	    	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
//	    	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
//	    	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
//	    	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
//	    	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
//	    	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
////	    	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &fsmcTiming;
////	    	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &wfsmcTiming;
//
//	  	    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
//
//	  	    /* Enable FSMC Bank1_SRAM Bank */
//	  	    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
//
//}
