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

// ----------- ���������� ������ �������� ---------------------
//		void  USART1_IRQ(void)
//		{
//			if (USART1->SR & USART_SR_RXNE)
//			{
//				USART1->SR&=~USART_SR_RXNE;
//				uint8_t tmp =USART1->DR;
//				SerialPort::pUsart0->Received(tmp);
//			}
//		}
//
//		void  USART2_IRQ(void)
//		{
//			if (USART2->SR & USART_SR_RXNE)
//			{
//				USART2->SR&=~USART_SR_RXNE;
//				uint8_t tmp =USART2->DR;
//				SerialPort::pUsart1->Received(tmp);
//			}
//		}
//
//		void  USART3_IRQ(void)
//		{
//			if (USART3->SR & USART_SR_RXNE)
//			{
//				USART3->SR&=~USART_SR_RXNE;
//				uint8_t tmp =USART3->DR;
//				SerialPort::pUsart2->Received(tmp);
//			}
//		}
//
//		void  USART4_IRQ(void)
//		{
//			if (UART4->SR & USART_SR_RXNE)
//			{
//				UART4->SR&=~USART_SR_RXNE;
//				uint8_t tmp =UART4->DR;
//				SerialPort::pUsart3->Received(tmp);
//			}
//		}
//
//		void  USART5_IRQ(void)
//		{
//			if (UART5->SR & USART_SR_RXNE)
//			{
//				UART5->SR&=~USART_SR_RXNE;
//				uint8_t tmp =UART5->DR;
//				SerialPort::pUsart4->Received(tmp);
//			}
//		}

void DriverEvent(Source source, uint8_t communication)
{
	if(source.DriverSource!=DmaSource) return;

	SerialPort* pointer = 0;

	switch ((SerialPort::PortName)communication)
	{

	case SerialPort::COM1:
		pointer =SerialPort::pUsart0;
		break;
	case SerialPort::COM2:
		pointer =SerialPort::pUsart1;
		break;
	case SerialPort::COM3:
		pointer =SerialPort::pUsart2;
		break;
	case SerialPort::COM4:
		pointer =SerialPort::pUsart3;
		break;
	case SerialPort::COM5:
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

SerialPort::SerialPort(PortName port, uint16_t baud):ICommunicationObject()
{
	BuferSize=64;
	DriverObj =0;
	_ring=0;
	_tx=0;

	Port = port;
	Baud = baud;
	_head = 0;
	_tail = 0;



	switch (Port)
	{

	case COM1:
		pUsart0 = this;
		InterruptNumber = USART1_IRQn;
		break;
	case COM2:
		pUsart1 = this;
		InterruptNumber = USART2_IRQn;
		break;
	case COM3:
		pUsart2 = this;
		InterruptNumber = USART3_IRQn;
		break;
	case COM4:
		pUsart3 = this;
		InterruptNumber = UART4_IRQn;
		break;
	case COM5:
		pUsart4 = this;
		InterruptNumber = UART5_IRQn;
		break;

	}

	switch (Port)
	{
	case COM1:
		/* Enable GPIOA clock                                                   */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�


		USART_InitStruct.USART_BaudRate            = baud;
		USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
		USART_InitStruct.USART_StopBits            = USART_StopBits_1;
		USART_InitStruct.USART_Parity              = USART_Parity_No ;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct);


		USART_Cmd((USART_TypeDef*)Port, ENABLE);
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(USART1_IRQn,InterruptWraper);
		InterruptController::EnableChannel(USART1_IRQn);


		break;
	case COM2:

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOA, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOA, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = USART_Parity_No; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART2

		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(USART2_IRQn,InterruptWraper);
		InterruptController::EnableChannel(USART2_IRQn);


		break;
	case COM3:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = USART_Parity_No; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART3
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(USART3_IRQn,InterruptWraper);
		InterruptController::EnableChannel(USART3_IRQn);

		break;
	case COM4:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOC, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOC, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = USART_Parity_No; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART4
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(UART4_IRQn,InterruptWraper);
		InterruptController::EnableChannel(UART4_IRQn);

		break;
	case COM5:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOC, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOD, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = USART_Parity_No; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART4
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(UART5_IRQn,InterruptWraper);
		InterruptController::EnableChannel(UART5_IRQn);


		break;

	default:
		break;
	}

}

SerialPort::SerialPort(PortName port, uint16_t baud, PortParity parity)
{
	BuferSize=64;
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

	case COM1:
		pUsart0 = this;
		break;
	case COM2:
		pUsart1 = this;
		break;
	case COM3:
		pUsart2 = this;
		break;
	case COM4:
		pUsart3 = this;
		break;
	case COM5:
		pUsart4 = this;
		break;

	}
	switch (Port)
	{
	case COM1:
		/* Enable GPIOA clock                                                   */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�


		/*Configure peripheral I/O remapping */


		USART_InitStruct.USART_BaudRate            = baud;
		USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
		USART_InitStruct.USART_StopBits            = USART_StopBits_1;
		USART_InitStruct.USART_Parity              = parity ;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct);


		USART_Cmd((USART_TypeDef*)Port, ENABLE);
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(USART1_IRQn,InterruptWraper);
		InterruptController::EnableChannel(USART1_IRQn);

		break;
	case COM2:

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOA, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOA, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = parity; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART2

		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(USART2_IRQn,InterruptWraper);
		InterruptController::EnableChannel(USART2_IRQn);

		break;
	case COM3:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOB, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = parity; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART3
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(USART3_IRQn,InterruptWraper);
		InterruptController::EnableChannel(USART3_IRQn);

		break;
	case COM4:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOC, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOC, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = parity; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART4
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(UART4_IRQn,InterruptWraper);
		InterruptController::EnableChannel(UART4_IRQn);

		break;
	case COM5:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		//������������� ������ PA9 - USART1_Tx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12; //��������� ������ PA9
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�������� ����� ������������
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; //����� �������������� �������, ����� Push-Pull
		GPIO_Init(GPIOC, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� ������ PA10 - USART1_Rx
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; //��������� ������ PA10
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Input floating
		GPIO_Init(GPIOD, &GPIO_InitStruct); //�������� ��������� ��������� � ��������� GPIO�

		//������������� USART1
		USART_InitStruct.USART_BaudRate = baud; //�������� ������ 9600 ���
		USART_InitStruct.USART_WordLength = USART_WordLength_8b; //����� ����� 8 ���
		USART_InitStruct.USART_StopBits = USART_StopBits_1; //1 ����-���
		USART_InitStruct.USART_Parity = parity; //��� �������� ��������
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��� ����������� ��������
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //������� ���������� � �������� USART1
		USART_Init((USART_TypeDef*)Port, &USART_InitStruct); //�������� ��������� ��������� � ��������� USART1

		USART_Cmd((USART_TypeDef*)Port, ENABLE); //�������� USART4
		USART_ITConfig((USART_TypeDef*)Port, USART_IT_RXNE, ENABLE);
		InterruptController::SetHandler(UART5_IRQn,InterruptWraper);
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

	case COM1:
		while(!(USART1->SR & USART_SR_TXE));
		USART1->DR = byte;
		break;

	case COM2:
		while(!(USART2->SR & USART_SR_TXE));
		USART2->DR = byte;
		break;

	case COM3:
		while(!(USART3->SR & USART_SR_TXE));
		USART3->DR = byte;
		break;

	case COM4:
		while(!(UART4->SR & USART_SR_TXE));
		UART4->DR = byte;
		break;
	case COM5:
		while(!(UART5->SR & USART_SR_TXE));
		UART5->DR = byte;
		break;
	}

}

void SerialPort::WriteByte(uint8_t byte, uint8_t addr)
{
	WriteByte(addr);
	WriteByte(byte);
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

uint8_t SerialPort::ReadByte(uint8_t addr)
{
	WriteByte(addr);
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
 * ������� �� ������� ���������� (dma, pwm, timer ...) � ������ ������ ��� ���
 */
 //todo �����������
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

 void SerialPort::InterruptWraper(void)
 {

	 if (USART1->SR & USART_SR_RXNE)
	 {
		 USART1->SR&=~USART_SR_RXNE;
		 uint8_t tmp =USART1->DR;
		 SerialPort::pUsart0->Received(tmp);
		 return;
	 }
	 else

	 if (USART2->SR & USART_SR_RXNE)
	 {
		 USART2->SR&=~USART_SR_RXNE;
		 uint8_t tmp =USART2->DR;
		 SerialPort::pUsart1->Received(tmp);
		 return;
	 }
	 else

	 if (USART3->SR & USART_SR_RXNE)
	 {
		 USART3->SR&=~USART_SR_RXNE;
		 uint8_t tmp =USART3->DR;
		 SerialPort::pUsart2->Received(tmp);
		 return;
	 }
	 else

	 if (UART4->SR & USART_SR_RXNE)
	 {
		 UART4->SR&=~USART_SR_RXNE;
		 uint8_t tmp =UART4->DR;
		 SerialPort::pUsart3->Received(tmp);
		 return;
	 }
	 else

	 if (UART5->SR & USART_SR_RXNE)
	 {
		 UART5->SR&=~USART_SR_RXNE;
		 uint8_t tmp =UART5->DR;
		 SerialPort::pUsart4->Received(tmp);
		 return;
	 }


 }

}
