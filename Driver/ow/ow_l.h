/*
 * ow.h
 *
 *  Created on: 16 янв. 2018 г.
 *      Author: hudienko_a
 */

#ifndef OW_L_H_
#define OW_L_H_

#include "Driver/platform.h"
#include "Extention/delay.h"
/* Определяем куда подключен датчик */
#define TERM_PORT GPIOA
#define TERM_PIN GPIO_Pin_5


void THERM_LOW()
{
	GPIO_ResetBits(TERM_PORT,TERM_PIN);
}
void THERM_HIGH()
{
	GPIO_SetBits(TERM_PORT,TERM_PIN);
}

uint8_t READINPUT()
{
	return GPIO_ReadInputDataBit(TERM_PORT,TERM_PIN);
}

void THERM_INPUT_MODE()
{
	//THERM_HIGH();
	GPIO_InitTypeDef GPIO_InitStructure;

				// Configure pin in output push/pull mode
				GPIO_InitStructure.GPIO_Pin = TERM_PIN;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
				GPIO_Init(TERM_PORT, &GPIO_InitStructure);
}

void THERM_OUTPUT_MODE()
{

	GPIO_InitTypeDef GPIO_InitStructure;

				// Configure pin in output push/pull mode
				GPIO_InitStructure.GPIO_Pin = TERM_PIN;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
				GPIO_Init(TERM_PORT, &GPIO_InitStructure);
}

// сброс датчика
uint8_t therm_reset(){
    uint8_t i;
    // опускаем ногу вниз на 480uS

    THERM_OUTPUT_MODE();
    THERM_LOW();
    _delay_us(480);             // замените функцию задержки на свою
    // подымаем линию на 60uS
    THERM_INPUT_MODE();
    _delay_us(65);
    // получаем значение на линии в период 480uS
    i=READINPUT();
    _delay_us(420);
    // возвращаем значение (0=OK, 1=датчик не найден)
    return i;
}


// функция отправки бита
void therm_write_bit(uint8_t bit){
    // опускаем на 1uS

    THERM_OUTPUT_MODE();
    THERM_LOW();

    _delay_us(1);
    // если хотим отправить 1, поднимаем линию (если нет, оставляем как есть)
    if(bit) THERM_HIGH(); else THERM_LOW();
    // ждем 60uS и поднимаем линию
    _delay_us(60);
    //THERM_INPUT_MODE();
    THERM_HIGH();
}

// чтение бита
uint8_t therm_read_bit(void){
    uint8_t bit=0;
    // опускаем на 1uS

    THERM_OUTPUT_MODE();
    THERM_LOW();

    _delay_us(1);
    // поднимаем на 14uS
    THERM_INPUT_MODE();
    _delay_us(14);
    // читаем состояние
    bit=READINPUT();
    // ждем 45 мкс и возвращаем значение
    _delay_us(60);
    return bit;
}

// читаем байт
uint8_t therm_read_byte(void){
    uint8_t i=8, n=0;
    while(i--){
        // сдвигаем в право на 1 и сохраняем следующее значение
        n>>=1;
        n|=(therm_read_bit()<<7);
    }
    return n;
}

// отправляем байт
void therm_write_byte(uint8_t byte){
    uint8_t i=8;
    while(i--){
        // отправляем бит и сдвигаем вправо на 1
        therm_write_bit(byte&1);
        byte>>=1;
    }
}


// команды управления датчиком
#define THERM_CMD_CONVERTTEMP 0x44
#define THERM_CMD_RSCRATCHPAD 0xbe
#define THERM_CMD_WSCRATCHPAD 0x4e
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM 0xb8
#define THERM_CMD_RPWRSUPPLY 0xb4
#define THERM_CMD_SEARCHROM 0xf0
#define THERM_CMD_READROM 0x33
#define THERM_CMD_MATCHROM 0x55
#define THERM_CMD_SKIPROM 0xcc
#define THERM_CMD_ALARMSEARCH 0xec

#define THERM_DECIMAL_STEPS_12BIT 625 //.0625

void Init()
{
	therm_reset();
	therm_write_byte(0xCC);
	therm_write_byte(0x4E);
	therm_write_byte(0x4B);
	therm_write_byte(0x46);
	therm_write_byte(0x5F);
	 therm_reset();
}

// читаем температуру с датчика
void therm_read_temperature(char *buffer){
    uint8_t temperature[9];
    int8_t digit;
    uint16_t decimal;

    therm_reset();
    therm_write_byte(THERM_CMD_SKIPROM);
    therm_write_byte(THERM_CMD_CONVERTTEMP);

    //while(!therm_read_bit());
    _delay_us(750);


    therm_reset();
    therm_write_byte(THERM_CMD_SKIPROM);
    therm_write_byte(THERM_CMD_RSCRATCHPAD);

    temperature[0]=therm_read_byte();
    temperature[1]=therm_read_byte();

    therm_reset();


    uint16_t _temperature = (temperature[1]<<8) |  temperature[0] ;

//    uint16_t temp =0;
//    uint16_t temp_int=0;
    float temp_float=0;
//
//    if ((temperature[1]&128) == 0)
//    	; // то плюс , зажигаем красный диод
//    else
//    {
//    	temp = ((unsigned int) temperature[1]<<8) | temperature[0]; /* приравниваем двухбайтное число*/
//    	/* В DS18B20 для представления отрицательной температуры используется дополнительный код. Особенность этого кода заключается в том, что он позволяет осуществлять операции вычитания через сложение. Чтобы получить дополнительный код числа, нужно выполнить над числом поразрядную инверсию (~) и прибавить к результату единицу. */
//    	temp = ~temp + 1;    //инвертируем
//    	temperature[0] = temp;
//    	temperature[1] = temp>>8;
//    	;  //то минус , зажигаем зеленый диод
//    }
//
//    temp_int = ((temperature[1]&7)<<4)|(temperature[0]>>4); /*выводим  целое знач. температуры*/
//    //выделяем с помощью битовой маски дробную часть
//    temp_float = (temperature[0]&15);
//    //преобразуем в целое число
//    /* Для перевода дробной части температуры в градусы, нужно умножить ее значение на 0,0625.*/
//    temp_float *= 0.0625; //делим на 16 или умножаем на 0.0625
//    sprintf(buffer, "%+i.%i C", temp_int,temp_float );

    if (_temperature & 0x8000) {
    	_temperature = ~_temperature + 1;
    	temp_float = 0.0 - (_temperature / 16.0);
    	//return digit;
    }
    else
    {

    	temp_float = _temperature / 16.0;
    }





    digit=temperature[0]>>4;
    digit|=(temperature[1]&0x7)<<4;

    decimal=temperature[0]&0xf;
    decimal*=THERM_DECIMAL_STEPS_12BIT;
    sprintf(buffer, "%.2f C", temp_float);
        // я вывожу данные на порт
//    PORTA = digit;
//    PORTB = decimal >> 8;


}





#endif /* OW_L_H_ */
