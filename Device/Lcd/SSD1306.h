/*
 * SSD1306.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DEVICE_LCD_SSD1306_H_
#define DEVICE_LCD_SSD1306_H_

#include "../deviceConfig.h"
#include "../../Driver/platform.h"

#include "CharGen.h"
#include "../../Driver/i2c/i2c.h"
#include "stdlib.h"
using namespace Driver;

namespace Device
{

	class SSD1306 : public IDisplay
	{
	public:
			unsigned short Width;
			unsigned short Height;

		void Init(ICommunicationObject* obj,bool with_buff);

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
		void PutInt(char x , char y, int i);
		void PutStr(const char *str);

		void WriteBytes(char* _buffer, int frame_size,int size);

		~SSD1306(){}

	private:
		ICommunicationObject* _obj;
		unsigned char* _buffer;
		char _int_buffer[5];
		bool _with_buffer;
	};

}


#endif /* DEVICE_LCD_SSD1306_H_ */
