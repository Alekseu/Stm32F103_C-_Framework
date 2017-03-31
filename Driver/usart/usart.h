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
		 * Конструкторы
		 */
		SerialPort(uint8_t port, uint16_t baud);


		SerialPort(uint8_t port, uint16_t baud, uint16_t parity);

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


		/*
		 * метод чтения байта с кольцевого буфера
		 */
		bool ReadByte(uint8_t* value, uint16_t timeOut);


		/*
		 * метод чтения байта с кольцевого буфера
		 */
		uint8_t ReadByte();


		/*
		 * метод отправки одного байта
		 */
		void WriteWord(uint16_t word);


		/*
		 * метод чтения байта с кольцевого буфера
		 */
		uint16_t ReadWord();


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

		/*
		 * object
		 */
		const char* toString();

	};
}

#endif /* CUSART_H_ */
