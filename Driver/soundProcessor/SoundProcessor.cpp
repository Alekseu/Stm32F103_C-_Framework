///*
// * SoundProcessor.cpp
// *
// *  Created on: 18 мая 2017 г.
// *      Author: hudienko_a
// */
//
//#include "SoundProcessor.h"
//
//#include "../Extention/delay.h"
//
//SoundProcessor *SoundProcessor::sObj = 0;
//
//#define _44KHZ 22
//#define _22KHZ 45
//#define _16KHZ 62
//#define _11KHZ 90
//#define _8KHZ 125
//
//
//extern "C"
//{
//	void DMA2_Channel3_IRQHandler(void)
//	{
//		if(DMA2->ISR & DMA2_IT_HT3)
//		{
//			DMA2->IFCR =DMA2_IT_HT3;
//			SoundProcessor::sObj->Half_Play_Complete();
//			return;
//		}
//		if (DMA2->ISR & DMA2_IT_TC3)
//		{
//			DMA2->IFCR =DMA2_IT_TC3;
//			SoundProcessor::sObj->Play_Complete();
//			return;
//		}
//	}
//
//	void DMA1_Channel1_IRQHandler(void)
//		{
//			if(DMA1->ISR & DMA1_IT_HT1)
//			{
//				DMA1->IFCR =DMA1_IT_HT1;
//				SoundProcessor::sObj->Half_Record_Complete();
//				return;
//			}
//			if (DMA1->ISR & DMA1_IT_TC1)
//			{
//				DMA1->IFCR =DMA1_IT_TC1;
//				SoundProcessor::sObj->Record_Complete();
//				return;
//			}
//		}
//
////	void TIM3_IRQHandler(void){
////		if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
////		{
////			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
////
////			//SoundProcessor::sObj->NextSample_event();
////			TIM_ClearITPendingBit(TIM3, TIM_IT_Trigger);
////		}
////	}
////	void TIM6_IRQHandler(void) {
////	  TIM6->SR &= ~TIM_SR_UIF; //Сбрасываем флаг UIF
////	//  SoundProcessor::sObj->NextSample_event();
////
////	}
////
////	void TIM8_UP_IRQHandler(void) {
////		  TIM8->SR &= ~TIM_SR_UIF; //Сбрасываем флаг UIF
////		//  SoundProcessor::sObj->NextSample_event();
////
////		}
//
//}
//
//
//SoundProcessor::SoundProcessor()
//{
//	_count =0;
//	_playcount=0;
//	_aru =15;
//	SoundLoopBack = false;
//	_file_play =false;
//	SoundQality = Sample_22KHZ;
//	bw=0;
//	br=0;
//	_data_count =0;
//	res = FR_INT_ERR;
//}
//
//void SoundProcessor::Init()
//{
//	sObj = this;
//	f_mount(0, &fs);
//
//	//without dma
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//////	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
////
////	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
////	GPIO_Init(GPIOB, &GPIO_InitStructure);
////
////	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
////	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
////	TIM_TimeBaseStructure.TIM_Period = _22KHZ;
////	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
////	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
////	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
////
////	//TIM_Cmd(TIM4, ENABLE);
////
////	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
////	TIM_TimeBaseStructure.TIM_Prescaler = 0;
////	TIM_TimeBaseStructure.TIM_Period = 250;
////	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
////	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
////	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
////
////	TIM_OCStructInit(&timerPWM);
////	timerPWM.TIM_Pulse = 0 ;
////	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
////	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
////	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
////	TIM_OC1Init(TIM4, &timerPWM);
////	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
////	TIM_Cmd(TIM4, ENABLE);
////
////	//TIM_GenerateEvent(TIM4,TIM_EventSource_CC1);
////	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
////	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
////	NVIC_EnableIRQ(TIM3_IRQn);
////	TIM_Cmd(TIM3, ENABLE);
//// tim 6 with dac without dma
////	  TIM6->PSC = 0;
////	  TIM6->ARR = 500;
////	  TIM6->DIER |= TIM_DIER_UIE; //разрешаем прерывание от таймера
////	  TIM6->CR1 |= TIM_CR1_CEN; // Начать отсчёт!
////	  NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
////
////	  /* Включить DAC1 */
////	  DAC->CR |= DAC_CR_EN1;
//
//
//	//dma
//
//
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC | RCC_APB1Periph_TIM6, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseStructure.TIM_Period = 72-1;   //
//	switch(SoundQality)
//	{
//		case Sample_8KHZ:
//			TIM_TimeBaseStructure.TIM_Prescaler = _8KHZ;
//		break;
//		case Sample_11KHZ:
//			TIM_TimeBaseStructure.TIM_Prescaler = _11KHZ;
//		break;
//		case Sample_16KHZ:
//					TIM_TimeBaseStructure.TIM_Prescaler = _16KHZ;
//				break;
//		case Sample_22KHZ:
//			TIM_TimeBaseStructure.TIM_Prescaler = _22KHZ;
//		break;
//		case Sample_44KHZ:
//			TIM_TimeBaseStructure.TIM_Prescaler = _44KHZ;
//		break;
//	}
//
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
//
//	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
//
//	DMA_DeInit(DMA2_Channel3);
//	DMA_InitStructure_DAC.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR8R1);       // (uint32_t) & DAC->DHR12R1
//	DMA_InitStructure_DAC.DMA_MemoryBaseAddr = (uint32_t)&_buffer1[0];                 // sinus_12bit
//	DMA_InitStructure_DAC.DMA_DIR = DMA_DIR_PeripheralDST;
//	DMA_InitStructure_DAC.DMA_BufferSize = BUFFER_SIZE;
//	DMA_InitStructure_DAC.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure_DAC.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure_DAC.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   // DMA_PeripheralDataSize_HalfWord
//	DMA_InitStructure_DAC.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           // DMA_MemoryDataSize_HalfWord
//	DMA_InitStructure_DAC.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure_DAC.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure_DAC.DMA_M2M = DMA_M2M_Disable;
//	DMA_Init(DMA2_Channel3, &DMA_InitStructure_DAC);
//
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel =DMA2_Channel3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init( &NVIC_InitStructure );
//
//	DMA_ClearFlag(DMA2_IT_TC3|DMA2_IT_HT3);
//
//	DMA_ITConfig(DMA2_Channel3, DMA_IT_TC|DMA_IT_TE, ENABLE);
//
//	DMA_Cmd(DMA2_Channel3, ENABLE);
//
//	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
//	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;        // DAC_OutputBuffer_Disable;
//	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits8_0;
//	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
//
//	DAC_DMACmd(DAC_Channel_1, ENABLE);
//	DAC_Cmd(DAC_Channel_1, ENABLE);
//
//	TIM_Cmd(TIM6, DISABLE);
//
//
////	//record config
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
////
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
////	GPIO_Init(GPIOC, &GPIO_InitStructure);
////
////	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
////	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
////	TIM_TimeBaseStructure.TIM_Period = 72-1;   //
////	switch(SoundQality)
////		{
////			case Sample_8KHZ:
////				TIM_TimeBaseStructure.TIM_Prescaler = _8KHZ;
////			break;
////			case Sample_11KHZ:
////				TIM_TimeBaseStructure.TIM_Prescaler = _11KHZ;
////			break;
////			case Sample_16KHZ:
////				TIM_TimeBaseStructure.TIM_Prescaler = _16KHZ;
////				break;
////			case Sample_22KHZ:
////				TIM_TimeBaseStructure.TIM_Prescaler = _22KHZ;
////			break;
////			case Sample_44KHZ:
////				TIM_TimeBaseStructure.TIM_Prescaler = _44KHZ;
////			break;
////		}
////	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
////	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
////	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
////
////	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
////
////	DMA_DeInit(DMA1_Channel1);
////	DMA_InitStructure_ADC.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//ADC1_DR_Address;
////	DMA_InitStructure_ADC.DMA_MemoryBaseAddr = (uint32_t)&_buffer3[0];
////	DMA_InitStructure_ADC.DMA_DIR = DMA_DIR_PeripheralSRC;
////	DMA_InitStructure_ADC.DMA_BufferSize = BUFFER_SIZE;
////	DMA_InitStructure_ADC.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
////	DMA_InitStructure_ADC.DMA_MemoryInc = DMA_MemoryInc_Enable;
////	DMA_InitStructure_ADC.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
////	DMA_InitStructure_ADC.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
////	DMA_InitStructure_ADC.DMA_Mode = DMA_Mode_Circular;
////	DMA_InitStructure_ADC.DMA_Priority = DMA_Priority_High;
////	DMA_InitStructure_ADC.DMA_M2M = DMA_M2M_Disable;
////	DMA_Init(DMA1_Channel1, &DMA_InitStructure_ADC);
////
////
////	NVIC_InitStructure.NVIC_IRQChannel =DMA1_Channel1_IRQn;
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////	NVIC_Init( &NVIC_InitStructure );
////
////	DMA_ClearFlag(DMA1_IT_TC1|DMA1_IT_HT1);
////
////	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC|DMA_IT_TE, ENABLE);
////
////	DMA_Cmd(DMA1_Channel1, ENABLE);
////
////
////	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
////	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
////	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
////	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
////	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
////	ADC_InitStructure.ADC_NbrOfChannel = 15;
////
////	ADC_Init(ADC1, &ADC_InitStructure);
////
////	/* ADC1 regular channel14 configuration */
////	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_7Cycles5);
////
////	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
////
////	/* Enable ADC1 DMA */
////	ADC_DMACmd(ADC1, ENABLE);
//
//}
//
//void SoundProcessor::Play()
//{
//	 _count =0;
//	 DAC_Cmd(DAC_Channel_1, ENABLE);
//	 DMA_Cmd(DMA2_Channel3, ENABLE);
//	 TIM_Cmd(TIM6, ENABLE);
//}
//
//void SoundProcessor::Play(unsigned char* filename)
//{
//	Stop();
//	_file_play = true;
//	char file[64];
//		memset(file,0,64);
//		memcpy(file,&filename[0],strlen((const char*)filename));
//		res =f_open(&fp,file,FA_READ);
//		if(res!=FR_OK)
//		{
//			res =f_open(&fp,file,FA_READ);
//			if(res==FR_OK)
//			{
//				res = f_lseek(&fp,44);
//				f_read(&fp,_buffer1,BUFFER_SIZE,&br);
//				DAC_Cmd(DAC_Channel_1, ENABLE);
//				DMA_Cmd(DMA2_Channel3, ENABLE);
//				TIM_Cmd(TIM6, ENABLE);
//			}
//		}else
//		{
//			res = f_lseek(&fp,44);
//
//				  f_read(&fp,_buffer1,BUFFER_SIZE,&br);
//
//			_dma_init_size(BUFFER_SIZE,true);
//
//			 NowPlay = true;
//			 DAC_Cmd(DAC_Channel_1, ENABLE);
//			 DMA_Cmd(DMA2_Channel3, ENABLE);
//			 TIM_Cmd(TIM6, ENABLE);
//		}
//}
//
//void SoundProcessor::Play_half(unsigned char* filename)
//{
//	_file_play = true;
//		char file[64];
//			memset(file,0,64);
//			memcpy(file,&filename[0],strlen((const char*)filename));
//			res =f_open(&fp,file,FA_READ);
//			if(res!=FR_OK)
//			{
//				res =f_open(&fp,file,FA_READ);
//				if(res==FR_OK)
//				{
//					res = f_lseek(&fp,44);
//					f_read(&fp,_buffer1,BUFFER_SIZE,&br);
//					DAC_Cmd(DAC_Channel_1, ENABLE);
//					DMA_Cmd(DMA2_Channel3, ENABLE);
//					TIM_Cmd(TIM6, ENABLE);
//				}
//			}else
//			{
//				res = f_lseek(&fp,44);
//					if(fp.fsize<BUFFER_SIZE)
//					{
//					  f_read(&fp,_buffer1,((fp.fsize-44)-113),&br);
//					  _hapf_buffer_play = true;
//					  _dma_init_size((fp.fsize-44),false);
//					  _file_play = true;
//					}
//					else
//					{
//						 f_read(&fp,_buffer1,BUFFER_SIZE,&br);
//						 _dma_init_size(BUFFER_SIZE,true);
//						 _file_play = true;
//					}
//
//
//				 NowPlay = true;
//				 DAC_Cmd(DAC_Channel_1, ENABLE);
//				 DMA_Cmd(DMA2_Channel3, ENABLE);
//				 TIM_Cmd(TIM6, ENABLE);
//			}
//}
//
//void SoundProcessor::Play_Buffer(char* buff, unsigned int size)
//{
//	//_playcount = size;
//	//_play_buf_count+=
//
//	if(size> BUFFER_SIZE)
//	{
//		return;
//	}
//
//	memcpy(_buffer1,buff,size);
//
//	_dma_init_size(size,false);
//
//	TIM_Cmd(TIM6, ENABLE);
//
//}
//
//void SoundProcessor::Half_Play_Complete()
//{
//	if(_hapf_buffer_play)
//	{
//
//	}
//	else
//	{
//		if(!SoundLoopBack)
//		{
//			if(_file_play)
//			{
//				f_read(&fp,&_buffer1[0],HALF_BUFFER_SIZE,&br);//то читаем в первую половину буфера инфу с флешки
//				if (br < HALF_BUFFER_SIZE) //если буфер не содержит 256 значений значит конец файла
//				{
//					Stop();
//					NowPlay = false;
//				}
//			}
//			else
//			{
//				_count +=HALF_BUFFER_SIZE;
//			}
//		}
//		else
//		{
//			memcpy(&_buffer1[0],&_buffer2[0],HALF_BUFFER_SIZE);
//		}
//	}
//
//}
//
//void SoundProcessor::Play_Complete()
//{
//	if(_hapf_buffer_play)
//	{
//		Stop();
//		_hapf_buffer_play = false;
//		NowPlay = false;
//	}
//	else
//	{
//	if(!SoundLoopBack)
//	{
//		if(_file_play)
//		{
//			f_read(&fp,&_buffer1[HALF_BUFFER_SIZE],HALF_BUFFER_SIZE,&br);
//			if (br < HALF_BUFFER_SIZE) //конец файла
//			{
//				Stop();
//				NowPlay = false;
//			}
//		}
//		else
//		{
//			_count=0;
//		}
//	}
//	else
//	{
//	   memcpy(&_buffer1[HALF_BUFFER_SIZE],&_buffer2[HALF_BUFFER_SIZE],HALF_BUFFER_SIZE);
//	}
//	}
//
//}
//
//void SoundProcessor::Half_Record_Complete()
//{
//	for(int i=0;i<HALF_BUFFER_SIZE;i++)
//	{
//	  _buffer2[i] = (unsigned char)(_buffer3[i]/_aru);
//	}
//}
//
//void SoundProcessor::Record_Complete()
//{
//	for(int i=HALF_BUFFER_SIZE;i<BUFFER_SIZE;i++)
//	{
//	  _buffer2[i] = (unsigned char)(_buffer3[i]/_aru);;
//	}
//}
//
//void SoundProcessor::StartRecording()
//{
//	ADC_Cmd(ADC1, ENABLE);
//	DMA_Cmd(DMA1_Channel1, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
//
//	ADC_ResetCalibration(ADC1);
//	/* Check the end of ADC1 reset calibration register */
//	while(ADC_GetResetCalibrationStatus(ADC1));
//
//	/* Start ADC1 calibration */
//	ADC_StartCalibration(ADC1);
//	/* Check the end of ADC1 calibration */
//	while(ADC_GetCalibrationStatus(ADC1));
//
//	/* Start ADC1 Software Conversion */
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//}
//
//void SoundProcessor::StopRecording()
//{
//	 ADC_Cmd(ADC1, DISABLE);
//	 DMA_Cmd(DMA1_Channel1, DISABLE);
//	 TIM_Cmd(TIM3, DISABLE);
//}
//
//void SoundProcessor::Stop()
//{
//	DAC_Cmd(DAC_Channel_1, DISABLE);
//	DMA_Cmd(DMA2_Channel3, DISABLE);
//	TIM_Cmd(TIM6, DISABLE);
//	if(_file_play)
//	{
//		_file_play =false;
//		f_close(&fp);
//	}
//
//}
//
//
//void  SoundProcessor::_dma_init_size(unsigned int size, bool circular)
//{
//	DMA_DeInit(DMA2_Channel3);
//		DMA_InitStructure_DAC.DMA_PeripheralBaseAddr = (uint32_t)&(DAC->DHR8R1);       // (uint32_t) & DAC->DHR12R1
//		DMA_InitStructure_DAC.DMA_MemoryBaseAddr = (uint32_t)&_buffer1[0];                 // sinus_12bit
//		DMA_InitStructure_DAC.DMA_DIR = DMA_DIR_PeripheralDST;
//		DMA_InitStructure_DAC.DMA_BufferSize = size;
//		DMA_InitStructure_DAC.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//		DMA_InitStructure_DAC.DMA_MemoryInc = DMA_MemoryInc_Enable;
//		DMA_InitStructure_DAC.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   // DMA_PeripheralDataSize_HalfWord
//		DMA_InitStructure_DAC.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;           // DMA_MemoryDataSize_HalfWord
//		if(circular)
//		{
//		DMA_InitStructure_DAC.DMA_Mode = DMA_Mode_Circular;
//		}
//		else
//		{
//			DMA_InitStructure_DAC.DMA_Mode = DMA_Mode_Normal;
//		}
//		DMA_InitStructure_DAC.DMA_Priority = DMA_Priority_High;
//		DMA_InitStructure_DAC.DMA_M2M = DMA_M2M_Disable;
//		DMA_Init(DMA2_Channel3, &DMA_InitStructure_DAC);
//
//		NVIC_InitTypeDef NVIC_InitStructure;
//			NVIC_InitStructure.NVIC_IRQChannel =DMA2_Channel3_IRQn;
//			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//			NVIC_Init( &NVIC_InitStructure );
//
//			DMA_ClearFlag(DMA2_IT_TC3|DMA2_IT_HT3);
//
//			DMA_ITConfig(DMA2_Channel3, DMA_IT_TC|DMA_IT_TE, ENABLE);
//
//			DMA_Cmd(DMA2_Channel3, ENABLE);
//
//			DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
//			DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//			DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;        // DAC_OutputBuffer_Disable;
//			DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits8_0;
//			DAC_Init(DAC_Channel_1, &DAC_InitStructure);
//
//			DAC_DMACmd(DAC_Channel_1, ENABLE);
//			DAC_Cmd(DAC_Channel_1, ENABLE);
//
//			TIM_Cmd(TIM6, DISABLE);
//}
//
////without dma
////void SoundProcessor::NextSample_event()
////{
////	if(play)
////	{
////		//with 2 timers
////		//TIM4->CCR1 = _buffer1[count];
////
////		//with dac without dma
////		//DAC->DHR12R1=_buffer1[count];
////
////
////		if(++count>=8192)
////		{
////			count=0;
////			//play = false;
////		}
////	}
////}
////
////void SoundProcessor::Periodic()
////{
////
//////	if(!buf && count>16384) //если воспроизвелось больше 255 байт,
//////			{
//////				f_read(&fp,&_buffer1[0],16384,&br);//то читаем в первую половину буфера инфу с флешки
//////				buf=true;
//////				if (br < 16384) //если буфер не содержит 256 значений значит конец файла
//////					{
//////						counter++;
//////					}
//////			}
//////			if(buf && count<16384)
//////			{
//////				f_read(&fp,&_buffer1[16384],16384,&br);// читаем во вторую часть буфера с флешки
//////				buf = false;
//////				if (br < 16384)
//////					{
//////						counter++;
//////					}
//////			}
////
////
////}
