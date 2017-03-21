/*
 * Timer.h
 *
 *  Created on: 28.03.2013
 *      Author: YVShvets
 */

#ifndef TIMER_H_
#define TIMER_H_
//#include <avr/interrupt.h>
#include <string.h>
#include <stdint.h>

//extern "C" void TIMER0_OVF_vect(void) __attribute__ ((signal));

typedef void CallBack(void);

struct Timers
{
	int time;
	int tick;
	bool enabled;
	char id;
	CallBack* function;
};

class Timer
{
public:
	// tick - время срабатывания * 10 us
	Timer();
	void Init();
	uint8_t setTimer(unsigned char id, int time, CallBack f = 0);
	void killTimer(uint8_t id);
	void setTimeOut(uint8_t id, int time = -1);
	bool isTimeOut(uint8_t id);
	void Enable(uint8_t id);
	void Disable(uint8_t id);

	bool Exist(uint8_t id);

	static unsigned long SystemTick;
	static void onTick();
	static Timer* pObject;

private:
	Timers* timers_;
	char timerCount_;


	char getTimeById(char id);
};

class Wait
{
public:

	Wait(unsigned int wait)
	{
		_wait = wait;

		_stamp = Timer::SystemTick;
	}

	bool IsElapsed()
	{
		if (_stamp > Timer::SystemTick) return true;

		if (_stamp + _wait < Timer::SystemTick) return true;

		return false;
	}

	void Reset() { _stamp = Timer::SystemTick; }

private:

	unsigned int 	_wait;
	unsigned long 	_stamp;
};

#endif /* TIMER_H_ */
