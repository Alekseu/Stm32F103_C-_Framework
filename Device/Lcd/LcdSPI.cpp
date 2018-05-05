/*
 * N3310.cpp
 *
 *  Created on: 05 мая 2018 г.
 *      Author: hudienko_a
 */
#include "LcdSPI.h"
#include <stdlib.h>

namespace Device
{

	void LcdSPI::Init(bool with_buff)
	{
		_with_buffer = with_buff;
		if(_with_buffer)
		{
			_buffer = new unsigned char[(WIDTH*HEIGHT)/8+1];
		}
		lcd_tx(0x21,0);
		asm("nop");
		lcd_tx(0x09,0);
		asm("nop");
		lcd_tx(0xe0,0);
		asm("nop");
		lcd_tx(0x14,0);
		asm("nop");
		lcd_tx(0x05,0);
		asm("nop");
		lcd_tx(0x20,0);
		asm("nop");
		lcd_tx(0x0c,0);
		asm("nop");
		lcd_tx(0x80,0);
		asm("nop");
		asm("nop");
	}

	//with buffer
	void LcdSPI::SetPixel( char X, char Y, char State )
	{
		if(_with_buffer)
		{
			if ((X >= WIDTH) || (Y >= (HEIGHT)) || (X < 0) || (Y < 0))
				return;

			unsigned char ByteIdxy = Y/8;
			unsigned char  ByteIdxx = X;

			unsigned char BitIdy = (Y%8); // ¬ысота относительно строки байт (0<=Y<=7)

			unsigned char tmp=0;
			tmp =  _buffer[(ByteIdxx+(ByteIdxy*WIDTH))];

			if (State)
			{
				tmp|= (1 << BitIdy);
			}
			else
			{
				tmp&= ~(1 << BitIdy);
			}
			_buffer[(ByteIdxx+(ByteIdxy*WIDTH))] = tmp;
		}
	}

	void LcdSPI::DrawLine(char x1, char y1, char x2, char y2)
	{
		const unsigned short deltaX = abs(x2 - x1);
		const unsigned short deltaY = abs(y2 - y1);
		const unsigned short signX = x1 < x2 ? 1 : -1;
		const unsigned short signY = y1 < y2 ? 1 : -1;

		unsigned short error = deltaX - deltaY;

		SetPixel(x2, y2, 1);

		while (x1 != x2 || y1 != y2)
		{
			SetPixel(x1, y1, 0);
			const unsigned short error2 = error * 2;

			if (error2 > -deltaY)
			{
				error -= deltaY;
				x1 += signX;
			}
			if (error2 < deltaX)
			{
				error += deltaX;
				y1 += signY;
			}
		}
	}

	void LcdSPI::DrawRactangle(char x1, char y1, char x2, char y2)
	{
		DrawLine(x1, y1, x1, y2);
		DrawLine(x2, y1, x2, y2);
		DrawLine(x1, y1, x2, y1);
		DrawLine(x1, y2, x2, y2);
	}

	void LcdSPI::DrawFillRactangle(char x1, char y1, char x2, char y2 , char state)
	{
		for(int i=x1;i<x1+x2;i++)
		{
			for(int j=y1;j<y1+y2;j++)
			{
				SetPixel(i,j,state);
			}
			//lcd_invalidate();
		}
	}

	void LcdSPI::PutChar(char f, char x, char y, char size)
	{

		if(f>0x7D) //если єто руские буквы
		{
			//отнимаем значение первой руской буквы
			//и прибавляем длинну английского знакогенератора
			f=f-0xC0+94;
		}else
		{
			f=f-0x20; //
		}

		if(size <=1)
		{
			for(char i=0;i<5;i++)
			{
				char charater = (char)(Font[f][i]);
				for(char j=0;j<7;j++)
				{
					if((charater&(1<<j)))
						SetPixel(x+i,y+j,1);
					else
						SetPixel(x+i,y+j,0);
				}
			}
		}
		else
		{
			for(char i=1;i<6;i++)
			{
				char charater = (char)(Font[f][i-1]);
				for(char j=1;j<8;j++)
				{
					if((charater&(1<<(j-1))))
						DrawFillRactangle(x+(i*size), y+(j*size), size,size ,1);
					else
						DrawFillRactangle(x+(i*size), y+(j*size), size, size ,0);
				}

			}
		}


	}

	void LcdSPI::PutStr(char x, char y, char size, const char *str) //вывод строки находящийся в оперативе
	{
		char k = strlen(str);
		for(int i=0;i<k;i++)
		{
			PutChar(str[i], x,y,size);
			if(size<=2)
			{
				x+=(5*size)+1;
			}
			else
			{
				x+=(5*size)+2;
			}
		}
	}

	void LcdSPI::Invalidate()
	{
		if(_with_buffer)
		{
			GotoXY(0,0);
			for(int i=0;i<((WIDTH*HEIGHT)/8);i++)
			{
				lcd_putbyte(_buffer[i]);
			}
		}
	}

	//without buffer

	void LcdSPI::GotoXY(char x , char y)  //переместить на позицию ху
	{
		lcd_tx(0x80+x,0);//x direction
		lcd_tx(0x40+y,0);//y direction
	}

	unsigned char LcdSPI::Clear()  //очистить экран
	{
		char x,y;
		x=0;y=0;
		while(y<6)
		{
			GotoXY(0,y);
			x=0;
			while(x<101)
			{
				lcd_tx(0x00,1);
				x++;
			}
			y++;
		}

		return 0;
	}

	void LcdSPI::PutChar(char f)
	{

		if(f>0x7D) //если єто руские буквы
		{
			//отнимаем значение первой руской буквы
			//и прибавляем длинну английского знакогенератора
			f=f-0xC0+94;
		}else
		{
			f=f-0x20; //
		}
		for(char i=0;i<5;i++)
		{
			char charater = (char)(Font[f][i]);
			lcd_tx(charater,1);
		}
		lcd_tx(0x00,1);

	}

	void LcdSPI::PutStr(const char *str)
	{
		char k;
		for(int i=0;i<14;i++)
		{
			PutChar(str[i]);
		}
	}

	//private
	unsigned char LcdSPI::lcd_tx(unsigned char tx,unsigned char dc)
	{
		unsigned char i=0;

		if (dc==0x00)GPIO_ResetBits(LCD_PORT,D_C); else GPIO_SetBits(LCD_PORT,D_C);

		for( i = 0; i < 8; i++)
		{
			GPIO_ResetBits(GPIOA,SCK);
			if ((tx&0x80)==0x00) GPIO_ResetBits(LCD_PORT,MOSI); else GPIO_SetBits(LCD_PORT,MOSI);

			GPIO_SetBits(LCD_PORT,SCK);
			tx<<=1;
		}
		GPIO_ResetBits(LCD_PORT,SCK);
		return 0;
	}

	void LcdSPI::lcd_putbyte(char byte)
	{
		lcd_tx(byte,1);
	}

	unsigned char LcdSPI::lcd_clear_s()  //очистить экран
	{
		if(_with_buffer)
		{
			for(int i=0;i<(WIDTH*HEIGHT/8);i++)
			{
				_buffer[i]=0;
			}
		}
		return 0;
	}


}
