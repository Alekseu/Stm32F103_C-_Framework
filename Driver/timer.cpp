/*
 * Timer.cpp
 *
 *  Created on: 28.03.2013
 *      Author: YVShvets
 */

#include "Timer.h"
#include "../StdPeriph/cmsis_boot/stm32f10x.h"
#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
#include "../StdPeriph/stm_lib/inc/misc.h"


Timer* Timer::pObject = 0;
unsigned long Timer::SystemTick = 0;


extern "C"
{
	void SysTick_Handler(){
		Timer::pObject->onTick();
	}
}
Timer::Timer () {
	// Config CRL
	timerCount_ = 0;
	timers_ = 0;

	pObject = this;
}


void Timer::Init()
{
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}

void Timer::onTick(){
	for(uint8_t i = 0; i < Timer::pObject->timerCount_; i++){
		if(Timer::pObject->timers_[i].enabled)
		{
			if (Timer::pObject->timers_[i].tick > 0){
				Timer::pObject->timers_[i].tick--;
				continue;
			}

			if(Timer::pObject->timers_[i].function != 0){
				Timer::pObject->timers_[i].tick = Timer::pObject->timers_[i].time;
				Timer::pObject->timers_[i].function();
			}
		}
	}

	SystemTick++;
}
uint8_t Timer::setTimer(unsigned char id, int time, CallBack* f){
	if (id <= 0)
		return 0;

	Timers *tmp = new Timers[timerCount_];

	memcpy(tmp, timers_, sizeof(Timers) * timerCount_);

	delete[] timers_;

	timers_ = new Timers[++timerCount_];

	memcpy(timers_, tmp, sizeof(Timers) * timerCount_ - 1);

	delete[] tmp;

	timers_[timerCount_ - 1].time = time;
	timers_[timerCount_ - 1].tick = time;
	timers_[timerCount_ - 1].function = f;
	timers_[timerCount_ - 1].id = id;
	timers_[timerCount_-1].enabled = false;

	return timerCount_ - 1;
}
bool Timer::isTimeOut(uint8_t id){
	unsigned char nTimer = getTimeById(id);

	if (nTimer <= 0)
		return false;

	if (timers_[nTimer].tick)
		return true;
	else
		return false;
}
void Timer::setTimeOut(uint8_t id, int time){
	unsigned char nTimer = getTimeById(id);

	if (nTimer <= 0)
		return;

	if (time == -1)
		timers_[nTimer].tick = timers_[nTimer].time;
	else{
		timers_[nTimer].tick = time;
		timers_[nTimer].time = time;
	}
}
void Timer::killTimer(uint8_t id){
	char nTimer = getTimeById(id);

	if (nTimer <= 0)
		return;

	Timers *tmp = new Timers[timerCount_];
	uint8_t index = 0;

	for(uint8_t i = 0; i < timerCount_; i++)
		if(i != nTimer)
			tmp[index++] = timers_[i];

	delete[] timers_;
	timers_ = new Timers[--timerCount_];

	memcpy(timers_, tmp, sizeof(Timers) * timerCount_);

	delete[] tmp;
}

bool Timer::Exist(uint8_t id)
{
	for (unsigned char i = 0; i < timerCount_; i++)
			if (timers_[i].id == id)
				return i;

		return -1;
}

char Timer::getTimeById(char id){
	for (unsigned char i = 0; i < timerCount_; i++)
		if (timers_[i].id == id)
			return i;

	return -1;
}

void Timer::Enable(uint8_t id)
{
	for (unsigned char i = 0; i < timerCount_; i++)
			if (timers_[i].id == id) timers_[i].enabled = true;
}

void Timer::Disable(uint8_t id)
{
	for (unsigned char i = 0; i < timerCount_; i++)
				if (timers_[i].id == id) timers_[i].enabled = false;
}

