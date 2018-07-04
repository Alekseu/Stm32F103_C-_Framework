/*
 * HSpi.cpp
 *
 *  Created on: 23 θώνÿ 2018 γ.
 *      Author: hudienko_a
 */
#include "PSpi.h"

namespace Driver
{

	PSpi* PSpi::HsObj =0;

	PSpi::PSpi(SpiType type)
	{
		PSpi::HsObj = this;
	}


	void PSpi::Init()
	{
		RCC_APB2PeriphClockCmd(LCD_RCC,ENABLE);

		GPIO_InitStruct.GPIO_Pin = SCK | MOSI|D_C|PSpi_CS|RES;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(LCD_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = MISO;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(LCD_PORT, &GPIO_InitStruct);

		GPIO_ResetBits(LCD_PORT,RES);
		asm("nop");
		asm("nop");
		asm("nop");
		GPIO_SetBits(LCD_PORT,RES);

	}

	uint8_t PSpi::ReadByte()
	{
		return 0;
	}
	uint8_t PSpi::ReadByte(uint8_t addr)
	{
		return 0;
	}

	bool PSpi::ReadByte(uint8_t* value, uint16_t timeOut)
	{
		return false;
	}

	void PSpi::WriteByte(uint8_t byte)
	{
		unsigned char i=0;

		for( i = 0; i < 8; i++)
		{
			GPIO_ResetBits(GPIOA,SCK);
			if ((byte&0x80)==0x00) GPIO_ResetBits(LCD_PORT,MOSI); else GPIO_SetBits(LCD_PORT,MOSI);

			GPIO_SetBits(LCD_PORT,SCK);
			byte<<=1;
		}
		GPIO_ResetBits(LCD_PORT,SCK);
	}

	void PSpi::WriteByte(uint8_t byte, uint8_t addr)
	{
		unsigned char i=0;

		if (addr==0x00)GPIO_ResetBits(LCD_PORT,D_C); else GPIO_SetBits(LCD_PORT,D_C);

		for( i = 0; i < 8; i++)
		{
			GPIO_ResetBits(GPIOA,SCK);
			if ((byte&0x80)==0x00) GPIO_ResetBits(LCD_PORT,MOSI); else GPIO_SetBits(LCD_PORT,MOSI);

			GPIO_SetBits(LCD_PORT,SCK);
			byte<<=1;
		}
		GPIO_ResetBits(LCD_PORT,SCK);
	}

	void PSpi::WriteByte(uint8_t byte, uint8_t baddr, uint8_t addr)
	{

		if (addr==0x00)GPIO_ResetBits(LCD_PORT,D_C); else GPIO_SetBits(LCD_PORT,D_C);

		WriteByte(baddr);
		WriteByte(byte);

	}

	void PSpi::WriteBytes(uint8_t* val, uint16_t size, uint8_t baddr, uint8_t addr)
	{
		if (addr==0x00)GPIO_ResetBits(LCD_PORT,D_C); else GPIO_SetBits(LCD_PORT,D_C);

		//WriteByte(baddr);
		for(int i=0;i<size;i++)
		{
			WriteByte(val[i]);
		}
	}


	uint16_t PSpi::ReadWord()
	{
		return 0;
	}

	void PSpi::WriteWord(uint16_t word)
	{

	}

	void PSpi::SendData(uint8_t* data, uint16_t length)
	{

	}

	void PSpi::Received(uint8_t byte)
	{

	}


}


