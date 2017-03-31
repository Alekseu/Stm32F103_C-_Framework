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



#include <string.h>
//
#include "Extention/MemoryManager.h"

#include "Protocols/Rs485/rs485.h"
#include "Protocols/commandProcessor/commandProcessor.h"

using namespace Driver;
using namespace Protocol;

Led _leds;

#endif /* MAIN_H_ */
