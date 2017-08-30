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

	// Hard Fault Status Register
	#define NVIC_HFSR (*(volatile unsigned int*) (0xE000ED2Cu))

	extern "C"
	{
		void hard_fault_handler_c( uint32_t *pStack )
		{

			//if (NVIC_HFSR & (1uL << 31)) {
				//					NVIC_HFSR |= (1uL << 31); // Reset Hard Fault status
										 *(pStack + 6u) += 2u; // PC is located on stack at SP + 24 bytes;
										 // increment PC by 2 to skip break instruction.
										// return; // Return to interrupted application
									//	}
		}
	}
	static void DefaultHandler(void)
	{
		 __asm volatile
		    (
		    		" movs r0,#4\n"
		    		 " movs r1, lr\n"
		    		 " tst r0, r1\n"
		    		 " beq _MSP\n"
		    		 " mrs r0, psp\n"
		    		 " b _HALT\n"
		    		"_MSP:\n"
		    		"  mrs r0, msp\n"
		    		"_HALT:\n"
		    		"  ldr r1,[r0,#20]\n"
		    		"  b hard_fault_handler_c\n"
		    );

//		while(true)
//		{
//
//		}
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
