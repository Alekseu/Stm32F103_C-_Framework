/*
 * HSpi.h
 *
 *  Created on: 23 θώνÿ 2018 γ.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SPI_PSPI_H_
#define DRIVER_SPI_PSPI_H_


#include "../platform.h"
//#include "../nvic/nvic.h"
#include "../Interface/CommunicationObject.h"
//#include "../../Extention/delay.h"

namespace Driver
{
	/*
	 * Example
	 * PSpi _pspi(PSpi::Master);
	 * _pspi.Init();
	 * _pspi.WriteByte(0x00);
	 */
	class PSpi: public ICommunicationObject
	{
	public:
		enum SpiType
				{
					Master,
					Slave
				};


	public:

		static PSpi* HsObj;

		PSpi(SpiType type);

		 void Init();

		 uint8_t ReadByte() ;
		 uint8_t ReadByte(uint8_t addr) ;

		 bool ReadByte(uint8_t* value, uint16_t timeOut);
		 void WriteByte(uint8_t byte);
		 void WriteByte(uint8_t byte, uint8_t addr);
		 void WriteByte(uint8_t byte, uint8_t baddr, uint8_t addr);
		 void WriteBytes(uint8_t* val, uint16_t size, uint8_t baddr, uint8_t addr);

		 uint16_t ReadWord() ;
		 void WriteWord(uint16_t word);

		 void SendData(uint8_t* data, uint16_t length);
		//virtual uint16_t ReadData(uint8_t* mass)=0;

		/*
		 * interrupt
		 */
		 void Received(uint8_t byte);

		 ~PSpi(){}

	private:
		GpioInit  GPIO_InitStruct;

	};
}


#endif /* DRIVER_SPI_PSPI_H_ */
