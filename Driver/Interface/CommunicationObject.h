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
	typedef void OnRecivedCallBack(uint8_t);

	class ICommunicationObject
	{
		public:
		OnRecivedCallBack* OnRecived;

		ICommunicationObject(){OnRecived=0;};
		virtual ~ICommunicationObject(){}

		virtual void Init()=0;

		virtual uint8_t ReadByte() =0;
		virtual uint8_t ReadByte(uint8_t addr) =0;

		virtual bool ReadByte(uint8_t* value, uint16_t timeOut)=0;
		virtual void WriteByte(uint8_t byte)=0;
		virtual void WriteByte(uint8_t byte, uint8_t addr)=0;
		virtual void WriteByte(uint8_t byte, uint8_t baddr, uint8_t addr)=0;
		virtual void WriteBytes(uint8_t* val, uint16_t size, uint8_t baddr, uint8_t addr)=0;

		virtual uint16_t ReadWord() =0;
		virtual void WriteWord(uint16_t word)=0;

		virtual void SendData(uint8_t* data, uint16_t length)=0;
		//virtual uint16_t ReadData(uint8_t* mass)=0;

		/*
		 * interrupt
		 */
		virtual void Received(uint8_t byte)=0;

	};
}


#endif /* DRIVER_INTERFACE_COMMUNICATIONOBJECT_H_ */
