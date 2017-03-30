/*
 * Blinker.h
 *
 *  Created on: 09 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_BLINKER_H_
#define DRIVER_BLINKER_H_

#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"

#define LedPin1 GPIO_Pin_0
#define LedPin2 GPIO_Pin_1

#define LedPort GPIOA

class Blinker
{
public:
	void Init()
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		GPIO_InitStructure.GPIO_Pin =  LedPin1| LedPin2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LedPort, &GPIO_InitStructure);
	}

	void On(unsigned char i)
	{
		switch(i)
		{
			case 1:
				LedPort->BSRR = LedPin1;
				break;
			case 2:
				LedPort->BSRR = LedPin2;
				break;

		}
	}

	void Off(unsigned char i)
	{
		switch(i)
		{
			case 1:
				LedPort->BRR = LedPin1;
				break;
			case 2:
				LedPort->BRR = LedPin2;
				break;

		}
	}

private:
	 GPIO_InitTypeDef  GPIO_InitStructure;
};



#endif /* DRIVER_BLINKER_H_ */
