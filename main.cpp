/*
 * main.cpp
 *
 *  Created on: 30 ���. 2017 �.
 *      Author: hudienko_a
 */
#include "Driver/platform.h"
#include "Extention/operators.h"
#include "Driver/usart/usart.h"


int main()
{

	Driver::ICommunicationObject* _port = new Driver::SerialPort(_USART0,9600);

	while(1)
	{

		_port->WriteByte('H');
		_delay_ms(250);

	}

	return 0;
}
