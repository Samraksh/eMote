#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "com_lwip_driver.h"
#include "com_lwip.h"
#include <USART_decl.h>

extern "C"
{
//#include "netif\etharp.h"
//#include "lwip\dhcp.h"
#include "lwip\tcpip.h"
//#include "lwip\dns.h"
#include "lwip\ip_addr.h"
}

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_COM_LWIP_Driver"
#endif

static COM_LWIP_Driver g_COM_LWIP_Driver;
static struct netif         g_COM_NetIF;

HAL_CONTINUATION    InterruptTaskContinuation;
HAL_COMPLETION      LwipUpTimeCompletion;
static BOOL         LwipNetworkStatus = FALSE;
static UINT32       LwipLastIpAddress = 0;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

extern COM_LWIP_DEVICE_CONFIG   g_COM_LWIP_Config;
extern NETWORK_CONFIG                g_NetworkConfig;

extern BOOL COM_get_link_status(int comPort);




//This is called by the IP stack to send a packet on the net ifterface
err_t COM_netif_output(struct netif *pNetIF, struct pbuf *pPBuf,
       ip_addr_t *ipaddr){

    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();
    UINT16                  length = 0;
    //UINT8                   perPacketControlByte;
    //UINT8                   dataByte;
    //SPI_CONFIGURATION*      SpiConf;
    //struct pbuf*            pTmp;
    //UINT8*                  pTx;
    //int                     idx;
    //int                     retries = 100;


    if ( !pNetIF )
    {
        return ERR_ARG;
    }

    int  comPort = g_COM_LWIP_Config.DeviceConfigs[0].comPort;
    length = pPBuf->tot_len;

    if (length > COM_IF_MAX_SIZE) // (COM_IF_MAX_SIZE+4))
    {
        debug_printf("xmit - length is too large, truncated \r\n" );
        HAL_ASSERT(FALSE);
        length = COM_IF_MAX_SIZE; // COM_IF_MAX_SIZE+4;         /* what a terriable hack! */
    }//

    //idx = 2;
    //write the startbytes
    int x;
    x=USART_Write(comPort, START_STOP_BYTES, START_STOP_BYTES_SIZE);
    if(x!=START_STOP_BYTES_SIZE){HAL_ASSERT(FALSE);}
    while(pPBuf)
    {
    	x=USART_Write(comPort,(const char*)pPBuf->payload, pPBuf->len);
    	if(x!=pPBuf->len){HAL_ASSERT(FALSE);}
        //memcpy(&pTx[idx], pPBuf->payload, pPBuf->len);
        //idx += pPBuf->len;
        pPBuf = pPBuf->next;
    }
    //write the stopbytes
    x=USART_Write(comPort, START_STOP_BYTES, START_STOP_BYTES_SIZE);
    if(x!=START_STOP_BYTES_SIZE){HAL_ASSERT(FALSE);}
    pbuf_free(pTmp);

    return ERR_OK;
}

err_t COM_netif_linkoutput(struct netif *netif, struct pbuf *p){

}

//this function will be called when a packet comes in through the com port
err_t com_netif_input(struct pbuf *p, struct netif *inp){

}


void COM_status_callback(struct netif *netif)
{
    if(LwipLastIpAddress != netif->ip_addr.addr)
    {
        Network_PostEvent( NETWORK_EVENT_TYPE_ADDRESS_CHANGED, 0 );
        LwipLastIpAddress = netif->ip_addr.addr;
    }

#if !defined(BUILD_RTM)
    lcd_printf("\f\n\n\n\n\n\nLink Update: %s\n", (netif_is_up(netif) ? "UP  " : "DOWN") );
    lcd_printf("         IP: %d.%d.%d.%d\n", (netif->ip_addr.addr >>  0) & 0xFF,
                                             (netif->ip_addr.addr >>  8) & 0xFF,
                                             (netif->ip_addr.addr >> 16) & 0xFF,
                                             (netif->ip_addr.addr >> 24) & 0xFF);
    lcd_printf("         SM: %d.%d.%d.%d\n", (netif->netmask.addr >>  0) & 0xFF,
                                             (netif->netmask.addr >>  8) & 0xFF,
                                             (netif->netmask.addr >> 16) & 0xFF,
                                             (netif->netmask.addr >> 24) & 0xFF);
    lcd_printf("         GW: %d.%d.%d.%d\n", (netif->gw.addr >>  0) & 0xFF,
                                             (netif->gw.addr >>  8) & 0xFF,
                                             (netif->gw.addr >> 16) & 0xFF,
                                             (netif->gw.addr >> 24) & 0xFF);
    debug_printf("IP Address: %d.%d.%d.%d\n", (netif->ip_addr.addr >>  0) & 0xFF,
                                             (netif->ip_addr.addr >>  8) & 0xFF,
                                             (netif->ip_addr.addr >> 16) & 0xFF,
                                             (netif->ip_addr.addr >> 24) & 0xFF);
#if LWIP_DNS
    if(netif->flags & NETIF_FLAG_DHCP)
    {
        struct ip_addr dns1 = dns_getserver(0);
        struct ip_addr dns2 = dns_getserver(1);

        lcd_printf("         dns1: %d.%d.%d.%d\n",(dns1.addr >>  0) & 0xFF,
                                                  (dns1.addr >>  8) & 0xFF,
                                                  (dns1.addr >> 16) & 0xFF,
                                                  (dns1.addr >> 24) & 0xFF);

        lcd_printf("         dns2: %d.%d.%d.%d\n",(dns2.addr >>  0) & 0xFF,
                                                  (dns2.addr >>  8) & 0xFF,
                                                  (dns2.addr >> 16) & 0xFF,
                                                  (dns2.addr >> 24) & 0xFF);
    }
#endif
#endif
}

err_t   COM_netif_init( netif * myNetIf)
{
#if defined(NETWORK_MEMORY_PROFILE_LWIP__small)
    myNetIf->mtu = MTU_SMALL;
#elif defined (NETWORK_MEMORY_PROFILE_LWIP__medium)
    myNetIf->mtu = MTU_MEDIUM;
#else
    myNetIf->mtu = MTU_BIG;
#endif

    //myNetIf->flags = NETIF_FLAG_IGMP | NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;
    myNetIf->flags = NETIF_FLAG_BROADCAST;

    // ethhw_init() is user-defined
    // use ip_input instead of ethernet_input for non-ethernet hardware
    // (this function is assigned to netif.input and should be called by the hardware driver)

    // Assign the xmit routine to the stack's netif and call the driver's Open
    myNetIf->output = &COM_net_output;
    myNetIf->linkoutput = &COM_net_linkoutput;
    myNetIf->status_callback = COM_status_callback;

    com_lwip_open( myNetIf );

    return ERR_OK;
}


void lwip_interrupt_continuation( )
{
    NATIVE_PROFILE_PAL_NETWORK();
    GLOBAL_LOCK(irq);

    if(!InterruptTaskContinuation.IsLinked())
    {
        InterruptTaskContinuation.Enqueue();
    }
}

void lwip_network_uptime_completion(void *arg)
{
    NATIVE_PROFILE_PAL_NETWORK();

    struct netif* pNetIf = (struct netif*)arg;

    BOOL status = com_get_link_status(&g_COM_LWIP_Config.DeviceConfigs[0].SPI_Config);

    if(status != LwipNetworkStatus)
    {
        if(status)
        {
            tcpip_callback((sys_timeout_handler)netif_set_link_up, (void*)pNetIf);
            tcpip_callback((sys_timeout_handler)netif_set_up, (void*)pNetIf);

            Network_PostEvent( NETWORK_EVENT_TYPE__AVAILABILITY_CHANGED, NETWORK_EVENT_FLAGS_IS_AVAILABLE );
        }
        else
        {
            tcpip_callback((sys_timeout_handler)netif_set_link_down, (void*)pNetIf);
            tcpip_callback((sys_timeout_handler)netif_set_down, (void*)pNetIf);

            Network_PostEvent( NETWORK_EVENT_TYPE__AVAILABILITY_CHANGED, 0);
        }

        Events_Set(SYSTEM_EVENT_FLAG_SOCKET);
        Events_Set(SYSTEM_EVENT_FLAG_NETWORK);

        LwipNetworkStatus = status;
    }

    LwipUpTimeCompletion.EnqueueDelta64( 2000000 );
}

void InitContinuations( struct netif* pNetIf )
{
    InterruptTaskContinuation.InitializeCallback( (HAL_CALLBACK_FPN)COM_lwip_interrupt, &g_COM_NetIF );

    LwipUpTimeCompletion.InitializeForUserMode( (HAL_CALLBACK_FPN)lwip_network_uptime_completion, pNetIf );

    LwipUpTimeCompletion.EnqueueDelta64( 500000 );
}

BOOL Network_Interface_Bind(int index)
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();
    if(index >= ARRAYSIZE(g_COM_LWIP_Config.DeviceConfigs)) return FALSE;

    return g_COM_LWIP_Driver.Bind(&g_COM_LWIP_Config.DeviceConfigs[index], index);
}
int  Network_Interface_Open(int index)
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();
    if(index >= ARRAYSIZE(g_COM_LWIP_Config.DeviceConfigs)) return -1;

    HAL_CONFIG_BLOCK::ApplyConfig( COM_LWIP_DEVICE_CONFIG::GetDriverName(), &g_COM_LWIP_Config, sizeof(g_COM_LWIP_Config) );

    return g_COM_LWIP_Driver.Open(&g_COM_LWIP_Config.DeviceConfigs[index], index);
}
BOOL Network_Interface_Close(int index)
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();
    if(index >= ARRAYSIZE(g_COM_LWIP_Config.DeviceConfigs)) return FALSE;

    return g_COM_LWIP_Driver.Close(&g_COM_LWIP_Config.DeviceConfigs[index], index);
}

int COM_LWIP_Driver::Open( COM_LWIP_DRIVER_CONFIG* config, int index )
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();

    /* Network interface variables */
    struct ip_addr ipaddr, netmask, gw;
    struct netif *pNetIF;
    int len;
    const SOCK_NetworkConfiguration *iface;

    if(config == NULL) return -1;

    iface = &g_NetworkConfig.NetworkInterfaces[index];

    if(0 == (iface->flags & SOCK_NETWORKCONFIGURATION_FLAGS_DHCP))
    {
        ipaddr.addr  = iface->ipaddr;
        gw.addr      = iface->gateway;
        netmask.addr = iface->subnetmask;
    }
    else
    {
        /* Set network address variables - this will be set by either DHCP or when the configuration is applied */
        IP4_ADDR(&gw     ,   0,   0,   0, 0);
        IP4_ADDR(&ipaddr ,   0,   0,   0, 0);
        IP4_ADDR(&netmask, 255, 255, 255, 0);
    }

    len = g_COM_NetIF.hwaddr_len;

    if(len == 0 || iface->macAddressLen < len)
    {
        len = iface->macAddressLen;
        g_COM_NetIF.hwaddr_len = len;
    }

    memcpy(g_COM_NetIF.hwaddr, iface->macAddressBuffer, len);

    pNetIF = netif_add( &g_COM_NetIF, &ipaddr, &netmask, &gw, NULL, COM_netif_init, com_netif_input );

    netif_set_default( pNetIF );

    LwipNetworkStatus = COM_get_link_status(&config->SPI_Config);

    /* Initialize the continuation routine for the driver interrupt and receive */
    InitContinuations( pNetIF );

    return g_COM_NetIF.num;

}

BOOL COM_LWIP_Driver::Close( COM_LWIP_DRIVER_CONFIG* config, int index )
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();

    if(config == NULL) return FALSE;

    LwipUpTimeCompletion.Abort();

    netif_set_link_down( &g_COM_NetIF );
    netif_set_down( &g_COM_NetIF );
    netif_remove( &g_COM_NetIF );

    /* Disable the INTERRUPT pin */
    CPU_GPIO_EnableInputPin2(config->INT_Pin,
                              FALSE,                         /* Glitch filter enable */
                              NULL,                          /* ISR                  */
                              0,                             /* minor number         */
                              GPIO_INT_NONE,                 /* Interrupt edge       */
                              RESISTOR_PULLUP);              /* Resistor State       */

    InterruptTaskContinuation.Abort();

    LwipNetworkStatus = FALSE;

    com_lwip_close( &g_COM_NetIF );

    memset(&g_COM_NetIF, 0, sizeof(g_COM_NetIF));

    return TRUE;

}

BOOL  COM_LWIP_Driver::Bind  ( COM_LWIP_DRIVER_CONFIG* config, int index )
{
    NATIVE_PROFILE_HAL_DRIVERS_ETHERNET();

    return TRUE;
}




