/*
 * i2c.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "i2c.h"

namespace Driver
{
	I2c* I2c::Iobj=0;

	I2c::I2c(){}
	I2c::~I2c(){}

	void I2c::Init(){}

	 uint8_t I2c::ReadByte(){}

	 void I2c::WriteByte(uint8_t byte){}

	 uint16_t ReadWord() {}

	 void WriteWord(uint16_t word){}



}
