/*
 * rtc.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_RTC_RTC_H_
#define DRIVER_RTC_RTC_H_

#include "../platform.h"

namespace Driver
{
	class Rtc
	{
		public:
			static Rtc* RtObj;
			void Init();

		private:
	};
}



#endif /* DRIVER_RTC_RTC_H_ */
