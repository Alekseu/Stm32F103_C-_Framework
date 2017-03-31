/*
 * CUsart.h
 *
 *  Created on: 22.03.2013
 *      Author: YVShvets
 */

#ifndef USART_H_
#define USART_H_

#include "../platform.h"

#include "../../Extention/delay.h"
#include "../../Extention/list.h"
#include "../Interface/CommunicationObject.h"

namespace Driver
{

	class SerialPort :public ICommunicationObject
	{
		UsartInit USART_InitStruct;
		GpioInit  GPIO_InitStruct;

		// rx
		uint8_t* _ring;
		//tx
		uint8_t* _tx;

		volatile uint8_t _head, _tail;
		volatile uint8_t _port;
		volatile uint16_t _parity;
		volatile bool _useInterupsTX;

	public:
		static SerialPort* pUsart0;
		static SerialPort* pUsart1;
		static SerialPort* pUsart2;
		static SerialPort* pUsart3;
		static SerialPort* pUsart4;
		static SerialPort* pUsart5;

		List<unsigned char> Data;
		uint16_t BuferSize;

		/*
		 * ������������
		 */
		SerialPort(uint8_t port, uint16_t baud);


		SerialPort(uint8_t port, uint16_t baud, uint16_t parity);

		/*
		 * ����������
		 */
		virtual ~SerialPort();

		/*
		 * �������� �����, ��������� ����� ������, ��������, ������������ ���� � �.�.
		 */
		void Init();


		/*
		 * ���������������� ����� ���������� �� ���� (����������)
		 */
		virtual void Received(uint8_t data);

		/*
		 * ���������������� ����� �������� �� ��������� (����������)
		 */
		virtual void Transmit();

		/*
		 * ����� "������ �����" �� ����������� ������� (���������������� �����, ��� ��� ����� ����� ����� � �� ���)
		 */
		virtual bool GetAnswer(uint16_t timeOut);

		/*
		 * ����� �������� ������ �����
		 */
		void WriteByte(uint8_t byte);


		/*
		 * ����� ������ ����� � ���������� ������
		 */
		bool ReadByte(uint8_t* value, uint16_t timeOut);


		/*
		 * ����� ������ ����� � ���������� ������
		 */
		uint8_t ReadByte();


		/*
		 * ����� �������� ������ �����
		 */
		void WriteWord(uint16_t word);


		/*
		 * ����� ������ ����� � ���������� ������
		 */
		uint16_t ReadWord();


		/*
		 * ������� ���������� ������
		 */
		void ClearBuffer();


		/*
		 * ��������� ���������� ������ (����������� �� ����� ������ �� ���)
		 */
		void DisableRxInterupt();

		/*
		 * ������� ���������� ������ ����� (����������� � ���� � ��� ��� ������ ������� ������ �����)
		 */
		void EnableRxInterupt();

		/*
		 *  ��������� ���������� �������� (����������� ��� �������� �� ���)
		 */
		void DisableTxInterupt();

		/*
		 *  �� ��� ���� ������ ��� ������� �������
		 */
		void EnableTxInterupt();

		/*
		 * object
		 */
		const char* toString();

	};
}

#endif /* CUSART_H_ */
