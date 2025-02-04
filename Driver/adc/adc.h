/*
 * adc.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DRIVER_ADC_ADC_H_
#define DRIVER_ADC_ADC_H_


#include "../platform.h"
#include "../Interface/DriverObject.h"
#include "../Interface/CommunicationObject.h"
#include "../nvic/nvic.h"

namespace Driver
{

const uint16_t V25 = 1750;// when V25=1.41V at ref 3.3V
const uint16_t Avg_Slope = 5; //when avg_slope=4.3mV/C at ref 3.3V

	class Adc  {
		AdcInit ADC_InitStructure;
		unsigned char adcInNum;

	public:
		static Adc* Adc1Obj1;
		static Adc* Adc1Obj2;
		static Adc* Adc1Obj3;
		static Adc* Adc1Obj4;
		static Adc* Adc1Obj5;
		static Adc* Adc1Obj6;
		static Adc* Adc1Obj7;
		static Adc* Adc1Obj8;
		static Adc* Adc1Obj9;
		static Adc* Adc1ObjTemp;

		enum AdcNum {
			Adc1 = ADC1_BASE,
			Adc2 = ADC2_BASE
		};

		enum AdcMode {
			AdcScanContinuous ,
			AdcDma,
			AdcInternalTermo
		};

		enum AdcChannel{
			ADC_CHANNEL_0,
			ADC_CHANNEL_1,
			ADC_CHANNEL_2,
			ADC_CHANNEL_3,
			ADC_CHANNEL_4,
			ADC_CHANNEL_5,
			ADC_CHANNEL_6,
			ADC_CHANNEL_7,
			ADC_CHANNEL_8,
			ADC_CHANNEL_9,
			ADC_CHANNEL_10,
			ADC_CHANNEL_11,
			ADC_CHANNEL_12,
			ADC_CHANNEL_13,
			ADC_CHANNEL_14,
			ADC_CHANNEL_15,
			ADC_CHANNEL_16,
			ADC_CHANNEL_17,
			TempSensor = ADC_Channel_TempSensor
		};

		Adc(unsigned char adcInNum, AdcNum adc, AdcMode mode, AdcChannel channel);
		virtual ~Adc();

		void Init();

		void AdcStart();
		void AdcStop();

		unsigned short ReadAdc();

		/*
		 * Interrupt
		 */
		static void InterruptWraper(void);

		const char* toString();

		private:

		AdcNum adcNum;
		AdcMode adcMode;
		AdcChannel adcChannel;

	};
}


//		 void adc_dma(){
//
//			 volatile uint16_t ADCBuffer[] = {0xAAAA, 0xAAAA, 0xAAAA, 0xAAAA};
//			 GPIO_InitTypeDef GPIO_InitStructure;
//			    ADC_InitTypeDef ADC_InitStructure;
//			    DMA_InitTypeDef DMA_InitStructure;
//
//			    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//			    /* Enable ADC1 and GPIOA clock */
//			    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
//			    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE );
//
//			    DMA_InitStructure.DMA_BufferSize = 4;
//			    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//			    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//			    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCBuffer;
//			    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//			    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//			    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//			    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
//			    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//			    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//			    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//			    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//			    DMA_Cmd(DMA1_Channel1 , ENABLE ) ;
//
//			    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//			    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//			    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//			    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//			    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//			    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
//			    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
//			    ADC_InitStructure.ADC_NbrOfChannel = 4;
//			    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
//			    ADC_Init(ADC1, &ADC_InitStructure);
//			    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_7Cycles5);
//			    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_7Cycles5);
//			    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_7Cycles5);
//			    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 4, ADC_SampleTime_7Cycles5);
//			    ADC_Cmd(ADC1 , ENABLE ) ;
//			    ADC_DMACmd(ADC1 , ENABLE ) ;
//			    ADC_ResetCalibration(ADC1);
//
//			    while(ADC_GetResetCalibrationStatus(ADC1));
//			    ADC_StartCalibration(ADC1);
//
//			    while(ADC_GetCalibrationStatus(ADC1));
//			    ADC_SoftwareStartConvCmd ( ADC1 , ENABLE ) ;
//		 }
//
//		 void internal_termo_adc(){
//
//			 int adc_value;
//			    int temperature;
//
//

//		 }


#endif /* DRIVER_ADC_ADC_H_ */
