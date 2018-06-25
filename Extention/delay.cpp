/*
 * delay.cpp
 *
 *  Created on: 29 июня 2016 г.
 *      Author: hudienko_a
 */

#include"delay.h"
#include "../Driver/platform.h"

extern "C"
{
#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_tim.h"
#include "../StdPeriph/cmsis_boot/system_stm32f10x.h"
}
void _delay_ms(int ms)
{
	//if(WITH_OS)  __asm__ __volatile__ ("cpsid i");



	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq (&RCC_Clocks);

	   TIM4->PSC = RCC_Clocks.PCLK1_Frequency/1000+1;	//устанавливаем предделитель
	     TIM4->ARR = ms;	//устанавливаем значение переполнения таймера, а значит и значение при котором генерируется Событие обновления
	     TIM4->EGR |= TIM_EGR_UG;	//Генерируем Событие обновления для записи данных в регистры PSC и ARR
	     TIM4->CR1 |= TIM_CR1_CEN|TIM_CR1_OPM;	//Запускаем таймер записью бита CEN и устанавливаем режим Одного прохода установкой бита OPM
	     while (TIM4->CR1&TIM_CR1_CEN!=0);

	//     if(WITH_OS)  __asm__ __volatile__ ("cpsie i");
}


void _delay_us(int us)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq (&RCC_Clocks);

	TIM4->PSC = RCC_Clocks.PCLK1_Frequency/1000000+1;	//устанавливаем предделитель
	TIM4->ARR = us;	//устанавливаем значение переполнения таймера, а значит и значение при котором генерируется Событие обновления
	TIM4->EGR |= TIM_EGR_UG;	//Генерируем Событие обновления для записи данных в регистры PSC и ARR
	TIM4->CR1 |= TIM_CR1_CEN|TIM_CR1_OPM;	//Запускаем таймер записью бита CEN и устанавливаем режим Одного прохода установкой бита OPM
	while (TIM4->CR1&TIM_CR1_CEN!=0);

}


//#define DELAY_TIM_FREQUENCY_US 1000000		/* = 1MHZ -> timer runs in microseconds */
//#define DELAY_TIM_FREQUENCY_MS 1000			/* = 1kHZ -> timer runs in milliseconds */
//
//// Init timer for Microseconds delays
//void _init_us() {
//	// Enable clock for TIM2
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
//
//	// Time base configuration
//	TIM_TimeBaseInitTypeDef TIM;
//	TIM_TimeBaseStructInit(&TIM);
//	TIM.TIM_Prescaler = (SystemCoreClock/DELAY_TIM_FREQUENCY_US)-1;
//	TIM.TIM_Period = 0xFFFF;
//	TIM.TIM_ClockDivision = 0;
//	TIM.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM2,&TIM);
//
//	// Enable counter for TIM2
//	TIM_Cmd(TIM2,ENABLE);
//}
//
//// Init and start timer for Milliseconds delays
//void _init_ms() {
//	// Enable clock for TIM2
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
//
//	// Time base configuration
//	TIM_TimeBaseInitTypeDef TIM;
//	TIM_TimeBaseStructInit(&TIM);
//	TIM.TIM_Prescaler = (SystemCoreClock/DELAY_TIM_FREQUENCY_MS)-1;
//	TIM.TIM_Period = 0xFFFF;
//	TIM.TIM_ClockDivision = 0;
//	TIM.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM2,&TIM);
//
//	// Enable counter for TIM2
//	TIM_Cmd(TIM2,ENABLE);
//}
//
//// Stop timer
//void _stop_timer() {
//	TIM_Cmd(TIM2,DISABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,DISABLE); // Powersavings?
//}
//
//// Do delay for nTime milliseconds
//void _delay_ms(int mSecs) {
//	// Init and start timer
//	_init_ms();
//
//	// Dummy loop with 16 bit count wrap around
//	volatile uint32_t start = TIM2->CNT;
//	while((TIM2->CNT-start) <= mSecs);
//
//	// Stop timer
//	_stop_timer();
//}
//
//// Do delay for nTime microseconds
//void _delay_us(int uSecs) {
//	// Init and start timer
//	_init_us();
//
//	// Dummy loop with 16 bit count wrap around
//	volatile uint32_t start = TIM2->CNT;
//	while((TIM2->CNT-start) <= uSecs);
//
//	// Stop timer
//	_stop_timer();
//}
