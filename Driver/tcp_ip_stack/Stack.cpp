/*
 * Stack.cpp
 *
 *  Created on: 07 окт. 2016 г.
 *      Author: hudienko_a
 */
#include "../Stack.h"
#include "../../Drivers/delay.h"
#include "../../Drivers/StdPeriph/include/stm32f4xx_rcc.h"
#include "../../Drivers/StdPeriph/include/stm32f4xx_tim.h"
#include "../../Drivers/StdPeriph/include/misc.h"

Stack* Stack::pStack=0;

extern "C"
{
	void TIM2_IRQHandler(void)
	{
	  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	  {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//		Stack::pStack->Input();
//		Stack::pStack->Process();
		if(!Stack::pStack->GetDriver()->GetLink())
		{
			Stack::pStack->dhcp_status = DHCP_INIT;
			Stack::pStack->dhcp_retry_time = NetTime::pNettime->rtime() + 5;
			Stack::pStack->IpAddr = 0;
			Stack::pStack->IpMask = 0;
			Stack::pStack->IpGate = 0;
		}
	  }
	}

	void SVCHandler_main(unsigned int * svc_args)
		{
			unsigned int svc_number;
			/*    * Stack contains:    * r0, r1, r2, r3, r12, r14, the return address and xPSR    * First argument (r0) is svc_args[0]    */
			svc_number = ((char *)svc_args[6])[-2];
			switch(svc_number)
			{        case 0:            /* Handle SVC 00 */
				break;
			case 1:            /* Handle SVC 01 */
				break;
			default:            /* Unknown SVC */
				break;
			}


		}

	void SVC_Handler(void)
	{
		asm volatile(
		"tst lr, #4\t\n" /* Check EXC_RETURN[2] */
		"ite eq\t\n"
		"mrseq r0, msp\t\n"
		"mrsne r0, psp\t\n"
		"b %[SVCHandler_main]\t\n"
		: /* no output */
		: [SVCHandler_main] "i" (SVCHandler_main) /* input */
		: "r0" /* clobber */
		);
	}

	int svc(int __no, ...)
	{
	    register int no asm("r0") = __no;
	    register int ret asm("r0");
	    asm("svc 0" : "=r"(ret) : "r"(no));
	    return ret;
	}
}

#define tcp_get_data(tcp)	((uint8_t*)(tcp) + tcp_head_size(tcp))


		Stack::Stack(uint8_t* mac, uint32_t ipAddr,uint32_t ipMask,uint32_t ipGate,uint32_t dns , IDriver* driver)
		{

			pStack =this;
			_driver = driver;
			fromAddr=0;
			fromPort=0;
			//_startClose = false;
			//_sendACK = false;
			if(ipAddr!=0)
			{
				IpAddr = ipAddr;
				IpMask = ipMask;
				IpGate = ipGate;
				Dns = dns;
				_dhcp =false;
			}
			else
			{
				_dhcp=true;
				dhcp_tr_id=0;
				dhcp_status = DHCP_INIT;
				dhcp_server=0;
				dhcp_renew_time=0;
				dhcp_retry_time=0;
				dhcp_tr_id=0;
			}
				current_socket_count=0;
				memcpy(_driver->MacAddr,mac,6);
				arp_cache_wr=0;

				for(int i=0;i<SOCKET_POOL_COUNT;i++)
				{
					_socket_pool[i].status = TCP_CLOSED;
				}
				_listen=false;
				_localPort =0;

		}

		bool Stack::Init()
		{
			if(!_driver->Init()) return false;



			NVIC_InitTypeDef NVIC_InitStructure;
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			/* Enable the TIM2 gloabal Interrupt */
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			/* TIM2 clock enable */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			/* Time base configuration */
			TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 1 MHz down to 1 KHz (1 ms)
			TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1; // 24 MHz Clock down to 1 MHz (adjust per your clock)
			TIM_TimeBaseStructure.TIM_ClockDivision = 0;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
			/* TIM IT enable */
			TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
			/* TIM2 enable counter */
			TIM_Cmd(TIM2, ENABLE);

			return true;
		}
		//todo нада продумать
		void Stack::Input()
		{
			uint16_t len=0;

			if(_driver->GetPacket((eth_frame*)buff,&len))
			{
				_ring.Add(buff,len);
				//_eth_filter((eth_frame*)buff,len);
			}
		}
		//todo нада продумать
		void Stack::Process()
		{

			uint16_t len=0;
			if(_ring.Get(buff,&len))
			{
			__disable_irq();
				_eth_filter((eth_frame*)buff,len);
			__enable_irq();
			}
			if(_dhcp)
			{
				dhcp_poll();
			}

		}

		bool Stack::LanUP()
		{
			if(_dhcp)
			{
				if(IpAddr>0)return true;
			}
			else
			{
				return arp_cache_wr!=0;
			}
			return false;
		}

		void Stack::PeriodicTime()
		{
			for(int i=0;i<SOCKET_POOL_COUNT;i++)
			{
				tcp_state_t* state = &_socket_pool[i];
				if(state->event_time <= NetTime::pNettime->second_count)
				{
					if(state->status == TCP_SYN_SENT || state->status == TCP_SYN_RECEIVED || state->status == TCP_FIN_WAIT1 || state->status == TCP_FIN_WAIT2)
					{
						eth_frame_t *frame = (eth_frame_t*)buff;
						ip_packet_t *ip = (ip_packet_t*)(frame->data);
						tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);
						tcp->window = htons(TCP_WINDOW_SIZE);
						tcp->flags |= TCP_FLAG_RST;
						tcp->seq_num = htonl(state->ack_num);
						tcp->ack_num = htonl(state->seq_num);
						tcp_xmit(i,frame,0);
						state->status=TCP_CLOSED;
					}
					else if(state->status == TCP_ESTABLISHED)
					{
						eth_frame_t *frame = (eth_frame_t*)_buffTx;
						ip_packet_t *ip = (ip_packet_t*)(frame->data);
						tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);

						tcp->seq_num = htonl( state->ack_num);
						tcp->ack_num = htonl( state->seq_num);

						tcp->flags = TCP_FLAG_ACK|TCP_FLAG_PSH;
						state->seq_num = ntohl(tcp->ack_num) ;
						state->ack_num = ntohl(tcp->seq_num);
						tcp->data[0]='1';
						tcp_xmit(i,frame,1);

						state->status = TCP_ESTABLISHED;
						state->event_time  = NetTime::pNettime->second_count+TCP_EVENT_SEC;
					}
				}
			}
		}

		IDriver* Stack::GetDriver()
		{
			return _driver;
		}

		uint8_t Stack::OpenTCP(uint32_t addr, uint16_t port)
		{
			uint8_t id=0xFF;
			// ищем свободный сокет
			for(int i=0;i<SOCKET_POOL_COUNT;i++)
			{
				tcp_state_t* state = &_socket_pool[i];
				if(state->status==TCP_CLOSED)
				{
					id =i;
					state->event_time = NetTime::pNettime->second_count+TCP_EVENT_SEC;
					eth_frame_t *frame = (eth_frame_t*)_buffTx;
					ip_packet_t *ip = (ip_packet_t*)(frame->data);
					tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);

					state->seq_num = NetTime::pNettime->gettc() + (NetTime::pNettime->gettc() << 16);
					state->ack_num=0;
					state->remote_addr = addr;
					state->remote_port = htons(port);
					state->local_port = (uint16_t)( NetTime::pNettime->gettc());
					state->ack_num =0;
					state->status = TCP_SYN_SENT;
					state->client =1;

					tcp->flags = TCP_FLAG_SYN;
					tcp->from_port = state->local_port;
					tcp->to_port =  state->remote_port;
					tcp->window = htons(TCP_WINDOW_SIZE);

					tcp_xmit(id,frame,0);

					uint16_t timeout =100;

					while(state->status!=TCP_ESTABLISHED&& --timeout)_delay_ms(150);
					if(timeout>0)
					{
						return id;
					}
					state->status = TCP_CLOSED;
				}
			}
			return 0xFF;
		}

		void Stack::CloseTCP(uint8_t id, bool fromIsr)
		{
			if(id==0xFF) return;
			if(!fromIsr)
			{
			tcp_state_t* state = &_socket_pool[id];
			if(state->status!=TCP_ESTABLISHED) return ;
			eth_frame_t *frame = (eth_frame_t*)_buffTx;
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);
			tcp->flags = 0;
			tcp->seq_num = htonl(state->ack_num);
			tcp->ack_num = htonl(state->seq_num);
			tcp->window = htons(TCP_WINDOW_SIZE);

			tcp->flags = TCP_FLAG_FIN|TCP_FLAG_ACK;
			if(state->client)
			{
				state->seq_num = ntohl(tcp->ack_num) ;
				state->ack_num = ntohl(tcp->seq_num);
				tcp_xmit(id,frame,0);
				state->status = TCP_FIN_WAIT;
			}
			else
			{
				state->seq_num = ntohl(tcp->ack_num) ;
				state->ack_num = ntohl(tcp->seq_num);
				memcpy(tcp->data,"Close",5);
				tcp_xmit(id,frame,5);
				state->status = TCP_FIN_WAIT1;
			}
			state->closing = true;
			tcp_xmit(id,frame,0);

			// для дебага
			uint16_t timeout =100;
			while(state->closing && --timeout)_delay_ms(150);
			}
			else
			{
				tcp_state_t* state = &_socket_pool[id];
				state->CloseFromISR= true;
			}

		}

		bool Stack::WriteTCP(uint8_t id, uint8_t* buff, uint16_t length)
		{
			if(id==0xFF) return false;
			tcp_state_t* state = &_socket_pool[id];
			if(state->status!=TCP_ESTABLISHED) return false;
			int timeout =20;
			state->waitACK =true;
			bool _state = false;//_ringTX.Add(buff,length);
			eth_frame_t *frame = (eth_frame_t*)_buffTx;
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);
			tcp->window = htons(TCP_WINDOW_SIZE);
			tcp->flags = TCP_FLAG_ACK|TCP_FLAG_PSH;
			tcp->seq_num = htonl(state->ack_num);
			tcp->ack_num = htonl(state->seq_num);

			tcp->flags = TCP_FLAG_ACK|TCP_FLAG_PSH;

			state->seq_num = ntohl(tcp->ack_num) ;
			state->ack_num += 0;
			memcpy(tcp->data,buff,length);
			tcp_xmit(id,frame,length);

			while(state->waitACK && --timeout)_delay_ms(150);

			_delay_ms(5);

			if(timeout<=0)
			{
				return false;
			}

			return state;
		}

		bool Stack::ReadTCP(uint8_t id, uint8_t* buff, uint16_t* length)
		{
			if(id==0xFF) return false;
			tcp_state_t* state = &_socket_pool[id];
			return state->_rx.Get(buff,length);
		}

		bool Stack::IsOpenTCP(uint8_t id)
		{
			if(id==0xFF) return false;
			tcp_state_t* state = &_socket_pool[id];
			return state->status == TCP_ESTABLISHED;
		}

		void  Stack::SetCallbackTCP(uint8_t id, CDelegate4<void,uint8_t,uint8_t*,uint16_t,uint16_t*>  _onreadwrite)
		{
			if(id==0xFF) return;
			_socket_pool[id].OnReadWrite = _onreadwrite;
			//_socket_pool[id].OnWrite = _write;
		}

		void Stack::DeleteCallbackTCP(uint8_t id)
		{
			if(id==0xFF) return;
			tcp_state_t* state = &_socket_pool[id];
			state->OnReadWrite.Remove();
			//state->OnWrite.Remove();
		}
		/*
		 * Ip
		 */

		void Stack::SendIp(eth_frame_t * frame, uint16_t length, uint32_t addr)
		{
			ip_send(frame,length,addr);
		}

		/*
		 * tcp
		 */

		void Stack::Listen(uint16_t port)
		{
			_localPort = htons(port);
		}

		void Stack::Deaf()
		{
			_localPort =0;
		}


		// process Ethernet frame
		void Stack::_eth_filter(eth_frame_t *frame, uint16_t len)
		{
			if(len >= sizeof(eth_frame_t))
			{
				switch(frame->type)
				{
				case ETH_TYPE_ARP:
					arp_filter(frame, len - sizeof(eth_frame_t));
					break;
				case ETH_TYPE_IP:
					ip_filter(frame, len - sizeof(eth_frame_t));
					break;
				}
			}
		}

		/*
		 * ARP
		 */
		uint8_t* Stack::arp_search_cache(uint32_t node_ip_addr)
		{
			uint8_t i;
			for(i = 0; i < ARP_CACHE_SIZE; ++i)
			{
				if(arp_cache[i].ip_addr == node_ip_addr)
					return arp_cache[i].mac_addr;
			}
			return 0;
		}

		void Stack::arp_filter(eth_frame_t *frame, uint16_t len)
		{
			arp_message_t *msg = (arp_message_t*)(frame->data);

			if(len >= sizeof(arp_message_t))
			{
				if( (msg->hw_type == ARP_HW_TYPE_ETH) &&
						(msg->proto_type == ARP_PROTO_TYPE_IP) &&
						(msg->ip_addr_to == IpAddr) )
				{
					switch(msg->type)
					{
					case ARP_TYPE_REQUEST:
						msg->type = ARP_TYPE_RESPONSE;
						memcpy(msg->mac_addr_to, msg->mac_addr_from, 6);
						memcpy(msg->mac_addr_from, _driver->MacAddr, 6);
						msg->ip_addr_to = msg->ip_addr_from;
						msg->ip_addr_from = IpAddr;
						_driver->EthReply(frame, sizeof(arp_message_t));
						break;
					case ARP_TYPE_RESPONSE:
						if(!arp_search_cache(msg->ip_addr_from))
						{
							arp_cache[arp_cache_wr].ip_addr = msg->ip_addr_from;
							memcpy(arp_cache[arp_cache_wr].mac_addr, msg->mac_addr_from, 6);
							arp_cache_wr++;
							if(arp_cache_wr == ARP_CACHE_SIZE)
								arp_cache_wr = 0;
						}
						break;
					}
				}
			}
		}

		uint8_t* Stack::arp_resolve(uint32_t node_ip_addr)
		{
			eth_frame_t *frame = (eth_frame_t*)_buffTx;
			arp_message_t *msg = (arp_message_t*)(frame->data);
			uint8_t *mac;

			// search arp cache
			if((mac = arp_search_cache(node_ip_addr)))
				return mac;

			// send request
			memset(frame->to_addr, 0xff, 6);
			frame->type = ETH_TYPE_ARP;

			msg->hw_type = ARP_HW_TYPE_ETH;
			msg->proto_type = ARP_PROTO_TYPE_IP;
			msg->hw_addr_len = 6;
			msg->proto_addr_len = 4;
			msg->type = ARP_TYPE_REQUEST;
			memcpy(msg->mac_addr_from, _driver->MacAddr, 6);
			msg->ip_addr_from = IpAddr;
			memset(msg->mac_addr_to, 0x00, 6);
			msg->ip_addr_to = node_ip_addr;

			_driver->EthSend(frame, sizeof(arp_message_t));
			//todo нада продумать
			uint16_t timeout = 10;
			while(!arp_search_cache(node_ip_addr)&& --timeout){Input();Process();_delay_ms(5);}
			if(timeout>0)
			{
				if((mac = arp_search_cache(node_ip_addr)))
					return mac;
			}

			return 0;
		}


		/*
		 * IP
		 */
		// process IP packet
		void Stack::ip_filter(eth_frame_t *frame, uint16_t len)
		{
			uint16_t hcs;
			ip_packet_t *packet = (ip_packet_t*)(frame->data);

			hcs = packet->cksum;
			packet->cksum = 0;

			if( (packet->ver_head_len == 0x45) &&
					(ip_cksum(0, (uint8_t*)packet, sizeof(ip_packet_t)) == hcs) &&
					((packet->to_addr == IpAddr) || (packet->to_addr == ip_broadcast)) )
			{
				len = ntohs(packet->total_len) -
						sizeof(ip_packet_t);

				switch(packet->protocol)
				{
				case IP_PROTOCOL_ICMP:
					icmp_filter(frame, len);
					break;

				case IP_PROTOCOL_UDP:
					udp_filter(frame, len);
					break;

				case IP_PROTOCOL_TCP:
					if(packet->to_addr != IpAddr)break;
					fromAddr = packet->from_addr;
					tcp_packet_t *tcp = (tcp_packet_t*)(packet->data);
					fromPort = tcp->from_port;
					tcp_filter(frame, len);
					break;
				}
			}
		}

		uint8_t Stack::ip_send(eth_frame_t *frame, uint16_t len,uint32_t to_addr)
		{
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			uint32_t route_ip;
			uint8_t *mac_addr_to;

			if(to_addr==0)
			{
				// set frame.dst
				if(ip->to_addr == ip_broadcast)
				{
					// use broadcast MAC
					memset(frame->to_addr, 0xff, 6);
				}
				else
				{
					// apply route
					if( ((ip->to_addr ^ IpAddr) & IpMask) == 0 )
						route_ip = ip->to_addr;
					else
						route_ip = IpGate;

					// resolve mac address
					if(!(mac_addr_to = arp_resolve(route_ip)))
						return 0;
					memcpy(frame->to_addr, mac_addr_to, 6);
				}
			}
			else
			{
				if(!(mac_addr_to = arp_resolve(to_addr)))
					return 0;
				memcpy(frame->to_addr, mac_addr_to, 6);

			}

			// set frame.type
			frame->type = ETH_TYPE_IP;

			// fill IP header
			len += sizeof(ip_packet_t);

			ip->ver_head_len = 0x45;
			ip->tos = 0;
			ip->total_len = htons(len);
			ip->fragment_id = 0;
			ip->flags_framgent_offset = 0;
			ip->ttl = IP_PACKET_TTL;
			ip->cksum = 0;
			ip->from_addr = IpAddr;
			ip->to_addr = to_addr;
			ip->cksum = ip_cksum(0, (uint8_t*)ip, sizeof(ip_packet_t));

			// send frame
			_driver->EthSend(frame, len);
			return 1;
		}

		//for udp and dhcp
		uint8_t Stack::_ip_send(eth_frame_t *frame, uint16_t len)
		{
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			uint32_t route_ip;
			uint8_t *mac_addr_to;

			// set frame.dst
			if(ip->to_addr == ip_broadcast)
			{
				// use broadcast MAC
				memset(frame->to_addr, 0xff, 6);
			}
			else
			{
				// apply route
				if( ((ip->to_addr ^ IpAddr) & IpMask) == 0 )
					route_ip = ip->to_addr;
				else
					route_ip = IpGate;

				// resolve mac address
				if(!(mac_addr_to = arp_resolve(route_ip)))
					return 0;
				memcpy(frame->to_addr, mac_addr_to, 6);
			}


			// set frame.type
			frame->type = ETH_TYPE_IP;

			// fill IP header
			len += sizeof(ip_packet_t);

			ip->ver_head_len = 0x45;
			ip->tos = 0;
			ip->total_len = htons(len);
			ip->fragment_id = 0;
			ip->flags_framgent_offset = 0;
			ip->ttl = IP_PACKET_TTL;
			ip->cksum = 0;
			ip->from_addr = IpAddr;

			ip->cksum = ip_cksum(0, (uint8_t*)ip, sizeof(ip_packet_t));

			// send frame
			_driver->EthSend(frame, len);
			return 1;
		}

		/*
		 * ICMP
		 */
		void Stack::icmp_filter(eth_frame_t *frame, uint16_t len)
		{
			ip_packet_t *packet = (ip_packet_t*)frame->data;
			icmp_echo_packet_t *icmp = (icmp_echo_packet_t*)packet->data;

			if(len >= sizeof(icmp_echo_packet_t) )
			{
				if(icmp->type == ICMP_TYPE_ECHO_RQ)
				{
					icmp->type = ICMP_TYPE_ECHO_RPLY;
					icmp->cksum += 8; // update cksum
					ip_send(frame, len,packet->from_addr);
				}
			}
		}

		/*
		 * UDP
		 */
		void Stack::udp_filter(eth_frame_t *frame, uint16_t len)
		{
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			udp_packet_t *udp = (udp_packet_t*)(ip->data);

			if(len >= sizeof(udp_packet_t))
			{
				len = ntohs(udp->len) - sizeof(udp_packet_t);

				switch(udp->to_port)
				{

				case DHCP_CLIENT_PORT:
					if(_dhcp)
					{
						dhcp_filter(frame, len);
					}
					break;

				default:
					//udp_packet(frame, len);
					break;
				}
			}
		}

		uint8_t Stack::udp_send(eth_frame_t *frame, uint16_t len)
		{
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			udp_packet_t *udp = (udp_packet_t*)(ip->data);

			len += sizeof(udp_packet_t);

			ip->protocol = IP_PROTOCOL_UDP;
			ip->from_addr = IpAddr;

			udp->len = htons(len);
			udp->cksum = 0;
			udp->cksum = ip_cksum(len + IP_PROTOCOL_UDP,(uint8_t*)udp-8, len+8);

			return _ip_send(frame, len);
		}

		#define dhcp_add_option(ptr, optcode, type, value) \
			((dhcp_option_t*)ptr)->code = optcode; \
			((dhcp_option_t*)ptr)->len = sizeof(type); \
			*(type*)(((dhcp_option_t*)ptr)->data) = value; \
			ptr += sizeof(dhcp_option_t) + sizeof(type); \
			if(sizeof(type)&1) *(ptr++) = 0;

		#define dhcp_add_option_str(ptr, optcode, value, length) \
			((dhcp_option_t*)ptr)->code = optcode; \
			((dhcp_option_t*)ptr)->len = length; \
			memcpy(((dhcp_option_t*)ptr)->data, value,length); \
			ptr += sizeof(dhcp_option_t) + length; \
			if(length&1) *(ptr++) = 0;

		void Stack::dhcp_filter(eth_frame_t *frame, uint16_t len)
		{
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			udp_packet_t *udp = (udp_packet_t*)(ip->data);
			dhcp_message_t *dhcp = (dhcp_message_t*)(udp->data);
			dhcp_option_t *option;
			uint8_t *op, optlen;
			uint32_t offered_net_mask = 0, offered_gateway = 0;
			uint32_t lease_time = 0, renew_time = 0, renew_server = 0;
			uint8_t type = 0;
			uint32_t temp;

			// Check if DHCP messages directed to us
			if( (len >= sizeof(dhcp_message_t)) &&
					(dhcp->operation == DHCP_OP_REPLY) &&
					(dhcp->transaction_id == dhcp_tr_id) &&
					(dhcp->magic_cookie == DHCP_MAGIC_COOKIE) )
			{
				len -= sizeof(dhcp_message_t);

				// parse DHCP message
				op = dhcp->options;
				while(len >= sizeof(dhcp_option_t))
				{
					option = (dhcp_option_t*)op;
					if(option->code == DHCP_CODE_PAD)
					{
						op++;
						len--;
					}
					else if(option->code == DHCP_CODE_END)
					{
						break;
					}
					else
					{
						switch(option->code)
						{
						case DHCP_CODE_MESSAGETYPE:
							type = *(option->data);
							break;
						case DHCP_CODE_SUBNETMASK:
							offered_net_mask = *(uint32_t*)(option->data);
							break;
						case DHCP_CODE_GATEWAY:
							offered_gateway = *(uint32_t*)(option->data);
							break;
						case DHCP_CODE_DHCPSERVER:
							renew_server = *(uint32_t*)(option->data);
							break;
						case DHCP_CODE_LEASETIME:
							temp = *(uint32_t*)(option->data);
							lease_time = ntohl(temp);
							if(lease_time > 21600)
								lease_time = 21600;
							break;
							/*case DHCP_CODE_RENEWTIME:
									temp = *(uint32_t*)(option->data);
									renew_time = ntohl(temp);
									if(renew_time > 21600)
										renew_time = 21600;
									break;*/
						}

						optlen = sizeof(dhcp_option_t) + option->len;
						op += optlen;
						len -= optlen;
					}
				}

				if(!renew_server)
					renew_server = ip->from_addr;

				switch(type)
				{
				// DHCP offer?
				case DHCP_MESSAGE_OFFER:
					if( (dhcp_status == DHCP_WAITING_OFFER) &&
							(dhcp->offered_addr != 0) )
					{
						dhcp_status = DHCP_WAITING_ACK;

						// send DHCP request
						ip->to_addr = inet_addr(255,255,255,255);

						udp->to_port = DHCP_SERVER_PORT;
						udp->from_port = DHCP_CLIENT_PORT;

						op = dhcp->options;
						dhcp_add_option(op, DHCP_CODE_MESSAGETYPE,
								uint8_t, DHCP_MESSAGE_REQUEST);
						dhcp_add_option(op, DHCP_CODE_REQUESTEDADDR,
								uint32_t, dhcp->offered_addr);
						dhcp_add_option(op, DHCP_CODE_DHCPSERVER,
								uint32_t, renew_server);
						*(op++) = DHCP_CODE_END;

						dhcp->operation = DHCP_OP_REQUEST;
						dhcp->offered_addr = 0;
						dhcp->server_addr = 0;
						dhcp->flags = DHCP_FLAG_BROADCAST;

						udp_send(frame, (uint8_t*)op - (uint8_t*)dhcp);
					}
					break;

					// DHCP ack?
				case DHCP_MESSAGE_ACK:
					if( (dhcp_status == DHCP_WAITING_ACK) &&
							(lease_time) )
					{
						if(!renew_time)
							renew_time = lease_time/2;

						dhcp_status = DHCP_ASSIGNED;
						dhcp_server = renew_server;
						dhcp_renew_time = NetTime::pNettime->rtime() + renew_time;
						dhcp_retry_time =  NetTime::pNettime->rtime() + lease_time;

						// network up
						IpAddr = dhcp->offered_addr;
						IpMask = offered_net_mask;
						IpGate = offered_gateway;
					}
					break;
				}
			}
		}

		void Stack::dhcp_poll()
		{
			eth_frame_t *frame = (eth_frame_t*)_buffTx;
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			udp_packet_t *udp = (udp_packet_t*)(ip->data);
			dhcp_message_t *dhcp = (dhcp_message_t*)(udp->data);
			uint8_t *op;
			if(NetTime::pNettime->rtime() >= dhcp_retry_time)
			{
				dhcp_status = DHCP_WAITING_OFFER;
				dhcp_retry_time = NetTime::pNettime->rtime() + 15;
				dhcp_tr_id = NetTime::pNettime->gettc() + (NetTime::pNettime->gettc() << 16);

				// network down
				IpAddr = 0;
				IpMask = 0;
				IpGate = 0;

				// send DHCP discover
				ip->to_addr = inet_addr(255,255,255,255);

				udp->to_port = DHCP_SERVER_PORT;
				udp->from_port = DHCP_CLIENT_PORT;

				memset(dhcp, 0, sizeof(dhcp_message_t));
				dhcp->operation = DHCP_OP_REQUEST;
				dhcp->hw_addr_type = DHCP_HW_ADDR_TYPE_ETH;
				dhcp->hw_addr_len = 6;
				dhcp->transaction_id = dhcp_tr_id;
				dhcp->flags = DHCP_FLAG_BROADCAST;
				memcpy(dhcp->hw_addr, _driver->MacAddr, 6);
				dhcp->magic_cookie = DHCP_MAGIC_COOKIE;

				op = dhcp->options;
				dhcp_add_option(op, DHCP_CODE_MESSAGETYPE, uint8_t, DHCP_MESSAGE_DISCOVER);
				dhcp_add_option_str(op, DHCP_CODE_HOST, "STM32",5);
				*(op++) = DHCP_CODE_END;

				udp_send(frame, (uint8_t*)op - (uint8_t*)dhcp);
			}

			// time to renew lease
			if( (NetTime::pNettime->rtime() >= dhcp_renew_time) &&
					(dhcp_status == DHCP_ASSIGNED) )
			{
				dhcp_tr_id = NetTime::pNettime->gettc() + (NetTime::pNettime->gettc() << 16);

				// send DHCP request
				ip->to_addr = dhcp_server;

				udp->to_port = DHCP_SERVER_PORT;
				udp->from_port = DHCP_CLIENT_PORT;

				memset(dhcp, 0, sizeof(dhcp_message_t));
				dhcp->operation = DHCP_OP_REQUEST;
				dhcp->hw_addr_type = DHCP_HW_ADDR_TYPE_ETH;
				dhcp->hw_addr_len = 6;
				dhcp->transaction_id = dhcp_tr_id;
				dhcp->client_addr = IpAddr;
				memcpy(dhcp->hw_addr, _driver->MacAddr, 6);
				dhcp->magic_cookie = DHCP_MAGIC_COOKIE;

				op = dhcp->options;
				dhcp_add_option(op, DHCP_CODE_MESSAGETYPE,
						uint8_t, DHCP_MESSAGE_REQUEST);
				dhcp_add_option(op, DHCP_CODE_REQUESTEDADDR,
						uint32_t, IpAddr);
				dhcp_add_option(op, DHCP_CODE_DHCPSERVER,
						uint32_t, dhcp_server);
				*(op++) = DHCP_CODE_END;

				if(!udp_send(frame, (uint8_t*)op - (uint8_t*)dhcp))
				{
					dhcp_renew_time = NetTime::pNettime->rtime() + 5;
					return;
				}

				dhcp_status = DHCP_WAITING_ACK;
			}
		}

		void Stack::tcp_filter(eth_frame_t *frame, uint16_t len)
		{
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);

			for(int i=0;i<SOCKET_POOL_COUNT;i++)
			{
				tcp_state_t* state = &_socket_pool[i];

				// для серверной части открытие нового соединения
				if(state->status == TCP_CLOSED && ((tcp->flags & TCP_FLAG_SYN) || (tcp->flags & (TCP_FLAG_SYN | TCP_FLAG_ACK))) && tcp->to_port == _localPort )
				{
					state->event_time = NetTime::pNettime->second_count+TCP_EVENT_SEC;
					state->remote_addr = ip->from_addr;//fromAddr;
					state->remote_port = tcp->from_port;//fromPort;
					state->local_port = _localPort;
					SocketEntry(i,frame,len);
					break;
				}else
					// для серверной части
					if(state->status!= TCP_CLOSED && state->remote_addr == ip->from_addr && (!(tcp->flags & TCP_FLAG_SYN)|| !(tcp->flags & (TCP_FLAG_SYN | TCP_FLAG_ACK))) )
					{
						if( state->local_port == tcp->to_port )
						{
							if( state->remote_port == tcp->from_port)
							{
								state->event_time = NetTime::pNettime->second_count+TCP_EVENT_SEC;
								SocketEntry(i,frame,len);
							}
						}
						break;
					}
					else
						// для клиентской части
						if(state->status!=TCP_CLOSED && ((tcp->flags & TCP_FLAG_SYN) || (tcp->flags & (TCP_FLAG_SYN | TCP_FLAG_ACK))))
						{
							if( state->local_port == tcp->to_port )
							{
								if( state->remote_port == tcp->from_port)
								{
									state->event_time = NetTime::pNettime->second_count+TCP_EVENT_SEC;
									SocketEntry(i,frame,len);
								}
							}
							break;
						}

			}

		}

		bool Stack::tcp_xmit(uint8_t id,eth_frame_t *frame, uint16_t len)
		{
			uint8_t status = 1;
			uint16_t temp, plen = len;

			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);
			tcp_state_t* state = &_socket_pool[id];

			// set packet fields
			ip->to_addr = state->remote_addr;
			ip->from_addr =IpAddr;
			ip->protocol = IP_PROTOCOL_TCP;
			tcp->to_port = state->remote_port;
			tcp->from_port =  state->local_port;

			if(tcp->flags & TCP_FLAG_SYN)
			{
				// add MSS option (max. segment size)
				tcp->data_offset = (sizeof(tcp_packet_t) + 4) << 2;
				tcp->data[0] = 2;//option: MSS
				tcp->data[1] = 4;//option len
				tcp->data[2] = TCP_SYN_MSS>>8;
				tcp->data[3] = TCP_SYN_MSS&0xff;
				plen = 4;
			}
			else
			{
				tcp->data_offset = sizeof(tcp_packet_t) << 2;
			}

			// set stream pointers
			tcp->seq_num = htonl( state->seq_num);
			tcp->ack_num = htonl( state->ack_num);

			// set checksum
			plen += sizeof(tcp_packet_t);
			tcp->cksum = 0;
			tcp->cksum = ip_cksum(plen + IP_PROTOCOL_TCP,
					(uint8_t*)tcp - 8, plen + 8);

			// send packet

			pStack->SendIp(frame, plen,ip->to_addr);


			// advance sequence number
			state->seq_num += len;
			if( (tcp->flags & TCP_FLAG_SYN) || (tcp->flags & TCP_FLAG_FIN) )
				state->seq_num++;

			return true;
		}

		void Stack::SocketEntry(uint8_t id,eth_frame_t *frame, uint16_t len)
		{
			ip_packet_t *ip = (ip_packet_t*)(frame->data);
			tcp_packet_t *tcp = (tcp_packet_t*)(ip->data);
			tcp_state_t* state = &_socket_pool[id];

			uint8_t tcpflags = tcp->flags & (TCP_FLAG_SYN | TCP_FLAG_ACK | TCP_FLAG_RST | TCP_FLAG_FIN|TCP_FLAG_PSH);

			len -= tcp_head_size(tcp);

			if((tcpflags==TCP_FLAG_RST)||(tcpflags==(TCP_FLAG_RST|TCP_FLAG_FIN))||(tcpflags==(TCP_FLAG_RST|TCP_FLAG_ACK)))
			{
				state->status=TCP_CLOSED;
				return;
			}
					switch(state->status)
					{

						case TCP_CLOSED:
							state->event_time = NetTime::pNettime->second_count+TCP_EVENT_SEC;
							if(tcpflags==TCP_FLAG_SYN)
							{
								tcp->flags = TCP_FLAG_SYN|TCP_FLAG_ACK;
								state->seq_num = NetTime::pNettime->gettc() + (NetTime::pNettime->gettc() << 16);
								state->ack_num = ntohl(tcp->seq_num) + 1;
								tcp_xmit(id,frame,len);
								state->status = TCP_SYN_RECEIVED;
								return;
							}
							break;
						case TCP_SYN_SENT:
							state->event_time = NetTime::pNettime->second_count+TCP_EVENT_SEC;
							if(tcpflags==(TCP_FLAG_ACK|TCP_FLAG_SYN))
							{
								tcp->flags = TCP_FLAG_ACK;
								state->seq_num = ntohl(tcp->ack_num) ;
								state->ack_num = ntohl(tcp->seq_num)+1 ;
								tcp_xmit(id,frame,0);
								state->status = TCP_ESTABLISHED;
								if(!OnConnect.IsNull())
								{
									OnConnect(id);
								}
								break;
							}
							break;
						case TCP_SYN_RECEIVED:
							state->event_time = NetTime::pNettime->second_count+TCP_EVENT_SEC;
							if(tcpflags==TCP_FLAG_ACK)
							{
								tcp->flags = TCP_FLAG_ACK;
								state->seq_num = ntohl(tcp->ack_num) ;
								state->ack_num = ntohl(tcp->seq_num) ;
								tcp_xmit(id,frame,0);
								state->status = TCP_ESTABLISHED;
								if(!OnConnect.IsNull())
								{
									OnConnect(id);
								}
								break;
							}
							break;
						case TCP_ESTABLISHED:
							state->event_time  = NetTime::pNettime->second_count+TCP_EVENT_SEC;
							if(tcpflags==(TCP_FLAG_ACK) || tcpflags==(TCP_FLAG_ACK|TCP_FLAG_PSH))
								{
									if(state->CloseFromISR)
									{
										tcp->flags = TCP_FLAG_FIN|TCP_FLAG_ACK;
										state->seq_num = ntohl(tcp->ack_num) ;
										state->ack_num +=len;
										tcp_xmit(id,frame,0);
										state->status = TCP_FIN_WAIT1;
										state->CloseFromISR=false;
									}
									else
									{
									tcp->flags = TCP_FLAG_ACK;//|TCP_FLAG_PSH;
									state->seq_num = ntohl(tcp->ack_num) ;
									state->ack_num +=len;


									uint16_t length=0;

										if(!state->OnReadWrite.IsNull())
										{
											state->OnReadWrite(id,tcp_get_data(tcp),len , &length);
										}

									state->waitACK =false;
									tcp_xmit(id,frame,length);
									state->status = TCP_ESTABLISHED;

									}
									break;

								}
								else
									if(tcpflags==(TCP_FLAG_ACK|TCP_FLAG_FIN))
									{
										tcp->flags = TCP_FLAG_ACK|TCP_FLAG_FIN;

										state->seq_num = ntohl(tcp->ack_num) ;
										state->ack_num = ntohl(tcp->seq_num)+len;

										uint16_t length=0;
										if(!state->OnReadWrite.IsNull())
										{
											state->OnReadWrite(id,tcp_get_data(tcp),len , &length);
										}
										state->closing = false;
										tcp_xmit(id,frame,0);
										state->status = TCP_FIN_WAIT;

										break;
									}
							break;
						case TCP_FIN_WAIT:
							if(tcpflags==(TCP_FLAG_ACK))
							{
								tcp->flags = TCP_FLAG_ACK;

								state->seq_num = ntohl(tcp->ack_num) ;
								state->ack_num = ntohl(tcp->seq_num) ;
								tcp_xmit(id,frame,0);

								state->status = TCP_CLOSED;

								if(!OnDisconnect.IsNull())
								{
									OnDisconnect(id);
								}

								break;
							}else
								if(tcpflags==(TCP_FLAG_ACK|TCP_FLAG_FIN))
								{
									tcp->flags = TCP_FLAG_ACK;

									state->seq_num = ntohl(tcp->ack_num) ;
									state->ack_num = ntohl(tcp->seq_num) ;
									tcp_xmit(id,frame,0);

									state->status = TCP_CLOSED;
									if(!OnDisconnect.IsNull())
									{
										OnDisconnect(id);
									}

									break;
								}

							break;

						case TCP_FIN_WAIT1:
							if(tcpflags==(TCP_FLAG_ACK))
							{
								state->status = TCP_FIN_WAIT2;
								state->closing = false;
							}

							break;
						case TCP_FIN_WAIT2:
							if(tcpflags==(TCP_FLAG_ACK|TCP_FLAG_FIN))
							{
								tcp->flags = TCP_FLAG_ACK;

								state->seq_num = ntohl(tcp->ack_num) ;
								state->ack_num = ntohl(tcp->seq_num)+1 ;
								tcp_xmit(id,frame,0);
								state->status = TCP_CLOSED;

								if(!OnDisconnect.IsNull())
								{
									OnDisconnect(id);
								}
							//	_startClose = false;
								state->closing = false;
							}
							if(tcpflags==(TCP_FLAG_FIN))
							{
								tcp->flags = TCP_FLAG_ACK;

								state->seq_num = ntohl(tcp->ack_num) ;
								state->ack_num = ntohl(tcp->seq_num)+1 ;
								tcp_xmit(id,frame,0);
								state->status = TCP_CLOSED;
								if(!OnDisconnect.IsNull())
								{
									OnDisconnect(id);
								}
								//_startClose = false;
								state->closing = false;
							}
							break;
					}//end switch
				}

