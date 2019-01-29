//
// usart/com based network driver
//

#include <tinyhal.h>
#include "net_decl_lwip.h"


#include "lwip\netif.h"
#include "lwip\pbuf.h"
#include "lwip\mem.h"

#include <USART_decl.h>
#include "com_lwip.h"

extern void lwip_interrupt_continuation( void );


/* ********************************************************************
   GLOBAL DATA
   ******************************************************************** */
   
extern  NETWORK_CONFIG                   g_NetworkConfig;
extern  COM_LWIP_DEVICE_CONFIG      g_COM_LWIP_Config;
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

void    com_lwip_write_com(int ComPortNum,
                                    UINT8 opcode, 
                                    UINT8 address, 
                                    UINT8 byteData);

void    com_lwip_read_com(int ComPortNum,
                                   UINT8 opcode, 
                                   UINT8 address, 
                                   UINT8 *byteData, 
                                   UINT32 numBytes,
                                   UINT8 offset);


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


void pfnUsartEventHandler (void* context, unsigned int event){

}


BOOL com_get_link_status(COM_LWIP_DRIVER_CONFIG  *g_COM_driver_Config)
{
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
    	return FALSE;
    }
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

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          SETUP RECEIVE FILTER                                       */



    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          MAC INITIALIZATION SETTINGS                                 */

    //Nothing to do

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                   Initialize buffers                                                                   */

    //s_COM_TRANSMIT_BUFFER_START = COM_TRANSMIT_BUFFER_START;
    //s_COM_RECEIVE_BUFFER_START  = COM_RECEIVE_BUFFER_START;

    return TRUE;
}

