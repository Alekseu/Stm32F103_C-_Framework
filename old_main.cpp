///*
// * main.cpp
// *
// *  Created on: 09 марта 2017 г.
// *      Author: Alekseu
// */
//
//#include "main.h"
//
//#include "Extention/sPtr.h"
//
//
//#include "StdPeriph/stm_lib/inc/misc.h"
//#include "StdPeriph/stm_lib/inc/stm32f10x_tim.h"
//
//void Overclocking(void) // Разгон микроконтроллера.
//{
//
//  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE); // Выбираем источником такторования
//  RCC_PLLCmd(DISABLE); // Выключаем умножитель.
//  RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_CFGR_PLLMULL16); // На сколько будем умножать частоту.
//  RCC_PLLCmd(ENABLE); // Включаем умножитель.
//  while ((RCC->CR & RCC_CR_PLLRDY) == 0);     // Ждем запуска умножителя.
//  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Выбираем источником тактирования умножитель.
//
//  SystemCoreClockUpdate(); // Вычисление тактовой частоты ядра.
//}
//
//
////CommandProcessor* _command;
////
//Usb com;
//
////SL _sl;
//
//void OnProcessCommand(uint8_t com, uint8_t* data, uint16_t length)
//{
//
//
//}
//
//
//void OnDMATxComplete(Source source, uint8_t comm)
//{
//	int a=0;
//}
//
////typedef void reset__(void);
////reset__* reset_ = 0;
//
//void write_byte(uint8_t data)
//{
//	com.WriteByte(data);
//}
//
//void write_buffer(uint8_t *data, int count)
//{
//	com.SendData(data,count);
//}
//
//TIM_OCInitTypeDef outputChannelInit;
//DMA_InitTypeDef   DMA_InitStructure;
//
//
//struct Pixel
//		{
//			union
//			{
//				struct
//				{
//					unsigned Blue:4;
//					unsigned Green:4;
//					unsigned Red:4;
//
//				};
//				unsigned  Data :8;
//			};
//		};
//
//unsigned char place[4][65];
//
//unsigned char _frame1[4*65];
//unsigned char _frame2[65];
//unsigned char _frame3[65];
//unsigned char _frame4[65];
//
//Pixel _frame[65];
//
//#define OE GPIO_Pin_2
//#define OE_PORT GPIOA
//
//#define CLK GPIO_Pin_3
//#define CLK_PORT GPIOA
//
//#define LAT GPIO_Pin_2
//#define LAT_PORT GPIOB
//
//#define R1 GPIO_Pin_0
//#define G1 GPIO_Pin_1
//#define B1 GPIO_Pin_2
//
//#define R2 GPIO_Pin_3
//#define G2 GPIO_Pin_4
//#define B2 GPIO_Pin_5
//
//#define Port GPIOC
//
//
//
//#define A GPIO_Pin_6
//#define B GPIO_Pin_5
//#define C GPIO_Pin_4
//
//#define C_Port GPIOE
//
//int current_row =0;
//unsigned char grayscale = 0;
//unsigned char _change=0;
//
//extern "C"
//{
////void DMA1_Channel7_IRQHandler(void) //tx
////{
////	if (DMA1->ISR & DMA1_IT_TC7)
////	{
////		DMA1->IFCR =DMA1_IT_TC7;
////
////		TIM2->CR1 &=~TIM_CR1_CEN;
////
////		LAT_PORT->BSRR=LAT;
////		for(int i=0;i<5;i++)
////		{
////			asm("nop");
////		}
////		LAT_PORT->BRR= LAT;
////
////		//
////		OE_PORT->BSRR= OE;
////		if((current_row&(1<<0))>0){TIM3->CCR3 = 10+grayscale;}else{TIM3->CCR3 = 70+grayscale;}
////		if((current_row&(1<<1))>0){TIM3->CCR4 = 10+grayscale;}else{TIM3->CCR4 = 70+grayscale;}
////
////		if(current_row ==4)
////		{
////			current_row=0;
////		}
////
////		for(int j=0;j<65;j++)
////		{
////
////			if(_frame[j].Red>grayscale){_frame1[j]|=(1<<0);}else{_frame1[j]&=~(1<<0);}
////			if(_frame[j].Green>grayscale){_frame1[j]|=(1<<1);}else{_frame1[j]&=~(1<<1);}
////			if(_frame[j].Blue>grayscale){_frame1[j]|=(1<<2);}else{_frame1[j]&=~(1<<2);}
////
////			if(_frame[j].Red>grayscale){_frame1[j]|=(1<<3);}else{_frame1[j]&=~(1<<3);}
////			if(_frame[j].Green>grayscale){_frame1[j]|=(1<<4);}else{_frame1[j]&=~(1<<4);}
////			if(_frame[j].Blue>grayscale){_frame1[j]|=(1<<5);}else{_frame1[j]&=~(1<<5);}
////
////		}
////
////		if(grayscale++==15)
////		{
////			grayscale =0;
////			current_row++;
////		}
////
//////		switch(current_row)
//////		{
//////		case 0:
//////			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_frame1[0];
//////			break;
//////		case 1:
//////			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_frame2[0];
//////				break;
//////		case 2:
//////			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_frame3[0];
//////				break;
//////		case 3:
//////			DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_frame4[0];
//////				break;
//////		}
//////		DMA_Init(DMA1_Channel7, &DMA_InitStructure);
////
////		OE_PORT->BRR= OE;
////
////
////		TIM2->CR1 |= TIM_CR1_CEN;
////		_change=1;
////
////		return;
////	}
//
////	if (DMA1->ISR & DMA1_IT_HT7)
////	{
////		DMA1->IFCR =DMA1_IT_HT7;
////		TIM2->CR1 &=~TIM_CR1_CEN;
////		for(int j=0;j<32;j++)
////		{
////
////			if(_frame[j].Red>grayscale){frame[j]|=(1<<0);}else{frame[j]&=~(1<<0);}
////			if(_frame[j].Green>grayscale){frame[j]|=(1<<1);}else{frame[j]&=~(1<<1);}
////			if(_frame[j].Blue>grayscale){frame[j]|=(1<<2);}else{frame[j]&=~(1<<2);}
////
////			if(_frame[j].Red>grayscale){frame[j]|=(1<<3);}else{frame[j]&=~(1<<3);}
////			if(_frame[j].Green>grayscale){frame[j]|=(1<<4);}else{frame[j]&=~(1<<4);}
////			if(_frame[j].Blue>grayscale){frame[j]|=(1<<5);}else{frame[j]&=~(1<<5);}
////
////
////		}
////		TIM2->CR1 |= TIM_CR1_CEN;
////	}
//
////}
//
//}
//
//
//
//
//int main()
//{
//	//Overclocking();
//	//ShareStorage _storage;
//
////	InterruptController::RemapToRam();
//	_leds.Init();
////	com.RxBufferSize = 64;
////	com.TxBufferSize = 64;
////	com.TypeUsb = VirtualComPort;
////	com.OnRecived = SumpProcessRequest;
////	com.Init();
////
////	SumpSetTXFunctions(write_byte,write_buffer);
//
//
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);
//
//	GpioInit  GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin =  R1|G1|B1|R2|G2|B2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(Port, &GPIO_InitStructure);
//
//	GPIO_ResetBits(Port,R1);
//	GPIO_ResetBits(Port,G1);
//	GPIO_ResetBits(Port,B1);
//
//	GPIO_ResetBits(Port,R2);
//	GPIO_ResetBits(Port,G2);
//	GPIO_ResetBits(Port,B2);
//
//	GPIO_InitStructure.GPIO_Pin =  OE;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(OE_PORT, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin =  CLK;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(CLK_PORT, &GPIO_InitStructure);
//
//	GPIO_InitStructure.GPIO_Pin =  LAT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(LAT_PORT, &GPIO_InitStructure);
//
//
//	GPIO_InitStructure.GPIO_Pin =  A|B|C;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
//	GPIO_Init(C_Port, &GPIO_InitStructure);
//
//	GPIO_ResetBits(C_Port,A);
//	GPIO_ResetBits(C_Port,B);
//	GPIO_ResetBits(C_Port,C);
//
//	OE_PORT->BRR= OE;
//
////	Overclocking();
////
////	MemoryManager mem(place,4096);
////	mem.RemovedElementsAutoCollectCount =5;
////	mem.UsingShareStorage = true;
////	mem.Init();
////
////
////	 char* array3 = new char[28];
////	 if(array3!=0)
////	 {
////		 memcpy(array3,"array3 !",strlen("array3 !"));
////	 }
////
////
////	 Rs485 _rs485(_USART0,9600);
////	 _rs485.Init();
////
////	 memcpy(array3,_rs485.toString(),strlen(_rs485.toString()));
////
////	 CommandProcessor _comProc((ICommunicationObject*)&_rs485);
////	 _comProc.OnCommand =OnProcessCommand;
////	 _comProc.Init();
////	 _comProc.SendCommand(0x10,0,0);
////
////	 memcpy(array3,_comProc.toString(),strlen(_comProc.toString()));
////
////
////
////	 SerialPort _port(_USART1, 9600);
////	 Dma _usartDma;
////	 _usartDma.Channel = CHANNEL_1;
////	 _usartDma.Init();
////
////	 // передаем обьект драйвера коммуникативному устройству
////	 _port.DriverObj = (DriverObject*)&_usartDma;
////	 _port.Init();
////
////	 memcpy(array3,_port.toString(),strlen(_port.toString()));
////
////
////	 const char* _source  ="Htis is test!";
////	 char destanation[255];
////	 memset(destanation,0,255);
////
////	 Dma _memcpyDma;
////	 _memcpyDma.Channel = CHANNEL_2;
////	 _memcpyDma.OnEvent = OnDMATxComplete;
////	 _memcpyDma.UseHalfInterrupts = true;
////	 _memcpyDma.Init();
////	 _memcpyDma.MemCpy((char*)_source,destanation,strlen(_source));
////
////
////
////	LList<char>* _list = new LList<char>();
////	for(int i=0;i<10;i++)
////	{
////		_list->add(i+1);
////	}
////
////	MemoryManager mem((uint32_t)0x60000000,0x80000);
////
////	ErrorStatus t = mem.Init();
////
////	char* array = new (&mem) char[130];
////	if(array!=0)
////	{
////
////		memcpy(array,"Hello world  this is playcement new!!!!",strlen("Hello world  this is playcement new!!!!"));
////
////		mem.ShowMemory();
////
//////		char* array = (char*)mem.Malloc(150);
//////
//////		memcpy(array,"Hello world !!!!",strlen("Hello world !!!!"));
////
//////		mem.ShowMemory();
//////
//////		mem.Free(array);
//////
//////		mem.ShowMemory();
////	}
////
////	_command = new CommandProcessor(_USART1,19200);
////
////	com.TypeUsb = VirtualComPort;
////	com.Init();
////
////	_sl.Width = 32;
////	_sl.Height = 16;
////	_sl.SLInit();
////
////	_command->Width = 32;
////	_command->Hieght = 16;
////	_command->UseDMA = false;
////	_command->OnCommand = OnProcessCommand;
////	_command->Init();
////
////	Set_System();
////	Set_USBClock();
////	USB_Interrupts_Config();
////    USB_Init();
////    while (bDeviceState != CONFIGURED)
////    {
////    }
////
////	Test* _port = new Test(_USART0,9600);
////
////	const char* _flashMemory = "HelloWorld";
////	char t[10];
////	char t2[10];
////
////	Dma* _dma = new Dma();
////	_dma->Init(3);
////
////	_dma->MemCpy((char*)_flashMemory,t2,10);
////
////	 mem.ShowMemory();
////
////	Overclocking();
//
//
//int color_counter=0;
//
//
//
//
//
//
//Pixel _pixel;
//
//_pixel.Data =0;
//
//int k=1;
//
////RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);
////
////TIM_TimeBaseInitTypeDef timerInitStructure;
////
////
////TIM_TimeBaseStructInit(&timerInitStructure);
////timerInitStructure.TIM_Prescaler =0; // set 1us resolution
////timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
////timerInitStructure.TIM_Period =70;
////timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
////timerInitStructure.TIM_RepetitionCounter = 0;
////TIM_TimeBaseInit(TIM2, &timerInitStructure);
////
////TIM_GenerateEvent(TIM2,TIM_EventSource_CC4);
////
////
////outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
////outputChannelInit.TIM_Pulse = 30;
////outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
////outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_Low;
////
////TIM_OC4Init(TIM2, &outputChannelInit);
////TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
////TIM_CtrlPWMOutputs(TIM2, ENABLE);
////
////
////DMA_DeInit(DMA1_Channel7);
////DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(Port->ODR);//0x40013804;
////DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&_frame1[0];
////DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
////DMA_InitStructure.DMA_BufferSize = 65;
////DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
////DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
////DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
////DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
////DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
////DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
////DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
////DMA_Init(DMA1_Channel7, &DMA_InitStructure);
////
////TIM_DMACmd(TIM2, TIM_DMA_CC4, ENABLE);
////TIM_DMAConfig (TIM2, TIM_DMABase_CCR4, TIM_DMABurstLength_1Transfer);
////
////
////NVIC_InitTypeDef NVIC_InitStructure;
////NVIC_InitStructure.NVIC_IRQChannel =DMA1_Channel7_IRQn;
////NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////NVIC_Init( &NVIC_InitStructure );
////
////DMA_ClearFlag(DMA1_IT_TC7|DMA1_IT_HT7);
////
////
////
////DMA_ITConfig(DMA1_Channel7, DMA_IT_TC|DMA_IT_TE, ENABLE);
////DMA_Cmd(DMA1_Channel7,ENABLE);
////TIM_Cmd(TIM2, ENABLE);
////
////
////TIM_TimeBaseStructInit(&timerInitStructure);
////timerInitStructure.TIM_Prescaler =0; // set 1us resolution
////timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
////timerInitStructure.TIM_Period =100;
////timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
////timerInitStructure.TIM_RepetitionCounter = 0;
////TIM_TimeBaseInit(TIM3, &timerInitStructure);
////
////
////
////
////outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
////outputChannelInit.TIM_Pulse = 1;//99;
////outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
////outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_Low;
////
////TIM_OC3Init(TIM3, &outputChannelInit);
////TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
////
////outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
////outputChannelInit.TIM_Pulse = 99;
////outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
////outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_Low;
////
////TIM_OC4Init(TIM3, &outputChannelInit);
////TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
////
////TIM_Cmd(TIM3, ENABLE);
//
//
//k=150;
//int max = 1;
//for(int i=0;i<65;i++)
//{
//
//		_frame[i].Green = 15;
//		k+=300;
//		if(k==4096){k=1;}
//
//}
//
//grayscale =0;
//bool trigger =false;
//
//	while(1){
//
//
//		for(int j=0;j<64; j++)
//		{
//			if(_frame[j].Red>0){Port->BSRR=R1;}else{Port->BRR=R1;}
//			if(_frame[j].Green>0){Port->BSRR=G1;}else{Port->BRR=G1;}
//			if(_frame[j].Blue>0){Port->BSRR=B1;}else{Port->BRR=B1;}
//
////			if(_frame[j].Red>0){_frame1[j]|=(1<<3);}else{_frame1[j]&=~(1<<3);}
////			if(_frame[j].Green>0){_frame1[j]|=(1<<4);}else{_frame1[j]&=~(1<<4);}
////			if(_frame[j].Blue>0){_frame1[j]|=(1<<5);}else{_frame1[j]&=~(1<<5);}
//
//			CLK_PORT->BSRR=CLK;
//
//			CLK_PORT->BRR= CLK;
//
//			LAT_PORT->BSRR=LAT;
//
//			LAT_PORT->BRR= LAT;
//
//
//
//		}
//
//		if((current_row&(1<<0))>0){C_Port->BSRR= A;}else{C_Port->BRR= A;}
//		if((current_row&(1<<1))>0){C_Port->BRR= B;}else{C_Port->BSRR= B;}
//		//if((current_row&(1<<1))>0){C_Port->BRR= C;}else{C_Port->BSRR= C;}
//
////		C_Port->BSRR=A;
////
////		C_Port->BRR= A;
//
//
//		if(current_row++ ==4)
//		{
//			current_row=0;
//		}
//
////		if(grayscale++==15)
////		{
////			grayscale =0;
////			current_row++;
////		}
//
//		if(color_counter++>=3000)
//		{
//			color_counter=0;
//			max=1;
//
////			for(int i=0;i<16;i++)
////			{
////				_frame[i].Red =max;
////						//k+=10;
////			}
////
////			for(int i=16;i<32;i++)
////			{
////				_frame[i].Green =max;
////				//k+=10;
////			}
////
////			for(int i=32;i<40;i++)
////			{
////				_frame[i].Blue =max;
////				//k+=10;
////			}
//
//			max++;
//			if(max>=15){max=1;}
//			if(k==255){k=1;}
//
//			 if(trigger)
//			 {
//				 _leds.On(1);
//				 trigger = false;
//			 }
//			 else
//			 {
//				 _leds.Off(1);
//				 trigger = true;
//			 }
//
//		}
//
//
////		for(int i=0;i<100;i++)
////		{
////			asm("nop");
////			asm("nop");
////			asm("nop");
////			asm("nop");
////			asm("nop");
////		}
//
//	};
//
//	return 0;
//}
