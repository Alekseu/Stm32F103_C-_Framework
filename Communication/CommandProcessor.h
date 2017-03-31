///*
// * CommandProcessor.h
// *
// *  Created on: 07 февр. 2017 г.
// *      Author: hudienko_a
// */
//
//#ifndef COMMUNICATION_COMMANDPROCESSOR_H_
//#define COMMUNICATION_COMMANDPROCESSOR_H_
//
//#include "../Driver/usart/usart.h"
//
//#include "CommandsEnum.h"
//#include "../Driver/timer/timer.h"
//
//#include <string.h>
//
//#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
//#include "../StdPeriph/stm_lib/inc/stm32f10x_dma.h"
//#include "../StdPeriph/stm_lib/inc/misc.h"
//
//#include "../Extention/Object.h"
//
//
//using namespace Driver;
//
//typedef void CommandProcessing(Command, unsigned char*, unsigned int);
//
//
//
//class CommandProcessor :public SerialPort, public Object
//{
//public:
//	static CommandProcessor* pObj;
//	DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef  NVIC_InitStructure;
//	CommandProcessing* OnCommand;
//	Timer* timer;
//	unsigned char Width;
//	unsigned char Hieght;
//	unsigned int _lastActivity;
//	bool UseDMA;
//
//	unsigned int _dataLength, _dataIndex;
//
//	bool _startPacket, _receivedPacket;
//
//	unsigned char dataLow;
//
//	unsigned char _crc;
//
//	unsigned char* _data;
//
//	unsigned char _tx[50];
//
//	CommandProcessor(unsigned char port, int baud):SerialPort(port,baud)
//	{
//		OnCommand=0;
//		_startPacket = false;
//		_receivedPacket = false;
//		_crc = 0;
//		_dataLength = 0;
//		_dataIndex = 0;
//		dataLow =0;
//		_data=0;
//		timer=0;
//		_lastActivity=0;
//		Width =0;
//		Hieght =0;
//		pObj = this;
//		UseDMA = false;
//		_transferComplete = true;
//
//	}
//
//	~CommandProcessor()
//	{
//		int a=0;
//	}
//
//	void Init()
//	{
//		_data = new unsigned char[((Width/2)*Hieght)+10];
//
//	}
//
//
//	void SendCommand(Command com, char* data, unsigned int length)
//	{
//		unsigned char* tmp = new unsigned char[length + 1];
//
//		tmp[0] = (unsigned char) com;
//
//		memcpy(&tmp[1], data, length);
//
//		SendData(tmp, length + 1);
//
//		delete[] tmp;
//	}
//
//	unsigned char* GetBuff()
//	{
//		return _data;
//	}
//
//	void Received(char data)
//	{
//		if ((data == 0x13 && !_startPacket && !_receivedPacket) || ((timer->SystemTick-_lastActivity)>500))
//		{
//			_startPacket = true;
//			_crc = 0;
//			_dataLength = 0;
//			_dataIndex = 0;
//		}
//
//		_lastActivity = timer->SystemTick;
//
//		if (_startPacket && _dataIndex == 0)
//		{
//			_dataIndex = 1;
//
//			_crc ^= data;
//
//			return;
//		}
//
//		if (_dataIndex == 1)
//		{
//			dataLow = (unsigned char)data;
//
//			_dataIndex++;
//
//			_crc ^= data;
//
//			return;
//		}
//
//		if(_dataIndex==2)
//		{
//			_dataLength = (dataLow<<8)|data;
//			_dataIndex++;
//			_crc^=data;
//
////			if(UseDMA)
////			{
////				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
////				DMA1_Channel3->CNDTR =  (_dataLength+2);
////				DMA1_Channel3->CCR |= DMA_CCR1_EN;
////			}
//			return;
//		}
//
////		if(!UseDMA)
////		{
//			if (_dataIndex - 3 >= _dataLength)
//			{
//				if (_crc == data)
//				{
//					_receivedPacket = true;
//
//					if(OnCommand!=0)
//					{
//						OnCommand((Command)_data[0],&_data[1],_dataLength);
//					}
//				}
//
//				_startPacket = false;
//				_receivedPacket = false;
//				_dataLength =0;
//				_dataIndex=0;
//			}
//			else
//			{
//				_crc ^= data;
//				_data[_dataIndex++ - 3] = data;
//			}
//		//}
//	}
//
//	void DmaReciveComplete()
//	{
////		DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR1_EN);
////
////		for(int i=_dataLength/2;i<_dataLength+1;i++)
////		{
////			_crc^=_data[i];
////		}
////
////		unsigned char paketcrc =_data[_dataLength+1];
////
////		if (_crc == paketcrc)
////		{
////			if(OnCommand!=0)
////			{
////				OnCommand((Command)_data[1],&_data[2],_dataLength);
////			}
////		}
////		_startPacket = false;
////		_receivedPacket = false;
////		_dataLength =0;
////		_dataIndex=0;
////		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	}
//
//	void DmaHalfReciveComplete()
//	{
//		for(int i=1;i<(_dataLength/2);i++)
//		{
//			_crc^=_data[i];
//		}
//	}
//
//	void DmaTransferComplete()
//	{
//		_transferComplete = true;
//	}
//
//private:
//
//	void SendData(unsigned char* data, int size)
//	{
//		unsigned char crc;
////		if(UseDMA )
////		{
////			DMA1_Channel2->CCR &= ~DMA_CCR1_EN;
////			_tx[0]= 0x13; crc = 0x13;
////			_tx[1]=(size>>8&0xFF); crc ^= (unsigned char)(size>>8&0xFF);
////			_tx[2]=(size&0xFF); crc ^= (unsigned char)(size&0xFF);
////			for (unsigned char i = 0; i < size; i++)
////			{
////				_tx[i+3]= data[i]; 	crc ^= data[i];
////			}
////			_tx[size+3] = crc;
////			while(!_transferComplete);
////			_transferComplete = false;
////			DMA1_Channel2->CNDTR =  (size+4);
////			DMA1_Channel2->CCR |= DMA_CCR1_EN;
////		}
////		else
////		{
//			WriteByte(0x13);			crc = 0x13;
//
//			WriteByte(size>>8&0xFF); crc ^= (unsigned char)(size>>8&0xFF);
//			WriteByte(size&0xFF); crc ^= (unsigned char)(size&0xFF);
//
//			for (unsigned char i = 0; i < size; i++)
//			{
//				WriteByte(data[i]); 	crc ^= data[i];
//			}
//
//			WriteByte(crc);
//		//}
//
//	}
//
//	bool _transferComplete;
//
//};
//
//
//
//#endif /* COMMUNICATION_COMMANDPROCESSOR_H_ */
