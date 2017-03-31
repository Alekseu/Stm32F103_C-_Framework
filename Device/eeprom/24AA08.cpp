/*
 * 24AA08.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "24AA08.h"

namespace Device
{

	EE24AA08* EE24AA08::EEObj =0;

	EE24AA08::EE24AA08()
	{
		EEObj = this;
	}

	EE24AA08::~EE24AA08()
	{

	}

	void EE24AA08::Init()
	{
		Iobj->Init();
	}

	bool EE24AA08::Write(void* obj, uint16_t length)
	{
		return false;
	}

	bool EE24AA08::Read(void* obj, uint16_t length)
	{
		return false;
	}

}

