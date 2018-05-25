/*
 * keyboard.cpp
 *
 *  Created on: 25 мая 2018 г.
 *      Author: hudienko_a
 */
#include "keyboard.h"

namespace Device
{

	Keyboard::Keyboard()
	{
		OnPressKey=0;
		OnPressEnter=0;
		_current_row=0;
		_current_col=0;
		_enter_key='D';
		_key=0;
		_index=0;
	}

	void Keyboard::Init()
		{
			RCC_APB2PeriphClockCmd(KEYBOARD_RCC,ENABLE);
			RCC_APB2PeriphClockCmd(KEYBOARD1_RCC,ENABLE);

			GPIO_InitStructure.GPIO_Pin = COL_1|COL_2|COL_3;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);


			GPIO_InitStructure.GPIO_Pin = COL_4;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(KEYBOARD1_PORT, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin =  ROW_1| ROW_2|ROW_3|ROW_4;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);

			_current_row=0;
			_current_col=0;
			_enter_key='D';
			_key=0;
			_index=0;
		}

	void Keyboard::Init(char enter_key)
			{
				RCC_APB2PeriphClockCmd(KEYBOARD_RCC,ENABLE);
				RCC_APB2PeriphClockCmd(KEYBOARD1_RCC,ENABLE);

				GPIO_InitStructure.GPIO_Pin = COL_1|COL_2|COL_3;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);


				GPIO_InitStructure.GPIO_Pin = COL_4;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_Init(KEYBOARD1_PORT, &GPIO_InitStructure);

				GPIO_InitStructure.GPIO_Pin =  ROW_1| ROW_2|ROW_3|ROW_4;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
				GPIO_Init(KEYBOARD_PORT, &GPIO_InitStructure);

				_current_row=0;
				_current_col=0;
				_enter_key=enter_key;
				_key=0;
				_index=0;
			}

	char Keyboard::ReadKey(int timeout )
		{
			char key=0;
			while (!key && --timeout)
			{
				key= _readKey();
				_delay_ms(1);
			}

			return key;
		}

	void Keyboard::Process()
		{
			_key = _readKey();

			if(_key!=0)
			{
				if(OnPressKey!=0)
				{
					OnPressKey(_key);
				}

				if(_key==_enter_key)
				{
					if(OnPressEnter!=0)
					{
						OnPressEnter(input_text,_index);
					}
					_index=0;
					memset(input_text,0,15);
				}
				else
				{
					if(_index>=20)_index=0;

					input_text[_index++]=_key;
				}

			}
		}

	char Keyboard::_readKey()
			{
				char text=0;

					switch(_current_col)
					{
						case 0:
							GPIO_SetBits(KEYBOARD1_PORT,COL_4);
							break;
						case 1:
							GPIO_SetBits(KEYBOARD_PORT,COL_3);
							break;
						case 2:
							GPIO_SetBits(KEYBOARD_PORT,COL_2);
							break;
						case 3:
							GPIO_SetBits(KEYBOARD_PORT,COL_1);
							break;
					}

					switch(_current_row)
					{
						case 0:

							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_4))
							{
								text= '1';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_3))
							{
								text= '2';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_2))
							{
								text= '3';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_1))
							{
								text= 'A';
							}
							break;

						case 1:
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_4))
							{
								text= '4';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_3))
							{
								text= '5';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_2))
							{
								text= '6';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_1))
							{
								text= 'B';
							}
							break;

						case 2:
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_4))
							{
								text= '7';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_3))
							{
								text= '8';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_2))
							{
								text= '9';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_1))
							{
								text= 'C';
							}

							break;

						case 3:

							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_4))
							{
								text= '*';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_3))
							{
								text= '0';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_2))
							{
								text= '#';
							}else
							if(GPIO_ReadInputDataBit(KEYBOARD_PORT, ROW_1))
							{
								text= 'D';
							}
							break;
					}

					switch(_current_col)
								{
									case 0:
										GPIO_ResetBits(KEYBOARD1_PORT,COL_4);
										break;
									case 1:
										GPIO_ResetBits(KEYBOARD_PORT,COL_3);
										break;
									case 2:
										GPIO_ResetBits(KEYBOARD_PORT,COL_2);
										break;
									case 3:
										GPIO_ResetBits(KEYBOARD_PORT,COL_1);
										break;
								}






					if(_current_col++>=4 )
					{
						_current_col=0;

					}

					if(_current_row++>=4)
					{
						_current_row=0;
					}

					return text;
			}

}


