/*
 * encoder.h
 *
 *  Created on: 04 ���� 2018 �.
 *      Author: hudienko_a
 */

#ifndef DEVICE_ENCODER_ENCODER_H_
#define DEVICE_ENCODER_ENCODER_H_
#include "../deviceConfig.h"
#include "../../Driver/Interface/IHDriver.h"

namespace Device
{
	/*
	 * Example
	 *
	 * Encoder _enc;
	 * _enc.StartScan();
	 * 	_enc.GetEncoderData()
	 *
	 * Tim _tim(Tim::Timer2,10,Tim::InterruptType::IT_Update);
	 * 	_enc.SetTimer(&_tim);
	 *
	 */

	using namespace Driver;
	class Encoder
	{

	public:
		static Encoder* eObj;

		enum Direction
		{
			Up,
			Down
		};

		typedef void EncoderKeyPressed();
		typedef void EncoderMove(Direction , uint32_t);

		Encoder();
		Encoder(IHDriver* timer_);

		void SetTimer(IHDriver* timer_);
		void StartScan();
		void StopScan();
		void Process();

		uint32_t GetEncoderData();
		void ClearCounter();

		EncoderKeyPressed* OnKeyPress;
		EncoderMove*       OnMove;

		static void TimerElapsedWrapper(void);


	private:
		GpioInit  GPIO_InitStructure;
		uint8_t _encState;
		uint32_t _encData;
		IHDriver* _timer;

		void _encoderScan(void);
		void _preEvent(bool type);
	};

};

#endif /* DEVICE_ENCODER_ENCODER_H_ */
