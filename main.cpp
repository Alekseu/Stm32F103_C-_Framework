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
#include "Driver/spi/spi.h"
#include "Driver/i2c/i2c.h"

#include "Driver/led/led.h"
#include "Extention/sPtr.h"

#include "Driver/tim/tim.h"
#include "Driver/systick/systick.h"
#include "Driver/gpio/gpio.h"
#include "Device/SGPIO/SGPIO.h"
#include "Device/RF433/RF433.h"
#include "Driver/usb/usb.h"

#include "Extention/GraphicTextFunctions.h"

//#include "ow.h"

using namespace Driver;
using namespace Device;

InterruptController _ic;
Led _leds;

bool trigger;

extern "C"
{
	void Led_RW_ON()
	{
		LedPort->BSRR = LedPin2;
	}

	void  Led_RW_OFF()
	{
		LedPort->BRR = LedPin2;
	}
}

int main()
{

	InterruptController::RemapToRam();

	_leds.Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	// Configure pin in output push/pull mode
	GPIO_InitStructure.GPIO_Pin = SCK|MOSI|D_C|CS|RES;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	GPIO_ResetBits(GPIOA, RES);
	_delay_ms(10);
	GPIO_SetBits(GPIOA, RES);

	GPIO_ResetBits(GPIOA, CS);


//		Init();
//		char buf[64];
//		therm_read_temperature(buf);


	Usb _com;
	_com.RxBufferSize = 64;
	_com.TxBufferSize = 64;
	_com.TypeUsb = HumanInterfaceDevice;
	_com.TypeHid = Hid_Mouse;
	_com.Init();

	int a=0;
	int index=0;
	while(1)
	{
		if(a++>=150 && a< 300)
			{
				_leds.On(1);

			}
			else if(a>=300)
			{
				a=0;
				_leds.Off(1);

			}

		if(a==160)
		{

			//_com.KeyboardSend("hello");
			_com.MouseMove(rand(),rand());
		}

		 _delay_ms(1);
	}

	return 0;
}

