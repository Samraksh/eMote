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
#include <loadTarget/loadTarget.h>

using namespace Samraksh::eMote::SF2;

INT8 SPI::SendData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
	uint8_t spi_tx_buff[16];
	uint8_t spi_rx_buff[16];
	uint16_t size;
	int retVerify = 0;
	UINT8* payload = param1.GetBuffer();

	mss_spi_slave_t slaveNum;

	if ((param0 < (int)MSS_SPI_MAX_NB_OF_SLAVES) && (param0 >= 0))
		slaveNum = (mss_spi_slave_t) param0;
	else
		slaveNum = 	MSS_SPI_SLAVE_0;
	
	MSS_SPI_set_slave_select( &g_mss_spi1, slaveNum );
	MSS_SPI_transfer_block(&g_mss_spi1, payload, param2, 0, 0 );
	MSS_SPI_clear_slave_select( &g_mss_spi1, slaveNum );

    return retVal;
}

INT8 SPI::ReceiveData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
	UINT8* payload = param1.GetBuffer();

	mss_spi_slave_t slaveNum;

	if ((param0 < (int)MSS_SPI_MAX_NB_OF_SLAVES) && (param0 >= 0))
		slaveNum = (mss_spi_slave_t) param0;
	else
		slaveNum = 	MSS_SPI_SLAVE_0;

	MSS_SPI_set_slave_select( &g_mss_spi1, slaveNum );
	MSS_SPI_transfer_block(&g_mss_spi1, 0, 0, payload, param2 );
	MSS_SPI_clear_slave_select( &g_mss_spi1, slaveNum );

    return retVal;
}

INT8 SPI::SendReceiveData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, CLR_RT_TypedArray_UINT8 param2, INT32 param3, HRESULT &hr )
{
    INT8 retVal = 0; 
	UINT8* sendPayload = param1.GetBuffer();
	UINT8* receivePayload = param2.GetBuffer();

	mss_spi_slave_t slaveNum;

	if ((param0 < (int)MSS_SPI_MAX_NB_OF_SLAVES) && (param0 >= 0))
		slaveNum = (mss_spi_slave_t) param0;
	else
		slaveNum = 	MSS_SPI_SLAVE_0;
	
	MSS_SPI_set_slave_select( &g_mss_spi1, slaveNum );
	MSS_SPI_transfer_block(&g_mss_spi1, sendPayload, param3, receivePayload, param3 );
	MSS_SPI_clear_slave_select( &g_mss_spi1, slaveNum );

    return retVal;
}
