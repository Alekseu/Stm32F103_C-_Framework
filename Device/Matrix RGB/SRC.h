/*
 * SRC.h
 *
 *  Created on: 05 мая 2018 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_MATRIX_RGB_SRC_H_
#define DEVICE_MATRIX_RGB_SRC_H_

///*
// * main.cpp
// *
// *  Created on: 09 марта 2017 г.
// *      Author: Alekseu
// */
//
//#include "main.h"
//
//#include "StdPeriph/cmsis_boot/stm32f10x.h"
//#include "StdPeriph/stm_lib/inc/misc.h"
//#include "StdPeriph/stm_lib/inc/stm32f10x_tim.h"
//#include "StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
//#include "StdPeriph/stm_lib/inc/stm32f10x_dma.h"
//#include "StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
//
//#include "Pixel.h"
//#include "GraphicTextFunctions.h"
//#include "Driver/usb/usb.h"
//
//#define OE GPIO_Pin_2
//#define OE_PORT GPIOA
//
//#define CLK GPIO_Pin_3
//#define CLK_PORT GPIOA
//
//#define LAT GPIO_Pin_2
//#define LAT_PORT GPIOB
//
//// Row1
//#define R1 GPIO_Pin_0
//#define G1 GPIO_Pin_1
//#define B1 GPIO_Pin_2
//
//#define R2 GPIO_Pin_3
//#define G2 GPIO_Pin_4
//#define B2 GPIO_Pin_5
//
////row2
//#define R22 GPIO_Pin_6
//#define G22 GPIO_Pin_7
//#define B22 GPIO_Pin_8
//
//#define R23 GPIO_Pin_9
//#define G23 GPIO_Pin_10
//#define B23 GPIO_Pin_11
//
//#define Port GPIOC
//
//
//#define A GPIO_Pin_5
//#define B GPIO_Pin_6
//#define C GPIO_Pin_4 //not used
//#define C GPIO_Pin_3 //not used
//
//#define C_Port GPIOE
//
//unsigned char current_row;
//unsigned int current_transfer_count;
//unsigned char _frame_index;
//
//
// unsigned short* _mtrix[SCAN_TYPE];
// unsigned short* _mtrix1[SCAN_TYPE];
//
// unsigned short _buff1[FRAME_BUFFER_LENGTH];
// unsigned short _buff2[FRAME_BUFFER_LENGTH];
// unsigned short _buff3[FRAME_BUFFER_LENGTH];
// unsigned short _buff4[FRAME_BUFFER_LENGTH];
//
// unsigned short _buff11[FRAME_BUFFER_LENGTH];
// unsigned short _buff22[FRAME_BUFFER_LENGTH];
// unsigned short _buff33[FRAME_BUFFER_LENGTH];
// unsigned short _buff44[FRAME_BUFFER_LENGTH];
//
//TIM_OCInitTypeDef outputChannelInit;
//DMA_InitTypeDef   DMA_InitStructure;
//
//Pixel MatrixBuffer[MATRIX_WIDTH*MATRIX_HEIGHT];
//
//unsigned char _MatrixWidth = MATRIX_WIDTH;
//unsigned char _MatrixHeight = MATRIX_HEIGHT;
//
//void Fill_Frame_Buffer_From_Matrix(unsigned char _current_frameBuffer);
//
//extern "C"
//{
//	void DMA1_Channel7_IRQHandler(void) //tx
//	{
//		if (DMA1->ISR & DMA1_IT_TC7)
//		{
//			DMA1->IFCR =DMA1_IT_TC7;
//
//			TIM2->CR1 &=~TIM_CR1_CEN;
//
//			LAT_PORT->BSRR=LAT;
//			for(int i=0;i<15;i++)
//			{
//				asm("nop");
//			}
//			LAT_PORT->BRR= LAT;
//
//
//			//OE_PORT->BSRR= OE;
//			OE_PORT->BRR= OE;
//			for(int i=0;i<50;i++)
//			{
//				asm("nop");
//			}
//
//			if(current_transfer_count >= (_FRAME_LENGTH*GRAY_SCALE))
//			{
//				current_transfer_count=0;
//				current_row++;
//				if(current_row==4)
//				{
//					current_row=0;
//				}
//			}
//			if(_frame_index==1)
//			{
//				DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_mtrix[current_row][current_transfer_count];
//				DMA_Init(DMA1_Channel7, &DMA_InitStructure);
//			}
//			else if(_frame_index==2)
//			{
//				DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_mtrix1[current_row][current_transfer_count];
//				DMA_Init(DMA1_Channel7, &DMA_InitStructure);
//			}
//
//			if((current_row&(1<<0))>0){C_Port->BSRR= A;}else{C_Port->BRR= A;}
//			if((current_row&(1<<1))>0){C_Port->BSRR= B;}else{C_Port->BRR= B;}
//
//			current_transfer_count +=_FRAME_LENGTH;
//
//
//			//OE_PORT->BRR= OE;
//			OE_PORT->BSRR= OE;
//
//			TIM2->CR1 |= TIM_CR1_CEN;
//
//
//			return;
//		}
//	}
//}
//
//Usb com;
//
//
//void Overclocking(void) // Разгон микроконтроллера.
//{
//
//  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); // Выбираем источником такторования
//  RCC_PLLCmd(DISABLE); // Выключаем умножитель.
//  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_CFGR_PLLMULL16); // На сколько будем умножать частоту.
//  RCC_PLLCmd(ENABLE); // Включаем умножитель.
//  while ((RCC->CR & RCC_CR_PLLRDY) == 0);     // Ждем запуска умножителя.
//  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Выбираем источником тактирования умножитель.
//
//  SystemCoreClockUpdate(); // Вычисление тактовой частоты ядра.
//}
//
//void init_gpio()
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);
//
//		GPIO_InitTypeDef  GPIO_InitStructure;
//		GPIO_InitStructure.GPIO_Pin =  R1|G1|B1|R2|G2|B2|R22|G22|B22|R23|G23|B23;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(Port, &GPIO_InitStructure);
//
//		GPIO_ResetBits(Port,R1);
//		GPIO_ResetBits(Port,G1);
//		GPIO_ResetBits(Port,B1);
//
//		GPIO_ResetBits(Port,R2);
//		GPIO_ResetBits(Port,G2);
//		GPIO_ResetBits(Port,B2);
//
//		GPIO_InitStructure.GPIO_Pin =  OE;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(OE_PORT, &GPIO_InitStructure);
//
//		GPIO_InitStructure.GPIO_Pin =  CLK;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_Init(CLK_PORT, &GPIO_InitStructure);
//
//		GPIO_InitStructure.GPIO_Pin =  LAT;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(LAT_PORT, &GPIO_InitStructure);
//
//
//		GPIO_InitStructure.GPIO_Pin =  A|B|C;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
//		GPIO_Init(C_Port, &GPIO_InitStructure);
//
//		GPIO_ResetBits(C_Port,A);
//		GPIO_ResetBits(C_Port,B);
//		GPIO_ResetBits(C_Port,C);
//
//		OE_PORT->BRR= OE;
//}
//
//void init_timer_dma()
//{
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);
//
//	TIM_TimeBaseInitTypeDef timerInitStructure;
//
//
//	TIM_TimeBaseStructInit(&timerInitStructure);
//	timerInitStructure.TIM_Prescaler =0; // set 1us resolution
//	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	timerInitStructure.TIM_Period =100;
//	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	timerInitStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM2, &timerInitStructure);
//
//	TIM_GenerateEvent(TIM2,TIM_EventSource_CC4);
//
//
//	outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
//	outputChannelInit.TIM_Pulse = 50;
//	outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
//	outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
//
//	TIM_OC4Init(TIM2, &outputChannelInit);
//	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
//	TIM_CtrlPWMOutputs(TIM2, ENABLE);
//
//
//	DMA_DeInit(DMA1_Channel7);
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(Port->ODR);//0x40013804;
//	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_mtrix[0][0];
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//	DMA_InitStructure.DMA_BufferSize =_FRAME_LENGTH;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
//
//	TIM_DMACmd(TIM2, TIM_DMA_CC4, ENABLE);
//	TIM_DMAConfig (TIM2, TIM_DMABase_CCR4, TIM_DMABurstLength_1Transfer);
//
//
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel =DMA1_Channel7_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init( &NVIC_InitStructure );
//
//	DMA_ClearFlag(DMA1_IT_TC7|DMA1_IT_HT7);
//
//	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC|DMA_IT_TE, ENABLE);
//
//}
//
//int main()
//{
//
//	//Overclocking();
//	 _MatrixWidth = MATRIX_WIDTH;
//	 _MatrixHeight = MATRIX_HEIGHT;
//	com.RxBufferSize = 64;
//	com.TxBufferSize = 64;
//	com.TypeUsb = VirtualComPort;
//
//	com.Init();
//
//	init_gpio();
//
//
//	current_row=0;
//	_frame_index =1;
//	current_transfer_count=0;
//
//	memset(_buff1,0,FRAME_BUFFER_LENGTH);
//	memset(_buff2,0,FRAME_BUFFER_LENGTH);
//	memset(_buff3,0,FRAME_BUFFER_LENGTH);
//	memset(_buff4,0,FRAME_BUFFER_LENGTH);
//	memset(_buff11,0,FRAME_BUFFER_LENGTH);
//	memset(_buff22,0,FRAME_BUFFER_LENGTH);
//	memset(_buff33,0,FRAME_BUFFER_LENGTH);
//	memset(_buff44,0,FRAME_BUFFER_LENGTH);
//
//	_mtrix[0] = _buff1;
//	_mtrix[1] = _buff2;
//	_mtrix[2] = _buff3;
//	_mtrix[3] = _buff4;
//
//	_mtrix1[0] = _buff11;
//	_mtrix1[1] = _buff22;
//	_mtrix1[2] = _buff33;
//	_mtrix1[3] = _buff44;
//
//	init_timer_dma();
//
//	// start matrix
//	DMA_Cmd(DMA1_Channel7,ENABLE);
//	TIM_Cmd(TIM2, ENABLE);
//
//
//	disp1color_Fill_Color(&MatrixBuffer[0],255);
//	Fill_Frame_Buffer_From_Matrix(1);
//	Fill_Frame_Buffer_From_Matrix(2);
//
//	unsigned char _data[65];
//	unsigned int _y =0;
//	unsigned int _x =0;
//	bool _complete=false;
//	while(1){
//
//
//		int count = com.ReadData(_data);
//		if(count>0)
//		{
//			if(_data[count-1]==0xAA )
//			{
//				_complete = false;
//				_y =0;
//				_x=0;
//			}
//
//			if(((_data[count-1]==0xBB||_data[count]==0xBB )|| _y>=MATRIX_HEIGHT) )
//			{
//				_complete = true;
//
//			}
//			else
//			{
//				uint16_t* mass = (uint16_t*)_data;
//
//				for(int i=0;i<(count/2);i++)
//				{
//					MatrixBuffer[(_x*MATRIX_HEIGHT)+(_y)].Data = mass[i];
//					_x++;
//					if(_x==MATRIX_WIDTH)
//					{
//						_x=0;
//						_y++;
//					}
//				}
//			}
//			memset(_data,0,64);
//
//		}
//		if(_complete && _y>=MATRIX_HEIGHT)
//		{
//			_y=0;
//			if(_frame_index == 1)
//			{
//				Fill_Frame_Buffer_From_Matrix(_frame_index);
//				_frame_index=2;
//			}
//			else if(_frame_index == 2)
//			{
//				Fill_Frame_Buffer_From_Matrix(_frame_index);
//				_frame_index =1;
//			}
//			_complete = false;
//
//		}
//
//	};
//
//	return 0;
//}
//
//
//void Fill_Matrix_Buffer()
//{
//
//}
//
//void Fill_Frame_Buffer_From_Matrix(unsigned char _current_frameBuffer)
//{
//	unsigned int x =0;
//	unsigned int y =0;
//
//	if(_current_frameBuffer==2)
//	{
//		memset(_buff1,0,FRAME_BUFFER_LENGTH);
//		memset(_buff2,0,FRAME_BUFFER_LENGTH);
//		memset(_buff3,0,FRAME_BUFFER_LENGTH);
//		memset(_buff4,0,FRAME_BUFFER_LENGTH);
//	}
//	if(_current_frameBuffer==1)
//	{
//		memset(_buff11,0,FRAME_BUFFER_LENGTH);
//		memset(_buff22,0,FRAME_BUFFER_LENGTH);
//		memset(_buff33,0,FRAME_BUFFER_LENGTH);
//		memset(_buff44,0,FRAME_BUFFER_LENGTH);
//	}
//
//	for(int i=0;i<4;i++)					// (Y)
//	{
//		x=0;
//		while((x) < MATRIX_WIDTH)				// (X)
//		{
//				Pixel num;
//				Pixel num1;
//				Pixel num2;
//				Pixel num3;
//
//				num =  MatrixBuffer[(x*MATRIX_HEIGHT)+(i+4)];
//				num1 = MatrixBuffer[(x*MATRIX_HEIGHT)+(i)];
//				num2 = MatrixBuffer[(x*MATRIX_HEIGHT)+(i+12)];
//				num3 = MatrixBuffer[(x*MATRIX_HEIGHT)+(i+8)];
//
//				if(_MatrixHeight>16)
//				{
//
//				}
//
////				num.Red>>=1;
////				num1.Red>>=1;
////				num2.Red>>=1;
////				num3.Red>>=1;
//
//				int frame_column=0;
//
//				if(_MatrixWidth<=32)
//				{
//					if(x<8)
//					{
//						frame_column=x;
//					}
//					else if(x>=8 && x<=15)
//					{
//						frame_column=(x-8)+16;
//					}
//					else if(x>=16 && x<=23)
//					{
//						frame_column=(x-16)+32;
//					}
//					else if(x>=24 && x<=31)
//					{
//						frame_column=(x-24)+48;
//					}
//				}
//				else if(_MatrixWidth>32)
//				{
//					if(x<8)
//					{
//						frame_column=x;
//					}
//					else if(x>=8 && x<=15)
//					{
//						frame_column=(x-8)+16;
//					}
//					else if(x>=16 && x<=23)
//					{
//						frame_column=(x-16)+32;
//					}
//					else if(x>=24 && x<=31)
//					{
//						frame_column=(x-24)+48;
//					}
//					else if(x>=32 && x<=39)
//					{
//						frame_column=(x-32)+64;
//					}
//					else if(x>=40 && x<=47)
//					{
//						frame_column=(x-40)+80;
//					}
//					else if(x>=48 && x<=55)
//					{
//						frame_column=(x-48)+96;
//					}
//					else if(x>=56 && x<=63)
//					{
//						frame_column=(x-56)+112;
//					}
//
//				}
//
//				if(i==0){y=0;}
//				if(i==1){y=2;}
//				if(i==2){y=1;}
//				if(i==3){y=3;}
//				if(i==4){y=0;}
//
//					for(int s=1;s<GRAY_SCALE;s++)
//					{
//						if(_current_frameBuffer==2)
//						{
//							if(num.Red>s)  {_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<0);}else{_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<0);}
//							if(num.Green>s){_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<1);}else{_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<1);}
//							if(num.Blue>s) {_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<2);}else{_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<2);}
//
//							if(num1.Red>s)  {_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<0);}else{_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<0);}
//							if(num1.Green>s){_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<1);}else{_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<1);}
//							if(num1.Blue>s) {_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<2);}else{_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<2);}
//
//							if(num2.Red>s)  {_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<3);}else{_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<3);}
//							if(num2.Green>s){_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<4);}else{_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<4);}
//							if(num2.Blue>s) {_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<5);}else{_mtrix[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<5);}
//
//							if(num3.Red>s)  {_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<3);}else{_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<3);}
//							if(num3.Green>s){_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<4);}else{_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<4);}
//							if(num3.Blue>s) {_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<5);}else{_mtrix[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<5);}
//
//							if(_MatrixHeight>16)
//							{
//
//							}
//
//						}
//						else if(_current_frameBuffer==1)
//						{
//							if(num.Red>s)  {_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<0);}else{_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<0);}
//							if(num.Green>s){_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<1);}else{_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<1);}
//							if(num.Blue>s) {_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<2);}else{_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<2);}
//
//							if(num1.Red>s)  {_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<0);}else{_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<0);}
//							if(num1.Green>s){_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<1);}else{_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<1);}
//							if(num1.Blue>s) {_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<2);}else{_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<2);}
//
//							if(num2.Red>s)  {_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<3);}else{_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<3);}
//							if(num2.Green>s){_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<4);}else{_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<4);}
//							if(num2.Blue>s) {_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)] |=(1<<5);}else{_mtrix1[y][(frame_column)+(s*_FRAME_LENGTH)]&=~(1<<5);}
//
//							if(num3.Red>s)  {_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<3);}else{_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<3);}
//							if(num3.Green>s){_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<4);}else{_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<4);}
//							if(num3.Blue>s) {_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)] |=(1<<5);}else{_mtrix1[y][(frame_column+8)+(s*_FRAME_LENGTH)]&=~(1<<5);}
//						}
//					}
//
//			x++;
//			}
//
//	}
//}
//
//




#endif /* DEVICE_MATRIX_RGB_SRC_H_ */
