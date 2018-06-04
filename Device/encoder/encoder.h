/*
 * encoder.h
 *
 *  Created on: 04 θώνÿ 2018 γ.
 *      Author: hudienko_a
 */

#ifndef DEVICE_ENCODER_ENCODER_H_
#define DEVICE_ENCODER_ENCODER_H_
#include "../deviceConfig.h"
#include "../../Driver/Interface/IHDriver.h"

namespace Device
{
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

		EncoderKeyPressed* OnKeyPress;
		EncoderMove*       OnMove;

		static void TimerElapsedWrapper();

		void _encoderScan(void);
	private:
		GpioInit  GPIO_InitStructure;
		uint8_t _encState;
		uint32_t _encData;
		IHDriver* _timer;


		void _preEvent(bool type);
	};

};

#endif /* DEVICE_ENCODER_ENCODER_H_ */
