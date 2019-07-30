#ifndef _NETIF_PARSER_H_
#define _NETIF_PARSER_H_

#include <stdint.h>
//#include <bool.h>

#define EMOTE_COM_NETIF 1
#if defined(EMOTE_COM_NETIF)
#define COM_NETIF COM2
#define NETIF_START_STOP_CHAR_SIZE 3
#define NETIF_START_STOP_CHAR 0xE8
#define NETIF_MTU 256
#define NETIF_MIN_PKT_SIZE 28 //20byte IP header+8byte udp header
#endif //EMOTE_COM_NETIF


//typedef char UINT8;
//typedef uint32_t UINT32;
//typedef UINT8 BOOL;

#define NETIF_BUF_SIZE NETIF_MTU+2*NETIF_START_STOP_CHAR_SIZE

//Parses the incoming bytes and marks the begining and ending of a network pkt
//in the pal buffer. Reads the packet from the pal buffer only when it detects a who packet.
//And finally notify the network driver that a packet has arrived
//TODO: Need to put in discard logic if packet is corrupted


#define hal_printf printf

#endif //_NETIF_PARSER_H_