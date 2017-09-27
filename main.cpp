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



//
//void SysTickCallback1()
//{
//   int a=0;
//}
//
//void SysTickCallback2()
//{
//	int a=0;
//}
//
//void GpioInterrupt1()
//{
//
//}
//
//void GpioInterrupt2()
//{
//	int a=0;
//}
//
//void InputsInterrupt()
//{
//
//}

#define L1 GPIO_Pin_0
#define L2 GPIO_Pin_2
#define L3 GPIO_Pin_1
#define L4 GPIO_Pin_3
#define PORT GPIOA
#define PHERIF RCC_APB2Periph_GPIOA

#define StepsStart 150

class Stepper
{
public:

	enum StepMode
	{
		SingleStep,
		MiniStep,
		MicroStep
	};

	Stepper(int startDelay, int workDelay, StepMode mode, bool holding)
	{
		_delay=workDelay;
		_startDelay =startDelay;
		_workDelay = workDelay;
		_mode = mode;
		_holding = holding;
		_currentStep=1;
	}

	void Init()
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;

		// Configure pin in output push/pull mode
		GPIO_InitStructure.GPIO_Pin = L1|L2|L3|L4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

	void OneStep(bool rotation)
	{

		switch(_mode)
		{

		case SingleStep:
			switch(_currentStep)
			{
				case 1:
					GPIO_ResetBits(GPIOA,L2);
					GPIO_ResetBits(GPIOA,L3);
					GPIO_ResetBits(GPIOA,L4);
					GPIO_SetBits(GPIOA,L1);

					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L1);
					}
					break;
				case 2:
					GPIO_ResetBits(GPIOA,L1);
					GPIO_ResetBits(GPIOA,L3);
					GPIO_ResetBits(GPIOA,L4);
					GPIO_SetBits(GPIOA,L2);

					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L2);
					}
					break;
				case 3:
					GPIO_ResetBits(GPIOA,L1);
					GPIO_ResetBits(GPIOA,L2);
					GPIO_ResetBits(GPIOA,L4);
					GPIO_SetBits(GPIOA,L3);
					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L3);
					}
					break;
				case 4:
					GPIO_ResetBits(GPIOA,L1);
					GPIO_ResetBits(GPIOA,L2);
					GPIO_ResetBits(GPIOA,L3);
					GPIO_SetBits(GPIOA,L4);
					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L4);
					}
					break;
				}

			if(rotation)
			{
				if(_currentStep++>=4)
				{
					_currentStep=1;
				}
			}
			else
			{
				if(_currentStep--<=1)
				{
					_currentStep=4;
				}
			}

		break;

		case MiniStep:
				switch(_currentStep)
				{
					case 1:
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L1);
						GPIO_SetBits(GPIOA,L2);
						break;
					case 2:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L2);
						GPIO_SetBits(GPIOA,L3);
						break;
					case 3:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);

						GPIO_SetBits(GPIOA,L3);
						GPIO_SetBits(GPIOA,L4);
						break;
					case 4:
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);

						GPIO_SetBits(GPIOA,L4);
						GPIO_SetBits(GPIOA,L1);
						break;
				}
				if(rotation)
				{
					if(_currentStep++>=4)
					{
						_currentStep=1;
					}
				}
				else
				{
					if(_currentStep--<=1)
					{
						_currentStep=4;
					}
				}

			break;

			case MicroStep:
				switch(_currentStep)
				{
					case 1:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L1);

						break;
					case 2:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L1);
						GPIO_SetBits(GPIOA,L2);
						break;
					case 3:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L2);
						break;
					case 4:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L2);
						GPIO_SetBits(GPIOA,L3);
						break;
					case 5:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);


						GPIO_SetBits(GPIOA,L3);
						break;
					case 6:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L3);
						GPIO_SetBits(GPIOA,L4);
						break;
					case 7:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);


						GPIO_SetBits(GPIOA,L4);
						break;
					case 8:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L4);
						GPIO_SetBits(GPIOA,L1);
						break;
				}
				if(rotation)
				{
					if(_currentStep++>=8)
					{
						_currentStep=1;
					}
				}
				else
				{
					if(_currentStep--<=1)
					{
						_currentStep=8;
					}
				}
			break;
		}

		_delay_ms(_delay);

	}

	void MultiStep(int steps, bool rotation)
	{
		if(steps<StepsStart*2)
		{
			for(int i=0; i<steps;i++)
			{
				OneStep(rotation);
			}
		}
		else
		{

			_delay = _startDelay;
			int counter=0;
			for(int i=0;i<StepsStart;i++)
			{
				OneStep(rotation);
				if(counter++>= 50)
				{
					counter=0;
					_delay--;
				}
			}
			_delay = _workDelay;
			for(int i=0;i<steps - StepsStart;i++)
			{
				OneStep(rotation);
			}
		}

	}


private:

	int _delay;
	int _startDelay;
	int _workDelay;
	bool _holding;
	StepMode _mode;

	char _currentStep;
};

Stepper _st(6,4,Stepper::StepMode::MiniStep,true);
bool t=true;

void TimerElapsed()
{
	_st.OneStep(t);
}

int main()
{





	//_ic.RemapToRam();
//
//
//	SystemTimer _systim(1000);
//	_systim.Init();
//	_systim.AddCallback(SysTickCallback1);
//	_systim.AddCallback(SysTickCallback2);
//	_systim.Enable();
//
//	Usb* _usb = new Usb();
//	_usb->RxBufferSize = 64;
//	_usb->TxBufferSize = 64;
//	_usb->TypeUsb = VirtualComPort;
//
//	_usb->Init();
//
//	SerialPort* _port = new SerialPort(SerialPort::COM1,9600);
//	_port->Init();
//
//
	_led.Init();
//
//	RF433* _rf = new RF433();
//	_rf->Init();
//	_rf->StartReceive();

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
//	Tim _tim(Tim::Timer2,1500,Tim::InterruptType::IT_Update);
//	_tim.OnElapsed = TimerElapsed;
//	_tim.Init();
//	_tim.Enable();

//	SGPIO * _gpio2 = new SGPIO();
//	_gpio2->Init();
//	_gpio2->SetOutput(SGPIO::pin1, true);
//	_gpio2->SetTimer(&_tim);
//	_gpio2->SetCallback(InputsInterrupt);
//	_tim.Enable();

		int counter=0;
		int delay = 3;
		char step =2;




		_st.Init();

	while(1)
	{
		//_st.OneStep(t);

		_st.MultiStep(200,t);


		if(counter++>=5)
		{
			counter=0;
			t=!t;
		}

//		switch(step)
//		{
//			case 1:
//				GPIO_SetBits(GPIOA,GPIO_Pin_0);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_2);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_1);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_3);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//
//				break;
//			case 2:
//
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_0);
//				GPIO_SetBits(GPIOA,GPIO_Pin_2);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//				GPIO_SetBits(GPIOA,GPIO_Pin_1);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//				GPIO_SetBits(GPIOA,GPIO_Pin_3);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//				GPIO_SetBits(GPIOA,GPIO_Pin_0);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//
//				break;
//
//
//		}
//
//		if(counter++>100)
//		{
//			counter=0;
//
//			if(t)
//			{
//				delay++;
//			}else
//			{
//				delay--;
//			}
//
//			if(delay >= 8 && t==true)
//			{
//				t=false;
//			}
//
//			if(delay<=1 && t==false)
//			{
//				t=true;
//			}
//		}

//		_port->SendData((uint8_t*)"Hello world\r\n",13);
//		_usb->SendData((uint8_t*)_rf->getString(),4);
//		//_usb->SendData((uint8_t*)"\r\n",2);
//		_led.On(1);
//		//_gpio1->Write(true);
//		if(_port->GetAnswer(150))
//		{
//			_port->SendData((uint8_t*)_port->Data.ToString(),_port->Data.Lendth());
//			_port->WriteByte('\r');
//			_port->WriteByte('\n');
//			_port->ClearBuffer();
//			_port->Data.Clear();
//		}
//
//
//		_led.Off(1);
//		//_gpio1->Write(false);
//		_delay_ms(150);
	}

	return 0;
}

