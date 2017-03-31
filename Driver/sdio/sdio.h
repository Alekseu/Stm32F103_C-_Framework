/*
 * sdio.h
 *
 *  Created on: 31 ����� 2017 �.
 *      Author: hudienko_a
 */

#ifndef DRIVER_SDIO_SDIO_H_
#define DRIVER_SDIO_SDIO_H_

#include "../platform.h"
#include "../Interface/CommunicationObject.h"

namespace Driver
{
	class Sdio : public ICommunicationObject
	{
	public:
		static Sdio* SdObj;

		void Init();

	private:
	};
}



#endif /* DRIVER_SDIO_SDIO_H_ */
