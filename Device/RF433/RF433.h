/*
 * RF433.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DEVICE_RF433_RF433_H_
#define DEVICE_RF433_RF433_H_

#include "../deviceConfig.h"
#include "../../Driver/tim/tim.h"
#include "../../Driver/gpio/gpio.h"

using namespace Driver;

namespace Device
{

	typedef void RF433Callback(void);

	class RF433
	{
	public:
		static RF433* RFObj;

		RF433();
		virtual ~RF433();

		void Init();

		void StartReceive();
		void StopReceive();

		char* getString();

		void SetCallback(RF433Callback* callback);

		virtual void Received(uint16_t inp);

		static void InterruptTimerWraper();
		static void InterruptGpioWraper();

		const char* toString();

	private:
		RF433Callback* _callback;
		Tim* _timer;
		GPIO* _gpio;
		bool _oldState;
		uint8_t _bitCount;
		uint8_t _interruptCount;
		uint16_t _input;
		char _tail;
		char _buff[5];

	};
}



#endif /* DEVICE_RF433_RF433_H_ */
