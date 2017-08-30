/*
 * main.cpp
 *
 *  Created on: 30 рту. 2017 у.
 *      Author: hudienko_a
 */
#include "Driver/platform.h"
#include "Extention/operators.h"
#include "Driver/nvic/nvic.h"
#include "Driver/usart/usart.h"
#include "Driver/led/led.h"
#include "Extention/sPtr.h"

using namespace Driver;


typedef void reset__(void);
reset__* reset_ = 0;

void memory_handler()
{
	uint8_t reg =  *((uint8_t       *)     0xE000ED28      ) ;
}

void bus_handler()
{
	uint8_t reg =  *((uint8_t       *)     0xE000ED29      ) ;
}

void usage_handler()
{
	uint16_t reg =  *((uint16_t       *)     0xE000ED2A      ) ;
	reg|=(1<<1);

	SCB->AIRCR = SCB_AIRCR_VECTKEY | SCB_AIRCR_VECTRESET;
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


}


int main()
{

	InterruptController _ic;
	_ic.RemapToRam();

	_ic.SetHandler( MemoryManagement_IRQn,memory_handler);
	_ic.EnableChannel(MemoryManagement_IRQn);

	_ic.SetHandler( BusFault_IRQn,bus_handler);
	_ic.EnableChannel(BusFault_IRQn);

	_ic.SetHandler( UsageFault_IRQn,usage_handler);
	_ic.EnableChannel(UsageFault_IRQn);

	SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk
	 | SCB_SHCSR_BUSFAULTENA_Msk
	 | SCB_SHCSR_MEMFAULTENA_Msk;

	Led _led;
	_led.Init();

	ICommunicationObject* _port = new SerialPort(_USART0,9600);

	while(1)
	{
		_led.On(1);

		reset_();

		_port->SendData((uint8_t*)"Hello world\r\n",13);
		_delay_ms(250);
		_led.Off(1);
		_delay_ms(250);
	}

	return 0;
}
