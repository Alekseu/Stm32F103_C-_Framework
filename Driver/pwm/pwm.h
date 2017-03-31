/*
 * pwm.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_PWM_PWM_H_
#define DRIVER_PWM_PWM_H_

#include "../platform.h"

namespace Driver
{
	class Pwm
	{
	public:
		static Pwm* PwmObj;
		void Init();
	private:

	};
}




#endif /* DRIVER_PWM_PWM_H_ */
