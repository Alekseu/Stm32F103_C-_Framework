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

using namespace Driver;
using namespace Device;

InterruptController _ic;
Led _led;
bool trigger;


void TimerElapsed()
{
	if(trigger)
		{
			_led.On(2);
			trigger = false;
		}
		else
		{
			_led.Off(2);
			trigger = true;
		}
}

void SysTickCallback1()
{
   int a=0;
}

void SysTickCallback2()
{
	int a=0;
}

void GpioInterrupt1()
{

}

void GpioInterrupt2()
{
	int a=0;
}

void InputsInterrupt()
{

}

int main()
{
	_ic.RemapToRam();


	SystemTimer _systim(1000);
	_systim.Init();
	_systim.AddCallback(SysTickCallback1);
	_systim.AddCallback(SysTickCallback2);
	_systim.Enable();

	Usb* _usb = new Usb();
	_usb->RxBufferSize = 64;
	_usb->TxBufferSize = 64;
	_usb->TypeUsb = VirtualComPort;

	_usb->Init();

	SerialPort* _port = new SerialPort(SerialPort::COM1,9600);
	_port->Init();


	_led.Init();

	RF433* _rf = new RF433();
	_rf->Init();
	_rf->StartReceive();

//	I2c* _i2c = new I2c(I2c::in_I2C1,0xc, I2c::Master,I2c::s_50kHz);
//	_i2c->Init();
//
//	Spi* _spi = new Spi(Spi::sn_SPI1,Spi::Master,Spi::Sys_div_128);
//	_spi->Init();

//	GPIO* _gpio = new GPIO(GPIO::Port::PORTC,GPIO::Pin::Pin10,GPIO::Mode::IN_FLOATING,GPIO::Speed::Speed_2MHz);
//	_gpio->Init();
//	_gpio->SetCallback(GpioInterrupt1,GPIO::InterruptType::Rising);
//
//	GPIO* _gpio1 = new GPIO(GPIO::Port::PORTE,GPIO::Pin::Pin2);
//	_gpio1->Init();
//
//	Tim _tim(Tim::Timer2,2400,Tim::InterruptType::IT_Update);
//	_tim.OnElapsed = TimerElapsed;
//	_tim.Init();
//	_tim.Enable();

//	SGPIO * _gpio2 = new SGPIO();
//	_gpio2->Init();
//	_gpio2->SetOutput(SGPIO::pin1, true);
//	_gpio2->SetTimer(&_tim);
//	_gpio2->SetCallback(InputsInterrupt);
//	_tim.Enable();

	while(1)
	{
		_port->SendData((uint8_t*)"Hello world\r\n",13);
		_usb->SendData((uint8_t*)_rf->getString(),5);
		_led.On(1);
		//_gpio1->Write(true);
		if(_port->GetAnswer(150))
		{
			_port->SendData((uint8_t*)_port->Data.ToString(),_port->Data.Lendth());
			_port->WriteByte('\r');
			_port->WriteByte('\n');
			_port->ClearBuffer();
			_port->Data.Clear();
		}


		_led.Off(1);
		//_gpio1->Write(false);
		_delay_ms(150);
	}

	return 0;
}

