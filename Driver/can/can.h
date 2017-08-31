/*
 * can.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DRIVER_CAN_CAN_H_
#define DRIVER_CAN_CAN_H_

#include "../platform.h"
#include "../Interface/CommunicationObject.h"

namespace Driver
{
 	 class Can :public ICommunicationObject
	 {
	 public:
 		 static Can* CanObj;

	 };
}






#endif /* DRIVER_CAN_CAN_H_ */
