/*
 * keyboard.h
 *
 *  Created on: 25 мая 2018 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_KEYBOARD_KEYBOARD_H_
#define DRIVER_KEYBOARD_KEYBOARD_H_

#include "../deviceConfig.h"

#include <string.h>

namespace Device
{

typedef void KeyPressed(char);
typedef void EnterPressed(char*, unsigned char);

class Keyboard
{
public:
	KeyPressed* OnPressKey;
	EnterPressed* OnPressEnter;

	Keyboard();

	void Init();

	void Init(char enter_key);

	char ReadKey(int timeout = 150);

	void Process();

private:
	unsigned char _current_row;
	unsigned char _current_col;

	char _enter_key;
	char _key;

	char input_text[20];
	unsigned char _index;

	GpioInit  GPIO_InitStructure;


	char _readKey();

};

}


#endif /* DRIVER_KEYBOARD_KEYBOARD_H_ */
