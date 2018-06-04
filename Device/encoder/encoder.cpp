/*
 * encoder.cpp
 *
 *  Created on: 04 июня 2018 г.
 *      Author: hudienko_a
 */
#include "encoder.h"

namespace Device
{
	Encoder* Encoder::eObj =0;

	Encoder::Encoder()
	{
		_encState=0;
		_encData=0;
		OnKeyPress=0;
		OnMove=0;
		_timer=0;
		RCC_APB2PeriphClockCmd(ENCODER_RCC,ENABLE);

		GPIO_InitStructure.GPIO_Pin =  ENCODER_KEY_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =  ENCODER_PIN1|ENCODER_PIN2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);
		eObj = this;
	}

	Encoder::Encoder(IHDriver* timer_)
	{
		_encState=0;
		_encData=0;
		OnKeyPress=0;
		OnMove=0;
		_timer=0;
		RCC_APB2PeriphClockCmd(ENCODER_RCC,ENABLE);

		GPIO_InitStructure.GPIO_Pin =  ENCODER_KEY_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =  ENCODER_PIN1|ENCODER_PIN2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);
		_timer = timer_;
		_timer->OnElapsed =TimerElapsedWrapper;
			eObj = this;
	}

	void Encoder::SetTimer(IHDriver* timer_)
	{
		_timer = timer_;
	}

	void Encoder::StartScan()
	{
		if(_timer!=0)
		{
			_timer->Enable();
		}
	}

	void Encoder::StopScan()
	{
		if(_timer!=0)
		{
			_timer->Disable();
		}
	}

	void Encoder::Process()
	{
		_encoderScan();
	}

	uint32_t Encoder::GetEncoderData()
	{
		return _encData;
	}

	void TimerElapsedWrapper()
	{
		if(Encoder::eObj!=0)
		{
			Encoder::eObj->_encoderScan();
		}
	}

	void Encoder::_encoderScan(void)
	{
		uint8_t New=0;

		if(GPIO_ReadInputDataBit(ENCODER_PORT, GPIO_Pin_1))
			New|=(1<<0);
		else
			New&=~(1<<0);

		if(GPIO_ReadInputDataBit(ENCODER_PORT, GPIO_Pin_2))
			New|=(1<<1);
		else
			New&=~(1<<1);

		if(GPIO_ReadInputDataBit(ENCODER_PORT, ENCODER_KEY_PIN))
		{
			if(OnKeyPress!=0)
			{
				OnKeyPress();
			}
		}

		// Смотря в какую сторону оно поменялось -- увеличиваем
		// Или уменьшаем счетный регистр

		switch(_encState)
		{
		case 2:
		{
			if(New == 3) {_encData++;_preEvent(true);}
			if(New == 0) {_encData--;_preEvent(false);}
			break;
		}

		case 0:
		{
			if(New == 2) {_encData++;_preEvent(true);}
			if(New == 1) {_encData--;_preEvent(false);}
			break;
		}
		case 1:
		{
			if(New == 0) {_encData++;_preEvent(true);}
			if(New == 3) {_encData--;_preEvent(false);}
			break;
		}
		case 3:
		{
			if(New == 1) {_encData++;_preEvent(true);}
			if(New == 2) {_encData--;_preEvent(false);}
			break;
		}
		}

		_encState = New;		// Записываем новое значение
		// Предыдущего состояния
	}

	void Encoder::_preEvent(bool type)
	{
		if(OnMove!=0)
		{
			if(type)
			{
				OnMove(Up,_encData);
			}
			else
			{
				OnMove(Down,_encData);
			}
		}
	}


}
