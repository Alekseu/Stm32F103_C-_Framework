/*
 * platform.h
 *
 *  Created on: 31 ����� 2017 �.
 *      Author: hudienko_a
 */

#ifndef DRIVER_PLATFORM_H_
#define DRIVER_PLATFORM_H_


/*
 * ��� ������������ ��� ������������������ ����������
 *  ���� � ����� ��� ���������, ����������� ������������ ���������� ��������� (�����), ��������� � �.�.
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

#define GpioInit  GPIO_InitTypeDef


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

#define BUFFER_SIZE 32

#define _USART0	0x00
#define _USART1	0x01
#define _USART2	0x02
#define _USART3	0x03
#define _USART4	0x04

#define _UART_PARITY_NONE USART_Parity_No
#define _UART_PARITY_EVEN USART_Parity_Even
#define _UART_PARITY_ODD USART_Parity_Odd

#define UsartInit USART_InitTypeDef


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
