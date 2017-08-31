/*
 * spi.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "spi.h"

namespace Driver
{

	Spi* Spi::SObj =0;

	Spi::Spi(SpiNum spiNum, SpiType type, SpiSpeed speed)
	{
		_spiNum = spiNum;
		_type = type;
		_speed = speed;
	}

	Spi::~Spi()
	{

	}


	void Spi::Init()
	{
//		RCC_APBPeriphClockCmd_SPI_W5500(RCC_APBPeriph_SPI_W5500, ENABLE);
//			RCC_APBPeriphClockCmd_CS_W5500(RCC_APB2Periph_GPIO_W5500_CS, ENABLE);
//			RCC_APBPeriphClockCmd_SPI_W5500(RCC_APB2Periph_GPIO_W5500, ENABLE);
//
//			GPIO_InitTypeDef GPIO_InitStruct;
//
//			// MOSI & CLK
//			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_SCK | GPIO_Pin_SPI_W5500_MOSI;
//			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//			GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);
//
//			// MISO
//			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_MISO;
//			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//			GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);
//
//			// CS
//			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_CS_W5500;
//			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//
//			GPIO_Init(GPIO_W5500_CS, &GPIO_InitStruct);
//
//
//			SPI_InitTypeDef SPI_InitStruct;
//
//			SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_SPI_SD;
//			SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//			SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//			SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
//			SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
//			SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
//			SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
//			SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//			SPI_InitStruct.SPI_CRCPolynomial = 7;
//			SPI_Init(SPI_W5500, &SPI_InitStruct);
//
//
//			SPI_Cmd(SPI_W5500, ENABLE);

	}

	uint8_t Spi::ReadByte()
	{
		uint8_t tmp;
		return tmp;
	}

	uint8_t Spi::ReadByte(uint8_t addr)
	{
		uint8_t tmp;
		return tmp;
	}

	bool Spi::ReadByte(uint8_t* value, uint16_t timeOut)
	{
		return false;
	}

	void Spi::WriteByte(uint8_t byte)
	{

	}

	void Spi::WriteByte(uint8_t byte, uint8_t addr)
	{

	}

	uint16_t Spi::ReadWord()
	{
		uint16_t tmp;
		return tmp;
	}

	void Spi::WriteWord(uint16_t word)
	{

	}

	void Spi::SendData(uint8_t* data, uint16_t length)
	{

	}

	/*
	 * interrupt
	 */
	void Spi::Received(uint8_t byte)
	{

	}

	/*
	 * Interrupt
	 */
	void Spi::InterruptWraper(void)
	{

	}

	const char* Spi::toString()
	{
		return "SPI";
	}


}

