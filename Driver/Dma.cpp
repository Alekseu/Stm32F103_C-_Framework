/*
 * Dma.cpp
 *
 *  Created on: 09 марта 2017 г.
 *      Author: Alekseu
 */
#include "Dma.h"

Dma *Dma::pDma0 = 0;
Dma *Dma::pDma1 = 0;
Dma *Dma::pDma2 = 0;
Dma *Dma::pDma3 = 0;
Dma *Dma::pDma4 = 0;
Dma *Dma::pDma5 = 0;
Dma *Dma::pDma6 = 0;
Dma *Dma::pDma7 = 0;
Dma *Dma::pDma8 = 0;
Dma *Dma::pDma9 = 0;
Dma *Dma::pDma10 = 0;
Dma *Dma::pDma11 = 0;
Dma *Dma::pDma12 = 0;

extern "C"
{
// ------------ Прерывания DMA ------------------

void DMA1_Channel1_IRQHandler(void) //память память
{
	if (DMA1->ISR & DMA1_IT_TC1)
	{
		DMA1->IFCR =DMA1_IT_TC1;
		Dma::pDma0->RecivedDmaComplete();
		Dma::pDma0->TransmitDmaComplete();
	}
	if (DMA1->ISR & DMA1_IT_HT1)
	{
		DMA1->IFCR =DMA1_IT_HT1;
		Dma::pDma0->HalfRecivedDmaComplete();
		Dma::pDma0->HalfTransmitDmaComplete();
	}
}

//UART1
void DMA1_Channel5_IRQHandler(void) //rx
{
	if (DMA1->ISR & DMA1_IT_TC5)
	{
		DMA1->IFCR =DMA1_IT_TC5;
		Dma::pDma1->RecivedDmaComplete();
	}
	if (DMA1->ISR & DMA1_IT_HT5)
	{
		DMA1->IFCR =DMA1_IT_HT5;
		Dma::pDma1->HalfRecivedDmaComplete();
	}
}

void DMA1_Channel4_IRQHandler(void) //tx
{
	if (DMA1->ISR & DMA1_IT_TC4)
	{
		DMA1->IFCR =DMA1_IT_TC4;
		Dma::pDma1->TransmitDmaComplete();
	}
	if (DMA1->ISR & DMA1_IT_HT4)
	{
		DMA1->IFCR =DMA1_IT_HT4;
		Dma::pDma1->HalfTransmitDmaComplete();
	}
}

//UART2
void DMA1_Channel6_IRQHandler(void) //rx
{
	if (DMA1->ISR & DMA1_IT_TC6)
	{
		DMA1->IFCR =DMA1_IT_TC6;
		Dma::pDma2->RecivedDmaComplete();
	}
	if (DMA1->ISR & DMA1_IT_HT6)
	{
		DMA1->IFCR =DMA1_IT_HT6;
		Dma::pDma2->HalfRecivedDmaComplete();
	}
}

void DMA1_Channel7_IRQHandler(void) //tx
{
	if (DMA1->ISR & DMA1_IT_TC7)
	{
		DMA1->IFCR =DMA1_IT_TC7;
		Dma::pDma2->TransmitDmaComplete();
	}
	if (DMA1->ISR & DMA1_IT_HT7)
	{
		DMA1->IFCR =DMA1_IT_HT7;
		Dma::pDma2->HalfTransmitDmaComplete();
	}
}

//UART3
void DMA1_Channel3_IRQHandler(void) //rx
{
	if (DMA1->ISR & DMA1_IT_TC3)
	{
		DMA1->IFCR =DMA1_IT_TC3;
		Dma::pDma3->RecivedDmaComplete();
	}
	if (DMA1->ISR & DMA1_IT_HT3)
	{
		DMA1->IFCR =DMA1_IT_HT3;
		Dma::pDma3->HalfRecivedDmaComplete();
	}
}

void DMA1_Channel2_IRQHandler(void) //tx
{
	if (DMA1->ISR & DMA1_IT_TC2)
	{
		DMA1->IFCR =DMA1_IT_TC2;
		Dma::pDma1->TransmitDmaComplete();
	}
	if (DMA1->ISR & DMA1_IT_HT2)
	{
		DMA1->IFCR =DMA1_IT_HT2;
		Dma::pDma1->HalfTransmitDmaComplete();
	}
}

//UART4
void DMA2_Channel3_IRQHandler(void) //rx
{
	if (DMA2->ISR & DMA2_IT_TC3)
	{
		DMA2->IFCR =DMA2_IT_TC3;
		Dma::pDma2->RecivedDmaComplete();
	}
	if (DMA2->ISR & DMA2_IT_HT3)
	{
		DMA2->IFCR =DMA2_IT_HT3;
		Dma::pDma2->HalfRecivedDmaComplete();
	}
}

void DMA2_Channel5_IRQHandler(void) //tx
{
	if (DMA2->ISR & DMA2_IT_TC5)
	{
		DMA2->IFCR =DMA2_IT_TC5;
		Dma::pDma4->TransmitDmaComplete();
	}
	if (DMA2->ISR & DMA2_IT_HT5)
	{
		DMA2->IFCR =DMA2_IT_HT5;
		Dma::pDma4->HalfTransmitDmaComplete();
	}
}


//5-й юарт не имеет дма каналов

// -----------------------------------------------
}


	Dma::Dma()
	{

	}

	Dma::~Dma(){

	}



 	 void  Dma::Init(unsigned char channel)
 	 {
 		_channel = channel;
 		 switch(channel)
 		 {
 		 case CHANNEL_1:
 			 	 pDma0 = this;
 			      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
 				  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
 				  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 				  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 				  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 				  NVIC_Init(&NVIC_InitStructure);

 			 break;

 		 case CHANNEL_2:
				 pDma1 = this;
				 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
				 NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
				 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
				 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
				 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				 NVIC_Init(&NVIC_InitStructure);
 		 	break;

 		 case CHANNEL_3:
				 pDma2 = this;
				 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
				 NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
				 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
				 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
				 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				 NVIC_Init(&NVIC_InitStructure);
 			 break;

 		 }
 	 }

 	void Dma::InitPherif(void* pherif)
 	{
 		switch(_channel)
 		{
 		case CHANNEL_1:
 			DMA_DeInit(DMA1_Channel1);
 			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)pherif;//0x40013804;
 			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;
 			DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
 			DMA_InitStructure.DMA_BufferSize = 0;
 			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 			//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 			DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
 			DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
 			DMA_Init(DMA1_Channel1, &DMA_InitStructure);
 			DMA_ClearFlag(DMA1_IT_TC1|DMA1_IT_HT1);
 			DMA_ITConfig(DMA1_Channel1, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);

 			break;
 		case CHANNEL_2:
// 			DMA_DeInit(DMA1_Channel2);
// 			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst;//0x40013804;
// 			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src;
// 			DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
// 			DMA_InitStructure.DMA_BufferSize = length;
// 			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
// 			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 			DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
// 			DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
// 			DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
// 			DMA_Init(DMA1_Channel2, &DMA_InitStructure);
// 			DMA_ClearFlag(DMA1_IT_TC2|DMA1_IT_HT2);
// 			DMA_ITConfig(DMA1_Channel2, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);
// 			DMA_Cmd(DMA1_Channel2,ENABLE);
 			break;

 		case CHANNEL_3:
// 			DMA_DeInit(DMA1_Channel3);
// 			DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst;//0x40013804;
// 			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src;
// 			DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
// 			DMA_InitStructure.DMA_BufferSize = length;
// 			DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
// 			DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
// 			DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
// 			DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
// 			DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
// 			DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
// 			DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
// 			DMA_Init(DMA1_Channel3, &DMA_InitStructure);
// 			DMA_ClearFlag(DMA1_IT_TC3|DMA1_IT_HT3);
// 			DMA_ITConfig(DMA1_Channel3, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);
// 			DMA_Cmd(DMA1_Channel3,ENABLE);
 			break;


 		}

 	}


 	/*
 	 * Приаем половины пакета по дма (переопределяемый метод, прерывание)
 	 */
 	 void  Dma::HalfRecivedDmaComplete(){}

 	/*
 	 * Прием всего пакета по дма  (переопределяемый метод, прерывание)
 	 */
 	 void  Dma::RecivedDmaComplete(){
 		 int a=0;
 	 }

 	/*
 	 * передача половины пакета дма (переопределяемый метод, прерывание )
 	 */
 	 void  Dma::HalfTransmitDmaComplete(){}

 	/*
 	 * Передача всего пакета дма (переопределяемый метод, прерывание)
 	 */
 	void  Dma::TransmitDmaComplete(){
 		int a=0;
 	}



 	void  Dma::MemCpy(char* src, char* dst, int length)
 	{
 		switch(_channel)
 		 		 {
 		 		 case CHANNEL_1:
 		 			 DMA_DeInit(DMA1_Channel1);
 		 			 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst;//0x40013804;
 		 			 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src;
 		 			 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
 		 			 DMA_InitStructure.DMA_BufferSize = length;
 		 			 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 		 			 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
 		 			 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 		 			 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 		 			 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 		 			 DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
 		 			 DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
 		 			 DMA_Init(DMA1_Channel1, &DMA_InitStructure);
 		 			 DMA_ClearFlag(DMA1_IT_TC1|DMA1_IT_HT1);
 		 			 DMA_ITConfig(DMA1_Channel1, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);
 		 			 DMA_Cmd(DMA1_Channel1,ENABLE);

 		 			 break;
 		 		 case CHANNEL_2:
 		 			 DMA_DeInit(DMA1_Channel2);
 		 			 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst;//0x40013804;
 		 			 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src;
 		 			 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
 		 			 DMA_InitStructure.DMA_BufferSize = length;
 		 			 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 		 			 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
 		 			 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 		 			 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 		 			 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 		 			 DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
 		 			 DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
 		 			 DMA_Init(DMA1_Channel2, &DMA_InitStructure);
 		 			 DMA_ClearFlag(DMA1_IT_TC2|DMA1_IT_HT2);
 		 			 DMA_ITConfig(DMA1_Channel2, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);
 		 			 DMA_Cmd(DMA1_Channel2,ENABLE);
 		 			 break;

 		 		 case CHANNEL_3:
 		 			 DMA_DeInit(DMA1_Channel3);
 		 			 DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst;//0x40013804;
 		 			 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src;
 		 			 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
 		 			 DMA_InitStructure.DMA_BufferSize = length;
 		 			 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 		 			 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
 		 			 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 		 			 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 		 			 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 		 			 DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
 		 			 DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
 		 			 DMA_Init(DMA1_Channel3, &DMA_InitStructure);
 		 			 DMA_ClearFlag(DMA1_IT_TC3|DMA1_IT_HT3);
 		 			 DMA_ITConfig(DMA1_Channel3, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);
 		 			 DMA_Cmd(DMA1_Channel3,ENABLE);
 		 			 break;

 		 		 }



 	}


 	/*
 	 * запустить прием по дма
 	 */
 	void  Dma::StartReciveDma(int length){}


 	/*
 	 * пнуть дма на передачу
 	 */
 	void  Dma::WriteArrayDma(unsigned char* array, int length){
 		switch(_channel)
 		{
 			case CHANNEL_1:
 				DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)array;
 				DMA_InitStructure.DMA_BufferSize = length;
 				DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
 				DMA_Cmd(DMA1_Channel1,ENABLE);
 			break;
 		}
 	}


 	void Dma::StartTransmitArrayDmaCircle(unsigned char* src,  char* dst, int length)
 	{
 		switch(_channel)
 		 		{
 		 			case CHANNEL_2:

 		 					configured = true;
 		 					DMA_DeInit(DMA1_Channel2);
 		 					DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)dst;//0x40013804;
 		 					DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)src;
 		 					DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
 		 					DMA_InitStructure.DMA_BufferSize = length;
 		 					DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 		 					DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
 		 					DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
 		 					DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
 		 					DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
 		 					DMA_InitStructure.DMA_Priority = DMA_Priority_High;
 		 					DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
 		 					DMA_Init(DMA1_Channel2, &DMA_InitStructure);
 		 					DMA_ClearFlag(DMA1_IT_TC2|DMA1_IT_HT2);
 		 					DMA_ITConfig(DMA1_Channel2, DMA_IT_TC|DMA_IT_HT|DMA_IT_TE, ENABLE);
 		 					DMA_Cmd(DMA1_Channel2,ENABLE);

 		 			break;
 		 		}
 	}



