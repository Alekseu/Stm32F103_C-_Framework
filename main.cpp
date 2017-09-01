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
#include "Driver/i2c/i2c.h"

#include "Driver/led/led.h"
#include "Extention/sPtr.h"

#include "Driver/timer/tim.h"
#include "Driver/systick/systick.h"

using namespace Driver;
InterruptController _ic;

void TimerElapsed()
{
	int a=0;
}

void SysTickCallback1()
{
   int a=0;
}

void SysTickCallback2()
{
	int a=0;
}

int main()
{
	_ic.RemapToRam();

	SystemTimer _systim(1000);
	_systim.Init();
	_systim.AddCallback(SysTickCallback1);
	_systim.AddCallback(SysTickCallback2);
	_systim.Enable();

	SerialPort* _port = new SerialPort(SerialPort::COM1,9600);
	_port->Init();

	Led _led;
	_led.Init();

	I2c* _i2c = new I2c(I2c::in_I2C1,0xc, I2c::Master,I2c::s_50kHz);
	_i2c->Init();

	Tim _tim(Tim::Timer2,1500,Tim::InterruptType::IT_Update);
	_tim.OnElapsed = TimerElapsed;
	_tim.Init();
	_tim.Enable();

	while(1)
	{
		_port->SendData((uint8_t*)"Hello world\r\n",13);
		_led.On(1);
		if(_port->GetAnswer(150))
		{
			_port->SendData((uint8_t*)_port->Data.ToString(),_port->Data.Lendth());
			_port->WriteByte('\r');
			_port->WriteByte('\n');
			_port->ClearBuffer();
			_port->Data.Clear();
		}


		_led.Off(1);
		_delay_ms(150);
	}

	return 0;
}

