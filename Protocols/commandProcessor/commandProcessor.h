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

	/*
	 * CommandProcessor example
	 *
	 *  В конструктор можно передать ссылку на любой обьект типа ICommunicationObject (usb,serialport,spi,i2c ...)
	 *
	 *
	 *  CommandProcessor _comProc((ICommunicationObject*)&_rs485);
	 *  _comProc.OnCommand =OnProcessCommand;
	 *   _comProc.Init();
	 *   _comProc.SendCommand(0x10,0,0);
	 *
	 *	void OnProcessCommand(uint8_t com, uint8_t* data, uint16_t length)
	 *	{
	 *		обработка комманды
	 *	}
	 *
	 */


	using namespace Driver;
	typedef void CommandProcessing(uint8_t, uint8_t*, uint16_t);

	class CommandProcessor : public Object
	{
		public:
		static CommandProcessor* ComPrObj;
		CommandProcessing* OnCommand;


		/*
		 * Конструктор с параметром
		 */
		CommandProcessor(ICommunicationObject* obj);
		~CommandProcessor();

		void Init();

		void SendCommand(uint8_t command, uint8_t* data, uint16_t length);

		void Recived(uint8_t data);


		const char* toString();

		private:
		ICommunicationObject* _comObj;

		unsigned int _lastActivity;

		unsigned int _dataLength, _dataIndex;

		bool _startPacket, _receivedPacket;

		unsigned char _dataLow;

		unsigned char _crc;

		unsigned char* _data;

		void SendData(uint8_t* data, uint16_t size);

	};
}



#endif /* PROTOCOLS_COMMANDPROCESSOR_COMMANDPROCESSOR_H_ */
