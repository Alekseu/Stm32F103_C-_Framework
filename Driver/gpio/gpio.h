/*
 * gpio.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DRIVER_GPIO_GPIO_H_
#define DRIVER_GPIO_GPIO_H_


#include "../platform.h"
#include "../Interface/CommunicationObject.h"

namespace Driver
{
	class GpIO
	{

	public:
		static GpIO* GObj;

	};
}


#endif /* DRIVER_GPIO_GPIO_H_ */
