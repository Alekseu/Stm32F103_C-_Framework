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
		 * Конструкторы
		 */
		SerialPort(PortName port, uint16_t baud);


		SerialPort(PortName port, uint16_t baud, PortParity parity);

		/*
		 * деструктор
		 */
		virtual ~SerialPort();

		/*
		 * Открытие порта, создается масив приема, передачи, настривается порт и т.д.
		 */
		void Init();


		/*
		 * переопределяемый метод прерывания по юарт (прерывание)
		 */
		virtual void Received(uint8_t data);

		/*
		 * переопределяемый метод передачи по прерываию (прерывание)
		 */
		virtual void Transmit();

		/*
		 * метод "получи ответ" за определеный таймаут (переопределяемый метод, так как можно ждать пакет и по дма)
		 */
		virtual bool GetAnswer(uint16_t timeOut);

		/*
		 * метод отправки одного байта
		 */
		void WriteByte(uint8_t byte);

		void WriteByte(uint8_t byte, uint8_t addr);


		/*
		 * метод чтения байта с кольцевого буфера
		 */
		bool ReadByte(uint8_t* value, uint16_t timeOut);


		/*
		 * метод чтения байта с кольцевого буфера
		 */
		uint8_t ReadByte();

		uint8_t ReadByte(uint8_t addr);


		/*
		 * метод отправки одного байта
		 */
		void WriteWord(uint16_t word);


		/*
		 * метод чтения байта с кольцевого буфера
		 */
		uint16_t ReadWord();

		/*
		 * метод отправки группы байт
		 */
		void SendData(uint8_t* data, uint16_t length);


		/*
		 * Очистка кольцевого буфера
		 */
		void ClearBuffer();


		/*
		 * События от внешних источников (dma, pwm, timer ...) в данном случае для дма, переопределяемые методы
		 */
		 virtual void RxComplete();
		 virtual void TxComplete();
		 virtual void RxHalfComplete();
		 virtual void TxHalfComplete();


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
