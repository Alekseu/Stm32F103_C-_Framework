/*
 * w5500.cpp
 *
 *  Created on: 31 рту. 2017 у.
 *      Author: hudienko_a
 */
#include "w5500.h"

#include "string.h"

#include "../../StdPeriph/stm_lib/inc/stm32f10x_spi.h"
#include "../../StdPeriph/stm_lib/inc/stm32f10x_gpio.h"
#include "../../StdPeriph/stm_lib/inc/stm32f10x_rcc.h"
#include "../../Driver/nvic/nvic.h"

using namespace Driver;

namespace Device
{


		extern "C"
		{
			#include "Ethernet/wizchip_conf.h"
			#include "Ethernet/W5500/w5500.h"
			#include "Ethernet/socket.h"
			#include "Ethernet/DHCP/dhcp.h"

			#define DATA_BUF_SIZE   1024
			#define SOCK_DHCP       6
			uint8_t my_dhcp_retry = 0;
			char ip_ok=0;
			uint8_t gDATABUF[DATA_BUF_SIZE];
			wiz_NetInfo gWIZNETINFO;

			unsigned char W5500_rxtx(unsigned char data)
			{
				while(SPI_I2S_GetFlagStatus(SPI_W5500, SPI_I2S_FLAG_TXE)==RESET);
				SPI_I2S_SendData(SPI_W5500,data);
				while(SPI_I2S_GetFlagStatus(SPI_W5500, SPI_I2S_FLAG_RXNE)==RESET);
				return SPI_I2S_ReceiveData(SPI_W5500);
			}

			void  wizchip_select(void)
			{
				GPIO_ResetBits(GPIO_W5500_CS, GPIO_Pin_CS_W5500);
			}

			void  wizchip_deselect(void)
			{
				GPIO_SetBits(GPIO_W5500_CS, GPIO_Pin_CS_W5500);
			}

			void  wizchip_write(uint8_t wb)
			{
				W5500_rxtx(wb);
			}

			uint8_t wizchip_read()
			{
				return W5500_rxtx(0xFF);
			}

			void my_ip_assign(void)
			{
				getIPfromDHCP(gWIZNETINFO.ip);
				getGWfromDHCP(gWIZNETINFO.gw);
				getSNfromDHCP(gWIZNETINFO.sn);
				getDNSfromDHCP(gWIZNETINFO.dns);
				gWIZNETINFO.dhcp = NETINFO_DHCP;
				/* Network initialization */
				ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);    // apply from DHCP
				ip_ok=1;
			}

			void my_ip_conflict(void)
			{

			}

			void interrupt_wrapper()
			{
				 EXTI_ClearITPendingBit(EXTI_Line3);


				wizchip_clrinterrupt(IK_SOCK_0);
				wizchip_setinterruptmask(IK_SOCK_0);

				 int soc_status = getSn_SR(0);
				 int len=0;

				 			switch(soc_status)
				 			{
				 				case SOCK_CLOSED:
				 					socket(0,Sn_MR_TCP,1050,0);
				 					break;
				 				case SOCK_INIT:

				 					break;
				 				case SOCK_LISTEN:

				 					break;
				 				case SOCK_ESTABLISHED:
				 					len= getSn_RX_RSR(0);
				 					if(len >0)
				 					{
				 						len = recv(0,gDATABUF,DATA_BUF_SIZE);
				 						if(len>0)
				 						{

				 						}
				 					}

				 					break;
				 				case SOCK_CLOSE_WAIT:
				 					close(0);
				 					socket(0,Sn_MR_TCP,1050,0);
				 					break;
				 			}



			}


		}



	bool W5500::init()
	{

		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
		    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

			GPIO_InitTypeDef GPIO_InitStruct;

			// MOSI & CLK
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_SCK | GPIO_Pin_SPI_W5500_MOSI;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

			GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);

			// MISO
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_SPI_W5500_MISO;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

			GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);

			// CS
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_CS_W5500;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

			GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);

			//RES
			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_RES_W5500;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

			GPIO_Init(GPIO_SPI_W5500, &GPIO_InitStruct);

			GPIO_SetBits(GPIO_SPI_W5500, GPIO_Pin_RES_W5500);

			//INT

			GPIO_InitStruct.GPIO_Pin = GPIO_Pin_W5500_INT;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

			GPIO_Init(GPIO_W5500_INT, &GPIO_InitStruct);

			EXTI_InitTypeDef EXTI_InitStructure;
			GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);
			EXTI_InitStructure.EXTI_Line = EXTI_Line3;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;
			EXTI_Init(&EXTI_InitStructure);

			InterruptController::SetHandler((IRQn_Type)EXTI3_IRQn,interrupt_wrapper);


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

			uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

			reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
			reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);

			uint8_t tmpstr[6] = {0,};

			ctlwizchip(CW_GET_ID,(void*)tmpstr) ;

			static const uint8_t myPHYCFGR = 0xD8;
			setPHYCFGR(myPHYCFGR);

			int time =100;
			uint8_t phy_l;
			do
			{
				ctlwizchip(CW_GET_PHYLINK, (void*)&phy_l);

			}while(phy_l != PHY_LINK_ON && --time );

			if(time>0)
			{

				wizchip_setinterruptmask(IK_SOCK_0);


//			wizchip_setinterruptmask(IK_SOCK_ALL);
//			wizchip_clrinterrupt(IK_SOCK_ALL);

			memcpy(gWIZNETINFO.mac,"\x00\x0A\x0B\x0C\x0D\x0E",6);
			ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);

			DHCP_init(SOCK_DHCP, gDATABUF);
			reg_dhcp_cbfunc(my_ip_assign, my_ip_assign, my_ip_conflict);
			while(1)
			{
				uint8_t cl = DHCP_run();
				if(ip_ok)
				{
					InterruptController::EnableChannel((IRQn_Type)EXTI3_IRQn);
					break;
				}
				switch(cl)
				{
				case DHCP_IP_ASSIGN:
				case DHCP_IP_CHANGED:
					break;
				case DHCP_IP_LEASED:
					break;
				case DHCP_FAILED:
					my_dhcp_retry++;
					if(my_dhcp_retry > 2)
					{
						gWIZNETINFO.dhcp = NETINFO_STATIC;
						DHCP_stop();
						my_dhcp_retry = 0;
					}
					break;
				default:
					break;
				}
			}
			}


	}


	void W5500::init_socket()
	{
		int _listenSocket  = socket(0,Sn_MR_TCP,1050,0);

				listen(_listenSocket);
	}
}

