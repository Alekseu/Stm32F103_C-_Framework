/*
 * w5500.h
 *
 *  Created on: 31 ���. 2017 �.
 *      Author: hudienko_a
 */

#ifndef DEVICE_W5500_W5500_H_
#define DEVICE_W5500_W5500_H_

#include "../../Driver/platform.h"
#include "../../Driver/gpio/gpio.h"

namespace Device
{

	class W5500
	{
		public:

		bool init();

		void init_socket();

	};
}


#endif /* DEVICE_W5500_W5500_H_ */
