/*
 * CUsart.cpp
 *
 *  Created on: 22.03.2013
 *      Author: YVShvets
 */

#include "usart.h"
#include "../nvic/nvic.h"

namespace Driver
{

	SerialPort *SerialPort::pUsart0 = 0;
	SerialPort *SerialPort::pUsart1 = 0;
	SerialPort *SerialPort::pUsart2 = 0;
	SerialPort *SerialPort::pUsart3 = 0;
	SerialPort *SerialPort::pUsart4 = 0;

		// ----------- Прерывания приема передачи ---------------------
		void  USART1_IRQ(void)
		{
			if (USART1->SR & USART_SR_RXNE)
			{
				USART1->SR&=~USART_SR_RXNE;
				uint8_t tmp =USART1->DR;
				SerialPort::pUsart0->Received(tmp);
			}
		}

		void  USART2_IRQ(void)
		{
			if (USART2->SR & USART_SR_RXNE)
			{
				USART2->SR&=~USART_SR_RXNE;
				uint8_t tmp =USART2->DR;
				SerialPort::pUsart1->Received(tmp);
			}
		}

		void  USART3_IRQ(void)
		{
			if (USART3->SR & USART_SR_RXNE)
			{
				USART3->SR&=~USART_SR_RXNE;
				uint8_t tmp =USART3->DR;
				SerialPort::pUsart2->Received(tmp);
			}
		}

		void  USART4_IRQ(void)
		{
			if (UART4->SR & USART_SR_RXNE)
			{
				UART4->SR&=~USART_SR_RXNE;
				uint8_t tmp =UART4->DR;
				SerialPort::pUsart3->Received(tmp);
			}
		}

		void  USART5_IRQ(void)
		{
			if (UART5->SR & USART_SR_RXNE)
			{
				UART5->SR&=~USART_SR_RXNE;
				uint8_t tmp =UART5->DR;
				SerialPort::pUsart4->Received(tmp);
			}
		}

		void DriverEvent(Source source, uint8_t communication)
		{
			if(source.DriverSource!=DmaSource) return;

			SerialPort* pointer = 0;

			switch (communication)
			{

			case _USART0:
				pointer =SerialPort::pUsart0;
				break;
			case _USART1:
				pointer =SerialPort::pUsart1;
				break;
			case _USART2:
				pointer =SerialPort::pUsart2;
				break;
			case _USART3:
				pointer =SerialPort::pUsart3;
				break;
			case _USART4:
				pointer=SerialPort::pUsart4;
				break;

			}

			switch(source.EventType)
			{
				case RxComplete:
					pointer->RxComplete();
					break;
				case TxComplete:
					pointer->TxComplete();
					break;

				case RxHalfComplete:
					pointer->RxHalfComplete();
					break;

				case TxHalfComplete:
					pointer->TxHalfComplete();
					break;
			}
		}


	SerialPort::SerialPort(uint8_t port, uint16_t baud):ICommunicationObject()
	{
		BuferSize=0;
		DriverObj =0;
		_ring=0;
		_tx=0;

		Port = port;
		Baud = baud;
		_head = 0;
		_tail = 0;

		switch (Port)
		{

		case _USART0:
			pUsart0 = this;
			InterruptNumber = USART1_IRQn;
			break;
		case _USART1:
			pUsart1 = this;
			InterruptNumber = USART2_IRQn;
			break;
		case _USART2:
			pUsart2 = this;
			InterruptNumber = USART3_IRQn;
			break;
		case _USART3:
			pUsart3 = this;
			InterruptNumber = UART4_IRQn;
			break;
		case _USART4:
			pUsart4 = this;
			InterruptNumber = UART5_IRQn;
			break;

		}

		switch (Port)
		{
		case _USART0:
				  /* Enable GPIOA clock                                                   */
				  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
				  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);
				  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
				  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
				  //Инициализации вывода PA9 - USART1_Tx
				  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; //Настройки вывода PA9
				  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
				  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
				  GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				  //Инициализации вывода PA10 - USART1_Rx
				  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //Настройки вывода PA10
				  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
				  GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА


				  USART_InitStruct.USART_BaudRate            = baud;
				  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
				  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
				  USART_InitStruct.USART_Parity              = USART_Parity_No ;
				  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
				  USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
				  USART_Init(USART1, &USART_InitStruct);


				  USART_Cmd(USART1, ENABLE);
				  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
				  InterruptController::SetHandler(USART1_IRQn,USART1_IRQ);
				  InterruptController::EnableChannel(USART1_IRQn);


			break;
		case _USART1:

			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
			 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			//Инициализации вывода PA9 - USART1_Tx
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //Настройки вывода PA9
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
			GPIO_Init(GPIOA, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

			//Инициализации вывода PA10 - USART1_Rx
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; //Настройки вывода PA10
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
			GPIO_Init(GPIOA, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

			//Инициализация USART1
			USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
			USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
			USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
			USART_InitStruct.USART_Parity = USART_Parity_No; //Без проверки четности
			USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
			USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
			USART_Init(USART2, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

			USART_Cmd(USART2, ENABLE); //Включаем USART2

			USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
			InterruptController::SetHandler(USART2_IRQn,USART2_IRQ);
			InterruptController::EnableChannel(USART2_IRQn);


			break;
		case _USART2:
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
					 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			//Инициализации вывода PA9 - USART1_Tx
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //Настройки вывода PA9
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
			GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

			//Инициализации вывода PA10 - USART1_Rx
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //Настройки вывода PA10
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
			GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

			//Инициализация USART1
			USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
			USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
			USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
			USART_InitStruct.USART_Parity = USART_Parity_No; //Без проверки четности
			USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
			USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
			USART_Init(USART3, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

			USART_Cmd(USART3, ENABLE); //Включаем USART3
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
			InterruptController::SetHandler(USART3_IRQn,USART3_IRQ);
			InterruptController::EnableChannel(USART3_IRQn);

			break;
		case _USART3:
			   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
			   RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			//Инициализации вывода PA9 - USART1_Tx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //Настройки вывода PA9
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
				GPIO_Init(GPIOC, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализации вывода PA10 - USART1_Rx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //Настройки вывода PA10
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
				GPIO_Init(GPIOC, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализация USART1
				USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
				USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
				USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
				USART_InitStruct.USART_Parity = USART_Parity_No; //Без проверки четности
				USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
				USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
				USART_Init(UART4, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

				USART_Cmd(UART4, ENABLE); //Включаем USART4
				USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
				InterruptController::SetHandler(UART4_IRQn,USART4_IRQ);
				InterruptController::EnableChannel(UART4_IRQn);

			break;
		case _USART4:
			   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
					   RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
			//Инициализации вывода PA9 - USART1_Tx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; //Настройки вывода PA9
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
				GPIO_Init(GPIOC, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализации вывода PA10 - USART1_Rx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //Настройки вывода PA10
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
				GPIO_Init(GPIOD, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализация USART1
				USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
				USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
				USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
				USART_InitStruct.USART_Parity = USART_Parity_No; //Без проверки четности
				USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
				USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
				USART_Init(UART5, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

				USART_Cmd(UART5, ENABLE); //Включаем USART4
				USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
				InterruptController::SetHandler(UART5_IRQn,USART5_IRQ);
				InterruptController::EnableChannel(UART5_IRQn);


			break;

		default:
			break;
		}

		//sei();
	}

	SerialPort::SerialPort(uint8_t port, uint16_t baud, uint16_t parity)
	{
		BuferSize=0;
		DriverObj=0;
		InterruptNumber=0;
		_ring=0;
		_tx=0;

		Port = port;
		Baud=baud;
		_head = 0;
		_tail = 0;

		switch (Port)
		{

		case _USART0:
			pUsart0 = this;
			break;
		case _USART1:
			pUsart1 = this;
			break;
		case _USART2:
			pUsart2 = this;
			break;
		case _USART3:
			pUsart3 = this;
			break;
		case _USART4:
			pUsart4 = this;
			break;

		}
		switch (Port)
			{
		case _USART0:
					  /* Enable GPIOA clock                                                   */
					  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
					  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
					  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);
					  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
					  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
					  //Инициализации вывода PA9 - USART1_Tx
					  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; //Настройки вывода PA9
					  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
					  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
					  GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

					  //Инициализации вывода PA10 - USART1_Rx
					  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //Настройки вывода PA10
					  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
					  GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА


					  /*Configure peripheral I/O remapping */


					  USART_InitStruct.USART_BaudRate            = baud;
					  USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
					  USART_InitStruct.USART_StopBits            = USART_StopBits_1;
					  USART_InitStruct.USART_Parity              = parity ;
					  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
					  USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
					  USART_Init(USART1, &USART_InitStruct);


					  USART_Cmd(USART1, ENABLE);
					  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
					  InterruptController::SetHandler(USART1_IRQn,USART1_IRQ);
					  InterruptController::EnableChannel(USART1_IRQn);

				break;
			case _USART1:

				 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
				 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
				//Инициализации вывода PA9 - USART1_Tx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //Настройки вывода PA9
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
				GPIO_Init(GPIOA, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализации вывода PA10 - USART1_Rx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; //Настройки вывода PA10
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
				GPIO_Init(GPIOA, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализация USART1
				USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
				USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
				USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
				USART_InitStruct.USART_Parity = parity; //Без проверки четности
				USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
				USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
				USART_Init(USART2, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

				USART_Cmd(USART2, ENABLE); //Включаем USART2

				USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
				InterruptController::SetHandler(USART2_IRQn,USART2_IRQ);
				InterruptController::EnableChannel(USART2_IRQn);

				break;
			case _USART2:
				 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
						 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
				//Инициализации вывода PA9 - USART1_Tx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //Настройки вывода PA9
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
				GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализации вывода PA10 - USART1_Rx
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //Настройки вывода PA10
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
				GPIO_Init(GPIOB, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

				//Инициализация USART1
				USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
				USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
				USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
				USART_InitStruct.USART_Parity = parity; //Без проверки четности
				USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
				USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
				USART_Init(USART3, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

				USART_Cmd(USART3, ENABLE); //Включаем USART3
				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
				InterruptController::SetHandler(USART3_IRQn,USART3_IRQ);
				InterruptController::EnableChannel(USART3_IRQn);

				break;
			case _USART3:
				   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
				   RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
				//Инициализации вывода PA9 - USART1_Tx
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //Настройки вывода PA9
					GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
					GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
					GPIO_Init(GPIOC, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

					//Инициализации вывода PA10 - USART1_Rx
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //Настройки вывода PA10
					GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
					GPIO_Init(GPIOC, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

					//Инициализация USART1
					USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
					USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
					USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
					USART_InitStruct.USART_Parity = parity; //Без проверки четности
					USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
					USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
					USART_Init(UART4, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

					USART_Cmd(UART4, ENABLE); //Включаем USART4
					USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
					InterruptController::SetHandler(UART4_IRQn,USART4_IRQ);
					InterruptController::EnableChannel(UART4_IRQn);

				break;
			case _USART4:
				   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
						   RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
				//Инициализации вывода PA9 - USART1_Tx
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; //Настройки вывода PA9
					GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //Скорость порта максимальная
					GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //Режим альтернативной функции, выход Push-Pull
					GPIO_Init(GPIOC, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

					//Инициализации вывода PA10 - USART1_Rx
					GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //Настройки вывода PA10
					GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
					GPIO_Init(GPIOD, &GPIO_InitStruct); //Заданные настройки сохраняем в регистрах GPIOА

					//Инициализация USART1
					USART_InitStruct.USART_BaudRate = baud; //Скорость обмена 9600 бод
					USART_InitStruct.USART_WordLength = USART_WordLength_8b; //Длина слова 8 бит
					USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 стоп-бит
					USART_InitStruct.USART_Parity = parity; //Без проверки четности
					USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Без аппаратного контроля
					USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Включен передатчик и приемник USART1
					USART_Init(UART5, &USART_InitStruct); //Заданные настройки сохраняем в регистрах USART1

					USART_Cmd(UART5, ENABLE); //Включаем USART4
					USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
					InterruptController::SetHandler(UART5_IRQn,USART5_IRQ);
					InterruptController::EnableChannel(UART5_IRQn);

				break;

			default:
				break;
			}

		//sei();
	}

	SerialPort::~SerialPort()
	{
	}

	void SerialPort::Init()
	{
		_ring = new uint8_t[BuferSize];
		if(DriverObj!=0)
		{
			DriverObj->OnEvent = DriverEvent;
			DriverObj->CommunicationDriver = Port;
		}
	}

	void SerialPort::WriteByte(uint8_t byte)
	{
		switch (Port)
		{

		case _USART0:
			while(!(USART1->SR & USART_SR_TXE));
			  USART1->DR = byte;
			break;

		case _USART1:
				while(!(USART2->SR & USART_SR_TXE));
				USART2->DR = byte;
			break;

		case _USART2:
			while(!(USART3->SR & USART_SR_TXE));
				  USART3->DR = byte;
			break;

		case _USART3:
				while(!(UART4->SR & USART_SR_TXE));
			  UART4->DR = byte;
			break;
		case _USART4:
			while(!(UART5->SR & USART_SR_TXE));
				  UART5->DR = byte;
			break;
		}

	}

	bool SerialPort::ReadByte(uint8_t* value, uint16_t timeOut)
	{
		while (_head == _tail && --timeOut) _delay_ms(1);

		if (timeOut)
		{
			*value = _ring[_tail++];

			if (_tail >= BUFFER_SIZE) _tail = 0;

			return true;
		}
		else
		{
			return false;
		}
	}

	uint8_t SerialPort::ReadByte()
	{
		uint8_t byte =0;

		ReadByte(&byte,50);

		return byte;
	}

	void SerialPort::WriteWord(uint16_t word)
	{
		WriteByte(word>>8&0xFF);
		WriteByte(word&0xFF);
	}

	uint16_t SerialPort::ReadWord()
	{
		uint8_t byte =0;
		uint8_t byte1 =0;
		uint16_t word=0;

		ReadByte(&byte,50);
		ReadByte(&byte1,50);
		word = byte<<8|byte1;

		return word;
	}

	void SerialPort::SendData(uint8_t* data, uint16_t length)
	{
		for(int i=0;i<length;i++)
		{
			WriteByte(data[i]);
		}
	}

	void SerialPort::Received(uint8_t data)
	{
		_ring[_head++] = data;
		if (_head >= BUFFER_SIZE) _head = 0;
	}

	void SerialPort::Transmit()
	{
	}

	bool SerialPort::GetAnswer(uint16_t timeOut)
		{
			uint8_t tmp;

			if (ReadByte(&tmp, timeOut))
			{
				Data.Add(tmp);

				while (ReadByte(&tmp, 25))
				{
					Data.Add(tmp);
				}

				return true;
			}

			return false;
		}

	void SerialPort::ClearBuffer()
		{
			_head = 0;
			_tail = 0;
		}

	/*
	 * События от внешних источников (dma, pwm, timer ...) в данном случае для дма
	 */
	//todo реализовать
	void SerialPort::RxComplete()
	{

	}
	void SerialPort::TxComplete()
	{

	}
	void SerialPort::RxHalfComplete()
	{

	}
	void SerialPort::TxHalfComplete()
	{

	}

	const char* SerialPort::toString()
	{
		return "SerialPort";
	}
}
