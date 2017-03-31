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
		 * Конструкторы
		 */
		SerialPort(unsigned char port, int baud);
		//SerialPort(unsigned char port, int baud, bool useInterupsTx_orDma); //true =InterupsTx , false = DMA


		SerialPort(unsigned char port, int baud, int parity);
		//SerialPort(unsigned char port, int baud, int parity,bool useInterupsTx_orDma); //true =InterupsTx , false = DMA


		/*
		 * Открытие порта, создается масив приема, передачи, настривается порт
		 */
		void Init();

		/*
		 * деструктор
		 */
		virtual ~SerialPort();

		/*
		 * переопределяемый метод прерывания по юарт (прерывание)
		 */
		virtual void Received(char data);

		/*
		 * переопределяемый метод передачи по прерываию (прерывание)
		 */
		virtual void Transmit();
	//
	//	/*
	//	 * Приаем половины пакета по дма (переопределяемый метод, прерывание)
	//	 */
	//	virtual void HalfRecivedDmaComplete();
	//
	//	/*
	//	 * Прием всего пакета по дма  (переопределяемый метод, прерывание)
	//	 */
	//	virtual void RecivedDmaComplete();
	//
	//	/*
	//	 * передача половины пакета дма (переопределяемый метод, прерывание )
	//	 */
	//	virtual void HalfTransmitDmaComplete();
	//
	//	/*
	//	 * Передача всего пакета дма (переопределяемый метод, прерывание)
	//	 */
	//	virtual void TransmitDmaComplete();

		/*
		 * метод "получи ответ" за определеный таймаут (переопределяемый метод, так как можно ждать пакет и по дма)
		 */
		virtual bool GetAnswer(int timeOut);

		/*
		 * метод отправки одного байта
		 */
		void WriteByte(char byte);

	//	/*
	//	 * пнуть дма на передачу
	//	 */
	//	void WriteArrayDma(char* array, int length);

		/*
		 * метод чтения байта с кольцевого буфера
		 */
		bool ReadByte(char* value, unsigned int timeOut);

	//
	//	/*
	//	 * запустить прием по дма
	//	 */
	//	void StartReciveDma(int length);

		/*
		 * Очистка кольцевого буфера
		 */
		void ClearBuffer();


		/*
		 * отключить прерывание приема (применяется во время приема по дма)
		 */
		void DisableRxInterupt();

		/*
		 * Включит прерывание приема байта (применяется в паре с дма для приема пакетов разной длины)
		 */
		void EnableRxInterupt();

		/*
		 *  отключить прерывание передачи (применяется при отправке по дма)
		 */
		void DisableTxInterupt();

		/*
		 *  хз для чего просто для полноты картины
		 */
		void EnableTxInterupt();


		//todo под вопросом так как это может сделать класс дма если ему передать устройство
		void EnableDmaRx();

		void EnableDmaTx();


	};
}

#endif /* CUSART_H_ */
