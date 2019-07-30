
#ifndef __COM_H__
#define __COM_H__

#include <tinyhal.h>
#include "com_lwip_driver.h"

bool        com_lwip_open( struct netif *pNetIF );
void        com_lwip_close ( struct netif *pNetIF  );
err_t       com_lwip_xmit ( struct netif *pNetIF , struct pbuf *pPBuf );
int         com_lwip_recv ( struct netif *pNetIF, UINT16 length );
void        com_lwip_interrupt ( struct netif *pNetIF );
void        com_lwip_pre_interrupt (GPIO_PIN Pin, BOOL PinState, void *pArg );
void        com_lwip_setup_recv_buffer( struct netif *pNetIF, SPI_CONFIGURATION  *SpiConf);


/*          FUNCTION PROTOTYPE                                          */


/*          STRUCTURES                                                  */

#define COM_IF_MAX_SIZE  1024   /* maximum number of bytes in ETHERNET packet */
                                /* (used by ethernet drivers)   */

#define COM_IF_MIN_LEN    64  /* minimum number of bytes in an ETHERNET */
                             /* packet   */


/*              COM CONFIGURATION                                  */
#define     CFG_NUM_COM            NETWORK_INTERFACE_COUNT
#define     CFG_MAX_PACKETS_PROCESSED   10          /* The maximum number of packets to process in one
                                                        shot */

#define     COM_FULL_DUPLEX        0           /* DON'T CHANGE adds support for full duplex */
#define     COM_MAXIMUM_FRAME_SIZE 1530        /* maximum frame sizes to be transmitted */
                                    
#define     COM_MAC_ADDRESS0       0xBA        /* Configures the MAC address to */
#define     COM_MAC_ADDRESS1       0xAD        /* these values */
#define     COM_MAC_ADDRESS2       0xCA
#define     COM_MAC_ADDRESS3       0xAB
#define     COM_MAC_ADDRESS4       0x04
#define     COM_MAC_ADDRESS5       0x05

#define     COM_PHYID1              0x0083      /* Unique device id */
#define     COM_PHYID2              0x1400      /* Unique device id */

        




/*              COM control register banks                         */
#define COM_CONTROL_REGISTER_BANK0     0
#define COM_CONTROL_REGISTER_BANK1     1
#define COM_CONTROL_REGISTER_BANK2     2
#define COM_CONTROL_REGISTER_BANK3     3

/*              COM control register map                           */

#define COM_ERDPTL             0x00
                                    
#define COM_EHT0               0x00
                                    
#define COM_MACON1             0x00
#define COM_MACON1_LOOPBK_BIT  4
#define COM_MACON1_TXPAUS_BIT  3
#define COM_MACON1_RXPAUS_BIT  2
#define COM_MACON1_PASSALL_BIT 1
#define COM_MACON1_MARXEN_BIT  0
                                    
#define COM_MAADR2             0x05
                                    
#define COM_ERDPTH             0x01
                                    
#define COM_EHT1               0x01
                                    
#define COM_MACON2             0x01
#define COM_MACON2_MARST_BIT   7
#define COM_MACON2_RNDRST_BIT  6
#define COM_MACON2_MARXRST_BIT 3
#define COM_MACON2_RFUNRST_BIT 2
#define COM_MACON2_MATXRST_BIT 1
#define COM_MACON2_TFUNRST_BIT 0

#define COM_MAADR1             0x04
                                    
#define COM_EWRPTL             0x02
                                    
#define COM_EHT2               0x02
                                    
#define COM_MACON3             0x02
#define COM_MACON3_PADCFG2_BIT 7
#define COM_MACON3_PADCFG1_BIT 6
#define COM_MACON3_PADCFG0_BIT 5
#define COM_MACON3_TXCRCEN_BIT 4
#define COM_MACON3_PHDRLEN_BIT 3
#define COM_MACON3_HFRMEN_BIT  2
#define COM_MACON3_FRMLNEN_BIT 1
#define COM_MACON3_FULDPX_BIT  0
                                  
#define COM_MAADR4             0x03
                                    
#define COM_EWRPTH             0x03
                                    
#define COM_EHT3               0x03
                                    
#define COM_MACON4             0x03
#define COM_MACON4_DEFER_BIT   6
#define COM_MACON4_BPEN_BIT    5
#define COM_MACON4_NOBKOFF_BIT 4
#define COM_MACON4_LONGPRE_BIT 1
#define COM_MACON4_PUREPRE_BIT 0
                                    
#define COM_MAADR3             0x02
                                    
#define COM_ETXSTL             0x04
                                    
#define COM_EHT4               0x04
                                    
#define COM_MABBIPG            0x04
                                    
#define COM_MAADR6             0x01
                                    
#define COM_ETXSTH             0x05
                                    
#define COM_EHT5               0x05
                                    
#define COM_MAADR5             0x00
                                    
#define COM_ETXNDL             0x06
                                    
#define COM_EHT6               0x06
                                    
#define COM_MAIPGL             0x06
                                    
#define COM_EBSTSD             0x06
                                    
#define COM_ETXNDH             0x07
                                    
#define COM_EHT7               0x07
                                    
#define COM_MAIPGH             0x07
                                    
#define COM_EBSTCON            0x07
                                    
#define COM_ERXSTL             0x08

#define COM_EPMM0              0x08
                                    
#define COM_MACLCON1           0x08
                                    
#define COM_EBSTCSL            0x08
                                    
#define COM_ERXSTH             0x09
                                    
#define COM_EPMM1              0x09
                                    
#define COM_MACLCON2           0x09
                                    
#define COM_EBSTCSH            0x09
                                    
#define COM_ERXNDL             0x0A
                                    
#define COM_EPMM2              0x0A
                                    
#define COM_MAMXFLL            0x0A
                                    
#define COM_MISTAT             0x0A
#define COM_MISTAT_NVALID_BIT  2
#define COM_MISTAT_SCAN_BIT    1
#define COM_MISTAT_BUSY_BIT    0
                                     
#define COM_ERXNDH             0x0B
                                    
#define COM_EPMM3              0x0B
                                    
#define COM_MAMXFLH            0x0B
                                    
#define COM_ERXRDPTL           0x0C
                                    
#define COM_EPMM4              0x0C
                                    
#define COM_ERXRDPTH           0x0D
                                    
#define COM_EPMM5              0x0D
                                    
#define COM_MAPHSUP            0x0D
                                    
#define COM_ERXWRPTL           0x0E
                                    
#define COM_EPMM6              0x0E
                                    
#define COM_ERXWRPTH           0x0F
                                    
#define COM_EPMM7              0x0F
                                    
#define COM_EDMASTL            0x10
                                    
#define COM_EPMCSL             0x10
                                    
#define COM_EDMASTH            0x11
                                    
#define COM_EPMCSH             0x11
                                    
#define COM_MICON              0x11
                                    
#define COM_EDMANDL            0x12
                                    
#define COM_MICMD              0x12
#define COM_MICMD_MISCAN_BIT   1
#define COM_MICMD_MIIRD_BIT    0
                                   
#define COM_EREVID             0x12
                                    
#define COM_EDMANDH            0x13
                                    
#define COM_EDMADSTL           0x14
                                    
#define COM_EPMOL              0x14
                                    
#define COM_MIREGADR           0x14
                                    
#define COM_EDMADSTH           0x15
                                    
#define COM_EPMOH              0x15
                                    
#define COM_ECOCON             0x15
                                    
#define COM_EDMACSL            0x16
                                    
#define COM_EWOLIE             0x16
                                    
#define COM_MIWRL              0x16
                                    
#define COM_EDMACSH            0x17
                                    
#define COM_EWOLIR             0x17
                                    
#define COM_MIWRH              0x17
                                    
#define COM_EFLOCON            0x17
                                    
#define COM_ERXFCON            0x18
#define COM_ERXFCON_UCEN_BIT   7
#define COM_ERXFCON_ANDOR_BIT  6
#define COM_ERXFCON_CRCEN_BIT  5
#define COM_ERXFCON_PMEN_BIT   4
#define COM_ERXFCON_MPEN_BIT   3
#define COM_ERXFCON_HTEN_BIT   2
#define COM_ERXFCON_MCEN_BIT   1
#define COM_ERXFCON_BCEN_BIT   0
                                    
#define COM_MIRDL              0x18
                                    
#define COM_EPAUSL             0x18
                                    
#define COM_EPKTCNT            0x19
                                    
#define COM_MIRDH              0x19
                                    
#define COM_EPAUSH             0x19
                                    
#define COM_EIE                0x1B
#define COM_EIE_INTIE_BIT      7
#define COM_EIE_PKTIE_BIT      6
#define COM_EIE_DMAIE_BIT      5
#define COM_EIE_LINKIE_BIT     4
#define COM_EIE_TXIE_BIT       3
#define COM_EIE_WOLIE_BIT      2
#define COM_EIE_TXERIE_BIT     1
#define COM_EIE_RXERIE_BIT     0
                                    
#define COM_EIR                0x1C
#define COM_EIR_PKTIF_BIT      6
#define COM_EIR_DMAIF_BIT      5
#define COM_EIR_LINKIF_BIT     4
#define COM_EIR_TXIF_BIT       3
#define COM_EIR_WOLIF_BIT      2
#define COM_EIR_TXERIF_BIT     1
#define COM_EIR_RXERIF_BIT     0

#define COM_ESTAT              0x1D
#define COM_ESTAT_INT          7
#define COM_ESTAT_BUFER        6
#define COM_ESTAT_LATECOL      4
#define COM_ESTAT_RXBUSY       2
#define COM_ESTAT_TXABRT       1
#define COM_ESTAT_CLKRDY       0
                                    
#define COM_ECON2              0x1E
#define COM_ECON2_AUTOINC_BIT  7
#define COM_ECON2_PKTDEC_BIT   6
#define COM_ECON2_PWRSV_BIT    5
#define COM_ECON2_VRPS_BIT     4
    
#define COM_ECON1              0x1F
#define COM_ECON1_TXRST_BIT    7
#define COM_ECON1_RXRST_BIT    6
#define COM_ECON1_DMAST_BIT    5
#define COM_ECON1_CSUMEN_BIT   4
#define COM_ECON1_TXRTS_BIT    3
#define COM_ECON1_RXEN_BIT     2
#define COM_ECON1_BSEL1_BIT    1
#define COM_ECON1_BSEL0_BIT    0

#define COM_PHCON1             0x0
#define COM_PHCON1_PRST        15
#define COM_PHCON1_PLOOPBK     14
#define COM_PHCON1_PPWRSV      11
#define COM_PHCON1_PDPXMD      8

#define COM_PHSTAT1            0x1

#define COM_PHID1              0x2

#define COM_PHID2              0x3

#define COM_PHCON2             0x10
#define COM_PHCON2_FRCLNK      14
#define COM_PHCON2_TXDIS       13
#define COM_PHCON2_JABBER      10
#define COM_PHCON2_HDLDIS      8

#define COM_PHSTAT2            0x11
#define COM_PHSTAT2_LSTAT_BIT    10

#define COM_PHIE               0x12
#define COM_PHIR               0x13
#define COM_PHLCON             0x14
/*                  SPI Instruction set                                 */
#define COM_SPI_READ_CONTROL_REGISTER_OPCODE       0x0
            
#define COM_SPI_READ_BUFFER_MEMORY_OPCODE          0x1
#define COM_SPI_READ_BUFFER_MEMORY_ARGUMENT        0x1A
            
#define COM_SPI_WRITE_CONTROL_REGISTER_OPCODE      0x2
            
#define COM_SPI_WRITE_BUFFER_MEMORY_OPCODE         0x3
#define COM_SPI_WRITE_BUFFER_MEMORY_ARGUMENT       0x1A
            
#define COM_SPI_BIT_FIELD_SET_OPCODE               0x4
            
#define COM_SPI_BIT_FIELD_CLEAR_OPCODE             0x5
            
#define COM_SPI_SYSTEM_COMMAND_SOFT_RESET_OPCODE   0x7
#define COM_SPI_SYSTEM_COMMAND_SOFT_RESET_ARGUMENT 0x1F

#define COM_SPI_OPCODE_ARGUMENT(OP, ARG)           (OP << 5) | (ARG &0x1F)

/*                  XMIT format for per packet control                  */

#define COM_XMIT_CONTROL_PHUGEEN_BIT           3
#define COM_XMIT_CONTROL_PPADEN_BIT            2
#define COM_XMIT_CONTROL_PCRCEN_BIT            1
#define COM_XMIT_CONTROL_POVERRIDE_BIT         0

#endif  /* #ifndef __COM_H__   */
