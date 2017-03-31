/*
 * spi.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SPI_SPI_H_
#define DRIVER_SPI_SPI_H_

#include "../platform.h"

namespace Driver
{

	enum SpiType
	{
		Master,
		Slave
	};

	class Spi
	{
	public:
		static Spi* SObj;

		Spi();
		virtual ~Spi();

		virtual uint8_t Read8();
		virtual void Write8(uint8_t byte);

		virtual uint16_t Read16();
		virtual void Write16(uint16_t word);

	private:
		uint8_t rxtx(uint8_t byte);
		uint16_t rxtx(uint16_t byte);
	};

}

#endif /* DRIVER_SPI_SPI_H_ */
