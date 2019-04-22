// This file handles the interaction with an external compute processor.
// When using an external processor we are not expected to run a C# app.

#include <tinyhal.h>
#include "ComputeProcessor.h"
#include <drivers/mss_spi/mss_spi.h>
#include <cmsis/m2sxxx.h>

static bool CP_currently_present = FALSE;

static void CP_WantsTransaction(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	int buffSize = 128;
	uint8_t spi_tx_buff[buffSize];
	uint8_t spi_rx_buff[buffSize];
	int sendPos = 0;

	spi_tx_buff[0] = (uint8_t)'a';
	spi_tx_buff[1] = (uint8_t)'b';
	spi_tx_buff[2] = (uint8_t)'c';
	
	MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
	while ((sendPos < buffSize) && (CPU_GPIO_GetPinState(COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM) == false)){
		//MSS_SPI_transfer_block(&g_mss_spi1, &spi_tx_buff[sendPos], 1, &spi_rx_buff[sendPos], 1 );		
		MSS_SPI_transfer_block(&g_mss_spi1, 0, 0, &spi_rx_buff[sendPos], 1 );	
		HAL_Time_Sleep_MicroSeconds(400);	
		sendPos++;
	}
	MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );

	hal_printf("rx: ");
	for (int j=0; j < sendPos; j++){
		hal_printf("%c", spi_rx_buff[j]);
	}
	hal_printf("\r\n");
}

// returns TRUE if successful
bool CP_Init(void){


	hal_printf("Compute processor init\r\n");
	CPU_GPIO_EnableInputPin( COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM, FALSE, CP_WantsTransaction, GPIO_INT_EDGE_LOW, RESISTOR_DISABLED);
	if (CPU_GPIO_GetPinState(COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM) == false){
		CP_WantsTransaction(NULL, FALSE, NULL);
	}

	

	return TRUE;
}

bool CP_Present(void){
	return CP_currently_present;
}

bool CP_UnInit(void){
	return TRUE;
}
