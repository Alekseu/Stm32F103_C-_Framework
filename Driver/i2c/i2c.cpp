/*
 * i2c.cpp
 *
 *  Created on: 31 марта 2017 г.
 *      Author: hudienko_a
 */

#include "i2c.h"
#include "../nvic/nvic.h"

namespace Driver
{
	I2c* I2c::Iobj=0;

	I2c::I2c(uint16_t rate, uint8_t addr, I2CType type)
	{
		_rate = rate;
		_badr = addr;
		_type = type;
	}
	I2c::~I2c()
	{

	}

	void I2c::Init()
	{
		//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO , ENABLE);//
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

		GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
		/* Configure I2C1 pins: PB8->SCL and PB9->SDA */
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;

		GPIO_Init(GPIOB, &GPIO_InitStructure);

		I2C_DeInit(I2C_NUM);
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStructure.I2C_OwnAddress1 = 0x00;
		I2C_InitStructure.I2C_Ack =I2C_Ack_Disable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStructure.I2C_ClockSpeed =_rate;  /* 50kHz */

		I2C_Cmd(I2C_NUM, ENABLE);
		I2C_Init(I2C_NUM, &I2C_InitStructure);
		I2C_AcknowledgeConfig(I2C_NUM, ENABLE);
		I2C_ITConfig(I2C_NUM,I2C_IT_EVT,ENABLE);

		InterruptController::SetHandler(I2C1_EV_IRQn,InterruptWraper);
		InterruptController::EnableChannel(I2C1_EV_IRQn);

	}

	uint8_t I2c::ReadByte()
	{

	}

	uint8_t  I2c::ReadByte(uint8_t addr)
	{
		uint8_t tmp;
		// I2C_Cmd(I2C_EE, ENABLE);
		/* While the bus is busy */
		while(I2C_GetFlagStatus(I2C_NUM, I2C_FLAG_BUSY));

		/* Send START condition */
		I2C_GenerateSTART(I2C_NUM, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_MODE_SELECT));

		/* Send EEPROM address for write */
		I2C_Send7bitAddress(I2C_NUM, _badr, I2C_Direction_Transmitter);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));


		/* Send the EEPROM's internal address to read from: MSB of the address first */
		I2C_SendData(I2C_NUM, (uint8_t)(addr));

		/* Test on EV8 and clear it */
		while(!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send STRAT condition a second time */
		I2C_GenerateSTART(I2C_NUM, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_MODE_SELECT));

		I2C_Send7bitAddress(I2C_NUM, _badr, I2C_Direction_Receiver);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent(I2C_NUM,I2C_EVENT_MASTER_BYTE_RECEIVED));//I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

		tmp=I2C_ReceiveData(I2C_NUM);


		I2C_AcknowledgeConfig(I2C_NUM, DISABLE);

		/* Send STOP Condition */
		I2C_GenerateSTOP(I2C_NUM, ENABLE);

		// I2C_Cmd(I2C_EE, DISABLE);
		//_delay_ms(2);
					return tmp;
	}

	bool I2c::ReadByte(uint8_t* value, uint16_t timeOut)
	{
		return false;
	}

	void I2c::WriteByte(uint8_t byte)
	{

	}

	void I2c::WriteByte(uint8_t byte, uint8_t addr)
	{
		I2C_GenerateSTART(I2C_NUM, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_MODE_SELECT));


		/* Send EEPROM address for write */
		I2C_Send7bitAddress(I2C_NUM, _badr, I2C_Direction_Transmitter);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) ){};


		/* Send the EEPROM's internal address to write to : MSB of the address first */
		I2C_SendData(I2C_NUM, (uint8_t)(addr ));

		/* Test on EV8 and clear it */
		while(!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		I2C_SendData(I2C_NUM, byte);

		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent(I2C_NUM, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send STOP condition */
		I2C_GenerateSTOP(I2C_NUM, ENABLE);
		// I2C_Cmd(I2C_EE, DISABLE);
		//delay between write and read...not less 4ms
			//_delay_ms(10);
	}

	uint16_t I2c::ReadWord()
	{
		uint16_t word;
		return word;
	}

	void I2c::WriteWord(uint16_t word)
	{

	}

	void I2c::SendData(uint8_t* data, uint16_t length)
	{

	}


	/*
	 * interrupt
	 */
	void I2c::Received(uint8_t byte)
	{

	}

	void  I2c::InterruptWraper(void)
	{

	}

	const char* I2c::toString()
	 {
		 return "I2C";
	 }

}
