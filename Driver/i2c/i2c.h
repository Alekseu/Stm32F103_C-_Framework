/*
 * i2c.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_I2C_I2C_H_
#define DRIVER_I2C_I2C_H_

#include "../platform.h"

#include "../Interface/CommunicationObject.h"

namespace Driver
{

	class I2c :public ICommunicationObject
	{
		I2cInit  I2C_InitStructure;
		GpioInit  GPIO_InitStructure;

	public:
		enum I2CType
		{
			Master,
			Slave
		};

	public:
		static I2c* Iobj;

		I2c(uint16_t rate, uint8_t addr, I2CType type);
		~I2c();

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
		uint16_t _rate;
		uint8_t  _badr;
		 I2CType _type;

	};

}


#endif /* DRIVER_I2C_I2C_H_ */
