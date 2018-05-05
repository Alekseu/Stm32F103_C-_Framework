#include "../../StdPeriph/stm_lib/inc/stm32f10x_spi.h"
#include "../../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
#include "../../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
#include "spi.h"


uint8_t W5500_rxtx(uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPI_W5500, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI_W5500,data);
	while(SPI_I2S_GetFlagStatus(SPI_W5500, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPI_W5500);

//	uint8_t tmp =0;
//
//	for(int i=0;i<8;i++)
//	{
//
//		 if(data&(1<<i))
//		 {
//			 GPIO_SetBits(GPIOB,GPIO_Pin_SPI_W5500_MOSI);
//		 }
//		 else
//		 {
//			 GPIO_ResetBits(GPIOB,GPIO_Pin_SPI_W5500_MOSI);
//		 }
//
//		  GPIO_SetBits(GPIOB,GPIO_Pin_SPI_W5500_SCK);
//
//		 _delay_ms(1);
//
//		 if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_SPI_W5500_MISO))
//		 {
//			 tmp|=(1<<i);
//		 }
//		 else
//		 {
//			 tmp&=~(1<<i);
//		 }
//
//		 GPIO_ResetBits(GPIOB,GPIO_Pin_SPI_W5500_SCK);
//		 _delay_ms(1);
//	}
//	return tmp;
}



void spiW5500_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	DBGMCU->CR = 0x00000000; //disable trace swo on PB3
	// Initialize SPI and GPIO
	RCC_APBPeriphClockCmd_SPI_W5500(RCC_APBPeriph_SPI_W5500, ENABLE);
	RCC_APBPeriphClockCmd_CS_W5500(RCC_APB2Periph_GPIO_W5500_CS, ENABLE);
	RCC_APBPeriphClockCmd_SPI_W5500(RCC_APB2Periph_GPIO_W5500, ENABLE);
//	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

	// MOSI & CLK
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_SCK | GPIO_Pin_SPI_W5500_MOSI;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// MISO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_MISO;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// CS
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_CS_W5500;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//RES
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_RES_W5500;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_SetBits(GPIOB, GPIO_Pin_RES_W5500);

	SPI_InitTypeDef SPI_InitStruct;

	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_SPI_SD;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_W5500, &SPI_InitStruct);


	SPI_Cmd(SPI_W5500, ENABLE);

}
