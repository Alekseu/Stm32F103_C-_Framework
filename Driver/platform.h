/*
 * platform.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_PLATFORM_H_
#define DRIVER_PLATFORM_H_


/*
 *  Пины и порты для устройств, зависимость определенной аппаратной платформы (платы), настройки и т.д.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>


extern "C"
{
	#include "../StdPeriph/cmsis_boot/stm32f10x.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"

	#include "../StdPeriph/stm_lib/inc/stm32f10x_usart.h"
	#include "../StdPeriph/stm_lib/inc/stm32f10x_spi.h"

	#include "../StdPeriph/stm_lib/inc/stm32f10x_dma.h"
	#include "../StdPeriph/stm_lib/inc/misc.h"
}

/*
 * typedefs
 */

#define GPIO  GPIO_InitTypeDef


/*
 * Leds
 */
#define LedPin1 GPIO_Pin_0
#define LedPin2 GPIO_Pin_1
#define LedPort GPIOA


/*
 * usart's
 */
#define usart



/*
 *  spi's
 */
#define spi




/*
 *  i2c's
 */
#define i2c



/*
 * usb
 */
#define usb
#define RCC_APB2Periph_GPIO_DISCONNECT RCC_APB2Periph_GPIOA
#define USB_DISCONNECT_PIN	GPIO_Pin_10
#define USB_DISCONNECT  GPIOA


/*
 * timer's
 */
#define timer



/*
 * dma's
 */
#define dma


/*
 * sdio
 */
#define sdio



/*
 * OS
 */
#define os




#endif /* DRIVER_PLATFORM_H_ */
