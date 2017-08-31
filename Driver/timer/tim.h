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


	  public:
		 static Tim* TObj;

		 Tim(TimerNum tNum, uint16_t Period );
		 Tim(TimerNum tNum, uint16_t Period, CounterType type );
		 Tim(TimerNum tNum, uint16_t Period, CounterType type, ClockDivision div );


		 ~Tim();

		 void Init();

		 const char* toString();

	  private:
		 TimerNum _tNum;

	};
}



#endif /* DRIVER_TIMER_TIM_TIM_H_ */
