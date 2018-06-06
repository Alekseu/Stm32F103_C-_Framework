/*
 * RF433.cpp
 *
 *  Created on: 31 авг. 2017 г.
 *      Author: hudienko_a
 */

#include "RF433.h"



namespace Device
{
	RF433* RF433::RFObj =0;

	static uint16_t _totalPaketCounter;
	static		uint16_t _preambCounter;
	static		uint16_t _packetCounter;
	static		bool    _praget;

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

		_totalPaketCounter=0;
		_preambCounter=0;
		_packetCounter=0;
	}

	RF433::~RF433()
	{

	}

	void RF433::Init()
	{
		RF433::RFObj = this;
		_timer = new Tim(Tim::Timer2,100 ,Tim::InterruptType::IT_Update);
		_timer->Init();
		_timer->OnElapsed = InterruptTimerWraper;


		_gpio1 = new GPIO(GPIO::Port::PORTA,GPIO::Pin::Pin1,GPIO::Mode::Out_PP,GPIO::Speed::Speed_50MHz);
				_gpio1->Init();
				//_gpio1->Write(true);

		_gpio = new GPIO(GPIO::Port::PORTA,GPIO::Pin::Pin0,GPIO::Mode::IN_FLOATING,GPIO::Speed::Speed_50MHz);
		_gpio->Init();
		_gpio->SetCallback(InterruptGpioWraper,GPIO::InterruptType::Falling);




		//_gpio->EnableIrq();
		//_oldState = if(GPIO_ReadInputDataBit((GPIO_TypeDef*)_port, _pin))
	}

	void RF433::StartReceive()
	{
		_gpio->EnableIrq();
		//_timer->Enable();
	}
	void RF433::StopReceive()
	{
		_gpio->DisableIrq();
		//_timer->Disable();
	}

	void RF433::SetCallback(RF433Callback* callback)
	{
		_callback = callback;
	}

	void RF433::Received(uint32_t inp)
	{
		_buff[_tail++] = inp>>24&0xFF;
		_buff[_tail++] = inp>>16&0xFF;
		_buff[_tail++] = inp>>8&0xFF;
		_buff[_tail++] = inp&0xFF;
		if(_tail>=4)_tail=0;
	}

	char*  RF433::getString()
	{
		return _buff;
	}

	bool start=false;

	void RF433::InterruptTimerWraper()
	{

		bool _newState = RF433::RFObj->_gpio->Read();
		bool _oldstate = RF433::RFObj->_oldState;

		RF433::RFObj->_gpio1->Write(_praget);
//		RF433::RFObj->_gpio1->Write(start);
//		start = !start;

		if((!_newState && !_oldstate) && !_praget  )
		{
			if(_preambCounter++>= 150 && !_praget) //если 0 был длинной 15ms
			{
				_praget = true;
			}
		}

//
		if(_praget)
		{

			if(_newState !=_oldstate)
			{
				if(_newState && !_oldstate)
				{
					RF433::RFObj->_input&=~(1<<RF433::RFObj->_bitCount-1);
					RF433::RFObj->_bitCount--;
				}

				if(!_newState && _oldstate)
				{
					RF433::RFObj->_input|=(1<<RF433::RFObj->_bitCount-1);
					RF433::RFObj->_bitCount--;
				}


			}
			if(_packetCounter++>=500 ) //пакет принят
			{

				RF433::RFObj-> Received(RF433::RFObj->_input);
				RF433::RFObj->_timer->Disable();
				RF433::RFObj->_gpio->EnableIrq();
				_praget = false;

			}
//
		}
////
		RF433::RFObj->_oldState = _newState;

		if(_totalPaketCounter++>= 700) //если пает длиннее 65мс это ложный пакет
		{
			RF433::RFObj->_timer->Disable();
			RF433::RFObj->_gpio->EnableIrq();
			_packetCounter=0;
			_praget = false;
		}

	}

	void  RF433::InterruptGpioWraper()
	{
		if(RF433::RFObj==0) return;
		RF433::RFObj->_gpio->DisableIrq();
		RF433::RFObj->_oldState = RF433::RFObj->_gpio->Read();
		RF433::RFObj->_timer->Init();
		RF433::RFObj->_timer->Enable();
		RF433::RFObj->_input=0;
		RF433::RFObj->_bitCount=25;
		_totalPaketCounter=0;
		_preambCounter=0;
		_packetCounter=0;
		_praget=false;

	}



}


