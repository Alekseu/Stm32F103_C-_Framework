/*
 * N3310.h
 *
 *  Created on: 15 січ. 2018
 *      Author: Alekseu
 */

#ifndef DEVICE_LCD_LCDSPI_H_
#define DEVICE_LCD_LCDSPI_H_

#include "../deviceConfig.h"
#include "../../Driver/platform.h"

namespace Device
{

#include "CharGen.h"
	/*
	 * Graphic lcd example
	 *
	 * LcdSPI _lcdSpi;
	 * _lcdSpi.Init(&_pspi);
	 * _lcdSpi.Clear();
	 * _lcdSpi.PutStr(0,0,3,"Тест ");
	 *
	 */

	class LcdSPI :public IDisplay
	{
	public:
		// todo возможно переделать с дефайнов на эти переменные
		unsigned short Width;
		unsigned short Height;

		LcdSPI();
		~LcdSPI();


		void Init(ICommunicationObject* obj, bool with_buff = true);

		//with buffer
		void SetPixel( char X, char Y, char State );
		void DrawLine(char x1, char y1, char x2, char y2);
		void DrawRactangle(char x1, char y1, char x2, char y2);
		void DrawFillRactangle(char x1, char y1, char x2, char y2 , char state);
		void PutChar(char f, char x, char y, char size);
		void PutStr(char x, char y, char size, const char *str);
		void PutInt(char x , char y,char size, int i);
		void Invalidate();

		//without buffer
		void Clear();
		void GotoXY(char x , char y);
		void PutChar(char f);
		void PutStr(const char *str);
		void PutInt(char x , char y, int i);

		void WriteBytes(char* _buffer, int frame_size,int size);

	private:
		ICommunicationObject* _obj;
		unsigned char* _buffer;
		char _int_buffer[5];

		bool _with_buffer;

//		unsigned char lcd_tx(unsigned char tx,unsigned char dc);
//		void lcd_putbyte(char byte);
//		unsigned char lcd_clear_s();
	};

}

#endif /* DEVICE_LCD_LCDSPI_H_ */
