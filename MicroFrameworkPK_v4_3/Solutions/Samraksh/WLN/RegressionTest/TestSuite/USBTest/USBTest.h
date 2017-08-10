#include <tinyhal.h>
//#include "../../Log/Log.h"
#include <usb/netmf_usb.h>

#ifndef _USB_TEST_
#define _USB_TEST_  1

#define BUFFER_SIZE    8
#define BIG_BUFFER_SIZE 1024
#define MAX_EVENTS 100
class USBTest
{
    char    m_xmitBuffer[BUFFER_SIZE];    // for out-bound messages
    char    m_recvBuffer[BUFFER_SIZE];    // for in-bound messages
    char    bigBuffer[BIG_BUFFER_SIZE];
    int numberOfEvents;
    uint16_t randomBuffer[MAX_EVENTS];
    int m_tx_ep;
    int m_rx_ep;
    //int usbTestStream;
    int usbControler;
    int usbPort;
public:

            USBTest (int numberOfEvents, int tx_ep, int rx_ep );
            BOOL    Execute               ( );
            void    InitializeXmitBuffer  ( );
            BOOL    Validate              ( );
            void 	CloseStreams		  ( );

};

#endif
