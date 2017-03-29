/*
 * main.cpp
 *
 *  Created on: 09 ����� 2017 �.
 *      Author: Alekseu
 */

#include "main.h"


void Overclocking(void) // ������ ����������������.
{

  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); // �������� ���������� ������������
  RCC_PLLCmd(DISABLE); // ��������� ����������.
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_CFGR_PLLMULL16); // �� ������� ����� �������� �������.
  RCC_PLLCmd(ENABLE); // �������� ����������.
  while ((RCC->CR & RCC_CR_PLLRDY) == 0);     // ���� ������� ����������.
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // �������� ���������� ������������ ����������.

  SystemCoreClockUpdate(); // ���������� �������� ������� ����.
}


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

int main()
{
	InterruptController::RemapToRam();
	_leds.Init();
	Overclocking();

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
	//
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




	while(1){

		_leds.On(1);
////		memset(_data,0,256);
////		int l = com.ReadData(_data);
////		if(l>0)
////		{
////			com.SendData(_data,l);
////		}
		_delay_ms(150);
		_leds.Off(1);
		_delay_ms(150);
	};

	return 0;
}
