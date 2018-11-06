//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmoteSF2.h"
#include "SamrakshEmoteSF2_Samraksh_eMote_SF2_SPI.h"
#include <cmsis/m2sxxx.h>
#include <drivers/mss_spi/mss_spi.h>

using namespace Samraksh::eMote::SF2;

INT8 SPI::SendData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
	uint8_t spi_tx_buff[16];
	uint8_t spi_rx_buff[16];
	uint16_t size;
	UINT8* payload = param1.GetBuffer();

	if (payload[2] == (UINT8)'n'){
		hal_printf("on\r\n");
		// L ON
		spi_tx_buff[0] = 0x4E;
		size = 1;
		MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
		MSS_SPI_transfer_block(&g_mss_spi1, spi_tx_buff, size, 0, 0 );
		HAL_Time_Sleep_MicroSeconds(100000);
		MSS_SPI_transfer_block(&g_mss_spi1, 0, 0, spi_rx_buff, 1 );
		MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
		hal_printf("ard ret: %d\r\n",spi_rx_buff[0]);
	} else if (payload[2] == (UINT8)'f'){
		hal_printf("off\r\n");
		// L OFF
		spi_tx_buff[0] = 0x46;
		size = 1;
		MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
		MSS_SPI_transfer_block(&g_mss_spi1, spi_tx_buff, size, 0, 0 );
		HAL_Time_Sleep_MicroSeconds(100000);
		MSS_SPI_transfer_block(&g_mss_spi1, 0, 0, spi_rx_buff, 1 );
		MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
		hal_printf("ard ret: %d\r\n",spi_rx_buff[0]);
	} else if (payload[2] == (UINT8)'k'){
		hal_printf("hack\r\n");
		// L ON
		spi_tx_buff[0] = 0x49;
		size = 1;
		MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
		MSS_SPI_transfer_block(&g_mss_spi1, spi_tx_buff, size, 0, 0 );
		HAL_Time_Sleep_MicroSeconds(100000);
		MSS_SPI_transfer_block(&g_mss_spi1, 0, 0, spi_rx_buff, 1 );
		MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
		hal_printf("ard ret: %d\r\n",spi_rx_buff[0]);
	} else {
		hal_printf("unknown\r\n");
	}
    return retVal;
}

INT8 SPI::ReceiveData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

