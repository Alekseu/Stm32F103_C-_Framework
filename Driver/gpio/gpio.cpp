/*
 * gpio.cpp
 *
 *  Created on: 31 рту. 2017 у.
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
		_interruptNumber=0;
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
		_interruptNumber=0;
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
		//GPIO_InitStruct.GPIO_Pin = _pin;
		//		GPIO_InitStruct.GPIO_Speed = (GPIOSpeed_TypeDef)_speed;
		//		GPIO_InitStruct.GPIO_Mode = (GPIOMode_TypeDef)_mode;
		//		GPIO_Init((GPIO_TypeDef*)_port, &GPIO_InitStruct);
		switch(_pin)
		{
			case Pin0:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
				_pinSource = 0;
				break;
			case Pin1:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
				_pinSource = 1;
				break;
			case Pin2:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
				_pinSource = 2;
				break;
			case Pin3:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
				_pinSource = 3;
				break;
			case Pin4:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
				_pinSource = 4;
				break;
			case Pin5:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
				_pinSource = 5;
				break;
			case Pin6:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
				_pinSource = 6;
				break;
			case Pin7:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
				_pinSource = 7;
				break;
			case Pin8:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
				_pinSource = 8;
				break;
			case Pin9:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
				_pinSource = 9;
				break;
			case Pin10:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
				_pinSource = 10;
				break;
			case Pin11:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
				_pinSource = 11;
				break;
			case Pin12:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
				_pinSource = 12;
				break;
			case Pin13:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
				_pinSource = 13;
				break;
			case Pin14:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
				_pinSource = 14;
				break;
			case Pin15:
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
				_pinSource = 15;
				break;
		}

		switch(_speed)
		{
			case Speed_10MHz:
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
				break;
			case Speed_2MHz:
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
				break;
			case Speed_50MHz:
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
				break;
		}

		switch(_mode)
		{
			case AIN:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
				break;
			case IN_FLOATING:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
				break;
			case IPD:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
				break;
			case IPU:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
				break;
			case Out_OD:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
				break;
			case Out_PP:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				break;
			case AF_OD:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
				break;
			case AF_PP:
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				break;
		}

		switch(_port)
			{
			case PORTA:
				_portSource=0;
				GPIO_Init(GPIOA, &GPIO_InitStruct);
				break;
			case PORTB:
				_portSource=1;
				GPIO_Init(GPIOB, &GPIO_InitStruct);
				break;
			case PORTC:
				_portSource=2;
				GPIO_Init(GPIOC, &GPIO_InitStruct);
				break;
			case PORTD:
				_portSource=3;
				GPIO_Init(GPIOD, &GPIO_InitStruct);
				break;
			case PORTE:
				_portSource=4;
				GPIO_Init(GPIOE, &GPIO_InitStruct);
				break;
			}

		GPIO::GObjs[(15*_portSource)+pin] = this;
	}

	void GPIO::Write(bool state)
	{
		GPIO_TypeDef * __port;
		uint16_t GPIO_Pin;

		switch(_port)
		{
			case PORTA:
				__port = GPIOA;
				break;
			case PORTB:
				__port = GPIOB;
				break;
			case PORTC:
				__port = GPIOC;
				break;
			case PORTD:
				__port = GPIOD;
				break;
			case PORTE:
				__port = GPIOE;
				break;
		}


		switch(_pin)
		{
			case Pin0:
				GPIO_Pin = GPIO_Pin_0;
				break;
			case Pin1:
				GPIO_Pin = GPIO_Pin_1;
				break;
			case Pin2:
				GPIO_Pin = GPIO_Pin_2;
				break;
			case Pin3:
				GPIO_Pin = GPIO_Pin_3;
				break;
			case Pin4:
				GPIO_Pin = GPIO_Pin_4;
				break;
			case Pin5:
				GPIO_Pin = GPIO_Pin_5;
				break;
			case Pin6:
				GPIO_Pin = GPIO_Pin_6;
				break;
			case Pin7:
				GPIO_Pin = GPIO_Pin_7;
				break;
			case Pin8:
				GPIO_Pin = GPIO_Pin_8;
				break;
			case Pin9:
				GPIO_Pin = GPIO_Pin_9;
				break;
			case Pin10:
				GPIO_Pin = GPIO_Pin_10;
				break;
			case Pin11:
				GPIO_Pin = GPIO_Pin_11;
				break;
			case Pin12:
				GPIO_Pin = GPIO_Pin_12;
				break;
			case Pin13:
				GPIO_Pin = GPIO_Pin_13;
				break;
			case Pin14:
				GPIO_Pin = GPIO_Pin_14;
				break;
			case Pin15:
				GPIO_Pin = GPIO_Pin_15;
				break;
		}

		if(state)
		{
			GPIO_SetBits(__port, GPIO_Pin);
		}
		else
		{
			GPIO_ResetBits(__port, GPIO_Pin);
		}
	}

	bool GPIO::Read()
	{
		bool state;

		switch(_port)
		{
			case PORTA:
				__port = GPIOA;
				break;
			case PORTB:
				__port = GPIOB;
				break;
			case PORTC:
				__port = GPIOC;
				break;
			case PORTD:
				__port = GPIOD;
				break;
			case PORTE:
				__port = GPIOE;
				break;
		}


		switch(_pin)
		{
			case Pin0:
				GPIO_Pin = GPIO_Pin_0;
				break;
			case Pin1:
				GPIO_Pin = GPIO_Pin_1;
				break;
			case Pin2:
				GPIO_Pin = GPIO_Pin_2;
				break;
			case Pin3:
				GPIO_Pin = GPIO_Pin_3;
				break;
			case Pin4:
				GPIO_Pin = GPIO_Pin_4;
				break;
			case Pin5:
				GPIO_Pin = GPIO_Pin_5;
				break;
			case Pin6:
				GPIO_Pin = GPIO_Pin_6;
				break;
			case Pin7:
				GPIO_Pin = GPIO_Pin_7;
				break;
			case Pin8:
				GPIO_Pin = GPIO_Pin_8;
				break;
			case Pin9:
				GPIO_Pin = GPIO_Pin_9;
				break;
			case Pin10:
				GPIO_Pin = GPIO_Pin_10;
				break;
			case Pin11:
				GPIO_Pin = GPIO_Pin_11;
				break;
			case Pin12:
				GPIO_Pin = GPIO_Pin_12;
				break;
			case Pin13:
				GPIO_Pin = GPIO_Pin_13;
				break;
			case Pin14:
				GPIO_Pin = GPIO_Pin_14;
				break;
			case Pin15:
				GPIO_Pin = GPIO_Pin_15;
				break;
		}

		if(GPIO_ReadInputDataBit(__port, GPIO_Pin))
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



		// EXTI_Line15
		EXTI_InitTypeDef EXTI_InitStructure;

		uint8_t __pinSource=0;

		switch(_pin)
		{
		case Pin0:
			__pinSource =GPIO_PinSource0;
			EXTI_Line  = EXTI_Line0;
			break;
		case Pin1:
			__pinSource =GPIO_PinSource1;
			EXTI_Line  = EXTI_Line1;
			break;
		case Pin2:
			__pinSource =GPIO_PinSource2;
			EXTI_Line  = EXTI_Line2;
			break;
		case Pin3:
			__pinSource =GPIO_PinSource3;
			EXTI_Line  = EXTI_Line3;
			break;
		case Pin4:
			__pinSource =GPIO_PinSource4;
			EXTI_Line  = EXTI_Line4;
			break;
		case Pin5:
			__pinSource =GPIO_PinSource5;
			EXTI_Line  = EXTI_Line5;
			break;
		case Pin6:
			__pinSource =GPIO_PinSource6;
			EXTI_Line  = EXTI_Line6;
			break;
		case Pin7:
			__pinSource =GPIO_PinSource7;
			EXTI_Line  = EXTI_Line7;
			break;
		case Pin8:
			__pinSource =GPIO_PinSource8;
			EXTI_Line  = EXTI_Line8;
			break;
		case Pin9:
			__pinSource =GPIO_PinSource9;
			EXTI_Line  = EXTI_Line9;
			break;
		case Pin10:
			__pinSource =GPIO_PinSource10;
			EXTI_Line  = EXTI_Line10;
			break;
		case Pin11:
			__pinSource =GPIO_PinSource11;
			EXTI_Line  = EXTI_Line11;
			break;
		case Pin12:
			__pinSource =GPIO_PinSource12;
			EXTI_Line  = EXTI_Line12;
			break;
		case Pin13:
			__pinSource =GPIO_PinSource13;
			EXTI_Line  = EXTI_Line13;
			break;
		case Pin14:
			__pinSource =GPIO_PinSource14;
			EXTI_Line  = EXTI_Line14;
			break;
		case Pin15:
			__pinSource =GPIO_PinSource15;
			EXTI_Line  = EXTI_Line15;
			break;
		}

		switch(_port)
		{
		case PORTA:
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, _pinSource);
			break;
		case PORTB:
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, _pinSource);
			break;
		case PORTC:
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, _pinSource);
			break;
		case PORTD:
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, _pinSource);
			break;
		case PORTE:
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, _pinSource);
			break;
		}

		EXTI_InitStructure.EXTI_Line = EXTI_Line;
		    //Label range interrupt line EXTI_Line0~EXTI_Line15
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //Interrupt mode, optional values for the interrupt EXTI_Mode_Interrupt and event EXTI_Mode_Event.

		switch(_type)
		{
		case Rising:
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
			break;
		case Falling:
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
			break;
		case Rising_Falling:
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
			break;
		}

	//	EXTI_InitStructure.EXTI_Trigger = (EXTITrigger_TypeDef)_type; //Trigger mode, can be a falling edge trigger EXTI_Trigger_Falling, the rising edge triggered EXTI_Trigger_Rising, or any level (rising edge and falling edge trigger EXTI_Trigger_Rising_Falling)
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		//uint16_t InterruptNumber;
		switch(_pin)
		{
			case Pin0:
				_interruptNumber = EXTI0_IRQn;
				break;
			case Pin1:
				_interruptNumber = EXTI1_IRQn;
				break;
			case Pin2:
				_interruptNumber = EXTI2_IRQn;
				break;
			case Pin3:
				_interruptNumber = EXTI3_IRQn;
				break;
			case Pin4:
				_interruptNumber = EXTI4_IRQn;
				break;
			case Pin5:
			case Pin6:
			case Pin7:
			case Pin8:
			case Pin9:
				_interruptNumber = EXTI9_5_IRQn;
				break;
			case Pin10:
			case Pin11:
			case Pin12:
			case Pin13:
			case Pin14:
			case Pin15:
				_interruptNumber = EXTI15_10_IRQn;
				break;
		}


	}

	void GPIO::EnableIrq()
	{
		InterruptController::SetHandler((IRQn_Type)_interruptNumber,InterruptWraper);
				InterruptController::EnableChannel((IRQn_Type)_interruptNumber);
	}

	void GPIO::DisableIrq()
	{
		InterruptController::SetHandler((IRQn_Type)_interruptNumber,InterruptWraper);
				InterruptController::DisableChannel((IRQn_Type)_interruptNumber);
	}

	void GPIO::InterruptWraper(void)
	{
		uint16_t line =0;

		for(int i=0;i<(15*5);i++)
		{
			if(GPIO::GObjs[i]!=0 && GPIO::GObjs[i]->_callback!=0 )
			{
				if(EXTI_GetITStatus(GPIO::GObjs[i]->EXTI_Line) != RESET)
				{
					 EXTI_ClearITPendingBit(GPIO::GObjs[i]->EXTI_Line);
					GPIO::GObjs[i]->_callback();
				}
			}
		}

	}

}
