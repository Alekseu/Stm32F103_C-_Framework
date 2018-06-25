/*
 * systick.cpp
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */
#include "systick.h"
#include "../../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"

namespace Driver
{
	SystemTimer* SystemTimer::SysObj =0;

	SystemTimer::SystemTimer(uint16_t period)
	{
		_currentPosition=0;
		_period = period;
		SystemTimer::SysObj = this;
		_enabled = false;
	}

	SystemTimer::~SystemTimer()
	{

	}

	void SystemTimer::Init()
	{
		RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);

		if (_period < SysTick_LOAD_RELOAD_Msk)
		{
			SysTick->LOAD  = (RCC_Clocks.HCLK_Frequency/_period & SysTick_LOAD_RELOAD_Msk) - 1;

			InterruptController::SetHandler(SysTick_IRQn,InterruptWraper);
			InterruptController::EnableChannel(SysTick_IRQn);

			SysTick->VAL   = 0;
			SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
							 SysTick_CTRL_TICKINT_Msk   |
							 SysTick_CTRL_ENABLE_Msk;

			for(int i=0;i<CallbackCount;i++)
			{
				_callpacks[i]=0;
			}
			_total_Callbacks=0;
		}
	}

	void SystemTimer::Enable()
	{
		_enabled = true;
	}

	void SystemTimer::Disable()
	{
		_enabled = false;
	}

	uint8_t SystemTimer::AddCallback(SystemTimerCallback* callback)
	{
		_callpacks[_currentPosition++] = callback;
		_total_Callbacks++;
		return _currentPosition-1;

	}

	void SystemTimer::RemoveCallback(uint8_t id)
	{
		if(id>CallbackCount) return;
		_callpacks[id] =0;
		_total_Callbacks--;
	}

	const char* SystemTimer::toString()
	{
		return "Systick";
	}

	void SystemTimer::InterruptWraper(void)
	{
		if(SystemTimer::SysObj->_enabled)
		{
			for(int i=0;i<SystemTimer::SysObj->_total_Callbacks;i++)
			{
				if(SystemTimer::SysObj->_callpacks[i]!=0)
				{
					SystemTimer::SysObj->_callpacks[i]();
				}
			}
		}
	}

}


