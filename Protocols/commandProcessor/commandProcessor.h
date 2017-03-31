/*
 * commandProcessor.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef PROTOCOLS_COMMANDPROCESSOR_COMMANDPROCESSOR_H_
#define PROTOCOLS_COMMANDPROCESSOR_COMMANDPROCESSOR_H_

#include "../protocolConfig.h"
#include "../../Driver/Interface/CommunicationObject.h"

namespace Protocol
{
using namespace Driver;


	class CommandProcessor
	{
		public:
		/*
		 * Конструктор с параметром
		 */
		CommandProcessor(ICommunicationObject* obj){}
		~CommandProcessor(){}



	};
}



#endif /* PROTOCOLS_COMMANDPROCESSOR_COMMANDPROCESSOR_H_ */
