/*
 * matrix.cpp
 *
 *  Created on: 06 сент. 2017 г.
 *      Author: hudienko_a
 */

#include "ledmatrix.h"
//#include "../../StdPeriph/stm_lib/inc/misc.h"

#include "../../Extention/delay.h"
#include "../../Extention/GraphicTextFunctions.h"

namespace Driver
{

LedMatrix *LedMatrix::LMObj = 0;
DMA_InitTypeDef   DMA_InitStructure;


 unsigned short* _mtrix1[4];//[129*4];
 unsigned short* _mtrix2[4];//[129*4];
volatile unsigned char currentRow;
volatile unsigned char scale;
volatile unsigned char _class_scale;
volatile unsigned char _2grayScale;


extern "C"
{
volatile unsigned char width;
volatile unsigned char _height_total;
volatile unsigned char height;
volatile unsigned short _calc_wdth;
volatile unsigned short _fps;


//		void TIM2_IRQHandler()
//		{
//			if (TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
//			{
//				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//				LedMatrix::LMObj->UpdateScreen();
//			}
//		}

		void DMA1_Channel2_IRQHandler()
		{
			if (DMA1->ISR & DMA1_IT_TC2)
			{
				DMA1->IFCR =DMA1_IT_TC2;
				//TIM_Cmd(TIM1, DISABLE);
				TIM1->CR1 &= ~TIM_CR1_CEN;



				//GPIO_ResetBits(PORT_LATCH_ROWS,LATCH_ROWS);
				PORT_LATCH_ROWS->BSRR = LATCH_ROWS;
			//	GPIO_SetBits(PORT_LATCH_ROWS,LATCH_ROWS);
			//	for(int i=0;i<10;i++){__asm("nop");}
				//GPIO_ResetBits(PORT_LATCH_ROWS,LATCH_ROWS);
				PORT_LATCH_ROWS->BSRR = LATCH_ROWS;


				//GPIO_ResetBits(PORT_OE,OE);
				PORT_OE->BRR = OE;



				if(currentRow>=height)
				{
					currentRow=0;
					scale+=1;
				}

				_calc_wdth = width*currentRow;

				if(_class_scale>_2grayScale)
				{
					unsigned char t = scale*20;
					for(int i=0;i<t;i++){__asm("nop");}

					if(scale >=_class_scale)
					{
						scale=0;
						_fps++;
					}

					if(LedMatrix::LMObj->framebuff==1)
					{
						DMA1_Channel2->CMAR = (uint32_t)&_mtrix1[scale][_calc_wdth];
					}
					else
					{
						DMA1_Channel2->CMAR = (uint32_t)&_mtrix2[scale][_calc_wdth];
					}
				}
				else
				{
					for(int i=0;i<20;i++){__asm("nop");}
					if(LedMatrix::LMObj->framebuff==1)
					{
						DMA1_Channel2->CMAR = (uint32_t)&_mtrix1[0][_calc_wdth];
					}
					else
					{
						DMA1_Channel2->CMAR = (uint32_t)&_mtrix2[0][_calc_wdth];
					}
				}

				currentRow++;

				PORT_OE->BSRR = OE;
				//GPIO_ResetBits(PORT_OE,OE);
				//TIM_Cmd(TIM1, ENABLE);
				TIM1->CR1 |= TIM_CR1_CEN;
			}
		}


}



	LedMatrix::LedMatrix(uint8_t width,uint8_t height,MatrixType type, MatrixGrayScale scale)
	{
		_width = width;
		_height = height;
		_type = type;

		_scale = scale;
		_currentRow=1;

		_second_row =false;
		_third_row =false;
		_fourth_row = false;
		framebuff = 1;
	}

	LedMatrix::~LedMatrix(){}



	void LedMatrix::Init()
			{

				LMObj = this;

				width = _width;
				_height_total = _height;
				if(_height<=32)
				{
					height= _height/2;
				}
				else if(height<=64)
				{
					height= _height/4;
				}

				_class_scale = _scale;
				_2grayScale = LedMatrix::MatrixGrayScale::Scale_2;

				for(int i=0;i<_scale;i++)
				{
					_mtrix1[i] = new unsigned short[(height*(_width))];
					_mtrix2[i] = new unsigned short[(height*(_width))];
				}

				_matrixBuffer = new unsigned char[(_height*(_width/2))];

				for(int i=0;i<(_height*(_width/2));i++)
				{
					_matrixBuffer[i]=0;
				}

				RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);

				GPIO_InitStruct.GPIO_Pin = R_1_1|R_1_2|R_1_3|R_1_4| G_1_1|G_1_2|G_1_3|G_1_4;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(PORT_DATA_ROWS, &GPIO_InitStruct);

				GPIO_InitStruct.GPIO_Pin = CLK_ROWS;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
				GPIO_Init(PORT_CLK_ROWS, &GPIO_InitStruct);

				GPIO_InitStruct.GPIO_Pin = LATCH_ROWS;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(PORT_LATCH_ROWS, &GPIO_InitStruct);

				GPIO_InitStruct.GPIO_Pin = A|B|C|D;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(CONTROL_ROWS_PORT, &GPIO_InitStruct);


				GPIO_InitStruct.GPIO_Pin = OE_PWM|OE;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(PORT_OE, &GPIO_InitStruct);


				GPIO_InitStruct.GPIO_Pin = BUFFER_OE|BUFFER_TE;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(BUFFER_PORT, &GPIO_InitStruct);

				GPIO_SetBits(BUFFER_PORT,BUFFER_TE);
				GPIO_ResetBits(BUFFER_PORT,BUFFER_OE);



				RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
				RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

				NVIC_InitTypeDef NVIC_InitStructure;
				TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

				TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
				TIM_TimeBaseStructure.TIM_Prescaler =0; // set 1us resolution
				TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
				TIM_TimeBaseStructure.TIM_Period =100;
				TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
				TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
				TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
				TIM_ARRPreloadConfig( TIM1, ENABLE );

				TIM_GenerateEvent(TIM1,TIM_EventSource_CC1);

				TIM_OCInitTypeDef outputChannelInit;


				 TIM_OCStructInit(&outputChannelInit);
				 outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
				 outputChannelInit.TIM_Pulse = 55;
				 outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
				 outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
				 TIM_OC1Init(TIM1, &outputChannelInit);
				 TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);



					DMA_DeInit(DMA1_Channel5);
					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(GPIOC->ODR);//0x40013804;
					DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_mtrix1[0];
					DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
					DMA_InitStructure.DMA_BufferSize =(_width);
					DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
					DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
					DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
					DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
					DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
					DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
					DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
					DMA_Init(DMA1_Channel2, &DMA_InitStructure);

					TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);
					TIM_DMAConfig (TIM1, TIM_DMABase_CCR1, TIM_DMABurstLength_1Transfer);


					NVIC_InitStructure.NVIC_IRQChannel =DMA1_Channel2_IRQn;
					NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
					NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
					NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
					NVIC_Init( &NVIC_InitStructure );

					DMA_ClearFlag(DMA1_IT_TC2|DMA1_IT_HT2);

					DMA_ITConfig(DMA1_Channel2, DMA_IT_TC|DMA_IT_TE, ENABLE);

					bool t = false;
					char row =1;

					for(int s=0;s<_scale;s++)
					{
						for(int j=0;j<(height);j++)
						{
							 for(int i=0;i<(_width);i++)
							 {
								 *(_mtrix1[s]+((j*((_width)))+i))=j;
								 *(_mtrix2[s]+((j*((_width)))+i))=j;
								 *(_mtrix1[s]+((j*((_width)))+i)) &=~(1<<4);
								 *(_mtrix2[s]+((j*((_width)))+i)) &=~(1<<4);


								 *(_mtrix1[s]+((j*((_width)))+i)) |=(1<<5);
								 *(_mtrix1[s]+((j*((_width)))+i)) |=(1<<6);
								 *(_mtrix2[s]+((j*((_width)))+i)) |=(1<<5);
								 *(_mtrix2[s]+((j*((_width)))+i)) |=(1<<6);

								 *(_mtrix1[s]+((j*((_width)))+i)) |=(1<<9);
								 *(_mtrix1[s]+((j*((_width)))+i)) |=(1<<10);
								 *(_mtrix1[s]+((j*((_width)))+i)) |=(1<<11);
								 *(_mtrix1[s]+((j*((_width)))+i)) |=(1<<12);


							 }
						}
					}


				 TIM_CtrlPWMOutputs(TIM1, ENABLE);
				 DMA_Cmd(DMA1_Channel2,ENABLE);
				 TIM_Cmd(TIM1, ENABLE);

				int range = 0;

			}


	void LedMatrix::FillOutputBuffer()
	{

		if(_type== Scan_1_16)
		{
			unsigned short _width_ = _width/2;
			unsigned short** _active_buffer;

			if(framebuff==2)
			{
				_active_buffer = _mtrix1;
			}
			else
			{
				_active_buffer = _mtrix2;
			}

				for(int s=0;s<_scale;s++)
				{
				for(int i=0;i<_height ;i++)
				{
					unsigned short i__width = i*(_width);
					unsigned short i___width = i*(_width_);
					unsigned short i_16_width = (i-16)*(_width);
					unsigned short i_32_width = (i-32)*(_width);
					unsigned short i_48_width = (i-48)*(_width);

					for(int j=0;j<_width;j+=2)
					{
						unsigned char j_2 = j/2;
						char first = _matrixBuffer[(i___width)+(j_2)]>>4&0x0F;
						char second = _matrixBuffer[(i___width)+(j_2)]&0x0F;


							if(i<16)
							{
									if(first>s)
									{
										_active_buffer[s][((i__width)+j)] &=~(1<<5);
									}
									else
									{
										_active_buffer[s][((i__width)+j)] |=(1<<5);
									}
							}
							else if(i>=16 && i<32)
							{
									if(first>s)
									{
										_active_buffer[s][((i_16_width)+j)] &=~(1<<6);
									}
									else
									{
										_active_buffer[s][((i_16_width)+j)] |=(1<<6);
									}
							}

							else if(i>=32 && i<48)
							{
									if(first>s)
									{
										_active_buffer[s][((i_32_width)+j)] &=~(1<<7);
									}
									else
									{
										_active_buffer[s][((i_32_width)+j)] |=(1<<7);
									}
							}

							else if(i>=48 && i<64)
							{
									if(first>s)
									{
										_active_buffer[s][((i_48_width)+(j))] &=~(1<<8);
									}
									else
									{
										_active_buffer[s][((i_48_width)+(j))] |=(1<<8);
									}
							}



						// i+1
								if(i<16)
								{
										if(second>s)
										{
											_active_buffer[s][((i__width)+(j+1))] &=~(1<<5);
										}
										else
										{
											_active_buffer[s][((i__width)+(j+1))] |=(1<<5);
										}
								}
								else if(i>=16 && i<32)
								{
										if(second>s)
										{
											_active_buffer[s][((i_16_width)+(j+1))] &=~(1<<6);
										}
										else
										{
											_active_buffer[s][((i_16_width)+(j+1))] |=(1<<6);
										}
								}

								else if(i>=32 && i<48)
								{
										if(second>s)
										{
											_active_buffer[s][((i_32_width)+(j+1))] &=~(1<<7);
										}
										else
										{
											_active_buffer[s][((i_32_width)+(j+1))] |=(1<<7);
										}
								}

								else if(i>=48 && i<64)
								{
										if(second>s)
										{
											_active_buffer[s][((i_48_width)+(j+1))] &=~(1<<8);
										}
										else
										{
											_active_buffer[s][((i_48_width)+(j+1))] |=(1<<8);
										}
								}

					 }
				  }
			   }
		}


		if(framebuff==1)
		{
			framebuff=2;
		}
		else
		{
			framebuff=1;
		}

	}


	void LedMatrix::DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
	{
		disp1color_DrawLine(&_matrixBuffer[0],x1,y1,x2,y2);
		//FillOutputBuffer();
	}

	void  LedMatrix::DrawRectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
	{
		disp1color_DrawRectangle(&_matrixBuffer[0],x1,y1,x2,y2);
		//FillOutputBuffer();
	}

	void LedMatrix::DrawCircle(int16_t x0, int16_t y0, int16_t radius)
	{
		disp1color_DrawCircle(&_matrixBuffer[0],x0,y0,radius);
		//FillOutputBuffer();
	}

	void LedMatrix::DrawString(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, uint8_t color)
	{
		disp1color_DrawString(&_matrixBuffer[0],X,Y,FontID,Str,color);
		//FillOutputBuffer();
	}


	void LedMatrix::DrawString(int16_t X, int16_t Y, uint8_t FontID, uint8_t *Str, uint8_t len, uint8_t color)
	{
		disp1color_DrawString(&_matrixBuffer[0],X,Y,FontID,Str,len,color);
		//FillOutputBuffer();
	}

	void LedMatrix::SetPixel(int x, int y, char data, char color)
	{
		disp1color_DrawPixel(&_matrixBuffer[0],x,y,data,color);
	}

}
