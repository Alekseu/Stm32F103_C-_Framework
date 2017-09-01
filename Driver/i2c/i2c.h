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
#include "../../Extention/delay.h"

namespace Driver
{

	class I2c :public ICommunicationObject
	{
		I2cInit  I2C_InitStructure;
		GpioInit  GPIO_InitStructure;

	public:
		enum I2cNum
		{
			in_I2C1 = I2C1_BASE,
			in_I2C2 = I2C2_BASE
		};

		enum I2CType
		{
			Master,
			Slave
		};

		enum I2CSpeed
		{
			s_50kHz = 50000,
			s_100kHz= 100000,
			s_400kHz = 400000
		};

	public:
		static I2c* I1Obj;
		static I2c* I2Obj;

		I2c(I2cNum i2cNum, uint8_t addr, I2CType type, I2CSpeed speed);
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
		uint8_t  _badr;
		I2cNum _i2cNum;
		 I2CType _type;
		 I2CSpeed _speed;

	};

}


#endif /* DRIVER_I2C_I2C_H_ */
