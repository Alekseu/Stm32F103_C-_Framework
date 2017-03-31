/*
 * nvic.cpp
 *
 *  Created on: 23.12.2009
 *      Author: Flexz
 */

#include "nvic.h"

namespace Driver
{

	const int32_t isrCount = 0x100;
	//Таблица прерываний должна быть выравнена на 0x100 <BUG>байт</BUG> СЛОВ!!! а не байт
	//static InterruptHandler __attribute__((aligned(0x400))) isrTable[isrCount];
	#ifdef CFG_LINKER_HAS_ISR_SECTION
	static InterruptHandler __attribute__((section(".isr_section"))) isrTable[isrCount];
	#else
	static InterruptHandler __attribute__((aligned(0x200)))  isrTable[isrCount];
	#endif


	void DefaultHandler()
	{
		int a=0;

		while(true)
		{

		}
	}

	void InterruptController::RemapToRam()
	{
		isrTable[0]=((InterruptHandler*)0)[0];
		isrTable[1]=((InterruptHandler*)0)[1];
		for(int i = 2; i < isrCount; i++){
			isrTable[i] =  DefaultHandler;
		}
		NVIC_SetVectorTable(NVIC_VectTab_RAM,(uint32_t)&isrTable);
	}

	bool InterruptController::SetHandler(IRQn_Type channel, InterruptHandler handler)
	{
		//первое STMное прерывание имеет номер 0
		//стандартные кортесовские индексируются в минус (см определение IRQn_Type)
		const uint32_t shift = 16;
		if(channel < isrCount)
		{
			isrTable[channel+shift] = handler;
			return true;
		}
		else
		{
			return false;
		}
	}
}
