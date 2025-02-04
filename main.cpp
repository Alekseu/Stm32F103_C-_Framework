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
#include "Driver/spi/PSpi.h"
#include "Driver/i2c/i2c.h"

#include "Driver/fsmc/fsmc.h"

#include "Driver/led/led.h"
#include "Extention/sPtr.h"

#include "Driver/tim/tim.h"
#include "Driver/systick/systick.h"
#include "Driver/gpio/gpio.h"
#include "Device/SGPIO/SGPIO.h"
#include "Device/RF433/RF433.h"
#include "Driver/usb/usb.h"

#include "Device/encoder/encoder.h"

#include "Extention/GraphicTextFunctions.h"

#include "Device/Lcd/SSD1306.h"
#include "Device/Lcd/LcdSPI.h"

#include "Extention/animationProcessor/LoadingTable.h"

#include "Os/kernel.h"
#include "Device/sdCard/inc/ff.h"
#include "Device/w5500/w5500.h"

#include "Driver/adc/adc.h"

using namespace Driver;
using namespace Device;
using namespace Os;

Led _led;
Fsmc* sram;
//Adc* _adc;
bool trigger;

extern "C"
{
	void Led_RW_ON()
	{
		LedPort->BSRR = LedPin1;
	}

	void  Led_RW_OFF()
	{
		LedPort->BRR = LedPin1;
	}
}

int main()
{
	InterruptController::RemapToRam();
	_led.Init();

	sram = new Fsmc();
	sram->Init();
	sram->RamTest8();

	unsigned int counter=0;
	//_adc = new Adc(9,Adc::Adc1,Adc::AdcInternalTermo,Adc::TempSensor);
	//_adc->Init();
	unsigned short data=0;
	//_adc->AdcStart();
	while(1)
	{
		if(counter++>=5)
		{
			counter=0;
			_led.togle();
			//data = _adc->ReadAdc();
		}

		 _delay_ms(100);
	}

	return 0;
}

