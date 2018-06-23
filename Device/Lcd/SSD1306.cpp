/*
 * SSD1306.cpp
 *
 *  Created on: 20 июня 2018 г.
 *      Author: hudienko_a
 */
#include "SSD1306.h"

namespace Device
{

	void SSD1306::Init(ICommunicationObject* obj,bool with_buff =true)
	{
		_obj = obj;

		_with_buffer = with_buff;
		if(_with_buffer)
		{
			_buffer = new unsigned char[(WIDTH*HEIGHT)/8+1];
			memset(_buffer,0,(WIDTH*HEIGHT)/8+1);
		}

			_obj->WriteByte(0xAE,LCD_ADDR,0x00);

			_obj->WriteByte(0x00 | 0x0,LCD_ADDR,0x00);            // low col = 0

			_obj->WriteByte( 0x10 | 0x0,LCD_ADDR,0x00);           // hi col = 0
			_obj->WriteByte( 0x40 | 0x0,LCD_ADDR,0x00);            // line #0

			_obj->WriteByte(0x81,LCD_ADDR,0x00);                   // Set Contrast 0x81
			_obj->WriteByte( 0xCF,LCD_ADDR,0x00);
			_obj->WriteByte( 0xA1,LCD_ADDR,0x00);                    // Segremap - 0xA1
			_obj->WriteByte( 0xC8,LCD_ADDR,0x00);                    // COMSCAN DEC 0xC8 C0
			_obj->WriteByte( 0xA6,LCD_ADDR,0x00);                    // Normal Display 0xA6 (Invert A7)

			_obj->WriteByte( 0xA4,LCD_ADDR,0x00);                // DISPLAY ALL ON RESUME - 0xA4
			_obj->WriteByte( 0xA8,LCD_ADDR,0x00);                    // Set Multiplex 0xA8
			_obj->WriteByte( 0x3F,LCD_ADDR,0x00);                    // 1/64 Duty Cycle

			_obj->WriteByte( 0xD3,LCD_ADDR,0x00);                    // Set Display Offset 0xD3
			_obj->WriteByte( 0x0,LCD_ADDR,0x00);                     // no offset

			_obj->WriteByte( 0xD5,LCD_ADDR,0x00);                    // Set Display Clk Div 0xD5
			_obj->WriteByte( 0x80,LCD_ADDR,0x00);                    // Recommneded resistor ratio 0x80

			_obj->WriteByte( 0xD9,LCD_ADDR,0x00);                  // Set Precharge 0xd9
			_obj->WriteByte( 0xF1,LCD_ADDR,0x00);

			_obj->WriteByte( 0xDA,LCD_ADDR,0x00);                    // Set COM Pins0xDA
			_obj->WriteByte( 0x12,LCD_ADDR,0x00);

			_obj->WriteByte( 0xDB,LCD_ADDR,0x00);                 // Set VCOM Detect - 0xDB
			_obj->WriteByte( 0x40,LCD_ADDR,0x00);

			_obj->WriteByte( 0x20,LCD_ADDR,0x00);                    // Set Memory Addressing Mode
			_obj->WriteByte( 0x00,LCD_ADDR,0x00);                    // 0x00 - Horizontal

			_obj->WriteByte( 0x40 | 0x0,LCD_ADDR,0x00);              // Set start line at line 0 - 0x40

			_obj->WriteByte( 0x8D,LCD_ADDR,0x00);                    // Charge Pump -0x8D
			_obj->WriteByte( 0x14,LCD_ADDR,0x00);


			_obj->WriteByte( 0xA4,LCD_ADDR,0x00);              //--turn on all pixels - A5. Regular mode A4
			               //--turn on oled panel - AF

			Clear();

			_obj->WriteByte( 0xAF,LCD_ADDR,0x00);
	}

	//with buffer
	void SSD1306::SetPixel( char X, char Y, char State )
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

	void SSD1306::DrawLine(char x1, char y1, char x2, char y2)
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

	void SSD1306::DrawRactangle(char x1, char y1, char x2, char y2)
	{
		DrawLine(x1, y1, x1, y2);
		DrawLine(x2, y1, x2, y2);
		DrawLine(x1, y1, x2, y1);
		DrawLine(x1, y2, x2, y2);
	}

	void SSD1306::DrawFillRactangle(char x1, char y1, char x2, char y2 , char state)
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

	void SSD1306::PutChar(char f, char x, char y, char size)
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
						{
							DrawFillRactangle(x+(i*size), y+(j*size), size,size ,1);
						}
						else
						{
							DrawFillRactangle(x+(i*size), y+(j*size), size, size ,0);
						}
					}

				}
			}
	}

	void SSD1306::PutStr(char x, char y, char size, const char *str)
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

	void SSD1306::PutInt(char x , char y,char size, int i)
	{
		_int_buffer[0] = i/1000%10+0x30;
		_int_buffer[1] = i/100%10+0x30;
		_int_buffer[2] = i/10%10+0x30;
		_int_buffer[3] = i%10+0x30;
		_int_buffer[4] = 0;
		PutStr(x,y,size,_int_buffer);
	}

	void SSD1306::Invalidate()
	{
		if(_with_buffer)
		{
			GotoXY(0,0);
			int counter=0;
			for(int i=0;i<((HEIGHT)/8);i++)
			{
				_obj->WriteBytes((unsigned char* )&_buffer[i*(WIDTH)],WIDTH,LCD_ADDR,0x40);
			}
		}
	}

	//without buffer
	void SSD1306::Clear()
	{
		if (_with_buffer)
		{
			memset(_buffer,0,(WIDTH*HEIGHT)/8+1);
		}
		else
		{
		GotoXY(0,0);
			for(int i=0;i<HEIGHT/8;i++)
			{
				for(int i=0;i<WIDTH;i++)
				{
					_obj->WriteByte(0,LCD_ADDR,0x40);
				}
			}
		}
	}

	void SSD1306::GotoXY(char x , char y)
	{
		_obj->WriteByte(0xB0 + y,LCD_ADDR,0x00);//set page address
		_obj->WriteByte( x & 0xf,LCD_ADDR,0x00);//set lower column address
		_obj->WriteByte(0x10 | (x >> 4),LCD_ADDR,0x00);//set higher column address
	}

	void SSD1306::PutChar(char f)
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
					char charater = (Font[f][i]);
					_obj->WriteByte(charater,LCD_ADDR,0x40);
			    }
			   _obj->WriteByte(0x00,LCD_ADDR,0x40);
	}

	void SSD1306::PutStr(const char *str)
	{
		 char k;

		  while (k=*str++) {PutChar(k);}
	}

	void SSD1306::PutInt(char x , char y, int i)
	{
		GotoXY(x,y);
		_int_buffer[0] = i/1000%10+0x30;
		_int_buffer[1] = i/100%10+0x30;
		_int_buffer[2] = i/10%10+0x30;
		_int_buffer[3] = i%10+0x30;
		_int_buffer[4] = 0;
		PutStr(_int_buffer);
	}

	void SSD1306::WriteBytes(char* _buffer, int frame_size, int size)
	{
		int frame_count = size/frame_size;
		GotoXY(0,0);
		for(int i=0;i<frame_count;i++)
		{
			_obj->WriteBytes((unsigned char* )&_buffer[i*frame_size],frame_size,LCD_ADDR,0x40);
		}

	}
}


