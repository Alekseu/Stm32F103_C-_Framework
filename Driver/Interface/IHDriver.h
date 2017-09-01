/*
 * IHDriver.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DRIVER_INTERFACE_IHDRIVER_H_
#define DRIVER_INTERFACE_IHDRIVER_H_
#include "../platform.h"

namespace Driver
{
	typedef void HDriverCallback(void);

	class IHDriver : public Object
	{
	public:

		HDriverCallback* OnElapsed;

		virtual void Enable()=0;
		virtual void Disable()=0;

	};

}



#endif /* DRIVER_INTERFACE_IHDRIVER_H_ */
