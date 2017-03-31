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
	public:
		static I2c* Iobj;

		I2c();
		~I2c();

		void Init();

		 virtual uint8_t ReadByte() ;
		 virtual void WriteByte(uint8_t byte);

		 virtual uint16_t ReadWord() ;
		 virtual void WriteWord(uint16_t word);


	private:
	};

}


#endif /* DRIVER_I2C_I2C_H_ */
