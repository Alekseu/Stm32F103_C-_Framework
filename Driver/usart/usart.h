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
#include "../Interface/DriverObject.h"

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

		volatile uint16_t _parity;
		volatile bool _useInterupsTX;

	public:
		enum PortName
		{
			COM1 = USART1_BASE,
			COM2 = USART2_BASE,
			COM3 = USART3_BASE,
			COM4 = UART4_BASE,
			COM5 = UART5_BASE
		};

		enum PortParity
		{
			Parity_No =0,
			Parity_Even = 0x0400,
			Parity_Odd = 0x0600
		};

	public:
		volatile PortName Port;
		volatile uint16_t Baud;

		static SerialPort* pUsart0;
		static SerialPort* pUsart1;
		static SerialPort* pUsart2;
		static SerialPort* pUsart3;
		static SerialPort* pUsart4;
		static SerialPort* pUsart5;

		List<unsigned char> Data;
		uint16_t BuferSize;
		uint16_t InterruptNumber;
		DriverObject* DriverObj;

		/*
		 * ������������
		 */
		SerialPort(PortName port, uint16_t baud);


		SerialPort(PortName port, uint16_t baud, PortParity parity);

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

		void WriteByte(uint8_t byte, uint8_t addr);


		/*
		 * ����� ������ ����� � ���������� ������
		 */
		bool ReadByte(uint8_t* value, uint16_t timeOut);


		/*
		 * ����� ������ ����� � ���������� ������
		 */
		uint8_t ReadByte();

		uint8_t ReadByte(uint8_t addr);


		/*
		 * ����� �������� ������ �����
		 */
		void WriteWord(uint16_t word);


		/*
		 * ����� ������ ����� � ���������� ������
		 */
		uint16_t ReadWord();

		/*
		 * ����� �������� ������ ����
		 */
		void SendData(uint8_t* data, uint16_t length);


		/*
		 * ������� ���������� ������
		 */
		void ClearBuffer();


		/*
		 * ������� �� ������� ���������� (dma, pwm, timer ...) � ������ ������ ��� ���, ���������������� ������
		 */
		 virtual void RxComplete();
		 virtual void TxComplete();
		 virtual void RxHalfComplete();
		 virtual void TxHalfComplete();


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


		/*
		 * Interrupt
		 */
		static void InterruptWraper(void);

	};
}

#endif /* CUSART_H_ */
