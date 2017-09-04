/*
 * SGPIO.cpp
 *
 *  Created on: 04 сент. 2017 г.
 *      Author: hudienko_a
 */
#include "SGPIO.h"

namespace Device
{

	SGPIO* SGPIO::SGObj =0;

	SGPIO::SGPIO()
	{
		_callback = 0;
		_inputs.data =0;
		_outputs.data=0;
		_timer =0;
	}

	SGPIO::~SGPIO()
			{

			}

	void SGPIO::Init()
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE, ENABLE);

		GPIO_InitTypeDef GPIO_InitStructure;

		// Configure pin in output push/pull mode
		GPIO_InitStructure.GPIO_Pin = CLK|RD;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(INP_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = DI;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(INP_PORT, &GPIO_InitStructure);


		GPIO_SetBits(INP_PORT,RD);

		// Configure pin in output push/pull mode
		GPIO_InitStructure.GPIO_Pin = CLK1|DO|WR;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(OUT_PORT, &GPIO_InitStructure);
		SGPIO::SGObj = this;
		Invalidate();
	}

	void SGPIO::SetCallback(SGpioPinCallback* callback)
	{
		_callback = callback;
	}

	void SGPIO::SetTimer(Tim* obj)
	{
		_timer = obj;
		_timer->OnElapsed =InterruptWraper;
	}

	SGPIO::Inputs SGPIO::ReadInputs()
	{
		return _inputs;
	}

	bool SGPIO::ReadInput(Pin input)
	{
		return _inputs.data && input>0;
	}

	void SGPIO::SetOutputs(Outputs outputs)
	{
		_outputs.data = outputs.data;
	}

	void SGPIO::SetOutput(Pin output, bool state)
	{
		if(state)
		{
			_outputs.data|=output;
		}
		else
		{
			_outputs.data&=~output;
		}
	}

	void SGPIO::Invalidate()
	{
		uint8_t _newInputs;
		uint8_t _newOutputs;

		//read inputs
			  GPIO_ResetBits(INP_PORT,RD);
			  GPIO_ResetBits(INP_PORT,CLK);
			  GPIO_SetBits(INP_PORT,CLK);
			  GPIO_ResetBits(INP_PORT,CLK);
			  GPIO_SetBits(INP_PORT,RD);
			for(char i = 0; i <8 ; i++)
			{
				 if(!GPIO_ReadInputDataBit(INP_PORT, DI))
				 {
					 _newInputs|=(1<<i);
				 }
				 else
				 {
					 _newInputs&=~(1<<i);
				 }

				GPIO_SetBits(INP_PORT,CLK);
				GPIO_ResetBits(INP_PORT,CLK);
			}

		if(_newInputs!=_inputs.data)
		{
			_inputs.data = _newInputs;
			if(_callback!=0)
			{
				_callback();
			}
		}

		_newOutputs = _outputs.data;
		//set outputs
			GPIO_ResetBits(OUT_PORT,DO);
			GPIO_ResetBits(OUT_PORT,WR);
			GPIO_ResetBits(OUT_PORT,CLK1);

			for(uint8_t i = 8; i; i--)
			{
				GPIO_ResetBits(OUT_PORT,CLK1);

				_newOutputs & (1 << (i - 1))?  GPIO_SetBits(OUT_PORT,DO):  GPIO_ResetBits(OUT_PORT,DO);

				GPIO_SetBits(OUT_PORT,CLK1);

			}
			GPIO_ResetBits(OUT_PORT,CLK1);

			// защелкиваем данные
			GPIO_SetBits(OUT_PORT,WR);
			GPIO_ResetBits(OUT_PORT,WR);
			GPIO_ResetBits(OUT_PORT,DO);


	}

	void SGPIO::InterruptWraper()
	{
		SGPIO::SGObj->Invalidate();
	}

	const char* SGPIO::toString()
	{
		return "SGPIO";
	}


}
