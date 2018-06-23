/*
 * IDisplay.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DEVICE_INTERFACE_IDISPLAY_H_
#define DEVICE_INTERFACE_IDISPLAY_H_
#include "../../Driver/Interface/CommunicationObject.h"

using namespace Driver;

class IDisplay
{
public:

	virtual void Init(ICommunicationObject* obj,bool with_buff)=0;

			//with buffer
	virtual	void SetPixel( char X, char Y, char State )=0;
	virtual	void DrawLine(char x1, char y1, char x2, char y2)=0;
	virtual	void DrawRactangle(char x1, char y1, char x2, char y2)=0;
	virtual	void DrawFillRactangle(char x1, char y1, char x2, char y2 , char state)=0;
	virtual	void PutChar(char f, char x, char y, char size)=0;
	virtual	void PutStr(char x, char y, char size, const char *str)=0;
	virtual	void PutInt(char x , char y,char size, int i)=0;
	virtual	void Invalidate()=0;

			//without buffer
	virtual	void Clear()=0;
	virtual	void GotoXY(char x , char y)=0;
	virtual	void PutChar(char f)=0;
	virtual	void PutInt(char x , char y, int i)=0;
	virtual	void PutStr(const char *str)=0;

	virtual	void WriteBytes(char* _buffer, int frame_size,int size)=0;

	 virtual ~IDisplay(){}
};


#endif /* DEVICE_INTERFACE_IDISPLAY_H_ */
