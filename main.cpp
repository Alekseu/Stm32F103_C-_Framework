/*
 * main.cpp
 *
 *  Created on: 09 марта 2017 г.
 *      Author: Alekseu
 */

#include "main.h"

#include "Extention/sPtr.h"

void Overclocking(void) // Разгон микроконтроллера.
{

  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); // Выбираем источником такторования
  RCC_PLLCmd(DISABLE); // Выключаем умножитель.
  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_CFGR_PLLMULL16); // На сколько будем умножать частоту.
  RCC_PLLCmd(ENABLE); // Включаем умножитель.
  while ((RCC->CR & RCC_CR_PLLRDY) == 0);     // Ждем запуска умножителя.
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Выбираем источником тактирования умножитель.

  SystemCoreClockUpdate(); // Вычисление тактовой частоты ядра.
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

unsigned char place[4096];

int main()
{
	ShareStorage _storage;

	InterruptController::RemapToRam();
	_leds.Init();
	//Overclocking();

	MemoryManager mem(place,4096);
	mem.RemovedElementsAutoCollectCount =5;
	mem.UsingShareStorage = true;
	mem.Init();

	char* array = new char[128];
	if(array!=0)
	{
		memcpy(array,"array !",strlen("array !"));
	}

	char* array1 = new char[128];
	if(array1!=0)
	{
		memcpy(array1,"array1 !",strlen("array1 !"));
	}

	char* array2 = new char[128];
	if(array2!=0)
	{
		memcpy(array2,"array2 !",strlen("array2 !"));
	}


	 delete[] array;

	 char* array3 = new char[28];
	 if(array3!=0)
	 {
		 memcpy(array3,"array3 !",strlen("array3 !"));
	 }

	 SPtr<CommandProcessor> _command1(new CommandProcessor(_USART1,19200));
	 _command1->Width = 32;
	 _command1->Hieght = 64;
	 SPtr<CommandProcessor> _command2(new CommandProcessor(_USART2,19200));
	 _command2->Width = 128;
	 _command2->Hieght = 32;

	 delete[] array1;
	 delete[] array2;
	 delete[] array3;

	 mem.Collect();
	 mem.Collect();

	 _command1->UseDMA = false;
	 _command2->UseDMA = false;


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

	 mem.ShowMemory();



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
