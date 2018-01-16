/*
 * delay.h
 *
 *  Created on: 29 θώνÿ 2016 γ.
 *      Author: hudienko_a
 */

#ifndef EXTENTION_DELAY_H_
#define EXTENTION_DELAY_H_


#define delayUS_ASM(us) do {\
	asm volatile (	"MOV R0,%[loops]\n\t"\
			"1: \n\t"\
			"SUB R0, #1\n\t"\
			"CMP R0, #0\n\t"\
			"BNE 1b \n\t" : : [loops] "r" (16*us) : "memory"\
		      );\
} while(0)

void _delay_ms(int ms);
void _delay_us(int us);


#endif /* EXTENTION_DELAY_H_ */
