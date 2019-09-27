//
// usart/com based network driver
//

#include <tinyhal.h>
#include "net_decl_lwip.h"


#include "lwip\netif.h"
#include "lwip\pbuf.h"
#include "lwip\mem.h"

#include <USART_decl.h>
#include "mac_lwip.h"

extern void lwip_interrupt_continuation( void );

//implemented in usart.cpp in Pal
extern int Com_Netif_Read( int ComPortNum, char* Data, size_t size );
extern int Com_Netif_BytesInBuffer(int ComPortNum);
extern UINT16 CheckForNetIfPkt(int ComPortNum);

/* ********************************************************************
   GLOBAL DATA
   ******************************************************************** */
   
extern  NETWORK_CONFIG                   g_NetworkConfig;
extern  COM_LWIP_DEVICE_CONFIG      g_COM_LWIP_Config;
extern HAL_CONTINUATION    InterruptTaskContinuation;
UINT8 interruptPending=0;
char pUsartRcvContext[128];

/* Function Prototypes */

//void    com_lwip_soft_reset(int ComPortNum);

/*
void    com_lwip_write_com(int ComPortNum,
                                    UINT8 opcode, 
                                    UINT8 address, 
                                    UINT8 perPacketCtrlByte,
                                    UINT8 *byteData, 
                                    UINT32 numBytes);
*/

/*void    com_lwip_write_com(int ComPortNum,
                                    UINT8 opcode, 
                                    UINT8 address, 
                                    UINT8 byteData);

void    com_lwip_read_com(int ComPortNum,
                                   UINT8 opcode, 
                                   UINT8 address, 
                                   UINT8 *byteData, 
                                   UINT32 numBytes,
                                   UINT8 offset);

*/
bool    com_lwip_setup_device( struct netif *pNetIF );

//void    com_lwip_select_bank(int ComPortNum,
//                                       UINT8 bankNumber);


/*
void    com_lwip_write_phy_register(int ComPortNum,
                                              UINT8 registerAddress, 
                                              unsigned short data);
                                


unsigned 
short   com_lwip_read_phy_register(int ComPortNum,
                                              UINT8 registerAddress);
*/


// keep track of the receive pointer
//static unsigned short s_COM_TRANSMIT_BUFFER_START = COM_TRANSMIT_BUFFER_START;
//static unsigned short s_COM_RECEIVE_BUFFER_START  = COM_RECEIVE_BUFFER_START;


/* ********************************************************************
   open the com driver interface.
   
   This routine opens a com device driver
  
   The address of this function must be placed into the "devices" table in
   iface.c either at compile time or before a device open is called.
  
  ******************************************************************** */  
bool com_lwip_open( struct netif *pNetIF )
    
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();
    return com_lwip_setup_device( pNetIF );
}

/* ********************************************************************
   close the packet driver interface.
   
   This routine is called when the device interface is no longer needed
   it should stop the driver from delivering packets to the upper levels
   and shut off packet delivery to the network.
  
   The address of this function must be placed into the "devices" table in
   iface.c either at compile time or before a device open is called.
  
 */  
void com_lwip_close( struct netif *pNetIF )
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();

    int  comPort = g_COM_LWIP_Config.DeviceConfigs[0].comPort;

    g_COM_LWIP_Config.DeviceConfigs[0].ifStatus = NetIfUnInitialized ;
    //com_lwip_write_com(comPort, COM_SPI_BIT_FIELD_CLEAR_OPCODE, COM_EIE, (UINT8)((1 << COM_EIE_INTIE_BIT) | (1 << COM_EIE_PKTIE_BIT) | (1 << COM_EIE_TXIE_BIT) |(1 << COM_EIE_TXERIE_BIT)));
}

/* ********************************************************************
   Interrupt handling routine at the task level
   ******************************************************************** */
#define TRANSMIT_RETRIES 5
static UINT8 s_retriesTransmit = TRANSMIT_RETRIES;
#define RECEIVE_RETRIES 5
static UINT8 s_receiveRetries = 10;

//Signal from usart driver that a Net packet has arrived.
//beware, this is called from interrupt handler in usart pal layer in interrupt mode.
//just enque a Task continuation and getout.
void pfnUsartEventHandler (void* context, unsigned int event){
	if(!interruptPending){
		InterruptTaskContinuation.Enqueue();
	}
}

//Recv interrupt hanlder at the lwip task level, enqueued from above
void com_lwip_interrupt(struct netif *pNetIF ){
	//COM_LWIP_DRIVER_CONFIG  *g_COM_driver_Config = (COM_LWIP_DRIVER_CONFIG*)context;
	interruptPending=0;
	UINT16 size = CheckForNetIfPkt(g_COM_LWIP_Config.DeviceConfigs[0].comPort);
	if(size>0){
		hal_printf("com_lwip_interrupt: Reviced a net pkt, going to read \r\n");
		com_lwip_recv(g_COM_LWIP_Config.DeviceConfigs[0].pnetif, size);
	}
}

BOOL com_get_link_status(COM_LWIP_DRIVER_CONFIG  *g_COM_driver_Config)
{
	//hal_printf("com_get_link_status: \r\n");
    return (g_COM_driver_Config->ifStatus==NetIfActive);
}

bool com_lwip_setup_device( struct netif *pNetIF )
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();
    UINT8               byteData;
    UINT16              duplex;
    //UINT16              phyID1;
    //UINT16              phyID2;
    UINT16              shortData;
    INT16               nLoopCnt = 0;

    int comPort;

    comPort = g_COM_LWIP_Config.DeviceConfigs[0].comPort;


    /* ---------------------------------------------------------------------------------------------------- */
    //                                          CHECK IF THE PHY IS READY

    //We expect the debug com_port to be already initialized. otherwise give an error. We dont want to
    //initialize debug port with different values
    if(!USART_IsComPortInitialized(comPort)){
    	if(!USART_Initialize(comPort, NETIF_COM_BAUD, NETIF_COM_PARITY, NETIF_COM_DATABITS, NETIF_COM_STOPBITS, NETIF_COM_FLOW ))
    	{
    		hal_printf("Error initializing com port %d for network interface", ConvertCOM_ComPort(comPort));return FALSE;
    	}
    }

    //say hello to the comport
    /*USART_Write(comPort,"HELLO OLLEH",11);USART_Write(comPort,"\n\r",2);
    USART_Write(comPort,"HELLO OLLEH",11);USART_Write(comPort,"\n\r",2);
    USART_Write(comPort,"HELLO OLLEH",11);USART_Write(comPort,"\n\r",2);
    USART_Write(comPort,"HELLO OLLEH",11);USART_Write(comPort,"\n\r",2);
	*/

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                                  SOFT RESET                                          */

    //com_lwip_soft_reset(comPort);

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                              VERIFY THE DEVICE ID                                    */


    //Nothing to verify, we are going to just assume the given comport has the right ID
    //COM_PHYID1 COM_PHYID2

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          SETUP RECEIVE  BUFFER                                       */

    //com_lwip_setup_recv_buffer( pNetIF, comPort );
    //Nothing to be done here.
    USART_ConnectEventSink( comPort,USART_EVENT_TYPE_DATA,  (void*)pUsartRcvContext, &pfnUsartEventHandler, NULL );
    //USART_ConnectEventSink( comPort,USART_EVENT_DATA_NETIF,  (void*)pUsartRcvContext, &pfnUsartEventHandler, NULL );

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          SETUP RECEIVE FILTER                                       */



    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          MAC INITIALIZATION SETTINGS                                 */

    //Nothing to do

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                   Initialize buffers                                                                   */

    //s_COM_TRANSMIT_BUFFER_START = COM_TRANSMIT_BUFFER_START;
    //s_COM_RECEIVE_BUFFER_START  = COM_RECEIVE_BUFFER_START;
    g_COM_LWIP_Config.DeviceConfigs[0].pnetif= pNetIF;

    return TRUE;
}

/* ********************************************************************
   Receive. a packet over the packet driver interface.  This is called
   from the ISR or task ISR.

   This routine is called when a packet is received.

   Upon successful reading of received packet, the IP is
   signalled to process the packet.

   Returns the number of packets that remain to be processed

  ******************************************************************** */
int com_lwip_recv(struct netif *pNetIF , UINT16 length)
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();

    struct pbuf        *pPBuf;
    char              *dataRX;
    //UINT8               nextPktAndRecvStatusVector[6];
    //UINT16              length;
    //UINT8               byteData;
    //UINT8               packetsLeft=0;
    //UINT16              lastReceiveBuffer;
    //int                     numPacketsProcessed = 0;

    if ( !pNetIF )
    {
        return 1;
    }

    int comPort= g_COM_LWIP_Config.DeviceConfigs[0].comPort;

    do
    {
		//length=USART_BytesInBuffer( comPort, TRUE );
		//length=Com_Netif_BytesInBuffer(comPort);
		if (length > 0)
		{
			pPBuf = pbuf_alloc( PBUF_RAW, length, PBUF_RAM );

			if ( pPBuf )
			{
				dataRX = (char *)pPBuf->payload;
				//int readBytes= USART_Read(comPort, dataRX, length );
				int readBytes= Com_Netif_Read(comPort, dataRX, length );

				length -=readBytes;
				// invoke stack ip input - the stack should free the buffer when it is done,
				// so DON'T call pbuf_free on pPBuf!!!!!
				pNetIF->input( pPBuf, pNetIF );
			}
			else
			{
				hal_printf("com_lwip_recv: input alloc packet failed \r\n");
			}
		}
		else // no packets left - should we break??
		{
		}

    } while (length);

    return 0; //not sure how to find packets left in case of usart
}

