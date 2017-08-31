/*
 * rs485.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef PROTOCOLS_RS485_RS485_H_
#define PROTOCOLS_RS485_RS485_H_

#include "../protocolConfig.h"

namespace Protocol
{
	using namespace Driver;


	class Rs485 : SerialPort
	{
	public:
		static Rs485* Rs485Obj;

		Rs485(SerialPort::PortName port, uint16_t baud);

		~Rs485();

		void Init();

		 uint8_t ReadByte() ;
		 void WriteByte(uint8_t byte);

		 uint16_t ReadWord() ;
		 void WriteWord(uint16_t word);

		 void SendData(uint8_t* data, uint16_t length);

		 void Recived(uint8_t data);

		 /*
		  * object
		  */
		 bool Equals(Object* obj);
		 const char* toString();

	private:
		 GpioInit GpioType;
		 bool _transmit;

		 void _transmitEnable();
		 void _reciveEnable();

	};
}




#endif /* PROTOCOLS_RS485_RS485_H_ */
