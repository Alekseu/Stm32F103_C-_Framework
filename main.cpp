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
#include "Driver/i2c/i2c.h"

#include "Driver/led/led.h"
#include "Extention/sPtr.h"

#include "Driver/tim/tim.h"
#include "Driver/systick/systick.h"
#include "Driver/gpio/gpio.h"
#include "Device/SGPIO/SGPIO.h"
#include "Device/RF433/RF433.h"
#include "Driver/usb/usb.h"


using namespace Driver;
using namespace Device;

InterruptController _ic;
Led _led;
bool trigger;


#define SCK GPIO_Pin_0
#define MOSI GPIO_Pin_1
#define D_C GPIO_Pin_2
#define CS GPIO_Pin_3
#define RES GPIO_Pin_4
#define PORT GPIOA


//
//void SysTickCallback1()
//{
//   int a=0;
//}
//
//void SysTickCallback2()
//{
//	int a=0;
//}
//
//void GpioInterrupt1()
//{
//
//}
//
//void GpioInterrupt2()
//{
//	int a=0;
//}
//
//void InputsInterrupt()
//{
//
//}

//#define L1 GPIO_Pin_0
//#define L2 GPIO_Pin_2
//#define L3 GPIO_Pin_1
//#define L4 GPIO_Pin_3
//#define PORT GPIOA
//#define PHERIF RCC_APB2Periph_GPIOA
//
//#define StepsStart 150
//
//class Stepper
//{
//public:
//
//	enum StepMode
//	{
//		SingleStep,
//		MiniStep,
//		MicroStep
//	};
//
//	Stepper(int startDelay, int workDelay, StepMode mode, bool holding)
//	{
//		_delay=workDelay;
//		_startDelay =startDelay;
//		_workDelay = workDelay;
//		_mode = mode;
//		_holding = holding;
//		_currentStep=1;
//	}
//
//	void Init()
//	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//
//		GPIO_InitTypeDef GPIO_InitStructure;
//
//		// Configure pin in output push/pull mode
//		GPIO_InitStructure.GPIO_Pin = L1|L2|L3|L4;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//	}
//
//	void OneStep(bool rotation)
//	{
//
//		switch(_mode)
//		{
//
//		case SingleStep:
//			switch(_currentStep)
//			{
//				case 1:
//					GPIO_ResetBits(GPIOA,L2);
//					GPIO_ResetBits(GPIOA,L3);
//					GPIO_ResetBits(GPIOA,L4);
//					GPIO_SetBits(GPIOA,L1);
//
//					if(!_holding)
//					{
//						_delay_ms(1);
//						GPIO_ResetBits(GPIOA,L1);
//					}
//					break;
//				case 2:
//					GPIO_ResetBits(GPIOA,L1);
//					GPIO_ResetBits(GPIOA,L3);
//					GPIO_ResetBits(GPIOA,L4);
//					GPIO_SetBits(GPIOA,L2);
//
//					if(!_holding)
//					{
//						_delay_ms(1);
//						GPIO_ResetBits(GPIOA,L2);
//					}
//					break;
//				case 3:
//					GPIO_ResetBits(GPIOA,L1);
//					GPIO_ResetBits(GPIOA,L2);
//					GPIO_ResetBits(GPIOA,L4);
//					GPIO_SetBits(GPIOA,L3);
//					if(!_holding)
//					{
//						_delay_ms(1);
//						GPIO_ResetBits(GPIOA,L3);
//					}
//					break;
//				case 4:
//					GPIO_ResetBits(GPIOA,L1);
//					GPIO_ResetBits(GPIOA,L2);
//					GPIO_ResetBits(GPIOA,L3);
//					GPIO_SetBits(GPIOA,L4);
//					if(!_holding)
//					{
//						_delay_ms(1);
//						GPIO_ResetBits(GPIOA,L4);
//					}
//					break;
//				}
//
//			if(rotation)
//			{
//				if(_currentStep++>=4)
//				{
//					_currentStep=1;
//				}
//			}
//			else
//			{
//				if(_currentStep--<=1)
//				{
//					_currentStep=4;
//				}
//			}
//
//		break;
//
//		case MiniStep:
//				switch(_currentStep)
//				{
//					case 1:
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L1);
//						GPIO_SetBits(GPIOA,L2);
//						break;
//					case 2:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L2);
//						GPIO_SetBits(GPIOA,L3);
//						break;
//					case 3:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//
//						GPIO_SetBits(GPIOA,L3);
//						GPIO_SetBits(GPIOA,L4);
//						break;
//					case 4:
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//
//						GPIO_SetBits(GPIOA,L4);
//						GPIO_SetBits(GPIOA,L1);
//						break;
//				}
//				if(rotation)
//				{
//					if(_currentStep++>=4)
//					{
//						_currentStep=1;
//					}
//				}
//				else
//				{
//					if(_currentStep--<=1)
//					{
//						_currentStep=4;
//					}
//				}
//
//			break;
//
//			case MicroStep:
//				switch(_currentStep)
//				{
//					case 1:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L1);
//
//						break;
//					case 2:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L1);
//						GPIO_SetBits(GPIOA,L2);
//						break;
//					case 3:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L2);
//						break;
//					case 4:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L2);
//						GPIO_SetBits(GPIOA,L3);
//						break;
//					case 5:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//
//						GPIO_SetBits(GPIOA,L3);
//						break;
//					case 6:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L3);
//						GPIO_SetBits(GPIOA,L4);
//						break;
//					case 7:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//
//						GPIO_SetBits(GPIOA,L4);
//						break;
//					case 8:
//						GPIO_ResetBits(GPIOA,L1);
//						GPIO_ResetBits(GPIOA,L2);
//						GPIO_ResetBits(GPIOA,L3);
//						GPIO_ResetBits(GPIOA,L4);
//
//						GPIO_SetBits(GPIOA,L4);
//						GPIO_SetBits(GPIOA,L1);
//						break;
//				}
//				if(rotation)
//				{
//					if(_currentStep++>=8)
//					{
//						_currentStep=1;
//					}
//				}
//				else
//				{
//					if(_currentStep--<=1)
//					{
//						_currentStep=8;
//					}
//				}
//			break;
//		}
//
//		_delay_ms(_delay);
//
//	}
//
//	void MultiStep(int steps, bool rotation)
//	{
//		if(steps<StepsStart*2)
//		{
//			for(int i=0; i<steps;i++)
//			{
//				OneStep(rotation);
//			}
//		}
//		else
//		{
//
//			_delay = _startDelay;
//			int counter=0;
//			for(int i=0;i<StepsStart;i++)
//			{
//				OneStep(rotation);
//				if(counter++>= 50)
//				{
//					counter=0;
//					_delay--;
//				}
//			}
//			_delay = _workDelay;
//			for(int i=0;i<steps - StepsStart;i++)
//			{
//				OneStep(rotation);
//			}
//		}
//
//	}
//
//
//private:
//
//	int _delay;
//	int _startDelay;
//	int _workDelay;
//	bool _holding;
//	StepMode _mode;
//
//	char _currentStep;
//};
//
//Stepper _st(6,4,Stepper::StepMode::MiniStep,true);
//bool t=true;

//void TimerElapsed()
//{
//	_st.OneStep(t);
//}


 char Font[158][5]  =
{
 0x00, 0x00, 0x00, 0x00, 0x00,// 20 space
 0x00, 0x00, 0x5F, 0x00, 0x00,// 21 !
 0x00, 0x07, 0x00, 0x07, 0x00,// 22 "
 0x14, 0x7F, 0x14, 0x7F, 0x14,// 23 #
 0x24, 0x2A, 0x7F, 0x2A, 0x12,// 24 $
 0x23, 0x13, 0x08, 0x64, 0x62,// 25 %
 0x36, 0x49, 0x55, 0x22, 0x50,// 26 &
 0x00, 0x05, 0x03, 0x00, 0x00,// 27 '
 0x00, 0x1C, 0x22, 0x41, 0x00,// 28 (
 0x00, 0x41, 0x22, 0x1C, 0x00,// 29 )
 0x14, 0x08, 0x3E, 0x08, 0x14,// 2a *
 0x08, 0x08, 0x3E, 0x08, 0x08,// 2b +
 0x00, 0x50, 0x30, 0x00, 0x00,// 2c ,
 0x08, 0x08, 0x08, 0x08, 0x08,// 2d -
 0x00, 0x60, 0x60, 0x00, 0x00,// 2e .
 0x20, 0x10, 0x08, 0x04, 0x02,// 2f /
 0x3E, 0x51, 0x49, 0x45, 0x3E,// 30 0
 0x00, 0x42, 0x7F, 0x40, 0x00,// 31 1
 0x42, 0x61, 0x51, 0x49, 0x46,// 32 2
 0x21, 0x41, 0x45, 0x4B, 0x31,// 33 3
 0x18, 0x14, 0x12, 0x7F, 0x10,// 34 4
 0x27, 0x45, 0x45, 0x45, 0x39,// 35 5
 0x3C, 0x4A, 0x49, 0x49, 0x30,// 36 6
 0x01, 0x71, 0x09, 0x05, 0x03,// 37 7
 0x36, 0x49, 0x49, 0x49, 0x36,// 38 8
 0x06, 0x49, 0x49, 0x29, 0x1E,// 39 9
 0x00, 0x36, 0x36, 0x00, 0x00,// 3a :
 0x00, 0x56, 0x36, 0x00, 0x00,// 3b ;
 0x08, 0x14, 0x22, 0x41, 0x00,// 3c <
 0x14, 0x14, 0x14, 0x14, 0x14,// 3d =
 0x00, 0x41, 0x22, 0x14, 0x08,// 3e >
 0x02, 0x01, 0x51, 0x09, 0x06,// 3f ?
 0x32, 0x49, 0x79, 0x41, 0x3E,// 40 @
 0x7E, 0x11, 0x11, 0x11, 0x7E,// 41 A
 0x7F, 0x49, 0x49, 0x49, 0x36,// 42 B
 0x3E, 0x41, 0x41, 0x41, 0x22,// 43 C
 0x7F, 0x41, 0x41, 0x22, 0x1C,// 44 D
 0x7F, 0x49, 0x49, 0x49, 0x41,// 45 E
 0x7F, 0x09, 0x09, 0x09, 0x01,// 46 F
 0x3E, 0x41, 0x49, 0x49, 0x7A,// 47 G
 0x7F, 0x08, 0x08, 0x08, 0x7F,// 48 H
 0x00, 0x41, 0x7F, 0x41, 0x00,// 49 I
 0x20, 0x40, 0x41, 0x3F, 0x01,// 4a J
 0x7F, 0x08, 0x14, 0x22, 0x41,// 4b K
 0x7F, 0x40, 0x40, 0x40, 0x40,// 4c L
 0x7F, 0x02, 0x0C, 0x02, 0x7F,// 4d M
 0x7F, 0x04, 0x08, 0x10, 0x7F,// 4e N
 0x3E, 0x41, 0x41, 0x41, 0x3E,// 4f O
 0x7F, 0x09, 0x09, 0x09, 0x06,// 50 P
 0x3E, 0x41, 0x51, 0x21, 0x5E,// 51 Q
 0x7F, 0x09, 0x19, 0x29, 0x46,// 52 R
 0x46, 0x49, 0x49, 0x49, 0x31,// 53 S
 0x01, 0x01, 0x7F, 0x01, 0x01,// 54 T
 0x3F, 0x40, 0x40, 0x40, 0x3F,// 55 U
 0x1F, 0x20, 0x40, 0x20, 0x1F,// 56 V
 0x3F, 0x40, 0x38, 0x40, 0x3F,// 57 W
 0x63, 0x14, 0x08, 0x14, 0x63,// 58 X
 0x07, 0x08, 0x70, 0x08, 0x07,// 59 Y
 0x61, 0x51, 0x49, 0x45, 0x43,// 5a Z
 0x00, 0x7F, 0x41, 0x41, 0x00,// 5b [
 0x02, 0x04, 0x08, 0x10, 0x20,// 5c Yen Currency Sign
 0x00, 0x41, 0x41, 0x7F, 0x00,// 5d ]
 0x04, 0x02, 0x01, 0x02, 0x04,// 5e ^
 0x40, 0x40, 0x40, 0x40, 0x40,// 5f _
 0x00, 0x01, 0x02, 0x04, 0x00,// 60 `
 0x20, 0x54, 0x54, 0x54, 0x78,// 61 a
 0x7F, 0x48, 0x44, 0x44, 0x38,// 62 b
 0x38, 0x44, 0x44, 0x44, 0x20,// 63 c
 0x38, 0x44, 0x44, 0x48, 0x7F,// 64 d
 0x38, 0x54, 0x54, 0x54, 0x18,// 65 e
 0x08, 0x7E, 0x09, 0x01, 0x02,// 66 f
 0x0C, 0x52, 0x52, 0x52, 0x3E,// 67 g
 0x7F, 0x08, 0x04, 0x04, 0x78,// 68 h
 0x00, 0x44, 0x7D, 0x40, 0x00,// 69 i
 0x20, 0x40, 0x44, 0x3D, 0x00,// 6a j
 0x7F, 0x10, 0x28, 0x44, 0x00,// 6b k
 0x00, 0x41, 0x7F, 0x40, 0x00,// 6c l
 0x7C, 0x04, 0x18, 0x04, 0x78,// 6d m
 0x7C, 0x08, 0x04, 0x04, 0x78,// 6e n
 0x38, 0x44, 0x44, 0x44, 0x38,// 6f o
 0x7C, 0x14, 0x14, 0x14, 0x08,// 70 p
 0x08, 0x14, 0x14, 0x18, 0x7C,// 71 q
 0x7C, 0x08, 0x04, 0x04, 0x08,// 72 r
 0x48, 0x54, 0x54, 0x54, 0x20,// 73 s
 0x04, 0x3F, 0x44, 0x40, 0x20,// 74 t
 0x3C, 0x40, 0x40, 0x20, 0x7C,// 75 u
 0x1C, 0x20, 0x40, 0x20, 0x1C,// 76 v
 0x3C, 0x40, 0x30, 0x40, 0x3C,// 77 w
 0x44, 0x28, 0x10, 0x28, 0x44,// 78 x
 0x0C, 0x50, 0x50, 0x50, 0x3C,// 79 y
 0x44, 0x64, 0x54, 0x4C, 0x44,// 7a z
 0x00, 0x08, 0x36, 0x41, 0x00,// 7b <
 0x00, 0x00, 0x7F, 0x00, 0x00,// 7c |
 0x00, 0x41, 0x36, 0x08, 0x00,// 7d >
 0x7E, 0x11, 0x11, 0x11, 0x7E,// C0 ?
 0x7F, 0x49, 0x49, 0x49, 0x31,// C1 ?
 0x7F, 0x49, 0x49, 0x49, 0x36,// C2 ?
 0x7F, 0x01, 0x01, 0x01, 0x03,// C3 ?
 0x70, 0x29, 0x27, 0x21, 0x7F,// C4 ?
 0x7F, 0x49, 0x49, 0x49, 0x41,// C5 ?
 0x77, 0x08, 0x7F, 0x08, 0x77,// C6 ?
 0x41, 0x41, 0x41, 0x49, 0x76,// C7 ?
 0x7F, 0x10, 0x08, 0x04, 0x7F,// C8 ?
 0x7F, 0x10, 0x09, 0x04, 0x7F,// C9 ?
 0x7F, 0x08, 0x14, 0x22, 0x41,// CA ?
 0x20, 0x41, 0x3F, 0x01, 0x7F,// CB ?
 0x7F, 0x02, 0x0C, 0x02, 0x7F,// CC ?
 0x7F, 0x08, 0x08, 0x08, 0x7F,// CD ?
 0x3E, 0x41, 0x41, 0x41, 0x3E,// CE ?
 0x7F, 0x01, 0x01, 0x01, 0x7F,// CF ?
 0x7F, 0x09, 0x09, 0x09, 0x06,// D0 ?
 0x3E, 0x41, 0x41, 0x41, 0x22,// D1 ?
 0x01, 0x01, 0x7F, 0x01, 0x01,// D2 ?
 0x47, 0x28, 0x10, 0x08, 0x07,// D3 ?
 0x1E, 0x21, 0x7F, 0x21, 0x1E,// D4 ?
 0x63, 0x14, 0x08, 0x14, 0x63,// D5 ?
 0x3F, 0x20, 0x20, 0x20, 0x5F,// D6 ?
 0x07, 0x08, 0x08, 0x08, 0x7F,// D7 ?
 0x7F, 0x40, 0x7F, 0x40, 0x7F,// D8 ?
 0x3F, 0x20, 0x3F, 0x20, 0x5F,// D9 ?
 0x01, 0x7F, 0x48, 0x48, 0x30,// DA ?
 0x7F, 0x48, 0x30, 0x00, 0x7F,// DB ?
 0x00, 0x7F, 0x48, 0x48, 0x30,// DC ?
 0x41, 0x41, 0x41, 0x49, 0x3E,// DD ?
 0x7F, 0x08, 0x3E, 0x41, 0x3E,// DE ?
 0x46, 0x29, 0x19, 0x09, 0x7F,// DF ?
 0x20, 0x54, 0x54, 0x54, 0x78,// E0 ?
 0x3C, 0x4A, 0x4A, 0x49, 0x31,// E1 ?
 0x7C, 0x54, 0x54, 0x28, 0x00,// E2 ?
 0x7C, 0x04, 0x04, 0x04, 0x0C,// E3 ?
 0x72, 0x2A, 0x26, 0x22, 0x7E,// E4 ?
 0x38, 0x54, 0x54, 0x54, 0x18,// E5 ?
 0x6C, 0x10, 0x7C, 0x10, 0x6C,// E6 ?
 0x44, 0x44, 0x54, 0x54, 0x38,// E7 ?
 0x7C, 0x20, 0x10, 0x08, 0x7C,// E8 ?
 0x7C, 0x21, 0x12, 0x09, 0x7C,// E9 ?
 0x7C, 0x10, 0x28, 0x44, 0x00,// EA ?
 0x20, 0x44, 0x3C, 0x04, 0x7C,// EB ?
 0x7C, 0x08, 0x10, 0x08, 0x7C,// EC ?
 0x7C, 0x10, 0x10, 0x10, 0x7C,// ED ?
 0x38, 0x44, 0x44, 0x44, 0x38,// EE ?
 0x7C, 0x04, 0x04, 0x04, 0x7C,// EF ?
 0x7C, 0x14, 0x14, 0x14, 0x08,// F0 ?
 0x38, 0x44, 0x44, 0x44, 0x20,// F1 ?
 0x04, 0x04, 0x7C, 0x04, 0x04,// F2 ?
 0x44, 0x28, 0x10, 0x08, 0x04,// F3 ?
 0x08, 0x14, 0x7E, 0x14, 0x08,// F4 ?
 0x44, 0x28, 0x10, 0x28, 0x44,// F5 ?
 0x3C, 0x40, 0x40, 0x7C, 0x40,// F6 ?
 0x0C, 0x10, 0x10, 0x10, 0x7C,// F7 ?
 0x7C, 0x40, 0x7C, 0x40, 0x7C,// F8 ?
 0x3C, 0x20, 0x3C, 0x20, 0x7C,// F9 ?
 0x04, 0x7C, 0x50, 0x50, 0x20,// FA ?
 0x7C, 0x50, 0x20, 0x00, 0x7C,// FB ?
 0x00, 0x7C, 0x50, 0x50, 0x20,// FC ?
 0x28, 0x44, 0x44, 0x54, 0x38,// FD ?
 0x7C, 0x10, 0x38, 0x44, 0x38,// FE ?
 0x48, 0x54, 0x34, 0x14, 0x7C
 }; // FF ?

unsigned char lcd_tx(unsigned char tx,unsigned char dc)
{
		unsigned char i=0;

	if (dc==0x00)GPIO_ResetBits(GPIOA,D_C); else GPIO_SetBits(GPIOA,D_C);

          for( i = 0; i < 8; i++)
        {

        	  GPIO_ResetBits(GPIOA,SCK);
         if ((tx&0x80)==0x00) GPIO_ResetBits(GPIOA,MOSI); else GPIO_SetBits(GPIOA,MOSI);



        // asm("nop");
         GPIO_SetBits(GPIOA,SCK);
		//	asm("nop");
			  tx<<=1;
        }
          GPIO_ResetBits(GPIOA,SCK);
        return 0;
}

unsigned char lcd_init()
{

  lcd_tx(0x21,0);
  asm("nop");
  lcd_tx(0x09,0);
  asm("nop");
  lcd_tx(0xe0,0);
  asm("nop");
  lcd_tx(0x14,0);
  asm("nop");
  lcd_tx(0x05,0);
  asm("nop");
  lcd_tx(0x20,0);
  asm("nop");
  lcd_tx(0x0c,0);
  asm("nop");
  lcd_tx(0x80,0);
  asm("nop");
  asm("nop");

  return 0;
}

void lcd_gotoxy(char x , char y)  //переместить на позицию ху
{
 lcd_tx(0x80+x,0);//x direction
 lcd_tx(0x40+y,0);//y direction
}

unsigned char lcd_clear()  //очистить экран
{
  char x,y;
  x=0;y=0;
  while(y<6)
  {
   lcd_gotoxy(0,y);
   x=0;
   while(x<101)
   {
   lcd_tx(0x00,1);
   x++;
   }
   y++;
  }

 return 0;
}

void lcd_putchar(char f)
{

 if(f>0x7D) //если єто руские буквы
 {
    //отнимаем значение первой руской буквы
	//и прибавляем длинну английского знакогенератора
	f=f-0xC0+94;
 }else
 {
    f=f-0x20; //
 }
   for(char i=0;i<5;i++)
    {
		 char charater = (char)(Font[f][i]);
     lcd_tx(charater,1);
    }
   lcd_tx(0x00,1);

}

void lcd_str(const char *str) //вывод строки находящийся в оперативе
{
 char k;
 for(int i=0;i<14;i++)
 {
	 lcd_putchar(str[i]);
 }
 }

void lcd_putbyte(char byte)
{
	lcd_tx(byte,1);
}

int field_length = 6;
const char* mass[]=
{
		"field 1234567890",
		"field 1234567890",
		"field 1234567890",
		"field 1234567890",
		"field 1234567890",
		"field 1234567890"
};


void lcd_menu()
{
	for(int i=0;i<field_length;i++)
	{
		lcd_gotoxy(0,i);
		lcd_str(mass[i]);
	}
}

int main()
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

			GPIO_InitTypeDef GPIO_InitStructure;

			// Configure pin in output push/pull mode
			GPIO_InitStructure.GPIO_Pin = SCK|MOSI|D_C|CS|RES;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOA, &GPIO_InitStructure);


			GPIO_ResetBits(GPIOA, RES);
			_delay_ms(10);
			GPIO_SetBits(GPIOA, RES);

			GPIO_ResetBits(GPIOA, CS);

			lcd_init();
			lcd_clear();

	//_ic.RemapToRam();
//
//
//	SystemTimer _systim(1000);
//	_systim.Init();
//	_systim.AddCallback(SysTickCallback1);
//	_systim.AddCallback(SysTickCallback2);
//	_systim.Enable();
//
//	Usb* _usb = new Usb();
//
//	__enable_irq();
//
//	_usb->RxBufferSize = 64;
//	_usb->TxBufferSize = 64;
//	_usb->TypeUsb = VirtualComPort;
//
//	_usb->Init();
////
////	SerialPort* _port = new SerialPort(SerialPort::COM1,9600);
////	_port->Init();
////
////
//	_led.Init();
//
//	RF433* _rf = new RF433();
//	_rf->Init();
//	_rf->StartReceive();

//	I2c* _i2c = new I2c(I2c::in_I2C1,0xc, I2c::Master,I2c::s_50kHz);
//	_i2c->Init();
//
//	Spi* _spi = new Spi(Spi::sn_SPI1,Spi::Master,Spi::Sys_div_128);
//	_spi->Init();

//	GPIO* _gpio = new GPIO(GPIO::Port::PORTC,GPIO::Pin::Pin10,GPIO::Mode::IN_FLOATING,GPIO::Speed::Speed_2MHz);
//	_gpio->Init();
//	_gpio->SetCallback(GpioInterrupt1,GPIO::InterruptType::Rising);
//
//	GPIO* _gpio1 = new GPIO(GPIO::Port::PORTE,GPIO::Pin::Pin2);
//	_gpio1->Init();
//
//	Tim _tim(Tim::Timer2,1500,Tim::InterruptType::IT_Update);
//	_tim.OnElapsed = TimerElapsed;
//	_tim.Init();
//	_tim.Enable();

//	SGPIO * _gpio2 = new SGPIO();
//	_gpio2->Init();
//	_gpio2->SetOutput(SGPIO::pin1, true);
//	_gpio2->SetTimer(&_tim);
//	_gpio2->SetCallback(InputsInterrupt);
//	_tim.Enable();

		int counter=0;
//		int delay = 3;
//		char step =2;
//
//
//
//
//		_st.Init();

		lcd_menu();

	while(1)
	{
		//_st.OneStep(t);
		//lcd_tx(counter++,1);
		//lcd_putchar(counter++);

//		_st.MultiStep(200,t);
//
//
//		if(counter++>=5)
//		{
//			counter=0;
//			t=!t;
//		}
//		switch(step)
//		{
//			case 1:
//				GPIO_SetBits(GPIOA,GPIO_Pin_0);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_2);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_1);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_3);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//
//				break;
//			case 2:
//
//
//				GPIO_SetBits(GPIOA,GPIO_Pin_0);
//				GPIO_SetBits(GPIOA,GPIO_Pin_2);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//				GPIO_SetBits(GPIOA,GPIO_Pin_1);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//				GPIO_SetBits(GPIOA,GPIO_Pin_3);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//				GPIO_SetBits(GPIOA,GPIO_Pin_0);
//				_delay_ms(delay);
//				GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//
//				break;
//
//
//		}
//
//		if(counter++>100)
//		{
//			counter=0;
//
//			if(t)
//			{
//				delay++;
//			}else
//			{
//				delay--;
//			}
//
//			if(delay >= 8 && t==true)
//			{
//				t=false;
//			}
//
//			if(delay<=1 && t==false)
//			{
//				t=true;
//			}
//		}

//		_port->SendData((uint8_t*)"Hello world\r\n",13);
    	//_usb->SendData((uint8_t*)"Hello\r\n",7);

//		_led.On(1);
//		//_gpio1->Write(true);
//		if(_port->GetAnswer(150))
//		{
//			_port->SendData((uint8_t*)_port->Data.ToString(),_port->Data.Lendth());
//			_port->WriteByte('\r');
//			_port->WriteByte('\n');
//			_port->ClearBuffer();
//			_port->Data.Clear();
//		}
//
//
//		_led.Off(1);
//		//_gpio1->Write(false);
		_delay_ms(15);
	}

	return 0;
}

