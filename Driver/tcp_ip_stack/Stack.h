/*
 * Stack.h
 *
 *  Created on: 07 окт. 2016 г.
 *      Author: hudienko_a
 */

#ifndef NET_STACK_H_
#define NET_STACK_H_
#include "Interface/IDriver.h"
#include "../Utils/SRingBuffer.h"
#include <string.h>
#include "StackConfig.h"
#include "NetTime.h"
#include "Socket.h"
#include "../Utils/delegate.h"
//template<class T = IDriver >

class Stack
{
#define ip_broadcast (IpAddr | ~IpMask)
	public:
		static Stack* pStack;
		uint32_t IpAddr;
		uint32_t IpMask;
		uint32_t IpGate;
		uint32_t Dns;

		// коллбек при успешной установки соединения передает id сокета в пуле
		CDelegate1<void,uint8_t>OnConnect;
		//коллбек при разрыве соединения передает id завершенного соединения
		CDelegate1<void,uint8_t>OnDisconnect;
		// коллбек полученого udp пакета адресованного нам
		CDelegate2<void, uint8_t*, uint16_t> OnUDPRecive;


		//данные поля публичны для зануления при отсутствии линка (возможно сделать через метод)
		dhcp_status_code_t dhcp_status;
		uint32_t dhcp_retry_time;

		Stack(uint8_t* mac, uint32_t ipAddr,uint32_t ipMask,uint32_t ipGate,uint32_t dns , IDriver* driver);

		bool Init();

		void Input();
		void Process();

		bool LanUP();

		void PeriodicTime();

		/*
		 * Ip
		 */
		void SendIp(eth_frame_t * frame, uint16_t length);
		void SendIp(eth_frame_t * frame, uint16_t length, uint32_t addr);

		/*
		 * tcp
		 */
		//открыть соединение
		uint8_t OpenTCP(uint32_t addr, uint16_t port);
		//закрывает соединения с номером id
		void CloseTCP(uint8_t id,bool fromIsr =false);
		// плюнуть в поток напрямую не через коллбек
		bool WriteTCP(uint8_t id, uint8_t* buff, uint16_t length);
		// забрать с потока если не через коллбек
		bool ReadTCP(uint8_t id, uint8_t* buff, uint16_t* length);
		// проверить состояние соединения по id
		bool IsOpenTCP(uint8_t id);

		void SetCallbackTCP(uint8_t id, CDelegate4<void,uint8_t,uint8_t*,uint16_t,uint16_t*>  _onreadwrite);
		void DeleteCallbackTCP(uint8_t id);

		// для сервера слушать порт
		void Listen(uint16_t port);
		// перестать слушать порт
		void Deaf();

		// получить драйвер для прерываний т.как стек работает с прерывания
		IDriver* GetDriver();

	private:

		uint16_t _localPort;
		bool _listen;

		uint8_t current_socket_count;

		//хрень компилятора
		uint32_t fromAddr;
		uint16_t fromPort;

		//todo внимание может быть проблема с делегатами в структуре tcp_state_t (утечка памяти!!!!)
		tcp_state_t _socket_pool[SOCKET_POOL_COUNT];

		//буфер приема
		uint8_t buff[BUFFER_SIZE_FOR_ONE_PACKET];
		//буфер отправки
		uint8_t _buffTx[BUFFER_SIZE_FOR_ONE_PACKET];

		IDriver* _driver;
		SRingBuffer<STACK_RING_BUFFER_SIZE,STACK_RING_BUFFER_COUNT> _ring;

		// ARP cache
		uint8_t arp_cache_wr;
		arp_cache_entry_t arp_cache[ARP_CACHE_SIZE];

		// все что касается dhcp
		bool _dhcp;
		uint32_t dhcp_tr_id;
		uint32_t dhcp_server;
		uint32_t dhcp_renew_time;


		// process Ethernet frame
		void _eth_filter(eth_frame_t *frame, uint16_t len);

		/*
		 * ARP
		 */
		uint8_t *arp_search_cache(uint32_t node_ip_addr);

		void arp_filter(eth_frame_t *frame, uint16_t len);

		uint8_t *arp_resolve(uint32_t node_ip_addr);


		/*
		 * IP
		 */
		// process IP packet
		void ip_filter(eth_frame_t *frame, uint16_t len);

		uint8_t ip_send(eth_frame_t *frame, uint16_t len,uint32_t to_addr=0);

		uint8_t _ip_send(eth_frame_t *frame, uint16_t len);

		/*
		 * ICMP
		 */
		void icmp_filter(eth_frame_t *frame, uint16_t len);

		/*
		 * UDP
		 */
		void udp_filter(eth_frame_t *frame, uint16_t len);

		uint8_t udp_send(eth_frame_t *frame, uint16_t len);

		void dhcp_filter(eth_frame_t *frame, uint16_t len);

		void dhcp_poll();

		/*
		 * TCP
		 */
		void tcp_filter(eth_frame_t *frame, uint16_t len);

		bool tcp_xmit(uint8_t id,eth_frame_t *frame, uint16_t len);

		//TCP процессор принимает id соединения в пуле, фрейм и длинну
		void SocketEntry(uint8_t id,eth_frame_t *frame, uint16_t len);
};




#endif /* NET_STACK_H_ */
