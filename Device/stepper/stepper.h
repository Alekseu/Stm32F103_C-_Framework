/*
 * stepper.h
 *
 *  Created on: 05 мая 2018 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_STEPPER_STEPPER_H_
#define DEVICE_STEPPER_STEPPER_H_

#include "../../Driver/platform.h"
#include "../deviceConfig.h"

namespace Device
{

/*
 * Stepper example
 *
 * Stepper _st(6,4,Stepper::StepMode::MiniStep,true);
 *
 *
 */

class Stepper
{
public:

	enum StepMode
	{
		SingleStep,
		MiniStep,
		MicroStep
	};

	Stepper(int startDelay, int workDelay, StepMode mode, bool holding)
	{
		_delay=workDelay;
		_startDelay =startDelay;
		_workDelay = workDelay;
		_mode = mode;
		_holding = holding;
		_currentStep=1;
	}

	void Init()
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;

		// Configure pin in output push/pull mode
		GPIO_InitStructure.GPIO_Pin = L1|L2|L3|L4;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

	void OneStep(bool rotation)
	{

		switch(_mode)
		{

		case SingleStep:
			switch(_currentStep)
			{
				case 1:
					GPIO_ResetBits(GPIOA,L2);
					GPIO_ResetBits(GPIOA,L3);
					GPIO_ResetBits(GPIOA,L4);
					GPIO_SetBits(GPIOA,L1);

					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L1);
					}
					break;
				case 2:
					GPIO_ResetBits(GPIOA,L1);
					GPIO_ResetBits(GPIOA,L3);
					GPIO_ResetBits(GPIOA,L4);
					GPIO_SetBits(GPIOA,L2);

					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L2);
					}
					break;
				case 3:
					GPIO_ResetBits(GPIOA,L1);
					GPIO_ResetBits(GPIOA,L2);
					GPIO_ResetBits(GPIOA,L4);
					GPIO_SetBits(GPIOA,L3);
					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L3);
					}
					break;
				case 4:
					GPIO_ResetBits(GPIOA,L1);
					GPIO_ResetBits(GPIOA,L2);
					GPIO_ResetBits(GPIOA,L3);
					GPIO_SetBits(GPIOA,L4);
					if(!_holding)
					{
						_delay_ms(1);
						GPIO_ResetBits(GPIOA,L4);
					}
					break;
				}

			if(rotation)
			{
				if(_currentStep++>=4)
				{
					_currentStep=1;
				}
			}
			else
			{
				if(_currentStep--<=1)
				{
					_currentStep=4;
				}
			}

		break;

		case MiniStep:
				switch(_currentStep)
				{
					case 1:
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L1);
						GPIO_SetBits(GPIOA,L2);
						break;
					case 2:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L2);
						GPIO_SetBits(GPIOA,L3);
						break;
					case 3:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);

						GPIO_SetBits(GPIOA,L3);
						GPIO_SetBits(GPIOA,L4);
						break;
					case 4:
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);

						GPIO_SetBits(GPIOA,L4);
						GPIO_SetBits(GPIOA,L1);
						break;
				}
				if(rotation)
				{
					if(_currentStep++>=4)
					{
						_currentStep=1;
					}
				}
				else
				{
					if(_currentStep--<=1)
					{
						_currentStep=4;
					}
				}

			break;

			case MicroStep:
				switch(_currentStep)
				{
					case 1:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L1);

						break;
					case 2:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L1);
						GPIO_SetBits(GPIOA,L2);
						break;
					case 3:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L2);
						break;
					case 4:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L2);
						GPIO_SetBits(GPIOA,L3);
						break;
					case 5:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);


						GPIO_SetBits(GPIOA,L3);
						break;
					case 6:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L3);
						GPIO_SetBits(GPIOA,L4);
						break;
					case 7:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);


						GPIO_SetBits(GPIOA,L4);
						break;
					case 8:
						GPIO_ResetBits(GPIOA,L1);
						GPIO_ResetBits(GPIOA,L2);
						GPIO_ResetBits(GPIOA,L3);
						GPIO_ResetBits(GPIOA,L4);

						GPIO_SetBits(GPIOA,L4);
						GPIO_SetBits(GPIOA,L1);
						break;
				}
				if(rotation)
				{
					if(_currentStep++>=8)
					{
						_currentStep=1;
					}
				}
				else
				{
					if(_currentStep--<=1)
					{
						_currentStep=8;
					}
				}
			break;
		}

		_delay_ms(_delay);

	}

	void MultiStep(int steps, bool rotation)
	{
		if(steps<StepsStart*2)
		{
			for(int i=0; i<steps;i++)
			{
				OneStep(rotation);
			}
		}
		else
		{

			_delay = _startDelay;
			int counter=0;
			for(int i=0;i<StepsStart;i++)
			{
				OneStep(rotation);
				if(counter++>= 50)
				{
					counter=0;
					_delay--;
				}
			}
			_delay = _workDelay;
			for(int i=0;i<steps - StepsStart;i++)
			{
				OneStep(rotation);
			}
		}

	}


private:

	int _delay;
	int _startDelay;
	int _workDelay;
	bool _holding;
	StepMode _mode;

	char _currentStep;
};

}

#endif /* DEVICE_STEPPER_STEPPER_H_ */
