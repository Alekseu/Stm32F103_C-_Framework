/*
 * main.cpp
 *
 *  Created on: 09 марта 2017 г.
 *      Author: Alekseu
 */
#include "Driver/usart.h"
#include "Driver/Dma.h"
#include "Extention/operators.h"
#include "Driver/Usb.h"
#include "Driver/Blinker.h"

extern "C"
{
	//#include "StdPeriph/usb/inc/usb_lib.h"
//	#include "Driver/usb_com/usb_pwr.h"
//	#include "Driver/usb_com/usb_istr.h"
//	#include "Driver/usb_com/usb_desc.h"
//	#include "Driver/usb_com/hw_config.h"



}

//class Test : public SerialPort, Dma
//{
//public:
//	Test(unsigned char port, int baud): SerialPort(port,baud),Dma()
//	{
//
//	}
//
//	void HalfRecivedDmaComplete(){}
//
//};

char _data[256];
Blinker _leds;
Usb com;

int main()
{

	_leds.Init();
	com.Init();
//	Set_System();
//	Set_USBClock();
//	USB_Interrupts_Config();
//    USB_Init();
//    while (bDeviceState != CONFIGURED)
//    {
//    }

//	Test* _port = new Test(_USART0,9600);

//	const char* _flashMemory = "HelloWorld";
//	char t[10];
//	char t2[10];
//
//	Dma* _dma = new Dma();
//	_dma->Init(3);
//
//	_dma->MemCpy((char*)_flashMemory,t2,10);



	int a=0;
	while(1){
		a++;
		_leds.On(1);
		memset(_data,0,256);
		int l = com.ReadData(_data);
		if(l>0)
		{
			com.SendData(_data,l);
		}
		_delay_ms(150);
		_leds.Off(1);
		_delay_ms(150);
	};

	return 0;
}
