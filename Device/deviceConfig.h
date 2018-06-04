/*
 * deviceConfig.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_DEVICECONFIG_H_
#define DEVICE_DEVICECONFIG_H_

#include "../Extention/delay.h"

extern "C"
{
	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"

	#include "../StdPeriph/stm_lib/inc/stm32f10x_usart.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_spi.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_tim.h"

	#include "../StdPeriph/stm_lib/inc/stm32f10x_i2c.h"


	#include "../StdPeriph/stm_lib/inc/stm32f10x_dma.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_exti.h"
	#include "../StdPeriph/stm_lib/inc/misc.h"
}

/*
 * typedefs
 */

#define GpioInit  GPIO_InitTypeDef

/*
 * “ут подключаютс€ библиотеки драйверов,
 *  так-же определ€ютс€ определенные настройки дл€ устройств (если нужно)
 */

#define sgpio

#define CLK		GPIO_Pin_13
#define RD		GPIO_Pin_12
#define DI	    GPIO_Pin_11
#define INP_PORT GPIOE

#define CLK1    GPIO_Pin_2
#define DO		GPIO_Pin_0
#define WR		GPIO_Pin_1
#define OUT_PORT    GPIOC


/*
 * Lcd N3310
 */
#define WIDTH 84
#define HEIGHT 48

/*
 * Stepper config
 */
#define StepsStart 150



/*
 * Keyboard
 */
#define KEYBOARD_PORT GPIOA
#define KEYBOARD_RCC RCC_APB2Periph_GPIOA

#define KEYBOARD1_PORT GPIOB
#define KEYBOARD1_RCC RCC_APB2Periph_GPIOB

#define ROW_1 GPIO_Pin_1
#define ROW_2 GPIO_Pin_2
#define ROW_3 GPIO_Pin_3
#define ROW_4 GPIO_Pin_4

#define COL_1 GPIO_Pin_5
#define COL_2 GPIO_Pin_6
#define COL_3 GPIO_Pin_7
#define COL_4 GPIO_Pin_0



/*
 * Encoder
 */
#define ENCODER_PORT GPIOA
#define ENCODER_RCC RCC_APB2Periph_GPIOA

#define ENCODER_KEY_PIN GPIO_Pin_0
#define ENCODER_PIN1 GPIO_Pin_1
#define ENCODER_PIN2 GPIO_Pin_2


#endif /* DEVICE_DEVICECONFIG_H_ */
