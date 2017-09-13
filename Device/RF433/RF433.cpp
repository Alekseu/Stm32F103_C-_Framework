/*
 * RF433.cpp
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#include "RF433.h"



namespace Device
{
	RF433* RF433::RFObj =0;

	RF433::RF433()
	{
		_callback =0;
		_timer=0;
		_oldState =0;
		_gpio =0;
		_bitCount = 11;
		_input=0;
		_interruptCount=0;
		_tail=0;
	}

	RF433::~RF433()
	{

	}

	void RF433::Init()
	{
		RF433::RFObj = this;
		_timer = new Tim(Tim::Timer2,250,Tim::InterruptType::IT_Update);
		_timer->OnElapsed = InterruptTimerWraper;

		_gpio = new GPIO(GPIO::Port::PORTA,GPIO::Pin::Pin0,GPIO::Mode::IN_FLOATING,GPIO::Speed::Speed_50MHz);
		_gpio->Init();
		_gpio->SetCallback(InterruptGpioWraper,GPIO::InterruptType::Rising_Falling);

		//_gpio->EnableIrq();
		//_oldState = if(GPIO_ReadInputDataBit((GPIO_TypeDef*)_port, _pin))
	}

	void RF433::StartReceive()
	{
		_gpio->EnableIrq();
	}
	void RF433::StopReceive()
	{
		_gpio->DisableIrq();
		_timer->Disable();
	}

	void RF433::SetCallback(RF433Callback* callback)
	{
		_callback = callback;
	}

	void RF433::Received(uint16_t inp)
	{
		_buff[_tail++] = inp>>8&0xFF;
		if(_tail>=5)_tail=0;
	}

	char*  RF433::getString()
	{
		return _buff;
	}

	void RF433::InterruptTimerWraper()
	{
		//if(RF433::RFObj==0) return;

		if(RF433::RFObj->_bitCount==0)
		{
			RF433::RFObj-> Received(RF433::RFObj->_input);
			RF433::RFObj->_timer->Disable();
			RF433::RFObj->_gpio->EnableIrq();
		}

//		if(RF433::RFObj->_interruptCount++==44)
//		{
//			//RF433::RFObj-> Received(0);
//			RF433::RFObj->_timer->Disable();
//			RF433::RFObj->_gpio->EnableIrq();
//			RF433::RFObj->_interruptCount=0;
//		}

		bool _newState = RF433::RFObj->_gpio->Read();

//		if(_newState != RF433::RFObj->_oldState)
//		{
			if(_newState)
			{
				RF433::RFObj->_input|=(1<<RF433::RFObj->_bitCount-1);
				RF433::RFObj->_bitCount--;
			}
			else
			{
				RF433::RFObj->_input&=~(1<<RF433::RFObj->_bitCount-1);
				RF433::RFObj->_bitCount--;
			}
//	}

//			if(_newState && !RF433::RFObj->_oldState)
//			{
//				RF433::RFObj->_input&=~(1<<RF433::RFObj->_bitCount-1);
//				RF433::RFObj->_bitCount--;
//			}
//
//			if(!_newState && RF433::RFObj->_oldState)
//			{
//				RF433::RFObj->_input|=(1<<RF433::RFObj->_bitCount-1);
//				RF433::RFObj->_bitCount--;
//			}


		RF433::RFObj->_oldState = _newState;
		//RF433::RFObj->_bitCount--;

	}

	void  RF433::InterruptGpioWraper()
	{
		if(RF433::RFObj==0) return;
		RF433::RFObj->_gpio->DisableIrq();
		RF433::RFObj->_oldState = RF433::RFObj->_gpio->Read();
		RF433::RFObj->_timer->Init();
		RF433::RFObj->_timer->Enable();
		RF433::RFObj->_input=0;
		RF433::RFObj->_bitCount=22;
	}

	const char* RF433::toString()
	{
		return "RF433";
	}
}


