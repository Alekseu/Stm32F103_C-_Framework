/*
 * gpio.h
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */

#ifndef DRIVER_GPIO_GPIO_H_
#define DRIVER_GPIO_GPIO_H_


#include "../platform.h"


namespace Driver
{
	typedef void GpioPinCallback(void);

	class GPIO
	{
		GpioInit GPIO_InitStruct;
	public:
		static GPIO* GObjs[15*5];
		enum Pin
		{
			Pin0=0x0001,
			Pin1=0x0002,
			Pin2=0x0004,
			Pin3=0x0008,
			Pin4=0x0010,
			Pin5=0x0020,
			Pin6=0x0040,
			Pin7=0x0080,
			Pin8=0x0100,
			Pin9=0x0200,
			Pin10=0x0400,
			Pin11=0x0800,
			Pin12=0x1000,
			Pin13=0x2000,
			Pin14=0x4000,
			Pin15=0x8000,
			PinAll =0xFFFF
		};

		enum Port
		{
			PORTA = GPIOA_BASE,
			PORTB = GPIOB_BASE,
			PORTC = GPIOC_BASE,
			PORTD = GPIOD_BASE,
			PORTE = GPIOE_BASE
		};

		enum Mode
		{
			AIN = 0x0,
			IN_FLOATING = 0x04,
			IPD = 0x28,
			IPU = 0x48,
			Out_OD = 0x14,
			Out_PP = 0x10,
			AF_OD = 0x1C,
			AF_PP = 0x18
		};

		enum Speed
		{
			Speed_10MHz = 1,
			Speed_2MHz=2,
			Speed_50MHz=3
		};

		enum InterruptType
		{
			Rising = 0x08,
			Falling = 0x0C,
			Rising_Falling = 0x10
		};

	public:


		GPIO(Port port, Pin pin);
		GPIO(Port port, Pin pin, Mode mode, Speed speed);

		~GPIO();

		void Init();

		void EnableIrq();
		void DisableIrq();

		void Write(bool state);
		bool Read();

		void SetCallback(GpioPinCallback* callback , InterruptType type );

		static void InterruptWraper(void);


	private:
		Port _port;
		Pin _pin;
		Mode _mode;
		Speed _speed;
		InterruptType _type;
		uint8_t _portSource;
		uint8_t _pinSource;

		GpioPinCallback* _callback;
		uint16_t _interruptNumber;

	};
}


#endif /* DRIVER_GPIO_GPIO_H_ */
