/*
 * commandProcessor.h
 *
 *  Created on: 31 ����� 2017 �.
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
		 * ����������� � ����������
		 */
		CommandProcessor(ICommunicationObject* obj){}
		~CommandProcessor(){}



	};
}



#endif /* PROTOCOLS_COMMANDPROCESSOR_COMMANDPROCESSOR_H_ */
