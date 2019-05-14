#ifndef _SOCKETOPTS_H_
#define _SOCKETOPTS_H_

#include <tinyhal.h>
//#include <LWIP_Sockets.h>
#include <lwip/sockets.h>

#define SENDER_PORT_NUM 6000
#define SENDER_IP_ADDR "192.168.5.100"

#define SERVER_PORT_NUM 6001
#define SERVER_IP_ADDRESS "192.168.5.200"


typedef void *MsgRecv_t (void* msg, uint16_t size);

int ServerInit(BOOL isTcp, MsgRecv_t handler);
int ClientInit(BOOL isTcp, MsgRecv_t handler);
//BOOL SendHello(int socket_fd, uint32_t iter);
BOOL Recv(int socket_fd, uint32_t iter);

int client_fd, server_fd;
char data_buffer[256];

#endif //_SOCKETOPTS_H_
