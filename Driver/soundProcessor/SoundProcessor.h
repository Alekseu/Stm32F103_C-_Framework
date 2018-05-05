/*
 * SoundProcessor.h
 *
 *  Created on: 18 мая 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SOUNDPROCESSOR_H_
#define DRIVER_SOUNDPROCESSOR_H_

#include "../StdPeriph/stm_lib/inc/misc.h"
#include "../StdPeriph/cmsis_boot/stm32f10x.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_tim.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_exti.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_dma.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_dac.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_adc.h"

#include "../SDCard/inc/ff.h"
#include <string.h>
#include <stdio.h>

//minimum buffer size for 44100 is 256 bytes
#define BUFFER_SIZE 512
#define HALF_BUFFER_SIZE BUFFER_SIZE/2

enum Quality
{
	Sample_8KHZ=1,
	Sample_11KHZ=2,
	Sample_16KHZ=3,
	Sample_22KHZ=4,
	Sample_44KHZ=5,
};

class SoundProcessor
{
public:
	static SoundProcessor* sObj;
	bool SoundLoopBack;
	Quality SoundQality;
	bool NowPlay;

	SoundProcessor();

	void Init();
	void Play();
	void Play(unsigned char* filename);
	void Play_half(unsigned char* filename);
	void Play_Buffer(char* buff, unsigned int size);

	void Stop();

	void Half_Play_Complete();
	void Play_Complete();
	//void NextSample_event();


	void StartRecording();
	void StopRecording();

	void Half_Record_Complete();
	void Record_Complete();

private:

	unsigned int  _data_count;

	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef timerPWM;

	DAC_InitTypeDef DAC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure_DAC;
	DMA_InitTypeDef DMA_InitStructure_ADC;


	FATFS	fs;
	FIL fp;
	UINT br; //счетчик прочитанных байт
	UINT bw; //счетчик прочитанных байт
	FRESULT res;
	unsigned int _count;
	unsigned char  _buffer1[BUFFER_SIZE];
	unsigned char  _buffer2[BUFFER_SIZE];
	unsigned short _buffer3[BUFFER_SIZE];

	unsigned int   _playcount;
	unsigned int   _play_buf_count;
	unsigned char  _aru;
	bool           _file_play;
	bool 		   _hapf_buffer_play;

	void _dma_init_size(unsigned int size,bool circular);


};


#endif /* DRIVER_SOUNDPROCESSOR_H_ */
