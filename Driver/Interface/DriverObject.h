/*
 * DriverObject.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_INTERFACE_DRIVEROBJECT_H_
#define DRIVER_INTERFACE_DRIVEROBJECT_H_

#include "../platform.h"



//#define DMA_SOURCE 1
//#define PWM_SOURCE 2
//#define TIMER_SOURCE 3
//#define FSMC_SOURCE 4

//#define RX_COMPLETE 1
//#define TX_COMPLETE 2
//#define RX_HALF_COMPLETE 3
//#define TX_HALF_COMPLETE 4

 enum DrvSource
{
	DmaSource=1,
	PwmSource =2,
	TimerSource=3,
	FsmcSource =4
};

 enum EvType
{
	RxComplete =1,
	TxComplete =2,
	RxHalfComplete =3,
	TxHalfComplete = 4
};


struct Source
{
	union
	{
		struct
		{
			DrvSource  DriverSource		   :4;
			EvType  EventType		   :4;
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
