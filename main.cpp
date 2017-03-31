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

void OnProcessCommand(uint8_t com, uint8_t* data, uint16_t length)
{


}
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


	 char* array3 = new char[28];
	 if(array3!=0)
	 {
		 memcpy(array3,"array3 !",strlen("array3 !"));
	 }


	 Rs485 _rs485(_USART0,9600);
	 _rs485.Init();

	 memcpy(array3,_rs485.toString(),strlen(_rs485.toString()));

	 CommandProcessor _comProc((ICommunicationObject*)&_rs485);
	 _comProc.OnCommand =OnProcessCommand;
	 _comProc.Init();
	 _comProc.SendCommand(0x10,0,0);

	 memcpy(array3,_comProc.toString(),strlen(_comProc.toString()));



	 SerialPort _port(_USART1, 9600);
	 Dma _usartDma;
	 _usartDma.Channel = CHANNEL_1;
	 _usartDma.Init();

	 // передаем обьект драйвера коммуникативному устройству
	 _port.DriverObj = (DriverObject*)&_usartDma;
	 _port.Init();

	 memcpy(array3,_port.toString(),strlen(_port.toString()));

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
