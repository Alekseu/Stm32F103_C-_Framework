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

	#include "../StdPeriph/stm_lib/inc/stm32f10x_adc.h"

	#include "../StdPeriph/stm_lib/inc/stm32f10x_fsmc.h"
}

/*
 * typedefs
 */

#define GpioInit  GPIO_InitTypeDef


/*
 * Leds
 */
//#define LedPin1 GPIO_Pin_13
//#define LedPin2 GPIO_Pin_5
//#define LedPin3 GPIO_Pin_4
//#define LedRCC  RCC_APB2Periph_GPIOC
//#define LedPort GPIOC
#define LedPin1 GPIO_Pin_0
//#define LedPin2 GPIO_Pin_6
#define LedRCC  RCC_APB2Periph_GPIOA
#define LedPort GPIOA



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

// ���� ��� �������� ����� � ��������� �������
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
#define RCC_APB2Periph_GPIO_DISCONNECT RCC_APB2Periph_GPIOA
#define USB_DISCONNECT_PIN	GPIO_Pin_7
#define USB_DISCONNECT  GPIOA


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
 * adc
 */
#define AdcInit ADC_InitTypeDef


/*
 * sdio
 */
#define sdio



/*
 * OS
 */
#define os

#define WITH_OS 1


/*
 * PSpi pin port config
 */
#define LCD_RCC  RCC_APB2Periph_GPIOA
#define LCD_PORT GPIOA
#define SCK GPIO_Pin_0
#define MOSI GPIO_Pin_1
#define MISO GPIO_Pin_5
#define D_C GPIO_Pin_2
#define PSpi_CS GPIO_Pin_3
#define RES GPIO_Pin_4

/*
 * Stepper motor config
 */
#define L1 GPIO_Pin_0
#define L2 GPIO_Pin_2
#define L3 GPIO_Pin_1
#define L4 GPIO_Pin_3
#define PORT GPIOA
#define PHERIF RCC_APB2Periph_GPIOA



/*
 * w5500 config
 */
#define SPI_W5500                        SPI1
#define GPIO_W5500_CS                    GPIOA
#define RCC_APB2Periph_GPIO_W5500_CS     RCC_APB2Periph_GPIOA
#define GPIO_Pin_CS_W5500                GPIO_Pin_4
#define RCC_APBPeriphClockCmd_CS_W5500   RCC_APB2PeriphClockCmd
#define RCC_APB2Periph_GPIO_W5500        RCC_APB2Periph_GPIOA
#define GPIO_SPI_W5500                   GPIOA
#define GPIO_Pin_SPI_W5500_SCK           GPIO_Pin_5
#define GPIO_Pin_SPI_W5500_MISO          GPIO_Pin_6
#define GPIO_Pin_SPI_W5500_MOSI          GPIO_Pin_7
#define RCC_APBPeriphClockCmd_SPI_W5500  RCC_APB2PeriphClockCmd
#define RCC_APBPeriph_SPI_W5500          RCC_APB2Periph_SPI1

#define GPIO_W5500_RES GPIOE
#define GPIO_Pin_RES_W5500 GPIO_Pin_0

#define GPIO_W5500_INT GPIOA
#define GPIO_Pin_W5500_INT GPIO_Pin_3

#define SPI_BaudRatePrescaler_SPI_SD  SPI_BaudRatePrescaler_4


/*
 * fsmc sram
 */

#define RFsmcTiming FSMC_NORSRAMTimingInitTypeDef
#define WFsmcTiming FSMC_NORSRAMTimingInitTypeDef

#define AD0 GPIO_Pin_14
#define AD1 GPIO_Pin_15
#define AD2 GPIO_Pin_0
#define AD3 GPIO_Pin_1
#define AD4 GPIO_Pin_7
#define AD5 GPIO_Pin_8
#define AD6 GPIO_Pin_9
#define AD7 GPIO_Pin_10
#define AD8 GPIO_Pin_11
#define AD9 GPIO_Pin_12
#define AD10 GPIO_Pin_13
#define AD11 GPIO_Pin_14
#define AD12 GPIO_Pin_15
#define AD13 GPIO_Pin_8
#define AD14 GPIO_Pin_9
#define AD15 GPIO_Pin_10
#define A16  GPIO_Pin_11
#define A17  GPIO_Pin_12
#define A18  GPIO_Pin_13
#define NL   GPIO_Pin_7
#define NE1  GPIO_Pin_7
#define NOE  GPIO_Pin_4
#define NWE  GPIO_Pin_5
#define NBL  GPIO_Pin_0
#define NBH  GPIO_Pin_1



#endif /* DRIVER_PLATFORM_H_ */
