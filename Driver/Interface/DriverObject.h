/*
 * DriverObject.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_INTERFACE_DRIVEROBJECT_H_
#define DRIVER_INTERFACE_DRIVEROBJECT_H_

#include "../platform.h"



#define DMA_SOURCE 1
#define PWM_SOURCE 2
#define TIMER_SOURCE 3

#define RX_COMPLETE 1
#define TX_COMPLETE 2
#define RX_HALF_COMPLETE 3
#define TX_HALF_COMPLETE 4


struct Source
		{
			union
			{
				struct
				{
					 unsigned  DriverSource		   :4;
					 unsigned  EventType		   :4;
				};
				int Data :8;
			};
		};

typedef void DriverCallback(Source, uint8_t);

class DriverObject:public Object
{
public:
	DriverCallback* OnEvent;
	uint8_t CommunicationDriver;
};



#endif /* DRIVER_INTERFACE_DRIVEROBJECT_H_ */
