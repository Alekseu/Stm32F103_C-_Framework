/*
 * rs484.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "rs485.h"


namespace Protocol
{
	Rs485* Rs485::Rs485Obj=0;


	Rs485::Rs485(SerialPort::PortName port, uint16_t baud):SerialPort(port, baud)
	{
		_transmit = false;
	}

	Rs485::~Rs485()
	{

	}

	void Rs485::Init()
	{
		SerialPort::Init();
		RCC_APB2PeriphClockCmd(RxTxPheriph,ENABLE);
		GpioType.GPIO_Pin =  RxTxPin;
		GpioType.GPIO_Speed = GPIO_Speed_50MHz;
		GpioType.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(RxTxPort, &GpioType);
	}

	uint8_t Rs485::ReadByte()
	{
		return 0;
	}

	void Rs485::WriteByte(uint8_t byte)
	{

	}

	uint16_t Rs485::ReadWord()
	{
		return 0;
	}

	void Rs485::WriteWord(uint16_t word)
	{

	}

	void Rs485::SendData(uint8_t* data, uint16_t length)
	{
		_transmitEnable();
		SerialPort::SendData(data,length);
		_reciveEnable();
	}

	void Rs485::Recived(uint8_t data)
	{
		if(OnRecived!=0)
		{
			OnRecived(data);
		}
	}

	 const char* Rs485::toString()
	 {
		 return "Rs485";
	 }

	 void  Rs485::_transmitEnable()
	 {
		 //отключаем прерывание приема
		 InterruptController::DisableChannel((IRQn_Type)SerialPort::InterruptNumber);
		 RxTxPort->BSRR = RxTxPin;

	 }

	 void  Rs485::_reciveEnable()
	 {
		 InterruptController::EnableChannel((IRQn_Type)SerialPort::InterruptNumber);
		 RxTxPort->BRR = RxTxPin;
	 }

}


