/*
 * 24AA08.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_EEPROM_24AA08_H_
#define DEVICE_EEPROM_24AA08_H_

#include "../deviceConfig.h"

namespace Device
{

	#include "../../Driver/i2c/i2c.h"

	using namespace Driver;

	//todo возможно лучше передавать драйвер i2c
	class EE24AA08 :I2c
	{
		public:

		static EE24AA08* EEObj;

		EE24AA08();
		~EE24AA08();

		void Init();

		bool Write(void* obj, uint16_t length);

		bool Read(void* obj, uint16_t length);

		private:

	};

}


#endif /* DEVICE_EEPROM_24AA08_H_ */
