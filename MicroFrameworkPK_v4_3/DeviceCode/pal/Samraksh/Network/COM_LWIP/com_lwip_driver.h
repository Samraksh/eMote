#include "tinyhal.h" 

#include "lwip\netif.h"
#include "lwip\pbuf.h"
#include "lwip\dhcp.h"

#ifndef _COM_LWIP_DRIVER_H_
#define _COM_LWIP_DRIVER_H_ 1

#define NETIF_COM_BAUD 115200
#define NETIF_COM_PARITY 0
#define NETIF_COM_DATABITS 8
#define NETIF_COM_STOPBITS 1
#define NETIF_COM_FLOW 0

const char NETIF_START_STOP_BYTES[NETIF_START_STOP_CHAR_SIZE] = {NETIF_START_STOP_CHAR,NETIF_START_STOP_CHAR,NETIF_START_STOP_CHAR};


#define MTU_SMALL 128
#define MTU_MEDIUM 256
#define MTU_BIG 512

enum COM_NetIf_Status{
	NetIfActive,
	NetIfUnInitialized
};

struct COM_LWIP_DRIVER_CONFIG
{
    //SPI_CONFIGURATION   SPI_Config;
    int             comPort;
    COM_NetIf_Status ifStatus;
    struct netif* pnetif;
};


#ifndef NETWORK_INTERFACE_COUNT
#define NETWORK_INTERFACE_COUNT 1
#endif

struct COM_LWIP_DEVICE_CONFIG
{
    COM_LWIP_DRIVER_CONFIG DeviceConfigs[NETWORK_INTERFACE_COUNT];

    static LPCSTR GetDriverName() { return "COM_LWIP"; }
};

struct COM_LWIP_Driver
{
    static int  Open   ( COM_LWIP_DRIVER_CONFIG* config, int index );
    static BOOL Close  ( COM_LWIP_DRIVER_CONFIG* config, int index );
    static BOOL Bind   ( COM_LWIP_DRIVER_CONFIG* config, int index );
};

#endif /* _COM_LWIP_DRIVER_H_ */
