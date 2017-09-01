/*
 * tim.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DRIVER_TIMER_TIM_TIM_H_
#define DRIVER_TIMER_TIM_TIM_H_

#include "../platform.h"
#include "../Interface/IHDriver.h"
#include "../nvic/nvic.h"

namespace Driver
{

	class Tim : public IHDriver
	{
		TimInit timerInitStructure;

	public:
		enum TimerNum
		{
			Timer1 = TIM1_BASE,
			Timer2 = TIM2_BASE,
			Timer3 = TIM3_BASE,
			Timer4 = TIM4_BASE,
			Timer5 = TIM5_BASE,
			Timer6 = TIM6_BASE,
			Timer7 = TIM7_BASE,
			Timer8 = TIM8_BASE,
		};

		enum CounterType
		{
			Up = 0x0000,
			Down = 0x0010
		};

		enum ClockDivision
		{
			Div1 = 0x0000,
			Div2 = 0x0100,
			Div4 = 0x0200
		};

		enum InterruptType
		{
			IT_Update=0x0001,
			IT_CC1=0x0002,
			IT_CC2=0x0004,
			IT_CC3=0x0008,
			IT_CC4=0x0010,
			IT_COM=0x0020,
			IT_Trigger=0x0040,
			IT_Break=0x0080,
		};


	  public:
		 static Tim* TObj;

		 Tim(TimerNum tNum, uint16_t Period, InterruptType interupt);
		 Tim(TimerNum tNum, uint16_t Period, CounterType type,InterruptType interupt );
		 Tim(TimerNum tNum, uint16_t Period, CounterType type, ClockDivision div,InterruptType interupt );


		 ~Tim();

		 void Init();


		 void Enable();
		 void Disable();

		 const char* toString();

		 /*
		  * Interrupt
		  */
		 static void InterruptWraper(void);

	  private:
		 TimerNum _tNum;
		 CounterType _type;
		 ClockDivision _div;
		 uint16_t _period;
		 InterruptType _interrupt;
		 bool _enabled;
	};
}



#endif /* DRIVER_TIMER_TIM_TIM_H_ */
