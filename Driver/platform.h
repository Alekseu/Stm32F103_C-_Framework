/*
 * platform.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DRIVER_PLATFORM_H_
#define DRIVER_PLATFORM_H_


/*
 * Тут подключаются все платформозависимые библиотеки
 *  Пины и порты для устройств, зависимость определенной аппаратной платформы (платы), настройки и т.д.
 */
#include "../Global/object.h"


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
 * Leds
 */
#define LedPin1 GPIO_Pin_13
//#define LedPin2 GPIO_Pin_6
#define LedRCC  RCC_APB2Periph_GPIOC
#define LedPort GPIOC


/*
 * usart's
 */
#define usart

#define BUFFER_SIZE 32

#define UsartInit USART_InitTypeDef


/*
 *  spi's
 */
#define spi

#define SpiInit SPI_InitTypeDef

// пока без указаний пинов и возможных ремапов
//#define SPI1_RCC_Pheriph
//#define SpiMosi1
//#define SpiMiso1
//#define SpiSck1
//#define SpiSS1
//#define Remap_Spi1 0
//
//#define SPI2_RCC_Pheriph
//#define SpiMosi2
//#define SpiMiso2
//#define SpiSck2
//#define SpiSS2
//#define Remap_Spi2 0
//
//#define SPI3_RCC_Pheriph
//#define SpiMosi3
//#define SpiMiso3
//#define SpiSck3
//#define SpiSS3
//#define Remap_Spi3 0

/*
 *  i2c's
 */
#define  i2c
#define  I2cInit I2C_InitTypeDef
#define  Remap 0


/*
 * usb
 */
#define usb
#define RCC_APB2Periph_GPIO_DISCONNECT RCC_APB2Periph_GPIOC
#define USB_DISCONNECT_PIN	GPIO_Pin_9
#define USB_DISCONNECT  GPIOC


/*
 * timer's
 */
#define timer

#define TimInit TIM_TimeBaseInitTypeDef


#define systim

#define CallbackCount 10

/*
 * Ow's
 */
#define ow


/*
 * Can
 */
#define can


/*
 * dma's
 */
#define dma
#define DmaInit DMA_InitTypeDef

    #define CHANNEL_1 1
	#define CHANNEL_2 2
	#define CHANNEL_3 3
	#define CHANNEL_4 4
	#define CHANNEL_5 5
	#define CHANNEL_6 6
	#define CHANNEL_7 7
	#define CHANNEL_8 8
	#define CHANNEL_9 9
	#define CHANNEL_10 10
	#define CHANNEL_11 11
	#define CHANNEL_12 12

	#define _SPI 1
	#define _UART 2
	#define _I2C 3


/*
 * sdio
 */
#define sdio



/*
 * OS
 */
#define os




#endif /* DRIVER_PLATFORM_H_ */
