/*
 * spi.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "spi.h"

namespace Driver
{

	Spi* Spi::SObj =0;

	Spi::Spi(){}
	Spi::~Spi(){}


	void Spi::Init(){}

		 uint8_t Spi::ReadByte(){}

		 void Spi::WriteByte(uint8_t byte){}

		 uint16_t Spi::ReadWord(){}

		 void Spi::WriteWord(uint16_t word){}


}

