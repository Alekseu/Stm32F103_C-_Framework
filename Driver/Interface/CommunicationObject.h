/*
 * CommunicationObject.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_INTERFACE_COMMUNICATIONOBJECT_H_
#define DRIVER_INTERFACE_COMMUNICATIONOBJECT_H_

#include "../platform.h"

namespace Driver
{
	class ICommunicationObject
	{
		public:
		ICommunicationObject(){}
		virtual ~ICommunicationObject(){}

		virtual void Init()=0;

		virtual uint8_t ReadByte() =0;
		virtual void WriteByte(uint8_t byte)=0;

		virtual uint16_t ReadWord() =0;
		virtual void WriteWord(uint16_t word)=0;

	};
}


#endif /* DRIVER_INTERFACE_COMMUNICATIONOBJECT_H_ */
