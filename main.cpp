/*
 * main.cpp
 *
 *  Created on: 30 авг. 2017 г.
 *      Author: hudienko_a
 */
#include "Driver/platform.h"
#include "Extention/operators.h"
#include "Driver/nvic/nvic.h"

#include "Driver/usart/usart.h"
#include "Driver/spi/spi.h"
#include "Driver/spi/PSpi.h"
#include "Driver/i2c/i2c.h"

#include "Driver/led/led.h"
#include "Extention/sPtr.h"

#include "Driver/tim/tim.h"
#include "Driver/systick/systick.h"
#include "Driver/gpio/gpio.h"
#include "Device/SGPIO/SGPIO.h"
#include "Device/RF433/RF433.h"
#include "Driver/usb/usb.h"

#include "Device/encoder/encoder.h"

#include "Extention/GraphicTextFunctions.h"

#include "Device/Lcd/SSD1306.h"
#include "Device/Lcd/LcdSPI.h"

#include "Extention/animationProcessor/LoadingTable.h"

#include "Os/kernel.h"

//#include "ow.h"

using namespace Driver;
using namespace Device;
using namespace Os;

//InterruptController _ic;
Led _leds;
Kernel _ker;
//Encoder* _enc_;
SSD1306 _lcd_;
bool trigger;

extern "C"
{
	void Led_RW_ON()
	{
		LedPort->BSRR = LedPin2;
	}

	void  Led_RW_OFF()
	{
		LedPort->BRR = LedPin2;
	}
}



//void OnUsbReceive(unsigned char byte)
//{
//	if(byte=='A')
//	{
//		_enc_->ClearCounter();
//	}
//}


void Thread1()
{
	while(true)
	{
		_ker.EnterCritical();
		_lcd_.PutInt(0,0,2,_ker.CpuLoad);
		_lcd_.Invalidate();
		_ker.ExitCritical();
		_leds.On(1);
		_ker.Delay(50);
		_leds.Off(1);
		_ker.Delay(50);
	}
}


int main()
{
	//InterruptController::RemapToRam();
	_leds.Init();

	I2c _i2c(I2c::in_I2C1,LCD_ADDR,I2c::Master,I2c::s_400kHz);
	_i2c.Init();
	_lcd_.Init(&_i2c, true);
	_lcd_.Clear();
	_lcd_.PutInt(0,0,2,_ker.CpuLoad);
	_lcd_.Invalidate();

//	SystemTimer _systick(1000);
//	_systick.Init();
//	_systick.AddCallback(Kernel::OnTick);
//	_systick.Enable();
	_ker.Init();
	_ker.AddTask(Thread1,256,0,(char*)"test", 0,false);
	_ker.StartScheduler();

	//Tim _tim(Tim::Timer2,10,Tim::InterruptType::IT_Update);
	//Encoder _enc;
	//_enc.SetTimer(&_tim);

	//_enc.StartScan();
	//_enc_ = &_enc;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//	PSpi _pspi(PSpi::Master);
//	_pspi.Init();
//
//	LcdSPI _lcdSpi;
//	_lcdSpi.Init(&_pspi);
//	_lcdSpi.Clear();
//	_lcdSpi.PutStr(0,0,3,"Тест ");

//	SSD1306 _lcd_;
//	I2c _i2c(I2c::in_I2C1,LCD_ADDR,I2c::Master,I2c::s_400kHz);
//	_i2c.Init();
//	_lcd_.Init(&_i2c, true);
//
//	_lcd_.Clear();
//	//_lcd_.PutStr(0,0,1,"Типа русский текст");
//	_lcd_.Invalidate();
//	for(int i=0;i<50;i++)
//	{
//		ShowLoading(&_lcd_);
//	//_delay_ms(15);
//	}



//
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	// Configure pin in output push/pull mode
//	GPIO_InitStructure.GPIO_Pin = SCK|MOSI|D_C|CS|RES;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//
//	GPIO_ResetBits(GPIOA, RES);
//	_delay_ms(10);
//	GPIO_SetBits(GPIOA, RES);
//
//	GPIO_ResetBits(GPIOA, CS);
//
//
////		Init();
////		char buf[64];
////		therm_read_temperature(buf);
//
//
//
//	Usb _com;
//	_com.RxBufferSize = 64;
//	_com.TxBufferSize = 64;
//	_com.TypeUsb = VirtualComPort;
//
//	_com.Init();
//
//
//	_com.OnRecived = OnUsbReceive;

	int a=0;

	char data[100];

	int index=0, change=0,invalidate=0;

	while(1)
	{
		//_enc.Process();
		if(a++>=1500 && a< 3000)
			{
				_leds.On(1);

			}
			else if(a>=3000)
			{
				a=0;
				_leds.Off(1);
			}

		if(change++==10)
		{
			//_com.KeyboardSend("hello");
			//_com.MouseMove(rand(),rand());
			//_com.WriteWord(_enc.GetEncoderData());
//			_lcd_.Clear();
//			_lcd_.PutStr(index,0,3,"Тест ");
//			if(index>= 120)
//			{
//				index=-65;
//			}
//			index+=5;
//			change=0;
//
//			_lcd_.PutInt(0,27,2,a);
		}

//		if(invalidate++==10)
//		{
//			invalidate=0;
//			_lcd_.Invalidate();
//		}
		// _delay_ms(1);
	}

	return 0;
}

