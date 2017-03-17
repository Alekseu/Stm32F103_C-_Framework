/*
 * SL.h
 *
 *  Created on: 17 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SINGLELED_SL_H_
#define DRIVER_SINGLELED_SL_H_
#include "../Dma.h"

extern "C"
{
	#include "../../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
	#include "../../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
#include "../../StdPeriph/stm_lib/inc/stm32f10x_tim.h"


void TIM2_IRQHandler()
	{
		if (TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		}
	}
}

class SL:Dma
{
public:

	unsigned char Width;
	unsigned char Height;
	unsigned char** VideoBuffer;


	SL(){
		VideoBuffer=0;
		Width =0;
		Height=0;
		_frameBuffer=0;
	}

	void SLInit()
	{
		if(Width>0 && Height>0)
		{
			Init(CHANNEL_2);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			//InitPherif((void*)&(GPIOC->ODR));
			_frameBuffer = new unsigned char[(Width*2*Height/4)+2];
			int length = Width*2*Height/4+2;
			int counter =0;
			for(int i=0;i<length-2;i+=4)
			{
			_frameBuffer[i] = 0;
			_frameBuffer[i+1] = 1;
			_frameBuffer[i+2] = 4;
			_frameBuffer[i+3] = 5;
			}

			_frameBuffer[length-2] = 2;
			_frameBuffer[length-1] = 0;
    		StartTransmitArrayDmaCircle(_frameBuffer,(char*)&(GPIOC->ODR),Width*2*Height/4+2);
		}
	}

	void TransmitDmaComplete()
	{
		//StartTransmitArrayDmaCircle(_frameBuffer,(char*)&(GPIOC->ODR),Width*2*Height/4+2);
	}



private:
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned char* _frameBuffer;

};


#endif /* DRIVER_SINGLELED_SL_H_ */
