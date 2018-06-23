/*
 * spi.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "spi.h"

namespace Driver
{

	Spi* Spi::S1Obj =0;
	Spi* Spi::S2Obj =0;
	Spi* Spi::S3Obj =0;

	Spi::Spi(SpiNum spiNum, SpiType type, SpiSpeed speed)
	{
		_spiNum = spiNum;
		_type = type;
		_speed = speed;

		BuferSize=64;
		_ring=0;
		_head = 0;
		_tail = 0;

		switch(_spiNum)
		{
			case sn_SPI1:
				Spi::S1Obj = this;
				break;
			case sn_SPI2:
				Spi::S2Obj = this;
				break;
			case sn_SPI3:
				Spi::S3Obj = this;
				break;
		}

	}

	Spi::~Spi()
	{

	}


	void Spi::Init()
	{
		_ring = new uint8_t[BuferSize];
		switch(_spiNum)
		{
			case sn_SPI1:
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
				// MOSI & CLK
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOA, &GPIO_InitStruct);

				// MISO
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOA, &GPIO_InitStruct);

				// CS
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOA, &GPIO_InitStruct);
				InterruptController::SetHandler(SPI1_IRQn,InterruptWraper);
				InterruptController::EnableChannel(SPI1_IRQn);

				break;
			case sn_SPI2:
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
				// MOSI & CLK
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOB, &GPIO_InitStruct);

				// MISO
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOB, &GPIO_InitStruct);

				// CS
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOB, &GPIO_InitStruct);
				InterruptController::SetHandler(SPI2_IRQn,InterruptWraper);
				InterruptController::EnableChannel(SPI2_IRQn);

				break;
			case sn_SPI3:
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOB,ENABLE);
				// MOSI & CLK
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOB, &GPIO_InitStruct);

				// MISO
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOB, &GPIO_InitStruct);

				// CS
				GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
				GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

				GPIO_Init(GPIOA, &GPIO_InitStruct);

				InterruptController::SetHandler(SPI3_IRQn,InterruptWraper);
				InterruptController::EnableChannel(SPI3_IRQn);

				break;

				SPI_InitStruct.SPI_BaudRatePrescaler = _speed;
				SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
				SPI_InitStruct.SPI_Mode = _type;
				SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
				SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
				SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
				SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
				SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
				SPI_InitStruct.SPI_CRCPolynomial = 7;
				SPI_Init((SPI_TypeDef*)_spiNum, &SPI_InitStruct);
				SPI_Cmd((SPI_TypeDef*)_spiNum, ENABLE);

				SPI_I2S_ITConfig((SPI_TypeDef*)_spiNum, SPI_I2S_IT_RXNE, ENABLE);

		}

	}

	uint8_t Spi::ReadByte()
	{
		uint8_t byte =0;

			ReadByte(&byte,50);

			return byte;
	}

	uint8_t Spi::ReadByte(uint8_t addr)
	{
		WriteByte(addr);
		uint8_t byte =0;

		ReadByte(&byte,50);

		return byte;
	}

	bool Spi::ReadByte(uint8_t* value, uint16_t timeOut)
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

	void Spi::WriteByte(uint8_t byte)
	{
			while(SPI_I2S_GetFlagStatus((SPI_TypeDef*)_spiNum, SPI_I2S_FLAG_TXE)==RESET);
				SPI_I2S_SendData((SPI_TypeDef*)_spiNum,byte);

	}

	void Spi::WriteByte(uint8_t byte, uint8_t addr)
	{
		WriteByte(addr);
		WriteByte(byte);
	}

	void Spi::WriteByte(uint8_t byte, uint8_t baddr, uint8_t addr)
	{

	}

	void Spi::WriteByte(uint8_t* val, uint16_t size, uint8_t baddr, uint8_t addr)
	{

	}

	uint16_t Spi::ReadWord()
	{
		uint8_t byte =0;
		uint8_t byte1 =0;
		uint16_t word=0;

		ReadByte(&byte,50);
		ReadByte(&byte1,50);
		word = byte<<8|byte1;

		return word;
	}

	void Spi::WriteWord(uint16_t word)
	{
		WriteByte(word>>8&0xFF);
			WriteByte(word&0xFF);
	}

	void Spi::SendData(uint8_t* data, uint16_t length)
	{
		for(int i=0;i<length;i++)
		{
			WriteByte(data[i]);
		}
	}

	/*
	 * interrupt
	 */
	void Spi::Received(uint8_t byte)
	{
		_ring[_head++] = byte;
		if (_head >= BUFFER_SIZE) _head = 0;
	}

	/*
	 * Interrupt
	 */
	void Spi::InterruptWraper(void)
	{
		if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) {
			//Receive
			SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);

			Spi::S1Obj->Received(SPI1->DR);
		}
		else
			if (SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_RXNE) == SET) {
				//Receive
				SPI_I2S_ClearITPendingBit(SPI2, SPI_I2S_IT_RXNE);

				Spi::S1Obj->Received(SPI2->DR);
			}
			else
				if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET) {
					//Receive
					SPI_I2S_ClearITPendingBit(SPI3, SPI_I2S_IT_RXNE);

					Spi::S1Obj->Received(SPI3->DR);
				}
	}

	const char* Spi::toString()
	{
		return "SPI";
	}


}

