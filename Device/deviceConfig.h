/*
 * deviceConfig.h
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef DEVICE_DEVICECONFIG_H_
#define DEVICE_DEVICECONFIG_H_

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


#endif /* DEVICE_DEVICECONFIG_H_ */
