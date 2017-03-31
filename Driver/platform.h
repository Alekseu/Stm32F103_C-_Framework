/*
 * platform.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_PLATFORM_H_
#define DRIVER_PLATFORM_H_


/*
 *  Пины и порты для устройств, зависимость определенной аппаратной платформы (платы)
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
#define USART1_RX 0



/*
 *  spi's
 */
#define SPI1_MISO




/*
 *  i2c's
 */
#define I2C1_SCL



/*
 * usb
 */
#define USB
#define RCC_APB2Periph_GPIO_DISCONNECT RCC_APB2Periph_GPIOA
#define USB_DISCONNECT_PIN	GPIO_Pin_10
#define USB_DISCONNECT  GPIOA


/*
 * timer's
 */
#define TIMER



/*
 * dma's
 */
#define DMA




#endif /* DRIVER_PLATFORM_H_ */
