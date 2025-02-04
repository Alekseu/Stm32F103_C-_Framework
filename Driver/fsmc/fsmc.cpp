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


void Fsmc::Init()
{
	_useFsmc = true;
	offset = 0x00;
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	    GPIO_InitTypeDef GPIO_InitStructure;
	    //								NL
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
	   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	   	GPIO_Init(GPIOB, &GPIO_InitStructure);

	   	//                              AD2         AD3           NOE          NWE
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_4 | GPIO_Pin_5  |
	    //								NE1(#CE)    AD13          AD14         AD15
	    							  GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9 | GPIO_Pin_10 |
		//							      A16       A17              A18           AD0      AD1
									  GPIO_Pin_11| GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOD, &GPIO_InitStructure);

	    //                                NBL        NBH            AD4          AD5
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_7 | GPIO_Pin_8 |
	    //                             AD6          AD7           AD8         AD9
	    							  GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 |
		//                             AD10        AD11           AD12
									  GPIO_Pin_13|GPIO_Pin_14  |GPIO_Pin_15;
	  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  	GPIO_Init(GPIOE, &GPIO_InitStructure);



	  	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
		FSMC_NORSRAMTimingInitTypeDef fsmcTiming;
		FSMC_NORSRAMTimingInitTypeDef wfsmcTiming;
	  	    //-- FSMC Configuration ---------------------------------------------------
		FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &fsmcTiming;
		FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &wfsmcTiming;

		 FSMC_NORSRAMStructInit(&FSMC_NORSRAMInitStructure);

	    	fsmcTiming.FSMC_AddressSetupTime = 15;
	    	fsmcTiming.FSMC_AddressHoldTime = 15;
	    	fsmcTiming.FSMC_DataSetupTime = 255;
	    	fsmcTiming.FSMC_BusTurnAroundDuration = 15;
	    	fsmcTiming.FSMC_CLKDivision = 15;
	    	fsmcTiming.FSMC_DataLatency = 0;
	    	fsmcTiming.FSMC_AccessMode = FSMC_AccessMode_B;

	    	wfsmcTiming.FSMC_AddressSetupTime = 15;
	    	wfsmcTiming.FSMC_AddressHoldTime = 15;
	    	wfsmcTiming.FSMC_DataSetupTime = 255;
	    	wfsmcTiming.FSMC_BusTurnAroundDuration = 15;
	    	wfsmcTiming.FSMC_CLKDivision = 15;
	    	wfsmcTiming.FSMC_DataLatency = 0;
	    	wfsmcTiming.FSMC_AccessMode = FSMC_AccessMode_B;

	    	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	    	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
	    	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	    	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	    	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	    	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	    	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	    	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	    	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	    	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	    	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	    	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	    	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	    	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &fsmcTiming;
	    	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &wfsmcTiming;

	  	    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	  	    /* Enable FSMC Bank1_SRAM Bank */
	  	    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

}

uint16_t Fsmc::Read(uint32_t addr){
		return *(__IO uint16_t*) (SRAM_ADR+addr+offset);

}

void Fsmc::Write(uint32_t addr, uint16_t data){
		*(uint16_t *) (SRAM_ADR + addr+offset) = data;
}

uint8_t Fsmc::Read8(uint32_t addr){
		return *(__IO uint8_t*) (SRAM_ADR+addr+offset);

}

void Fsmc::Write8(uint32_t addr, uint8_t data){
		*(uint8_t *) (SRAM_ADR + addr+offset) = data;
}

bool Fsmc::RamTest(){
	uint32_t adr;
	uint16_t sollwert, istwert;
	  bool status = true;

	  //write to mwmory
	  sollwert=0xA2B2;
	  for(adr=0;adr<SRAM_MAX_ADR;adr++) {
		  Write(adr,sollwert);
		  sollwert++;
	  }
	  sollwert=0xA2B2;
	  for(adr=0;adr<SRAM_MAX_ADR;adr++) {
	 		  istwert=Read(adr);
	 		  if(istwert!=sollwert) {
	 			  status= false;
	 		  }else {
	 			  status = true;
	 		  }
	 		  sollwert++;
	 	  }
		return status;
}

bool Fsmc::RamTest8(){
	uint32_t adr;
	uint8_t sollwert, istwert;
	  bool status = true;

	  //write to mwmory
	  sollwert=0xA2;
	  for(adr=0;adr<SRAM_MAX_ADR;adr++) {
		  Write8(adr,sollwert);
		  sollwert++;
	  }
	  sollwert=0xA2;
	  for(adr=0;adr<SRAM_MAX_ADR;adr++) {
	 		  istwert=Read8(adr);
	 		  if(istwert!=sollwert) {
	 			  status= false;
	 		  }else {
	 			  status = true;
	 		  }
	 		  sollwert++;
	 	  }
		return status;
}


}
