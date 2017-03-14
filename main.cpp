/*
 * main.cpp
 *
 *  Created on: 09 марта 2017 г.
 *      Author: Alekseu
 */
#include "Driver/usart.h"
#include "Driver/Dma.h"
#include "Extention/operators.h"

class Test : public SerialPort, Dma
{
public:
	Test(unsigned char port, int baud): SerialPort(port,baud),Dma()
	{

	}

	void HalfRecivedDmaComplete(){}

};

int main()
{
	Test* _port = new Test(_USART0,9600);

	const char* _flashMemory = "HelloWorld";
	char t[10];
	char t2[10];

	Dma* _dma = new Dma();
	_dma->Init(2);

	_dma->MemCpy((char*)_flashMemory,t2,10);

	int a=0;
	while(1){
		a++;
	};

	return 0;
}
