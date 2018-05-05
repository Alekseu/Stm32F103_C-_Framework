/*
 * m.h
 *
 *  Created on: 05 мая 2018 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_MATRIXMONO_M_H_
#define DEVICE_MATRIXMONO_M_H_

//
///*
// * main.cpp
// *
// *  Created on: 06 сент. 2017 г.
// *      Author: hudienko_a
// */
//
//#include "main.h"
//
//#include <stdlib.h>
//#include "Animation.h"
//
//extern "C"
//{
//extern unsigned short _fps;
//       unsigned short _k;
//}
//
//
//int screensaverTimeout=0;
//int loadingLog_counter=0;
//bool boot = false;
//bool showLoad = false;
//// отривовка фпс-ов
//char f[8];
//void draw()
//{
//	f[0] = _fps/100+0x30;
//	f[1] = _fps/10%10+0x30;
//	f[2] = _fps%10+0x30;
//	f[3] = '/';
//	f[4] = _k/100+0x30;
//	f[5] = _k/10%10+0x30;
//	f[6] = _k%10+0x30;
//	f[7] = 0;
//	_matrix->DrawString(1,0,0,(uint8_t*)f,8);
//	_fps=0;
//	_k=0;
//
//
//}
//
//// обновление экрана
//void fillbuff()
//{
//	_matrix->FillOutputBuffer();
//	_k++;
//}
//
//// все что касается скринсавера
//int x =54;
//int y =32;
//bool x1=true;
//bool y1=true;
//char _f_color=1;
//
//unsigned char _screenServerText[15];
//unsigned char _screenServerFont=1;
//unsigned char _xConstant =20;
//unsigned char _yConstant = 15;
//unsigned int _total_bytes;
//int counter_color=0;
//void ScreenServer()
//{
//	if(y1)
//	{
//		_matrix->DrawString(x-1,y+1,_screenServerFont,(uint8_t*) "         ",_f_color);
//	}
//	if(x1)
//	{
//		_matrix->DrawString(x+1,y-1,_screenServerFont,(uint8_t*) "         ",_f_color);
//	}
//	_matrix->DrawString(x-1,y-1,_screenServerFont,(uint8_t*) "         ",_f_color);
//	_matrix->DrawString(x-21,y+1,_screenServerFont,(uint8_t*) "         ",_f_color);
//	_matrix->DrawString(x,y,_screenServerFont,(uint8_t*) _screenServerText,_f_color);
//
//
//	if(x1)x--;else x++;
//	if(y1)y--;else y++;
//
//	if(x<0)x1=false;
//	if(x>(_settings.Width-(_xConstant)))x1=true;
//
//	if(y<0)y1=false;
//	if(y>(_settings.Hieght-_yConstant))y1=true;
//
//	if(counter_color++>=10)
//	{
//		counter_color=0;
//	if(_f_color++>=4)
//	{
//		_f_color=1;
//	}
//	}
//}
//void ScreanTimeout()
//{
//	screensaverTimeout++;
//}
//
//int load_pointer=0;
//void Loading()
//{
//
//	unsigned char* ponter = (unsigned char*)load[load_pointer];
//		for(int j=0;j<64;j++)
//		{
//			for(int i=0;i<128;i++)
//			{
//				char t = ponter[0];
//				if(t>0)
//				{
//					_matrix->SetPixel(i,j,1,ponter[0]);
//				}
//				else
//				{
//					_matrix->SetPixel(i,j,0,ponter[0]);
//				}
//				ponter++;
//			}
//		}
//		load_pointer++;
//		if(load_pointer>=22)
//		{
//			load_pointer=0;
//		}
//
//		_matrix->DrawString(30,8*7,0,(uint8_t*)"Loading...",8);
//}
//
//// все что касается консоли
//char buf[8][22];
//void Write(const char* str, int len)
//{
//	//int count=0;
//	int offset=0;
//
//	for(int i=0;i<len;i++)
//	{
//		if(str[i]==']' || str[i]==')')
//			offset =i+1;
//			break;
//	}
//
//	str+=offset;
//		for(int i=0;i<7;i++)
//		{
//			memset(&buf[i][0],0,22);
//			memcpy(&buf[i][0],&buf[i+1][0],22);
//		}
//
//		memset(&buf[7][0],0,22);
//		memcpy(&buf[7][0],str,22);
//
//}
//void Invalidate()
//{
//	for(int i=0;i<8;i++)
//	{
//		//_matrix->DrawString(0,i*8,0,(uint8_t*)"                       ",4);
//		_matrix->DrawString(0,i*8,0,(uint8_t*)&buf[i][0],21,4);
//	}
//}
//
//
//
//
//int main()
//{
//	InitMcu();
//	int ton=0;
//	char x=0;
//	char y=0;
//
//	//установка таймера отрисовки фпс-ов
//	_timer.setTimer(1,1000,draw);
//	//_timer.Enable(1);
//	//таймер обновления экрана
//	_timer.setTimer(2,40,fillbuff);
//	_timer.Enable(2);
//	//текс скринсавера и таймер обновления скринсавера
//	memcpy(_screenServerText,"RP",2);
//	_timer.setTimer(3,40,ScreenServer);
//	//_timer.Enable(3);
//
//
//	_timer.setTimer(4,50,Loading);
//		//_timer.Enable(4);
//	_timer.setTimer(5,1000,ScreanTimeout);
//	_timer.Enable(5);
//
//	bool trigger=false;
//	bool screensaver = false;
//
//	char index=0;
//	char mass[128];
//	char mass1[64];
//
//	while(true)
//	{
//		if(trigger)
//		{
//		  _led.On(1);
//		}
//		else
//		{
//		  _led.Off(1);
//		}
//		trigger=!trigger;
//
//		unsigned char byte =0;
//
//		if(_console->ReadByte(&byte,3))
//		{
//
//			if(byte==0x0d && !showLoad)
//			{
//				Write(mass,index);
//					Invalidate();
//					screensaverTimeout =0;
//					loadingLog_counter++;
//
//					if(loadingLog_counter>=70)
//					{
//						showLoad = true;
//						_timer.Enable(4);
//					}
////				for(int i=0;i<index;i++)
////				{
////				_com.WriteByte(mass[i]);
////				}
////				_com.WriteByte('\r');
////				_com.WriteByte('\n');
//				memset(mass,0,64);
//				index=0;
//			}
//			else
//			{
//				mass[index++] = byte;
//			}
//		}
//		else
//		{
//
//		}
//
//		if(screensaverTimeout>=20 && !boot)
//		{
//			boot = true;
//			_matrix->Clear();
//			_timer.Enable(3);
//			_timer.Disable(4);
//			_timer.Disable(5);
//		}
//
//
////		int len =_com.ReadData((uint8_t*)&mass1);
////		if(len>0)
////		{
////
////			for(int i=0;i<len;i++)
////			{
////				_console->WriteByte(mass1[i]);
////			}
////		}
//
//	}
//}
//
//void InitMcu()
//{
//	__disable_irq();
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//	DBGMCU->CR = 0x00000000;
//
//	_timer.Init();
//
//
//
//	_led.Init();
//	//_ee.Init();
//	//_ee.Read(&_settings,sizeof(Settings));
//
//	_settings.Width = 128;
//	_settings.Hieght = 64;
//
//	_matrix = new LedMatrix(_settings.Width,_settings.Hieght,LedMatrix::MatrixType::Scan_1_16,LedMatrix::MatrixGrayScale::Scale_8);
//	_matrix->Init();
//
//	__enable_irq();
//	_com.RxBufferSize = 64;
//	_com.TxBufferSize = 64;
//	_com.TypeUsb = VirtualComPort;
//	_com.Init();
//
//	_console = new SerialPort(SerialPort::PortName::COM1,115200);
//	_console->Init();
//
//}




#endif /* DEVICE_MATRIXMONO_M_H_ */
