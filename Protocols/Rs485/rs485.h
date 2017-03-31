/*
 * rs485.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef PROTOCOLS_RS485_RS485_H_
#define PROTOCOLS_RS485_RS485_H_
#include "../../Driver/usart/usart.h"

namespace Protocol
{
	using namespace Driver;


	class Rs485 : SerialPort
	{
	public:
		static Rs485* Rs485Obj;

		Rs485(uint8_t port, uint16_t baud);

		~Rs485();

		void Init();

		 uint8_t ReadByte() ;
		 void WriteByte(uint8_t byte);

		 uint16_t ReadWord() ;
		 void WriteWord(uint16_t word);

	private:
	};
}




#endif /* PROTOCOLS_RS485_RS485_H_ */
