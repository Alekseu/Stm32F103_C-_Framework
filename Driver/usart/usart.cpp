/*
 * CUsart.cpp
 *
 *  Created on: 22.03.2013
 *      Author: YVShvets
 */

#include "usart.h"


SerialPort *SerialPort::pUsart0 = 0;
SerialPort *SerialPort::pUsart1 = 0;
SerialPort *SerialPort::pUsart2 = 0;
SerialPort *SerialPort::pUsart3 = 0;
SerialPort *SerialPort::pUsart4 = 0;

extern "C"
{
	// ----------- Прерывания приема передачи ---------------------
	void  USART1_IRQHandler(void)
	{
		if (USART1->SR & USART_SR_RXNE)
		{
			USART1->SR&=~USART_SR_RXNE;
			char tmp =USART1->DR;
			SerialPort::pUsart0->Received(tmp);
		}
	}

	void  USART2_IRQHandler(void)
	{
		if (USART2->SR & USART_SR_RXNE)
		{
			USART2->SR&=~USART_SR_RXNE;
			char tmp =USART2->DR;
			SerialPort::pUsart1->Received(tmp);
		}
	}

	void  USART3_IRQHandler(void)
	{
		if (USART3->SR & USART_SR_RXNE)
		{
			USART3->SR&=~USART_SR_RXNE;
			char tmp =USART3->DR;
			SerialPort::pUsart2->Received(tmp);
		}
	}

	void  UART4_IRQHandler(void)
	{
		if (UART4->SR & USART_SR_RXNE)
		{
			UART4->SR&=~USART_SR_RXNE;
			char tmp =UART4->DR;
			SerialPort::pUsart3->Received(tmp);
		}
	}

	void  UART5_IRQHandler(void)
	{
		if (UART5->SR & USART_SR_RXNE)
		{
			UART5->SR&=~USART_SR_RXNE;
			char tmp =UART5->DR;
			SerialPort::pUsart4->Received(tmp);
		}
	}

	// ------------------------------------------------

}


SerialPort::SerialPort(unsigned char port, int baud)
{
	_port = port;
	_head = 0;
	_tail = 0;

	switch (_port)
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

	switch (_port)
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
		      USART_InitStruct.USART_Parity              = USART_Parity_No ;
		      USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		      USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
		      USART_Init(USART1, &USART_InitStruct);


		      USART_Cmd(USART1, ENABLE);
		      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		      NVIC_Init(&NVIC_InitStructure);

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
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

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
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
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
			NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
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
			NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		break;

	default:
		break;
	}

	//sei();
}

SerialPort::SerialPort(unsigned char port, int baud, int parity)
{
	_port = port;
	_head = 0;
	_tail = 0;

	switch (_port)
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
	switch (_port)
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
			      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			      NVIC_Init(&NVIC_InitStructure);

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
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

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
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
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
				NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
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
				NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);
			break;

		default:
			break;
		}

	//sei();
}

SerialPort::~SerialPort()
{
}

void SerialPort::WriteByte(char byte)
{

	switch (_port)
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

bool SerialPort::ReadByte(char* value, unsigned int timeOut)
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

void SerialPort::Received(char data)
{
	_ring[_head++] = data;
	if (_head >= BUFFER_SIZE) _head = 0;
}

void SerialPort::Transmit()
{
}

bool SerialPort::GetAnswer(int timeOut)
	{
		char tmp;

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
