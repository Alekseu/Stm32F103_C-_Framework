/*
 * led.cpp
 *
 *  Created on: 31 ����� 2017 �.
 *      Author: hudienko_a
 */

#include "led.h"

namespace Driver
{

	Led* Led::Lobj=0;


	Led::Led()
	{
		Lobj = this;
		state = false;
	}

	Led::~Led()
	{

	}


	void Led::Init()
	{
		RCC_APB2PeriphClockCmd(LedRCC,ENABLE);
		GPIO_InitStructure.GPIO_Pin =  LedPin1;//| LedPin2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LedPort, &GPIO_InitStructure);
	}

	void Led::On(unsigned char i)
	{
		switch(i)
		{
		case 1:
			LedPort->BSRR = LedPin1;
			break;
		case 2:
			//LedPort->BSRR = LedPin2;
			break;

		}
	}

	void Led::Off(unsigned char i)
	{
		switch(i)
		{
		case 1:
			LedPort->BRR = LedPin1;
			break;
		case 2:
			//LedPort->BRR = LedPin2;
			break;

		}
	}
	void Led::togle(){
			if(state){
				this->On(1);
				state = false;
			} else {
				this->Off(1);
				state = true;
			}
		}
}
