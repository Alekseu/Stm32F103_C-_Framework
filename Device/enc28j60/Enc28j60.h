/*
 * Enc28j60.h
 *
 *  Created on: 07 окт. 2016 г.
 *      Author: hudienko_a
 */

#ifndef NET_ENC28J60_H_
#define NET_ENC28J60_H_
#include "../Drivers/delay.h"
#include "Interface/IDriver.h"
#include "Enc28j60Cfg.h"

class Enc28j60: public IDriver
{
public:
	Enc28j60()
	{
		enc28j60_current_bank=0;
		enc28j60_rxrdpt=0;
		lastFilter=0;
	}

	bool Init() override
	{
		// Initialize SPI
		GPIO_InitTypeDef gpio;

			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

			GPIO_PinAFConfig(ENC28J60_SPI_PORT, GPIO_PinSource5, GPIO_AF_SPI1);
			GPIO_PinAFConfig(ENC28J60_SPI_PORT, GPIO_PinSource6, GPIO_AF_SPI1);
			GPIO_PinAFConfig(ENC28J60_SPI_PORT, GPIO_PinSource7, GPIO_AF_SPI1);
			gpio.GPIO_Pin = ENC28J60_SPI_MOSI | ENC28J60_SPI_MISO | ENC28J60_SPI_SCK;
			gpio.GPIO_Speed = GPIO_Speed_50MHz;
			gpio.GPIO_Mode =   GPIO_Mode_AF;
			gpio.GPIO_OType = GPIO_OType_PP;
			gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
			GPIO_Init(ENC28J60_SPI_PORT, &gpio);

			gpio.GPIO_Pin = ENC28J60_SPI_CS;
			gpio.GPIO_Speed = GPIO_Speed_50MHz;
			gpio.GPIO_Mode =   GPIO_Mode_OUT;
			gpio.GPIO_OType = GPIO_OType_PP;
			gpio.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(ENC28J60_SPI_PORT, &gpio);


			gpio.GPIO_Pin = ENC28J60_SPI_INT;
			gpio.GPIO_Speed = GPIO_Speed_50MHz;
			gpio.GPIO_Mode =   GPIO_Mode_IN;
			gpio.GPIO_OType = GPIO_OType_PP;
			gpio.GPIO_PuPd = GPIO_PuPd_DOWN;
			GPIO_Init(ENC28J60_SPI_PORT1, &gpio);

			gpio.GPIO_Pin = ENC28J60_RESET;
			gpio.GPIO_Speed = GPIO_Speed_50MHz;
			gpio.GPIO_Mode =   GPIO_Mode_OUT;
			gpio.GPIO_OType = GPIO_OType_PP;
			gpio.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(ENC28J60_SPI_PORT1, &gpio);
			_delay_ms(1000);
			GPIO_SetBits(ENC28J60_SPI_PORT1,ENC28J60_RESET);
			_delay_ms(1500);

			SPI_InitTypeDef spi;
			int prescaler = SPI_BaudRatePrescaler_32;
			SPI_Cmd(ENC28J60_SPI, DISABLE);

			spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
			spi.SPI_Mode = SPI_Mode_Master;
			spi.SPI_DataSize = SPI_DataSize_8b;
			spi.SPI_CPOL = SPI_CPOL_Low;
			spi.SPI_CPHA = SPI_CPHA_1Edge;

			spi.SPI_NSS = SPI_NSS_Soft;
			spi.SPI_BaudRatePrescaler = prescaler;
			spi.SPI_FirstBit = SPI_FirstBit_MSB;
			spi.SPI_CRCPolynomial = 7;
			SPI_Init(ENC28J60_SPI, &spi);

			SPI_Cmd(ENC28J60_SPI, ENABLE);
			SPI_NSSInternalSoftwareConfig(ENC28J60_SPI, SPI_NSSInternalSoft_Set);

		// Reset ENC28J60
		enc28j60_soft_reset();

		// Setup Rx/Tx buffer
		enc28j60_wcr16(ERXST, ENC28J60_RXSTART);
		enc28j60_wcr16(ERXRDPT, ENC28J60_RXSTART);
		enc28j60_wcr16(ERXND, ENC28J60_RXEND);
		enc28j60_rxrdpt = ENC28J60_RXSTART;

		// Setup MAC
		enc28j60_wcr(MACON1, MACON1_TXPAUS| // Enable flow control
			MACON1_RXPAUS|MACON1_MARXEN); // Enable MAC Rx
		enc28j60_wcr(MACON2, 0); // Clear reset
		enc28j60_wcr(MACON3, MACON3_PADCFG0| // Enable padding,
			MACON3_TXCRCEN|MACON3_FRMLNEN|MACON3_FULDPX); // Enable crc & frame len chk
		enc28j60_wcr16(MAMXFL, ENC28J60_MAXFRAME);
		enc28j60_wcr(MABBIPG, 0x15); // Set inter-frame gap
		enc28j60_wcr(MAIPGL, 0x12);
		enc28j60_wcr(MAIPGH, 0x0c);
		enc28j60_wcr(MAADR5, MacAddr[0]); // Set MAC address
		enc28j60_wcr(MAADR4, MacAddr[1]);
		enc28j60_wcr(MAADR3, MacAddr[2]);
		enc28j60_wcr(MAADR2, MacAddr[3]);
		enc28j60_wcr(MAADR1, MacAddr[4]);
		enc28j60_wcr(MAADR0, MacAddr[5]);

		// Setup PHY
			enc28j60_write_phy(PHCON1, PHCON1_PDPXMD); // Force full-duplex mode
			enc28j60_write_phy(PHCON2, PHCON2_HDLDIS); // Disable loopback
			enc28j60_write_phy(PHLCON, PHLCON_LACFG2| // Configure LED ctrl
				PHLCON_LBCFG2|PHLCON_LBCFG1|PHLCON_LBCFG0|
				PHLCON_LFRQ0|PHLCON_STRCH);


			// Enable Rx packets
			enc28j60_bfs(ECON1, ECON1_RXEN);
		return true;
	}

	bool GetPacket(eth_frame_t* eth_frame, uint16_t* len)override
	{
		__disable_irq();
		*len = enc28j60_recv_packet((uint8_t*)eth_frame, BUFFER_SIZE_FOR_ONE_PACKET);
		__enable_irq();
		if(len>0) return true;
		return false;
	}

	bool GetLink()
	{
		if( (enc28j60_read_phy(PHSTAT1) & PHSTAT1_LLSTAT))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void EthSend(eth_frame_t *frame, uint16_t len)override
	{
		__disable_irq();
		memcpy(frame->from_addr, MacAddr, 6);
		enc28j60_send_packet((uint8_t*)frame, len +	sizeof(eth_frame_t));
		__enable_irq();
	}

	void EthReply(eth_frame_t *frame, uint16_t len)override
	{
		memcpy(frame->to_addr, frame->from_addr, 6);
		memcpy(frame->from_addr, MacAddr, 6);
		enc28j60_send_packet((uint8_t*)frame, len +	sizeof(eth_frame_t));
	}

	void EthResend(eth_frame_t *frame, uint16_t len)override
	{
		enc28j60_send_packet((uint8_t*)frame, len +
				sizeof(eth_frame_t));
	}
private:
	uint8_t enc28j60_current_bank;
	uint16_t enc28j60_rxrdpt ;
	uint8_t lastFilter;

	void enc28j60_send_packet(uint8_t *data, uint16_t len)
	{
		while(enc28j60_rcr(ECON1) & ECON1_TXRTS)
		{
			// TXRTS may not clear - ENC28J60 bug. We must reset
			// transmit logic in cause of Tx error
			if(enc28j60_rcr(EIR) & EIR_TXERIF)
			{
				enc28j60_bfs(ECON1, ECON1_TXRST);
				enc28j60_bfc(ECON1, ECON1_TXRST);
			}
		}

		enc28j60_wcr16(EWRPT, ENC28J60_TXSTART);
		enc28j60_write_buffer((uint8_t*)"\x00", 1);
		enc28j60_write_buffer(data, len);

		enc28j60_wcr16(ETXST, ENC28J60_TXSTART);
		enc28j60_wcr16(ETXND, ENC28J60_TXSTART + len);

		enc28j60_bfs(ECON1, ECON1_TXRTS); // Request packet send
	}

	uint16_t enc28j60_recv_packet(uint8_t *buf, uint16_t buflen)
	{
		uint16_t len = 0, rxlen, status, temp;
		uint8_t packet_count =enc28j60_rcr(EPKTCNT);

		uint8_t newFilter;

		if (packet_count < 10)
		{
			newFilter = ERXFCON_UCEN | ERXFCON_BCEN | ERXFCON_PMEN | ERXFCON_CRCEN;   // unicast, broadcast and pattern match (for multicast), and CRC checking
		}
		else if (packet_count < 20)
			newFilter = ERXFCON_UCEN | ERXFCON_PMEN | ERXFCON_CRCEN;   // we are getting backed up, let's ignore broadcast packets
		else
			newFilter = ERXFCON_UCEN | ERXFCON_CRCEN;   // big backlog... ignore all packets that are not directly addressed!

		if (newFilter != lastFilter)
		{
			// we need to apply a filter change...
			enc28j60_wcr(ERXFCON, newFilter);
			lastFilter = newFilter;
		}

		if(packet_count)
		{
			enc28j60_wcr16(ERDPT, enc28j60_rxrdpt);

			enc28j60_read_buffer((uint8_t*)&enc28j60_rxrdpt, sizeof(enc28j60_rxrdpt));
			enc28j60_read_buffer((uint8_t*)&rxlen, sizeof(rxlen));
			enc28j60_read_buffer((uint8_t*)&status, sizeof(status));

			if(status & 0x80) //success
			{
				len = rxlen - 4; //throw out crc
				if(len > buflen) len = buflen;
				enc28j60_read_buffer(buf, len);
			}

			// Set Rx read pointer to next packet
			temp = (enc28j60_rxrdpt - 1) & ENC28J60_BUFEND;
			enc28j60_wcr16(ERXRDPT, temp);

			// Decrement packet counter
			enc28j60_bfs(ECON2, ECON2_PKTDEC);
		}

		return len;
	}

	#define enc28j60_rx() enc28j60_rxtx(0xff)
	#define enc28j60_tx(data) enc28j60_rxtx(data)

   void enc28j60_select(){GPIO_ResetBits(ENC28J60_SPI_PORT,ENC28J60_SPI_CS);}
   void enc28j60_release(){	GPIO_SetBits(ENC28J60_SPI_PORT,ENC28J60_SPI_CS);}

   uint8_t enc28j60_rxtx(uint8_t data)
   {
	   SPI_I2S_SendData(ENC28J60_SPI, data);
	   while(SPI_I2S_GetFlagStatus(ENC28J60_SPI, SPI_I2S_FLAG_BSY) == SET);
	   return SPI_I2S_ReceiveData(ENC28J60_SPI);
   }


   // Generic SPI read command
   uint8_t enc28j60_read_op(uint8_t cmd, uint8_t adr)
   {
   	uint8_t data;

   	enc28j60_select();
   	enc28j60_tx(cmd | (adr & ENC28J60_ADDR_MASK));
   	if(adr & 0x80) // throw out dummy byte
   		enc28j60_rx(); // when reading MII/MAC register
   	data = enc28j60_rx();
   	enc28j60_release();
   	return data;
   }

   // Generic SPI write command
   void enc28j60_write_op(uint8_t cmd, uint8_t adr, uint8_t data)
   {
   	enc28j60_select();
   	enc28j60_tx(cmd | (adr & ENC28J60_ADDR_MASK));
   	enc28j60_tx(data);
   	enc28j60_release();
   }

   // Initiate software reset
   void enc28j60_soft_reset()
   {
   	enc28j60_select();
   	enc28j60_tx(ENC28J60_SPI_SC);
   	enc28j60_release();

   	enc28j60_current_bank = 0;
   	_delay_ms(1); // Wait until device initializes
   }


   /*
    * Memory access
    */

   // Set register bank
   void enc28j60_set_bank(uint8_t adr)
   {
   	uint8_t bank;

   	if( (adr & ENC28J60_ADDR_MASK) < ENC28J60_COMMON_CR )
   	{
   		bank = (adr >> 5) & 0x03; //BSEL1|BSEL0=0x03
   		if(bank != enc28j60_current_bank)
   		{
   			enc28j60_write_op(ENC28J60_SPI_BFC, ECON1, 0x03);
   			enc28j60_write_op(ENC28J60_SPI_BFS, ECON1, bank);
   			enc28j60_current_bank = bank;
   		}
   	}
   }

   // Read register
   uint8_t enc28j60_rcr(uint8_t adr)
   {
   	enc28j60_set_bank(adr);
   	return enc28j60_read_op(ENC28J60_SPI_RCR, adr);
   }

   // Read register pair
   uint16_t enc28j60_rcr16(uint8_t adr)
   {
   	enc28j60_set_bank(adr);
   	return enc28j60_read_op(ENC28J60_SPI_RCR, adr) |
   		(enc28j60_read_op(ENC28J60_SPI_RCR, adr+1) << 8);
   }

   // Write register
   void enc28j60_wcr(uint8_t adr, uint8_t arg)
   {
   	enc28j60_set_bank(adr);
   	enc28j60_write_op(ENC28J60_SPI_WCR, adr, arg);
   }

   // Write register pair
   void enc28j60_wcr16(uint8_t adr, uint16_t arg)
   {
   	enc28j60_set_bank(adr);
   	enc28j60_write_op(ENC28J60_SPI_WCR, adr, arg);
   	enc28j60_write_op(ENC28J60_SPI_WCR, adr+1, arg>>8);
   }

   // Clear bits in register (reg &= ~mask)
   void enc28j60_bfc(uint8_t adr, uint8_t mask)
   {
   	enc28j60_set_bank(adr);
   	enc28j60_write_op(ENC28J60_SPI_BFC, adr, mask);
   }

   // Set bits in register (reg |= mask)
   void enc28j60_bfs(uint8_t adr, uint8_t mask)
   {
   	enc28j60_set_bank(adr);
   	enc28j60_write_op(ENC28J60_SPI_BFS, adr, mask);
   }

   // Read Rx/Tx buffer (at ERDPT)
   void enc28j60_read_buffer(uint8_t *buf, uint16_t len)
   {
   	enc28j60_select();
   	enc28j60_tx(ENC28J60_SPI_RBM);
   	while(len--)
   		*(buf++) = enc28j60_rx();
   	enc28j60_release();
   }

   // Write Rx/Tx buffer (at EWRPT)
   void enc28j60_write_buffer(uint8_t *buf, uint16_t len)
   {
   	enc28j60_select();
   	enc28j60_tx(ENC28J60_SPI_WBM);
   	while(len--)
   		enc28j60_tx(*(buf++));
   	enc28j60_release();
   }

   // Read PHY register
   uint16_t enc28j60_read_phy(uint8_t adr)
   {
   	enc28j60_wcr(MIREGADR, adr);
   	enc28j60_bfs(MICMD, MICMD_MIIRD);
   	while(enc28j60_rcr(MISTAT) & MISTAT_BUSY)
   		;
   	enc28j60_bfc(MICMD, MICMD_MIIRD);
   	return enc28j60_rcr16(MIRD);
   }

   // Write PHY register
   void enc28j60_write_phy(uint8_t adr, uint16_t data)
   {
   	enc28j60_wcr(MIREGADR, adr);
   	enc28j60_wcr16(MIWR, data);
   	while(enc28j60_rcr(MISTAT) & MISTAT_BUSY);
   }



};


#endif /* NET_ENC28J60_H_ */
