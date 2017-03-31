/*
 * commandProcessor.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "commandProcessor.h"


namespace Protocol
{
	CommandProcessor* CommandProcessor::ComPrObj=0;

	// прерывание
	void CommunicationRxInterrupt(uint8_t byte)
	{
		CommandProcessor::ComPrObj->Recived(byte);
	}

	CommandProcessor::CommandProcessor(ICommunicationObject* obj)
	{
		ComPrObj = this;
		_comObj = obj;
		OnCommand=0;
		_dataLength=0;
		_dataIndex=0;
		_crc=0;
		_startPacket=false;
		_data=0;
		_dataLow =0;
		_receivedPacket = false;
		_lastActivity=0;
	}

	CommandProcessor::~CommandProcessor()
	{

	}

	void CommandProcessor::Init()
	{
		if(_comObj!=0)
		{
			_comObj->OnRecived = CommunicationRxInterrupt;
		}
	}

	void CommandProcessor::Recived(uint8_t data)
	{
		if ((data == 0x13 && !_startPacket && !_receivedPacket) )//|| ((timer->SystemTick-_lastActivity)>500))
		{
			_startPacket = true;
			_crc = 0;
			_dataLength = 0;
			_dataIndex = 0;
		}

		//_lastActivity = timer->SystemTick;

		if (_startPacket && _dataIndex == 0)
		{
			_dataIndex = 1;

			_crc ^= data;

			return;
		}

		if (_dataIndex == 1)
		{
			_dataLow = (unsigned char)data;

			_dataIndex++;

			_crc ^= data;

			return;
		}

		if(_dataIndex==2)
		{
			_dataLength = (_dataLow<<8)|data;
			_dataIndex++;
			_crc^=data;


			return;
		}


		if (_dataIndex - 3 >= _dataLength)
		{
			if (_crc == data)
			{
				_receivedPacket = true;

				if(OnCommand!=0)
				{
					OnCommand(_data[0],&_data[1],_dataLength);
				}
			}

			_startPacket = false;
			_receivedPacket = false;
			_dataLength =0;
			_dataIndex=0;
		}
		else
		{
			_crc ^= data;
			_data[_dataIndex++ - 3] = data;
		}


	}

	void CommandProcessor::SendCommand(uint8_t command, uint8_t* data, uint16_t length)
	{
		unsigned char* tmp = new unsigned char[length + 1];

		tmp[0] = (unsigned char) command;

		memcpy(&tmp[1], data, length);

		SendData(tmp, length + 1);

		delete[] tmp;
	}

	const char* CommandProcessor::toString()
	{
		return "CommandProcessor";
	}

	void CommandProcessor::SendData(uint8_t* data, uint16_t size)
	{
		unsigned char crc;

		_comObj->WriteByte(0x13);			crc = 0x13;

		_comObj->WriteByte(size>>8&0xFF); crc ^= (unsigned char)(size>>8&0xFF);
		_comObj->WriteByte(size&0xFF); crc ^= (unsigned char)(size&0xFF);

		for (unsigned char i = 0; i < size; i++)
		{
			_comObj->WriteByte(data[i]); 	crc ^= data[i];
		}

		_comObj->WriteByte(crc);


	}

}


