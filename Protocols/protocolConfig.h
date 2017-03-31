/*
 * protocolConfig.h
 *
 *  Created on: 31 ����� 2017 �.
 *      Author: hudienko_a
 */

#ifndef PROTOCOLS_PROTOCOLCONFIG_H_
#define PROTOCOLS_PROTOCOLCONFIG_H_

/*
 * ��� ������������ ��� ����������� ���������� ��� ����������,
 * ���-�� ����������� ��� ����������� ��������� ��� ������� �� ����������
 */

#include <string.h>

#include "../Driver/nvic/nvic.h"
#include "../Driver/usart/usart.h"


/*
 * rs485
 */
#define rs485
#define RxTxPin GPIO_Pin_0
#define RxTxPort GPIOA
#define RxTxPheriph RCC_APB2Periph_GPIOA


#endif /* PROTOCOLS_PROTOCOLCONFIG_H_ */
