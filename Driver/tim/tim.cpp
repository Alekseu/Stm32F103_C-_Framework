/*
 * tim.cpp
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#include "tim.h"

namespace Driver
{
	Tim* Tim::TObj =0;

	Tim::Tim(TimerNum tNum, uint16_t Period ,InterruptType interupt )
	{
		_tNum = tNum;
		_period = Period;
		_type = CounterType::Up;
		_div = ClockDivision::Div1;
		_interrupt = interupt;
		_enabled = false;
		Tim::TObj = this;
	}

	Tim::Tim(TimerNum tNum, uint16_t Period, CounterType type,InterruptType interupt )
	{
		_tNum = tNum;
		_period = Period;
		_type = type;
		_div = ClockDivision::Div1;
		_interrupt = interupt;
		_enabled = false;
		Tim::TObj = this;
	}

	Tim::Tim(TimerNum tNum, uint16_t Period, CounterType type, ClockDivision div,InterruptType interupt )
	{
		_tNum = tNum;
		_period = Period;
		_type = type;
		_div = div;
		_interrupt = interupt;
		_enabled = false;
		Tim::TObj = this;
	}

	Tim::~Tim()
	{

	}

	void Tim::Init()
	{
		switch (_tNum) {
		case Timer1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
			break;
		case Timer2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
			break;
		case Timer3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
			break;
		case Timer4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
			break;
		case Timer5:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
			break;
		case Timer6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
			break;
		case Timer7:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
			break;
		case Timer8:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
			break;

		default:
			break;
		}


		 //init screen update timer

		timerInitStructure.TIM_Period = _period*2;
		timerInitStructure.TIM_Prescaler = 32-1;
		timerInitStructure.TIM_ClockDivision = (uint16_t)_div;
		timerInitStructure.TIM_CounterMode = (uint16_t)_type;
		TIM_TimeBaseInit((TIM_TypeDef*) _tNum, &timerInitStructure );
		TIM_ARRPreloadConfig((TIM_TypeDef*) _tNum, ENABLE );
		TIM_ITConfig((TIM_TypeDef*) _tNum, _interrupt, ENABLE);
		TIM_Cmd((TIM_TypeDef*) _tNum, ENABLE);


		switch (_tNum) {
		case Timer1:
			InterruptController::SetHandler(TIM1_CC_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM1_CC_IRQn);
			break;
		case Timer2:
			InterruptController::SetHandler(TIM2_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM2_IRQn);
			break;
		case Timer3:
			InterruptController::SetHandler(TIM3_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM3_IRQn);
			break;
		case Timer4:
			InterruptController::SetHandler(TIM4_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM4_IRQn);
			break;
		case Timer5:
			InterruptController::SetHandler(TIM5_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM5_IRQn);
			break;
		case Timer6:
			InterruptController::SetHandler(TIM6_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM6_IRQn);
			break;
		case Timer7:
			InterruptController::SetHandler(TIM7_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM7_IRQn);
			break;
		case Timer8:
			InterruptController::SetHandler(TIM8_CC_IRQn,InterruptWraper);
			InterruptController::EnableChannel(TIM8_CC_IRQn);
			break;

		default:
			break;
		}
		 //TIM_Cmd((TIM_TypeDef*) _tNum, ENABLE);
	}

	 void Tim::Enable()
	 {
		// TIM_Cmd((TIM_TypeDef*) _tNum, ENABLE);
		 _enabled = true;
	 }

	 void Tim::Disable()
	 {
		 //TIM_Cmd((TIM_TypeDef*) _tNum, DISABLE);
		 _enabled = false;
	 }

	const char* Tim::toString()
	{
		return "TIM";
	}

	/*
	 * Interrupt
	 */
	void Tim::InterruptWraper(void)
	{
		bool interrupt = false;
		if (TIM_GetITStatus((TIM_TypeDef* )Tim::TObj->_tNum,Tim::TObj->_interrupt) != RESET)
		{
			TIM_ClearITPendingBit((TIM_TypeDef* )Tim::TObj->_tNum,Tim::TObj->_interrupt);
			if(Tim::TObj->_enabled && Tim::TObj->OnElapsed!=0)
			{
				Tim::TObj->OnElapsed();
			}
		}

	}
}



