/*
 * main.h
 *
 *  Created on: 09 марта 2017 г.
 *      Author: Alekseu
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "StdPeriph/cmsis_boot/stm32f10x.h"
#include "StdPeriph/cmsis_boot/system_stm32f10x.h"
#include "Driver/nvic/nvic.h"
#include "Driver/led/led.h"
#include "Extention/delay.h"
#include "Driver/usart/usart.h"

//#include "Driver/Dma.h"
//#include "Extention/operators.h"
//#include "Driver/usb/usb.h"

//
#include "Communication/CommandProcessor.h"
#include <string.h>
//
#include "Extention/MemoryManager.h"
//#include "Extention/MemoryPool.h"
//#include "Extention/LinkedList.h"
//#include "Extention/RingBuffer.h"

using namespace Driver;

Led _leds;

#endif /* MAIN_H_ */
