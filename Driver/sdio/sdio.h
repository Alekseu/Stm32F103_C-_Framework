/*
 * sdio.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SDIO_SDIO_H_
#define DRIVER_SDIO_SDIO_H_

#include "../platform.h"

namespace Driver
{
	class Sdio
	{
	public:
		static Sdio* SdObj;

		void Init();

	private:
	};
}



#endif /* DRIVER_SDIO_SDIO_H_ */
