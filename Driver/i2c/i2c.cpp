/*
 * i2c.cpp
 *
 *  Created on: 31 ����� 2017 �.
 *      Author: hudienko_a
 */

#include "i2c.h"
#include "../nvic/nvic.h"

namespace Driver
{
	I2c* I2c::I1Obj=0;
	I2c* I2c::I2Obj=0;

	I2c::I2c(I2cNum i2cNum,uint8_t addr, I2CType type, I2CSpeed speed)
	{
		_i2cNum =i2cNum;
		_badr = addr;
		_type = type;
		_speed = speed;
	}

	I2c::~I2c()
	{

	}

	void I2c::Init()
	{

		switch(_i2cNum)
		{
		case in_I2C1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO , ENABLE);//
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
			//GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_6;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;

			GPIO_Init(GPIOB, &GPIO_InitStructure);

			//InterruptController::SetHandler(I2C1_EV_IRQn,InterruptWraper);
			//InterruptController::EnableChannel(I2C1_EV_IRQn);

			break;
		case in_I2C2:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
			//GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
			GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;

			GPIO_Init(GPIOB, &GPIO_InitStructure);

			InterruptController::SetHandler(I2C2_EV_IRQn,InterruptWraper);
			InterruptController::EnableChannel(I2C2_EV_IRQn);
			break;
		}

		I2C_DeInit((I2C_TypeDef* )_i2cNum);
		I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStructure.I2C_OwnAddress1 = 0x00;
		I2C_InitStructure.I2C_Ack =I2C_Ack_Disable;
		I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStructure.I2C_ClockSpeed =_speed;

		I2C_Cmd((I2C_TypeDef* )_i2cNum, ENABLE);
		I2C_Init((I2C_TypeDef* )_i2cNum, &I2C_InitStructure);
		I2C_AcknowledgeConfig((I2C_TypeDef* )_i2cNum, ENABLE);
		I2C_ITConfig((I2C_TypeDef* )_i2cNum,I2C_IT_EVT,ENABLE);


	}

	uint8_t I2c::ReadByte()
	{
		uint8_t tmp;
		return tmp;
	}

	uint8_t  I2c::ReadByte(uint8_t addr)
	{
		uint8_t tmp;
		// I2C_Cmd(I2C_EE, ENABLE);
		/* While the bus is busy */
		while(I2C_GetFlagStatus((I2C_TypeDef* )_i2cNum, I2C_FLAG_BUSY));

		/* Send START condition */
		I2C_GenerateSTART((I2C_TypeDef* )_i2cNum, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_MODE_SELECT));

		/* Send EEPROM address for write */
		I2C_Send7bitAddress((I2C_TypeDef* )_i2cNum, _badr, I2C_Direction_Transmitter);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));


		/* Send the EEPROM's internal address to read from: MSB of the address first */
		I2C_SendData((I2C_TypeDef* )_i2cNum, (uint8_t)(addr));

		/* Test on EV8 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send STRAT condition a second time */
		I2C_GenerateSTART((I2C_TypeDef* )_i2cNum, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_MODE_SELECT));

		I2C_Send7bitAddress((I2C_TypeDef* )_i2cNum, _badr, I2C_Direction_Receiver);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum,I2C_EVENT_MASTER_BYTE_RECEIVED));//I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

		tmp=I2C_ReceiveData((I2C_TypeDef* )_i2cNum);


		I2C_AcknowledgeConfig((I2C_TypeDef* )_i2cNum, DISABLE);

		/* Send STOP Condition */
		I2C_GenerateSTOP((I2C_TypeDef* )_i2cNum, ENABLE);

		// I2C_Cmd(I2C_EE, DISABLE);
		_delay_ms(2);
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
		I2C_GenerateSTART((I2C_TypeDef* )_i2cNum, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_MODE_SELECT));


		/* Send EEPROM address for write */
		I2C_Send7bitAddress((I2C_TypeDef* )_i2cNum, _badr, I2C_Direction_Transmitter);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) ){};


		/* Send the EEPROM's internal address to write to : MSB of the address first */
		I2C_SendData((I2C_TypeDef* )_i2cNum, (uint8_t)(addr ));

		/* Test on EV8 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		I2C_SendData((I2C_TypeDef* )_i2cNum, byte);

		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send STOP condition */
		I2C_GenerateSTOP((I2C_TypeDef* )_i2cNum, ENABLE);
		// I2C_Cmd(I2C_EE, DISABLE);
		//delay between write and read...not less 4ms
			_delay_ms(10);
	}

	void I2c::WriteByte(uint8_t byte, uint8_t baddr, uint8_t addr)
	{
		I2C_GenerateSTART((I2C_TypeDef* )_i2cNum, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_MODE_SELECT));


		/* Send EEPROM address for write */
		I2C_Send7bitAddress((I2C_TypeDef* )_i2cNum, baddr, I2C_Direction_Transmitter);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) ){};


		/* Send the EEPROM's internal address to write to : MSB of the address first */
		I2C_SendData((I2C_TypeDef* )_i2cNum, (uint8_t)(addr ));

		/* Test on EV8 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		I2C_SendData((I2C_TypeDef* )_i2cNum, byte);

		/* Test on EV8 and clear it */
		while (!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* Send STOP condition */
		I2C_GenerateSTOP((I2C_TypeDef* )_i2cNum, ENABLE);
		// I2C_Cmd(I2C_EE, DISABLE);
		//delay between write and read...not less 4ms
		_delay_ms(10);
	}

	void I2c::WriteBytes(uint8_t* val, uint16_t size, uint8_t baddr, uint8_t addr)
	{

		/* Send START condition */
		I2C_GenerateSTART((I2C_TypeDef* )_i2cNum, ENABLE);

		/* Test on EV5 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_MODE_SELECT));


		/* Send EEPROM address for write */
		I2C_Send7bitAddress((I2C_TypeDef* )_i2cNum, baddr, I2C_Direction_Transmitter);

		/* Test on EV6 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) ){};


		/* Send the EEPROM's internal address to write to : MSB of the address first */
		I2C_SendData((I2C_TypeDef* )_i2cNum, (uint8_t)(addr ));

		/* Test on EV8 and clear it */
		while(!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		for(int i=0;i<size;i++)
		{
			((I2C_TypeDef* )_i2cNum)->DR = val[i];
			while (!I2C_CheckEvent((I2C_TypeDef* )_i2cNum, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		}

		I2C_GenerateSTOP((I2C_TypeDef* )_i2cNum, ENABLE);
		_delay_ms(2);

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
		for(int i=0;i<length;i++)
		{
			WriteByte(data[i]);
		}
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
