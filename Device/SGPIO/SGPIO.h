/*
 * SGPIO.h
 *
 *  Created on: 01 сент. 2017 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_SGPIO_SGPIO_H_
#define DEVICE_SGPIO_SGPIO_H_


#include "../deviceConfig.h"

/*
 * Driver 74HC595  - output  74HC168 - inputs
 */

namespace Device
{
	class SGPIO :public DeviceObject
	{
	  public:
		static SGPIO* SGObj;

		SGPIO();
		~SGPIO();

		void Init();

		const char* toString();
	  private:

	};
}


#endif /* DEVICE_SGPIO_SGPIO_H_ */
