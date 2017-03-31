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

	#define BUFFER_SIZE 32

	#define _USART0	0x00
	#define _USART1	0x01
	#define _USART2	0x02
	#define _USART3	0x03
	#define _USART4	0x04

	#define _UART_PARITY_NONE USART_Parity_No
	#define _UART_PARITY_EVEN USART_Parity_Even
	#define _UART_PARITY_ODD USART_Parity_Odd


	class SerialPort :public ICommunicationObject
	{
		USART_InitTypeDef USART_InitStruct;
		GPIO_InitTypeDef  GPIO_InitStruct;
		NVIC_InitTypeDef  NVIC_InitStructure;

		// rx
		char* _ring;
		//tx
		char* _tx;

		volatile unsigned char _head, _tail;
		volatile unsigned char _port;
		volatile unsigned int _parity;
		volatile bool _useInterupsTX;
		//volatile bool _useDmaRxTx;

	public:
		static SerialPort* pUsart0;
		static SerialPort* pUsart1;
		static SerialPort* pUsart2;
		static SerialPort* pUsart3;
		static SerialPort* pUsart4;
		static SerialPort* pUsart5;

		List<unsigned char> Data;
		unsigned int BuferSize;

		/*
		 * ������������
		 */
		SerialPort(unsigned char port, int baud);
		//SerialPort(unsigned char port, int baud, bool useInterupsTx_orDma); //true =InterupsTx , false = DMA


		SerialPort(unsigned char port, int baud, int parity);
		//SerialPort(unsigned char port, int baud, int parity,bool useInterupsTx_orDma); //true =InterupsTx , false = DMA


		/*
		 * �������� �����, ��������� ����� ������, ��������, ������������ ����
		 */
		void Init();

		/*
		 * ����������
		 */
		virtual ~SerialPort();

		/*
		 * ���������������� ����� ���������� �� ���� (����������)
		 */
		virtual void Received(char data);

		/*
		 * ���������������� ����� �������� �� ��������� (����������)
		 */
		virtual void Transmit();
	//
	//	/*
	//	 * ������ �������� ������ �� ��� (���������������� �����, ����������)
	//	 */
	//	virtual void HalfRecivedDmaComplete();
	//
	//	/*
	//	 * ����� ����� ������ �� ���  (���������������� �����, ����������)
	//	 */
	//	virtual void RecivedDmaComplete();
	//
	//	/*
	//	 * �������� �������� ������ ��� (���������������� �����, ���������� )
	//	 */
	//	virtual void HalfTransmitDmaComplete();
	//
	//	/*
	//	 * �������� ����� ������ ��� (���������������� �����, ����������)
	//	 */
	//	virtual void TransmitDmaComplete();

		/*
		 * ����� "������ �����" �� ����������� ������� (���������������� �����, ��� ��� ����� ����� ����� � �� ���)
		 */
		virtual bool GetAnswer(int timeOut);

		/*
		 * ����� �������� ������ �����
		 */
		void WriteByte(char byte);

	//	/*
	//	 * ����� ��� �� ��������
	//	 */
	//	void WriteArrayDma(char* array, int length);

		/*
		 * ����� ������ ����� � ���������� ������
		 */
		bool ReadByte(char* value, unsigned int timeOut);

	//
	//	/*
	//	 * ��������� ����� �� ���
	//	 */
	//	void StartReciveDma(int length);

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


		//todo ��� �������� ��� ��� ��� ����� ������� ����� ��� ���� ��� �������� ����������
		void EnableDmaRx();

		void EnableDmaTx();


	};
}

#endif /* CUSART_H_ */
