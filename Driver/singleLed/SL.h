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


#define CLK_BIT 0
#define LATCH_BIT 1
#define R1_BIT 2
#define R2_BIT 3
#define G1_BIT 4
#define G2_BIT 5
#define B1_BIT 6
#define B2_BIT 7

typedef struct Pixel
{
	union
	{
		struct
		{
			unsigned char P1 : 4;
			unsigned char P2 : 4;
		};
		unsigned char Data : 8;
	};
};

class SL:Dma
{
public:

	unsigned char Width;
	unsigned char Height;
	unsigned char* VideoBuffer;


	SL(){
		VideoBuffer=0;
		Width =0;
		Height=0;
		_frameBuffer=0;
		_colorCounter=0;

		_columnCounter=0;
		_rowCounter=0;
		_lengthFrameBuffer=0;
		_currentRow=0;
		_coef =0;
	}

	void SLInit()
	{
		if(Width>0 && Height>0)
		{
			Init(CHANNEL_2);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOB, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
			//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
			 TIM_TimeBaseStructInit(&timerInitStructure);
				 timerInitStructure.TIM_Prescaler =25-1; // set 1us resolution
				 timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
				 timerInitStructure.TIM_Period =225;
				 timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
				 timerInitStructure.TIM_RepetitionCounter = 0;
				 TIM_TimeBaseInit(TIM3, &timerInitStructure);
				 TIM_Cmd(TIM3, ENABLE);


				 outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
				 outputChannelInit.TIM_Pulse = 0;
				 outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
				 outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

				 TIM_OC1Init(TIM3, &outputChannelInit);
				 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

			//InitPherif((void*)&(GPIOC->ODR));

			VideoBuffer = new unsigned char[((Width/2)*Height)+2];
//			for(int i=0;i<Width/2;i++)
//			{
//				VideoBuffer[i] = new unsigned char[Width/2];
//			}
			unsigned char color = 0;

			for(int j=0;j<Height;j++)
			{
				for(int i=0;i<Width/2;i++)
				{
					VideoBuffer[j*i] = color<<4|(color+1);
					color+=2;
					if(color>=16)color=0;
				}
			}

			 _lengthFrameBuffer = (((Width*2)*(Height/4))+2);
			_frameBuffer = new unsigned char[_lengthFrameBuffer];
    		StartTransmitArrayDmaCircle(_frameBuffer,(char*)&(GPIOC->ODR),_lengthFrameBuffer);
		}
	}

	void TransmitDmaComplete()
	{
		DMA_Cmd(DMA1_Channel2,DISABLE);

//		outputChannelInit.TIM_Pulse = _colorCounter==0?_colorCounter:_colorCounter+5;
//		TIM_OC1Init(TIM3, &outputChannelInit);
//		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
//		asm("nop");
//		asm("nop");
//		asm("nop");
//		asm("nop");
//		asm("nop");
//		asm("nop");
		GPIOA->BSRR=GPIO_Pin_6;

		if(_currentRow>=4)_currentRow=0;

		if(_colorCounter>=15){_colorCounter=0;_currentRow++;}

		_rowCounter = _currentRow;
		_columnCounter =4;
		_coef =15;

		if((_currentRow&(1<<0))>0){GPIOB->BSRR=GPIO_Pin_0;}else{GPIOB->BRR= GPIO_Pin_0;}
		if((_currentRow&(1<<1))>0){GPIOB->BSRR=GPIO_Pin_1;}else{GPIOB->BRR =GPIO_Pin_1;}

			unsigned int count = ((_lengthFrameBuffer)-2);
			unsigned char _counter;
			for(int i=count;i>0;i-=8)
			{
					if(_columnCounter==0)
					{
						_columnCounter=4;
						_rowCounter+=4;
						if(_rowCounter==16)
						{
							_rowCounter=_currentRow;
							_coef-=4;
							if(_coef==0)
							{
								_coef=15;
							}
						};
					}


				_frameBuffer[(i+3)]=4;
				_frameBuffer[(i+2)]=4;
				_frameBuffer[(i+1)]=4;
				_frameBuffer[(i+0)]=4;

				_frameBuffer[(i+0)]|=(1<<CLK_BIT);
				_frameBuffer[(i+1)]&=~(1<<CLK_BIT);

				_frameBuffer[(i+2)]|=(1<<CLK_BIT);
				_frameBuffer[(i+3)]&=~(1<<CLK_BIT);

				_frameBuffer[(i+4)]|=(1<<CLK_BIT);
				_frameBuffer[(i+5)]&=~(1<<CLK_BIT);

				_frameBuffer[(i+6)]|=(1<<CLK_BIT);
				_frameBuffer[(i+7)]&=~(1<<CLK_BIT);

				_frameBuffer[(i+8)]|=(1<<CLK_BIT);
				_frameBuffer[(i+9)]&=~(1<<CLK_BIT);

				_frameBuffer[(i+10)]|=(1<<CLK_BIT);
				_frameBuffer[(i+11)]&=~(1<<CLK_BIT);

				_frameBuffer[(i+12)]|=(1<<CLK_BIT);
				_frameBuffer[(i+13)]&=~(1<<CLK_BIT);

				_frameBuffer[(i+14)]|=(1<<CLK_BIT);
				_frameBuffer[(i+15)]&=~(1<<CLK_BIT);


				pixel.Data =  VideoBuffer[(_rowCounter*Height)+(_columnCounter)];

				if(pixel.P2>=_colorCounter){
					_frameBuffer[(i+0)]&=~(1<<R1_BIT);
					_frameBuffer[(i+1)]&=~(1<<R1_BIT);
				}

				if(pixel.P1>=_colorCounter)
				{
					_frameBuffer[(i+2)]&=~(1<<R1_BIT);
					_frameBuffer[(i+3)]&=~(1<<R1_BIT);
				}

				pixel.Data =  VideoBuffer[(_rowCounter*Height)+(_columnCounter+1)];

				if(pixel.P2>=_colorCounter){
					_frameBuffer[(i+4)]&=~(1<<R1_BIT);
					_frameBuffer[(i+5)]&=~(1<<R1_BIT);
				}

				if(pixel.P1>=_colorCounter)
				{
					_frameBuffer[(i+6)]&=~(1<<R1_BIT);
					_frameBuffer[(i+7)]&=~(1<<R1_BIT);
				}

				pixel.Data =  VideoBuffer[(_rowCounter*Height)+(_columnCounter+2)];

				if(pixel.P2>=_colorCounter){
					_frameBuffer[(i+8)]&=~(1<<R1_BIT);
					_frameBuffer[(i+9)]&=~(1<<R1_BIT);
				}

				if(pixel.P1>=_colorCounter)
				{
					_frameBuffer[(i+10)]&=~(1<<R1_BIT);
					_frameBuffer[(i+11)]&=~(1<<R1_BIT);
				}

				pixel.Data =  VideoBuffer[(_rowCounter*Height)+(_columnCounter+3)];

				if(pixel.P2>=_colorCounter){
					_frameBuffer[(i+12)]&=~(1<<R1_BIT);
					_frameBuffer[(i+13)]&=~(1<<R1_BIT);
				}

				if(pixel.P1>=_colorCounter)
				{
					_frameBuffer[(i+14)]&=~(1<<R1_BIT);
					_frameBuffer[(i+14)]&=~(1<<R1_BIT);
				}


				_counter++;
				_columnCounter-=4;
		}


		_frameBuffer[_lengthFrameBuffer-2] = 2; //latch for row
		_frameBuffer[_lengthFrameBuffer-1] = 0; //---------
		_colorCounter++;
		//_currentRow++;

		GPIOA->BRR= GPIO_Pin_6;

//		outputChannelInit.TIM_Pulse = 0;
//		TIM_OC1Init(TIM3, &outputChannelInit);
//		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

		DMA_Cmd(DMA1_Channel2,ENABLE);
	}



private:
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef timerInitStructure;
	TIM_OCInitTypeDef outputChannelInit;
	unsigned char* _frameBuffer;
	unsigned int _lengthFrameBuffer;

	unsigned char _colorCounter;

	Pixel pixel;

	unsigned char _rowCounter;
	unsigned char _columnCounter;
	unsigned char _currentRow;
	unsigned char _coef;
};


#endif /* DRIVER_SINGLELED_SL_H_ */
