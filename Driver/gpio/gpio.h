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
	/*
	 * Gpio example
	 *
	 * void GpioInterrupt1()
	 * {
	 *  do thomething
	 * }
	 *
	 * 	GPIO* _gpio = new GPIO(GPIO::Port::PORTC,GPIO::Pin::Pin10,GPIO::Mode::IN_FLOATING,GPIO::Speed::Speed_2MHz);
	 * 	_gpio->Init();
	 * 	_gpio->SetCallback(GpioInterrupt1,GPIO::InterruptType::Rising);
	 *
	 * 	GPIO* _gpio1 = new GPIO(GPIO::Port::PORTE,GPIO::Pin::Pin2);
	 * 	_gpio1->Init();
	 *
	 *
	 *  _gpio1->Write(true);
	 *  _gpio1->Write(false);
	 *
	 *
	 *
	 */


	typedef void GpioPinCallback(void);

	class GPIO
	{
		GpioInit GPIO_InitStruct;
	public:
		static GPIO* GObjs[15*5];
		enum Pin
		{
			Pin0,
			Pin1,
			Pin2,
			Pin3,
			Pin4,
			Pin5,
			Pin6,
			Pin7,
			Pin8,
			Pin9,
			Pin10,
			Pin11,
			Pin12,
			Pin13,
			Pin14,
			Pin15,
			PinAll
		};

		enum Port
		{
			PORTA ,
			PORTB,
			PORTC,
			PORTD ,
			PORTE
		};

		enum Mode
		{
			AIN,
			IN_FLOATING ,
			IPD,
			IPU,
			Out_OD ,
			Out_PP ,
			AF_OD ,
			AF_PP
		};

		enum Speed
		{
			Speed_10MHz = 1,
			Speed_2MHz=2,
			Speed_50MHz=3
		};

		enum InterruptType
		{
			Rising,
			Falling ,
			Rising_Falling
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

		GPIO_TypeDef * __port;
		uint16_t GPIO_Pin;
		uint32_t EXTI_Line;

	};
}


#endif /* DRIVER_GPIO_GPIO_H_ */
