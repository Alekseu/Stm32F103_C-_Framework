/*
 * adc.cpp
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#include "adc.h"

namespace Driver
{
   Adc* Adc::Adc1Obj1 =0;
   Adc* Adc::Adc1Obj2 =0;
   Adc* Adc::Adc1Obj3 =0;
   Adc* Adc::Adc1Obj4 =0;
   Adc* Adc::Adc1Obj5 =0;
   Adc* Adc::Adc1Obj6 =0;
   Adc* Adc::Adc1Obj7 =0;
   Adc* Adc::Adc1Obj8 =0;
   Adc* Adc::Adc1Obj9 =0;
   Adc* Adc::Adc1ObjTemp =0;


   Adc::Adc(unsigned char adcInNum, AdcNum adc, AdcMode mode, AdcChannel channel){
	   adcNum = adc;
	   adcMode = mode;
	   adcChannel = channel;
	   this->adcInNum = adcInNum;

	   switch(adcInNum){
		   case 0:
			   Adc1Obj1 = this;
			   break;
		   case 1:
			   Adc1Obj2 = this;
			   break;
		   case 2:
			   Adc1Obj3 = this;
			   break;
		   case 3:
			   Adc1Obj4 = this;
			   break;
		   case 4:
			   Adc1Obj5 = this;
			   break;
		   case 5:
			   Adc1Obj6 = this;
			   break;
		   case 6:
			   Adc1Obj7 = this;
			   break;
		   case 7:
			   Adc1Obj8 = this;
			   break;
		   case 8:
			   Adc1Obj9 = this;
			   break;
		   case 9:
			   Adc1ObjTemp = this;
			   break;
	   }


   }

   Adc::~Adc(){

   }

  void Adc::Init(){
	  GPIO_InitTypeDef  GPIO_InitStructure;
	   switch(adcMode){
	   case AdcScanContinuous:
		   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
		   GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 ;        // that's ADC1 (PA1 on STM32)
		   GPIO_Init(GPIOA, &GPIO_InitStructure);
		   //clock for ADC (max 14MHz --> 72/6=12MHz)
		   RCC_ADCCLKConfig (RCC_PCLK2_Div6);
		   // enable ADC system clock
		   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		   ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		   ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		   ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // we work in continuous sampling mode
		   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		   ADC_InitStructure.ADC_NbrOfChannel = 1;

		   ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_28Cycles5); // define regular conversion config
		   ADC_Init ( ADC1, &ADC_InitStructure);   //set config of ADC1

		   // enable ADC
		   ADC_Cmd (ADC1,ENABLE);  //enable ADC1

		   //  ADC calibration (optional, but recommended at power on)
		   ADC_ResetCalibration(ADC1); // Reset previous calibration
		   while(ADC_GetResetCalibrationStatus(ADC1));
		   ADC_StartCalibration(ADC1); // Start new calibration (ADC must be off at that time)
		   while(ADC_GetCalibrationStatus(ADC1));
		   break;
	   case		AdcDma:
		   break;
	   case	AdcInternalTermo:
		   			 //ADC
		   			    ADC_InitTypeDef ADC_InitStructure;
		   			    // define ADC config
		   			    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		   			    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		   			    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  // we work in continuous sampling mode
		   			    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		   			    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		   			    ADC_InitStructure.ADC_NbrOfChannel = 1;

		   			    ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, ADC_SampleTime_239Cycles5); // define regular conversion config
		   			    ADC_Init (ADC1, &ADC_InitStructure);    //set config of ADC1

		   			    // Enable Temperature sensor
		   			    ADC_TempSensorVrefintCmd(ENABLE);

		   			    // Enable ADC
		   			    ADC_Cmd (ADC1, ENABLE); //enable ADC1

		   			    //  ADC calibration (optional, but recommended at power on)
		   			    ADC_ResetCalibration(ADC1); // Reset previous calibration
		   			    while(ADC_GetResetCalibrationStatus(ADC1));
		   			    ADC_StartCalibration(ADC1); // Start new calibration (ADC must be off at that time)
		   			    while(ADC_GetCalibrationStatus(ADC1));

		   break;
	   }
   }

  void Adc::AdcStart(){
	  switch(adcMode){
		   case AdcScanContinuous:
			   ADC_Cmd (ADC1,ENABLE);  //enable ADC1
			   			    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			   break;
		   case		AdcDma:
		  		   break;
		  	   case	AdcInternalTermo:
		  		 // start conversion
		  				   			    ADC_Cmd (ADC1,ENABLE);  //enable ADC1
		  				   			    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // start conversion (will be endless as we are in continuous mode)

		  		   break;
	  }
  }

  void Adc::AdcStop(){
	  switch(adcMode){
		  case AdcScanContinuous:

			  break;
		  case		AdcDma:
			  break;
		  case	AdcInternalTermo:
			  break;
	  }
  }

  unsigned short  Adc::ReadAdc(){
	  unsigned short data;
	  switch(adcMode){
		  case AdcScanContinuous:
			  data = ADC_GetConversionValue((ADC_TypeDef*)adcNum);
			  break;
		  case		AdcDma:
			  break;
		  case	AdcInternalTermo:
			  unsigned short adc_value;
			  adc_value = ADC_GetConversionValue(ADC1);
			  data = (uint16_t)((V25-adc_value)/Avg_Slope+25);
			  break;
	  }
	  return data;
  }

  const char* toString(){
	  return "Adc";
  }
}

