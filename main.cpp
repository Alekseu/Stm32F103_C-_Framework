/*
 * main.cpp
 *
 *  Created on: 09 марта 2017 г.
 *      Author: Alekseu
 */

#include "Driver/nvic.h"
#include "Driver/usart.h"
#include "Driver/Dma.h"
#include "Extention/operators.h"
#include "Driver/Usb.h"
#include "Driver/Blinker.h"

#include "Driver/singleLed/SL.h"
#include "Communication/CommandProcessor.h"
#include <string.h>

#include "Extention/MemoryManager.h"
#include "Extention/MemoryPool.h"
#include "Extention/LinkedList.h"
#include "Extention/RingBuffer.h"


extern "C"
{
//#include "StdPeriph/usb/inc/usb_lib.h"
//	#include "Driver/usb_com/usb_pwr.h"
//	#include "Driver/usb_com/usb_istr.h"
//	#include "Driver/usb_com/usb_desc.h"
//	#include "Driver/usb_com/hw_config.h"
}



//char _data[256];
Blinker _leds;
//CommandProcessor* _command;
//
//Usb com;
//SL _sl;

//void OnProcessCommand(Command com, unsigned char* data, unsigned int length)
//{
//
//	if(com==SetBuffer)
//	{
//		memcpy(_sl.VideoBuffer,data,length);
//		_command->SendCommand(com,(char*)0,0);
//	}
//}
//typedef void reset__(void);
//reset__* reset_ = 0;
int a;
void SysTickHandler(void)
{
	a++;
}

int main()
{
	InterruptController::RemapToRam();
	InterruptController::SetHandler(SysTick_IRQn,SysTickHandler);

	RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);
		SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);


	//MemPool pool((void*)0x60000001,0x80000);
//	MemoryManager mem((uint32_t)0x60000000,0x0FFFFF);
//	mem.Init();
//
//	char* array = new char[128];
//
//	if(array!=0)
//	{
//		memcpy(array,"Hello world  this is playcement new!!!!",strlen("Hello world  this is playcement new!!!!"));
//	}
//	char* array1 = new char[128];
//	char* array2 = new char[128];
//
//	 delete[] array;
//	 delete[] array1;
//	 delete[] array1;
//
//	LList<char>* _list = new LList<char>();
//	for(int i=0;i<10;i++)
//	{
//		_list->add(i+1);
//	}

//	MemoryManager mem((uint32_t)0x60000000,0x80000);
//
//	ErrorStatus t = mem.Init();
//
//	char* array = new (&mem) char[130];
//	if(array!=0)
//	{
//
//		memcpy(array,"Hello world  this is playcement new!!!!",strlen("Hello world  this is playcement new!!!!"));
//
//		mem.ShowMemory();
//
////		char* array = (char*)mem.Malloc(150);
////
////		memcpy(array,"Hello world !!!!",strlen("Hello world !!!!"));
//
////		mem.ShowMemory();
////
////		mem.Free(array);
////
////		mem.ShowMemory();
//	}

//	_command = new CommandProcessor(_USART1,19200);
	_leds.Init();
//	com.TypeUsb = VirtualComPort;
//	com.Init();

//	_sl.Width = 32;
//	_sl.Height = 16;
//	_sl.SLInit();
//
//	_command->Width = 32;
//	_command->Hieght = 16;
//	_command->UseDMA = false;
//	_command->OnCommand = OnProcessCommand;
//	_command->Init();

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
//		memset(_data,0,256);
//		int l = com.ReadData(_data);
//		if(l>0)
//		{
//			com.SendData(_data,l);
//		}
		_delay_ms(150);
		_leds.Off(1);
		_delay_ms(150);
	};

	return 0;
}
