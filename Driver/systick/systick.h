/*
 * systick.h
 *
 *  Created on: 01 сент. 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SYSTICK_SYSTICK_H_
#define DRIVER_SYSTICK_SYSTICK_H_

#include "../platform.h"
#include "../Interface/IHDriver.h"
#include "../nvic/nvic.h"

namespace Driver
{

	/*
	 * System timer example
	 *
	 * SystemTimer _systim(1000);
	 * _systim.Init();
	 * _systim.AddCallback(SysTickCallback1);
	 * _systim.AddCallback(SysTickCallback2);
	 * _systim.Enable();
	 *
	 * void SysTickCallback1()
	 * {
	 *   do thomething
	 * }
	 *
	 * void SysTickCallback2()
	 * {
	 * 	do thomething
	 * }
	 *
	 */


	typedef void SystemTimerCallback(void);

	class SystemTimer
	{
		public:

		static SystemTimer* SysObj;

		SystemTimer(uint16_t period);
		~SystemTimer();

		void Init();

		void Enable();
		void Disable();
		uint8_t AddCallback(SystemTimerCallback* callback);
		void RemoveCallback(uint8_t id);

		const char* toString();

		/*
		 * Interrupt
		 */
		static void InterruptWraper(void);

		private:
			bool  _enabled;
			uint16_t _period;
			uint8_t _currentPosition;
			uint8_t _total_Callbacks;
			SystemTimerCallback* _callpacks[CallbackCount];
	};

}


#endif /* DRIVER_SYSTICK_SYSTICK_H_ */
