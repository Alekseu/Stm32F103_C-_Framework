/*
 * Kernel.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef OS_KERNEL_H_
#define OS_KERNEL_H_

#include "../Driver/platform.h"

namespace Os
{

	class Kernel
	{
		public:
			static Kernel* KernelObj;
			Kernel();
			~Kernel();

			void Init();

			void SwitchContext();

		private:
	};
}


#endif /* OS_KERNEL_H_ */
