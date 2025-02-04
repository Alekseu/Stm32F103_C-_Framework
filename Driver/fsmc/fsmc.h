/*
 * fsmc.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_FSMC_FSMC_H_
#define DRIVER_FSMC_FSMC_H_

#include "../platform.h"

#define SRAM_ADR   ((uint32_t)0x60000000)  // Adresse vom RAM
#define  SRAM_MAX_ADR     0x20000

namespace Driver
{
	class Fsmc
	{
	public:
		static Fsmc* FsmcObj;
		void Init();
		void NInit();
		void RGI();
		bool RamTest();
		bool RamTest8();
	private:
		bool _useFsmc;
		uint8_t offset;
		uint16_t Read(uint32_t addr);
		void Write(uint32_t addr, uint16_t data);
		uint8_t Read8(uint32_t addr);
		void Write8(uint32_t addr, uint8_t data);

	private:
	};
}



#endif /* DRIVER_FSMC_FSMC_H_ */
