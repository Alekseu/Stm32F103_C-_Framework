/*
 * i2c.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_I2C_I2C_H_
#define DRIVER_I2C_I2C_H_

#include "../platform.h"

namespace Driver
{

	class I2c
	{
	public:
		static I2c* Iobj;

		I2c();
		~I2c();

		void Init();

		void Write(uint8_t byte);

		uint8_t Read();


	private:
	};

}


#endif /* DRIVER_I2C_I2C_H_ */
