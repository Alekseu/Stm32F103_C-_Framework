/*
 * fsmc.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_FSMC_FSMC_H_
#define DRIVER_FSMC_FSMC_H_

#include "../platform.h"

namespace Driver
{
	class Fsmc
	{
	public:
		static Fsmc* FsmcObj;
		void Init();
	private:
	};
}



#endif /* DRIVER_FSMC_FSMC_H_ */
