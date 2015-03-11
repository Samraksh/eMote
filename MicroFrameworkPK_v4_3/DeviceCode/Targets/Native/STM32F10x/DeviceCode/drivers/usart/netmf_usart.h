#ifndef _NETMF_USART_H_
#define _NETMF_USART_H_
#include <stm32f10x.h>

#define MAX_BAUDRATE 115200
#define MIN_BAUDRATE 0

static void UART_RX_DO(void *arg);

struct uart_rx_job {
	int len;
	int port;
	char *buf;
};

#endif
