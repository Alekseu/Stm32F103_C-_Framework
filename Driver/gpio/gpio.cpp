/*
 * gpio.cpp
 *
 *  Created on: 31 авг. 2017 г.
 *      Author: hudienko_a
 */

#include "gpio.h"
#include "../nvic/nvic.h"

namespace Driver
{
	GPIO* GPIO::GObjs[15*5]={0};



	GPIO::GPIO(Port port, Pin pin)
	{
		_port = port;
		_pin = pin;
		_mode = Mode::Out_PP;
		_speed= Speed::Speed_2MHz;
		_callback =0;
		_type = InterruptType::Rising;
		_portSource=0;
		_pinSource=0;
	}

	GPIO::GPIO(Port port, Pin pin, Mode mode, Speed speed)
	{
		_port = port;
		_pin = pin;
		_mode = mode;
		_speed= speed;
		_callback =0;
		_type = InterruptType::Rising;
		_portSource=0;
		_pinSource=0;
	}

	GPIO::~GPIO()
	{

	}

	void GPIO::Init()
	{
		uint8_t pin =0;
		switch(_port)
		{
		case PORTA:
			_portSource=0;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			break;
		case PORTB:
			_portSource=1;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			break;
		case PORTC:
			_portSource=2;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			break;
		case PORTD:
			_portSource=3;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
			break;
		case PORTE:
			_portSource=4;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
			break;
		}
		GPIO_InitStruct.GPIO_Pin = _pin; //Настройки вывода PA9
		GPIO_InitStruct.GPIO_Speed = (GPIOSpeed_TypeDef)_speed; //Скорость порта максимальная
		GPIO_InitStruct.GPIO_Mode = (GPIOMode_TypeDef)_mode; //Режим альтернативной функции, выход Push-Pull
		GPIO_Init((GPIO_TypeDef*)_port, &GPIO_InitStruct);

		switch(_pin)
				{
					case Pin0:
						_pinSource = 0;
						break;
					case Pin1:
						_pinSource = 1;
						break;
					case Pin2:
						_pinSource = 2;
						break;
					case Pin3:
						_pinSource = 3;
						break;
					case Pin4:
						_pinSource = 4;
						break;
					case Pin5:
						_pinSource = 5;
						break;
					case Pin6:
						_pinSource = 6;
						break;
					case Pin7:
						_pinSource = 7;
						break;
					case Pin8:
						_pinSource = 8;
						break;
					case Pin9:
						_pinSource = 9;
						break;
					case Pin10:
						_pinSource = 10;
						break;
					case Pin11:
						_pinSource = 11;
						break;
					case Pin12:
						_pinSource = 12;
						break;
					case Pin13:
						_pinSource = 13;
						break;
					case Pin14:
						_pinSource = 14;
						break;
					case Pin15:
						_pinSource = 15;
						break;
				}

		GPIO::GObjs[(15*_portSource)+pin] = this;
	}

	void GPIO::Write(bool state)
	{
		if(state)
		{
			GPIO_SetBits((GPIO_TypeDef*)_port, _pin);
		}
		else
		{
			GPIO_ResetBits((GPIO_TypeDef*)_port, _pin);
		}
	}

	bool GPIO::Read()
	{
		bool state;
		if(GPIO_ReadInputDataBit((GPIO_TypeDef*)_port, _pin))
		{
			state = true;
		}
		else
		{
			state = false;
		}
		return state;
	}


	void GPIO::SetCallback(GpioPinCallback* callback , InterruptType type)
	{
		_callback = callback;
		_type = type;

		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

		 GPIO_EXTILineConfig(_portSource, _pinSource);

		EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_InitStructure.EXTI_Line=(uint32_t)_pin;    //Label range interrupt line EXTI_Line0~EXTI_Line15
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //Interrupt mode, optional values for the interrupt EXTI_Mode_Interrupt and event EXTI_Mode_Event.
		EXTI_InitStructure.EXTI_Trigger = (EXTITrigger_TypeDef)_type; //Trigger mode, can be a falling edge trigger EXTI_Trigger_Falling, the rising edge triggered EXTI_Trigger_Rising, or any level (rising edge and falling edge trigger EXTI_Trigger_Rising_Falling)
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		uint16_t InterruptNumber;
		switch(_pin)
		{
			case Pin0:
				InterruptNumber = EXTI0_IRQn;
				break;
			case Pin1:
				InterruptNumber = EXTI1_IRQn;
				break;
			case Pin2:
				InterruptNumber = EXTI2_IRQn;
				break;
			case Pin3:
				InterruptNumber = EXTI3_IRQn;
				break;
			case Pin4:
				InterruptNumber = EXTI4_IRQn;
				break;
			case Pin5:
			case Pin6:
			case Pin7:
			case Pin8:
			case Pin9:
				InterruptNumber = EXTI9_5_IRQn;
				break;
			case Pin10:
			case Pin11:
			case Pin12:
			case Pin13:
			case Pin14:
			case Pin15:
				InterruptNumber = EXTI15_10_IRQn;
				break;
		}

		InterruptController::SetHandler((IRQn_Type)InterruptNumber,InterruptWraper);
		InterruptController::EnableChannel((IRQn_Type)InterruptNumber);
	}

	void GPIO::InterruptWraper(void)
	{
		uint16_t line =0;

		for(int i=0;i<(15*5);i++)
		{
			if(GPIO::GObjs[i]!=0 && GPIO::GObjs[i]->_callback!=0 )
			{
				if(EXTI_GetITStatus(GPIO::GObjs[i]->_pin) != RESET)
				{
					 EXTI_ClearITPendingBit(GPIO::GObjs[i]->_pin);
					GPIO::GObjs[i]->_callback();
				}
			}
		}

	}

}
