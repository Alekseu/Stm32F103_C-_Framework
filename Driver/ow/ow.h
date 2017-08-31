/*
 * ow.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DRIVER_OW_OW_H_
#define DRIVER_OW_OW_H_

#include "../platform.h"
#include "../Interface/CommunicationObject.h"

namespace Driver
{
 	 class Ow :public ICommunicationObject
	 {
	 public:
 		 static Ow* OwObj;

	 };
}



#endif /* DRIVER_OW_OW_H_ */
