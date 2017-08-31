/*
 * spi.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SPI_SPI_H_
#define DRIVER_SPI_SPI_H_

#include "../platform.h"
#include "../Interface/CommunicationObject.h"

namespace Driver
{

	class Spi :public ICommunicationObject
	{
	public:
		enum SpiType
		{
			Master,
			Slave
		};

	public:
		static Spi* SObj;

		Spi();
		virtual ~Spi();

		void Init();

		virtual uint8_t ReadByte() ;
		virtual void WriteByte(uint8_t byte);

		virtual uint16_t ReadWord();
		virtual void WriteWord(uint16_t word);

	private:
		uint8_t rxtx(uint8_t byte);
		uint16_t rxtx(uint16_t byte);
	};

}

#endif /* DRIVER_SPI_SPI_H_ */
