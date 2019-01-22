//
// usart/com based network driver
//

#include <tinyhal.h>
#include "net_decl_lwip.h"


#include "lwip\netif.h"
#include "lwip\pbuf.h"
#include "lwip\mem.h"

#include <USART_decl.h>

extern void lwip_interrupt_continuation( void );


/* ********************************************************************
   GLOBAL DATA
   ******************************************************************** */
   
extern  NETWORK_CONFIG                   g_NetworkConfig;
extern  COM_LWIP_DEVICE_CONFIG      g_COM_LWIP_Config;


/* Function Prototypes */

void    com_lwip_soft_reset(int ComPortNum);

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
static unsigned short s_com_TRANSMIT_BUFFER_START = com_TRANSMIT_BUFFER_START;
static unsigned short s_com_RECEIVE_BUFFER_START  = com_RECEIVE_BUFFER_START;


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

    SPI_CONFIGURATION*  comPort = &g_COM_LWIP_Config.DeviceConfigs[0].comPort;

    com_lwip_write_com(comPort, com_SPI_BIT_FIELD_CLEAR_OPCODE, com_EIE, (UINT8)((1 << com_EIE_INTIE_BIT) | (1 << com_EIE_PKTIE_BIT) | (1 << com_EIE_TXIE_BIT) |(1 << com_EIE_TXERIE_BIT)));
}

/* ********************************************************************
   Interrupt handling routine at the task level
   ******************************************************************** */
#define TRANSMIT_RETRIES 5
static UINT8 s_retriesTransmit = TRANSMIT_RETRIES;
#define RECEIVE_RETRIES 5
static UINT8 s_receiveRetries = 10;


BOOL com_get_link_status(SPI_CONFIGURATION* spiConf)
{
    GLOBAL_LOCK(irq);

    UINT16 phyStat = com_lwip_read_phy_register(spiConf, com_PHSTAT2);

    // linkstatus bit
    return (0 != (phyStat & (1ul << com_PHSTAT2_LSTAT_BIT)));
}

bool com_lwip_setup_device( struct netif *pNetIF )
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();
    UINT8               byteData;
    UINT16              duplex;
    UINT16              phyID1;
    UINT16              phyID2;
    UINT16              shortData;
    INT16               nLoopCnt = 0;

    int comPort;

    comPort = &g_COM_LWIP_Config.DeviceConfigs[0].comPort;


    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          CHECK IF THE PHY IS READY                                   */

    //We expect the debug com_port to be already initialized. otherwise give an error. We dont want to
    //initialize debug port with different values
    if(!USART_IsComPortInitialized(comPort)){
    	return FALSE;
    }
    /* ---------------------------------------------------------------------------------------------------- */
    /*                                                  SOFT RESET                                          */

    com_lwip_soft_reset(comPort);

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                              VERIFY THE DEVICE ID                                    */

    phyID1 = com_lwip_read_phy_register(comPort, com_PHID1);
    phyID2 = com_lwip_read_phy_register(comPort, com_PHID2);

    if ( (phyID1 != com_PHYID1) || (phyID2 != com_PHYID2))
    {
        hal_printf("com_lwip_open: Wrong Device");
    }

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          SETUP RECEIVE  BUFFER                                       */

    com_lwip_setup_recv_buffer( pNetIF, comPort );

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          SETUP RECEIVE FILTER                                       */

    /* Making sure to select the right bank */
    com_lwip_select_bank(comPort, com_CONTROL_REGISTER_BANK1);
    /* Disable packet filtering (Promiscuous mode) */
    byteData = 0;
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_ERXFCON, byteData);



    /* ---------------------------------------------------------------------------------------------------- */
    /*                                          MAC INITIALIZATION SETTINGS                                 */

    /* Making sure to select the right bank */
    com_lwip_select_bank(comPort, com_CONTROL_REGISTER_BANK2);

    /* Clear the MARST bit in MACON2 */
    //NEW MANUAL DOESN'T DO THIS
    //com_lwip_read_com(comPort, com_SPI_READ_CONTROL_REGISTER_OPCODE, com_MACON2, &byteData, 1, 1);
    //byteData &= ~(1 << com_MACON2_MARST_BIT);
    //com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MACON2, byteData);

    /* Making sure to select the right bank */
    com_lwip_select_bank(comPort, com_CONTROL_REGISTER_BANK2);

    /* Set the MARXEN bit in MACON1 */
    com_lwip_read_com(comPort, com_SPI_READ_CONTROL_REGISTER_OPCODE, com_MACON1, &byteData, 1, 1);
    byteData = (1 << com_MACON1_MARXEN_BIT);
#if (com_FULL_DUPLEX)
    byteData |= (1 << com_MACON1_TXPAUS_BIT);
    byteData |= (1 << com_MACON1_RXPAUS_BIT);
#endif

    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MACON1, byteData);

    /* Configure the PADCFT, TXCRECEN, AND FULLDPS bits
        of MACON3 */

    byteData = (5 << com_MACON3_PADCFG0_BIT) | com_MACON3_TXCRCEN_BIT;
    //(1 << com_MACON3_TXCRCEN_BIT);
#if (com_FULL_DUPLEX)
    byteData |= (1 << com_MACON3_FULDPX_BIT);
#endif
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MACON3, byteData);

    byteData = 0;
    /* Configure the bits in MACON4     */
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MACON4, byteData);

    /* Configure the MAMXFL registers */
    byteData = com_MAXIMUM_FRAME_SIZE & 0xFF;
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAMXFLL, byteData);
    byteData = (com_MAXIMUM_FRAME_SIZE >> 8) & 0xFF;
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAMXFLH, byteData);

    /* Configure the Back-to-back inter-packet gap */
#if (com_FULL_DUPLEX)
    byteData = 0x15;
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MABBIPG, byteData);
#else
    byteData = 0x12;
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MABBIPG, byteData);
#endif

    /* Configure the non-back-to-back inter-packet gap */
    byteData = 0x12;
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAIPGL, byteData);

#if (!com_FULL_DUPLEX)
    byteData = 0xC;
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAIPGH, byteData);
#endif

    /* Making sure to select the right bank */
    com_lwip_select_bank(comPort, com_CONTROL_REGISTER_BANK3);

    /* Setup the MAC address */
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAADR1, pNetIF->hwaddr[0]);
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAADR2, pNetIF->hwaddr[1]);
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAADR3, pNetIF->hwaddr[2]);
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAADR4, pNetIF->hwaddr[3]);
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAADR5, pNetIF->hwaddr[4]);
    com_lwip_write_com(comPort,  com_SPI_WRITE_CONTROL_REGISTER_OPCODE,com_MAADR6, pNetIF->hwaddr[5]);

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                  CHECK DUPLEX MODE                                                   */
    duplex = com_lwip_read_phy_register(comPort, com_PHCON1);

    com_lwip_select_bank(comPort, com_CONTROL_REGISTER_BANK2);
    com_lwip_read_com(comPort, com_SPI_READ_CONTROL_REGISTER_OPCODE, com_MACON3, &byteData, 1, 1);

    if ( ((duplex >> com_PHCON1_PDPXMD) & 1) !=
         ((byteData >> com_MACON3_FULDPX_BIT) & 1)
       )
    {
        hal_printf("com_lwip_open: check duplex setting \r\n");
    }

    /* ---------------------------------------------------------------------------------------------------- */
    /*                                                                                                      */

    /* The first time the ethernet is initialized, let us
        point the ETXST pointer to the beginning of the
        buffer space. Subsequent increments of this register
        will be done in the transmit function */

    s_com_TRANSMIT_BUFFER_START = com_TRANSMIT_BUFFER_START;
    s_com_RECEIVE_BUFFER_START  = com_RECEIVE_BUFFER_START;

    /* enabling reception */

    /* Making sure to select the right bank */
    com_lwip_select_bank(comPort, com_CONTROL_REGISTER_BANK0);

    /* setup the ERDPTL AND ERDPTH to the beginning of the receive buffer */
    byteData = com_RECEIVE_BUFFER_START & 0xFF;
    com_lwip_write_com(comPort, com_SPI_WRITE_CONTROL_REGISTER_OPCODE, com_ERDPTL, byteData);
    byteData = (com_RECEIVE_BUFFER_START >> 8) & 0xFF;
    com_lwip_write_com(comPort, com_SPI_WRITE_CONTROL_REGISTER_OPCODE, com_ERDPTH, byteData);

    /* -----------------------------------DISABLE LOOPBACK FOR HALF DUPLEX--------------------------------- */
    /*                                                                                                      */
#if (!com_FULL_DUPLEX)
    shortData = com_lwip_read_phy_register(comPort, com_PHCON2);
    shortData = 1 << com_PHCON2_HDLDIS;
    com_lwip_write_phy_register(comPort, com_PHCON2, shortData);
    shortData = com_lwip_read_phy_register(comPort, com_PHCON2);
#endif
    /* -----------------------------------DISABLE PHY INTERRUPTS ------------------------------------------ */
    /*                                                                                                      */
    shortData = 0x0;
    com_lwip_write_phy_register(comPort, com_PHIE, shortData);

    /* -------------------------------------- START THE DEVICE -------------------------------------------- */
    /*                                                                                                      */
    /* enable interrupts when a packet is received and when transmit is done */
    com_lwip_write_com(comPort, com_SPI_BIT_FIELD_SET_OPCODE, com_EIE, (UINT8)((1 << com_EIE_INTIE_BIT) | (1 << com_EIE_PKTIE_BIT) | (1 << com_EIE_TXERIE_BIT)));

    /* enable auto- increment */
    com_lwip_write_com(comPort, com_SPI_BIT_FIELD_SET_OPCODE, com_ECON2, (UINT8)(1 << com_ECON2_AUTOINC_BIT));

    /* enable packet reception */
    com_lwip_write_com(comPort, com_SPI_BIT_FIELD_SET_OPCODE, com_ECON1, (UINT8)(1 << com_ECON1_RXEN_BIT));

    return TRUE;
}

