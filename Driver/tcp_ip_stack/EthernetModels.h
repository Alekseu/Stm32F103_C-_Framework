///*
// * EthernetModels.h
// *
// *  Created on: 07 окт. 2016 г.
// *      Author: hudienko_a
// */
//
//#ifndef NET_ETHERNETMODELS_H_
//#define NET_ETHERNETMODELS_H_
//
//
///*
// * BE conversion
// */
//
//#define htons(a)			((((a)>>8)&0xff)|(((a)<<8)&0xff00))
//#define ntohs(a)			htons(a)
//
//#define htonl(a)			( (((a)>>24)&0xff) | (((a)>>8)&0xff00) |\
//								(((a)<<8)&0xff0000) | (((a)<<24)&0xff000000) )
//#define ntohl(a)			htonl(a)
//
//#define inet_addr(a,b,c,d)	( ((uint32_t)a) | ((uint32_t)b << 8) |\
//								((uint32_t)c << 16) | ((uint32_t)d << 24) )
//
//#define ETH_TYPE_ARP		htons(0x0806)
//#define ETH_TYPE_IP			htons(0x0800)
//
//typedef struct eth_frame {
//	uint8_t to_addr[6];
//	uint8_t from_addr[6];
//	uint16_t type   __attribute__ ((packed));
//	uint8_t data[];
//} eth_frame_t;
//
//
///*
// * ARP
// */
//
//#define ARP_HW_TYPE_ETH		htons(0x0001)
//#define ARP_PROTO_TYPE_IP	htons(0x0800)
//
//#define ARP_TYPE_REQUEST	htons(1)
//#define ARP_TYPE_RESPONSE	htons(2)
//
//typedef struct arp_message {
//	uint16_t hw_type __attribute__ ((packed));
//	uint16_t proto_type __attribute__ ((packed));
//	uint8_t hw_addr_len;
//	uint8_t proto_addr_len;
//	uint16_t type __attribute__ ((packed));
//	uint8_t mac_addr_from[6];
//	uint32_t ip_addr_from __attribute__ ((packed));
//	uint8_t mac_addr_to[6];
//	uint32_t ip_addr_to __attribute__ ((packed));
//} arp_message_t;
//
//typedef struct arp_cache_entry {
//	uint32_t ip_addr __attribute__ ((packed));
//	uint8_t mac_addr[6];
//} arp_cache_entry_t;
//
//
///*
// * IP
// */
//
//#define IP_PROTOCOL_ICMP	1
//#define IP_PROTOCOL_TCP		6
//#define IP_PROTOCOL_UDP		17
//
//typedef struct ip_packet {
//	uint8_t ver_head_len;
//	uint8_t tos;
//	uint16_t total_len __attribute__ ((packed));
//	uint16_t fragment_id __attribute__ ((packed));
//	uint16_t flags_framgent_offset __attribute__ ((packed));
//	uint8_t ttl;
//	uint8_t protocol;
//	uint16_t cksum __attribute__ ((packed));
//	uint32_t from_addr __attribute__ ((packed));
//	uint32_t to_addr __attribute__ ((packed));
//	uint8_t data[];
//} ip_packet_t;
//
//
///*
// * ICMP
// */
//
//#define ICMP_TYPE_ECHO_RQ	8
//#define ICMP_TYPE_ECHO_RPLY	0
//
//typedef struct icmp_echo_packet {
//	uint8_t type;
//	uint8_t code;
//	uint16_t cksum __attribute__ ((packed));
//	uint16_t id __attribute__ ((packed));
//	uint16_t seq __attribute__ ((packed));
//	uint8_t data[];
//} icmp_echo_packet_t;
//
//
///*
// * UDP
// */
//
//typedef struct udp_packet {
//	uint16_t from_port __attribute__ ((packed));
//	uint16_t to_port __attribute__ ((packed));
//	uint16_t len __attribute__ ((packed));
//	uint16_t cksum __attribute__ ((packed));
//	uint8_t data[];
//} udp_packet_t;
//
//
//
///*
// * DHCP
// */
//
//#define DHCP_SERVER_PORT		htons(67)
//#define DHCP_CLIENT_PORT		htons(68)
//
//#define DHCP_OP_REQUEST			1
//#define DHCP_OP_REPLY			2
//
//#define DHCP_HW_ADDR_TYPE_ETH	1
//
//#define DHCP_FLAG_BROADCAST		htons(0x8000)
//
//#define DHCP_MAGIC_COOKIE		htonl(0x63825363)
//
//typedef struct dhcp_message {
//	uint8_t operation;
//	uint8_t hw_addr_type;
//	uint8_t hw_addr_len;
//	uint8_t unused1;
//	uint32_t transaction_id __attribute__ ((packed));
//	uint16_t second_count __attribute__ ((packed));
//	uint16_t flags __attribute__ ((packed));
//	uint32_t client_addr __attribute__ ((packed));
//	uint32_t offered_addr __attribute__ ((packed));
//	uint32_t server_addr __attribute__ ((packed));
//	uint32_t unused2 __attribute__ ((packed));
//	uint8_t hw_addr[16];
//	uint8_t unused3[192];
//	uint32_t magic_cookie __attribute__ ((packed));
//	uint8_t options[];
//} dhcp_message_t;
//
//#define DHCP_CODE_PAD			0
//#define DHCP_CODE_END			255
//#define DHCP_CODE_SUBNETMASK	1
//#define DHCP_CODE_GATEWAY		3
//#define DHCP_CODE_REQUESTEDADDR	50
//#define DHCP_CODE_LEASETIME		51
//#define DHCP_CODE_MESSAGETYPE	53
//#define DHCP_CODE_HOST   		12
//#define DHCP_CODE_DHCPSERVER	54
//#define DHCP_CODE_RENEWTIME		58
//#define DHCP_CODE_REBINDTIME	59
//
//typedef struct dhcp_option {
//	uint8_t code;
//	uint8_t len;
//	uint8_t data[];
//} dhcp_option_t;
//
//#define DHCP_MESSAGE_DISCOVER	1
//#define DHCP_MESSAGE_OFFER		2
//#define DHCP_MESSAGE_REQUEST	3
//#define DHCP_MESSAGE_DECLINE	4
//#define DHCP_MESSAGE_ACK		5
//#define DHCP_MESSAGE_NAK		6
//#define DHCP_MESSAGE_RELEASE	7
//#define DHCP_MESSAGE_INFORM		8
//
//typedef enum dhcp_status_code {
//	DHCP_INIT,
//	DHCP_ASSIGNED,
//	DHCP_WAITING_OFFER,
//	DHCP_WAITING_ACK
//} dhcp_status_code_t;
//
//
//
///*
// * TCP
// */
//
//#define TCP_FLAG_URG		0x20
//#define TCP_FLAG_ACK		0x10
//#define TCP_FLAG_PSH		0x08
//#define TCP_FLAG_RST		0x04
//#define TCP_FLAG_SYN		0x02
//#define TCP_FLAG_FIN		0x01
//
//typedef struct tcp_packet {
//	uint16_t from_port __attribute__ ((packed));
//	uint16_t to_port __attribute__ ((packed));
//	uint32_t seq_num __attribute__ ((packed));
//	uint32_t ack_num __attribute__ ((packed));
//	uint8_t data_offset;
//	uint8_t flags;
//	uint16_t window __attribute__ ((packed));
//	uint16_t cksum __attribute__ ((packed));
//	uint16_t urgent_ptr __attribute__ ((packed));
//	uint8_t data[];
//} tcp_packet_t;
//
//#define tcp_head_size(tcp)	(((tcp)->data_offset & 0xf0) >> 2)
//#define tcp_get_data(tcp)	((uint8_t*)(tcp) + tcp_head_size(tcp))
//
//typedef enum tcp_status_code {
//	TCP_LISTEN,
//	TCP_CLOSED,
//	TCP_SYN_SENT,
//	TCP_SYN_RECEIVED,
//	TCP_ESTABLISHED,
//	TCP_FIN_WAIT,
//	TCP_FIN_WAIT1,
//	TCP_FIN_WAIT2
//} tcp_status_code_t;
//
//#include "../Utils/delegate.h"
//#include "../Utils/SRingBuffer.h"
//
//typedef struct tcp_state {
//	tcp_status_code_t status;
//	uint32_t event_time     ;
//	uint32_t seq_num        ;
//	uint32_t ack_num        ;
//	uint32_t remote_addr    ;
//	uint16_t remote_port    ;
//	uint16_t local_port      ;
//	uint8_t  keep_alive     ;
//	uint8_t  client         ;
//	bool waitACK       ;
//	bool closing;
//	bool CloseFromISR;
//	uint8_t rexmit_count     ;
//	uint32_t seq_num_saved  ;
//	CDelegate4<void,uint8_t,uint8_t*,uint16_t,uint16_t* > OnReadWrite;
//	//CDelegate3<void,uint8_t,uint8_t*,uint16_t*> OnWrite;
//	SRingBuffer<512,5> _rx;
//
//} tcp_state_t;
//
//#define TCP_OPTION_PUSH			0x01
//#define TCP_OPTION_CLOSE		0x02
//
//
//inline uint16_t ip_cksum(uint32_t sum, uint8_t *buf, uint16_t len)
//{
//	while(len >= 2)
//	{
//		sum += ((uint16_t)*buf << 8) | *(buf+1);
//		buf += 2;
//		len -= 2;
//	}
//
//	if(len)
//		sum += (uint16_t)*buf << 8;
//
//	while(sum >> 16)
//		sum = (sum & 0xffff) + (sum >> 16);
//
//	return ~htons((uint16_t)sum);
//}
//
//
//#endif /* NET_ETHERNETMODELS_H_ */
