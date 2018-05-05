/*
 * SGPIO.h
 *
 *  Created on: 01 сент. 2017 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_SGPIO_SGPIO_H_
#define DEVICE_SGPIO_SGPIO_H_


#include "../deviceConfig.h"
#include "../../Driver/tim/tim.h"

using namespace Driver;
/*
 * Driver 74HC595  - output  74HC168 - inputs
 */

namespace Device
{

	/*
	 * SGPIO example
	 *
	 * Without timer
	 * SGPIO * _gpio2 = new SGPIO();
	 * _gpio2->Init();
	 * _gpio2->SetOutput(SGPIO::pin1, true);
	 * _gpio2->SetCallback(InputsInterrupt);
	 * _gpio2->Invalidate(); /read inputs and write outputs
	 *
	 *
	 *
	 * With timer
	 * Tim _tim(Tim::Timer2,1500,Tim::InterruptType::IT_Update);
	 * _tim.OnElapsed = TimerElapsed;
	 * _tim.Init();
	 *
	 * SGPIO * _gpio2 = new SGPIO();
	 * _gpio2->Init();
	 * _gpio2->SetOutput(SGPIO::pin1, true);
	 * _gpio2->SetTimer(&_tim);
	 * _gpio2->SetCallback(InputsInterrupt);
	 * _tim.Enable();
	 *
	 */

	typedef void SGpioPinCallback(void);

	class SGPIO //:public DeviceObject
	{
	public:

		enum Pin
		{
			 pin1=0x01,
			 pin2=0x02,
			 pin3=0x04,
			 pin4=0x08,
			 pin5=0x10,
			 pin6=0x20,
			 pin7=0x40,
			 pin8=0x80
		};

		union Inputs
				{
					struct
					{
						unsigned input1	:1;
						unsigned input2	:1;
						unsigned input3	:1;
						unsigned input4	:1;
						unsigned input5	:1;
						unsigned input6	:1;
						unsigned input7	:1;
						unsigned input8	:1;
					};
					uint8_t data;
				};

		union Outputs
				{
					struct
					{
						unsigned output1	:1;
						unsigned output2	:1;
						unsigned output3	:1;
						unsigned output4	:1;
						unsigned output5	:1;
						unsigned output6	:1;
						unsigned output7	:1;
						unsigned output8	:1;
					};
					uint8_t data;
				};

	  public:
		static SGPIO* SGObj;

		SGPIO();
		~SGPIO();

		void Init();

		void SetCallback(SGpioPinCallback* callback);
		void SetTimer(Tim* obj);

		Inputs ReadInputs();
		bool ReadInput(Pin input);

		void SetOutputs(Outputs outputs);
		void SetOutput(Pin output, bool state);

		void Invalidate();

		static void InterruptWraper();

		const char* toString();

	  private:
		SGpioPinCallback* _callback;
		Tim* _timer;
		Inputs _inputs;
		Outputs _outputs;
	};
}


#endif /* DEVICE_SGPIO_SGPIO_H_ */
