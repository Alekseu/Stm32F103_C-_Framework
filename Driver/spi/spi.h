/*
 * spi.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SPI_SPI_H_
#define DRIVER_SPI_SPI_H_

#include "../platform.h"
#include "../nvic/nvic.h"
#include "../Interface/CommunicationObject.h"
#include "../../Extention/delay.h"

namespace Driver
{

/*
 * Spi example
 * Spi* _spi = new Spi(Spi::sn_SPI1,Spi::Master,Spi::Sys_div_128);
 * _spi->Init();
 */


	class Spi :public ICommunicationObject
	{
		SpiInit   SPI_InitStruct;
		GpioInit  GPIO_InitStruct;

	public:
		enum SpiNum
		{
			sn_SPI1 = SPI1_BASE,
			sn_SPI2 = SPI2_BASE,
			sn_SPI3 = SPI3_BASE
		};

		enum SpiType
		{
			Master,
			Slave
		};

		enum SpiSpeed
		{
			Sys_div_2 = 0x0000,
			Sys_div_4 = 0x0008,
			Sys_div_8 = 0x0010,
			Sys_div_16 = 0x0018,
			Sys_div_32 = 0x0020,
			Sys_div_64 = 0x0028,
			Sys_div_128 = 0x0030,
			Sys_div_256 = 0x0038
		};


	public:
		static Spi* S1Obj;
		static Spi* S2Obj;
		static Spi* S3Obj;

		Spi(SpiNum spiNum, SpiType type, SpiSpeed speed );

		virtual ~Spi();

		void Init();

		uint8_t ReadByte();

		uint8_t ReadByte(uint8_t addr);

		bool ReadByte(uint8_t* value, uint16_t timeOut);

		void WriteByte(uint8_t byte);

		void WriteByte(uint8_t byte, uint8_t addr);

		uint16_t ReadWord();

		void WriteWord(uint16_t word);

		void SendData(uint8_t* data, uint16_t length);

		/*
		 * interrupt
		 */
		virtual void Received(uint8_t byte);

		/*
		 * Interrupt
		 */
		static void InterruptWraper(void);

		const char* toString();

	private:

		SpiNum _spiNum;
		SpiType _type;
		SpiSpeed _speed;

		uint8_t* _ring;
		volatile uint8_t _head, _tail;
		uint16_t BuferSize;

	};

}

#endif /* DRIVER_SPI_SPI_H_ */
