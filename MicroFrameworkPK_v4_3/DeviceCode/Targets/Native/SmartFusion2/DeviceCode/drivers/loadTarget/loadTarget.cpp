/*--------- Includes ----------- */

#include "loadTarget.h"
#include <cmsis/m2sxxx.h>
#include <drivers/mss_spi/mss_spi.h>
#include "../eNVM/eNVM.h"

int loadArduinoSPI( uint8_t* address, uint16_t binarySize){
	const uint8_t target_page_size = 64;
	const uint8_t eNVM_read_size = target_page_size*2; // target is 16-bit while read is 8-bit
	uint8_t eNVM_read_buff[eNVM_read_size];
	uint8_t spi_tx_buff[16];
	uint8_t spi_rx_buff[16];
	uint16_t size, burning_location, reading_location, burn_command_location;
	uint8_t i,j,k;

	// Program enable
	spi_tx_buff[0] = 0xAC;
	spi_tx_buff[1] = 0x53;
	spi_tx_buff[2] = 0x00;
	spi_tx_buff[3] = 0x00;
	size = 4;
	MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, 0, 0 );
	MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

	HAL_Time_Sleep_MicroSeconds(100);


	// Read signature
	spi_tx_buff[0] = 0x30;
	spi_tx_buff[1] = 0x00;
	spi_tx_buff[2] = 0x00;
	size = 3;
	MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, spi_rx_buff, 1 );
	MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	if (spi_rx_buff[0] != 0x1E){
		return 1;
	}

	HAL_Time_Sleep_MicroSeconds(100);

	// Read signature
	spi_tx_buff[0] = 0x30;
	spi_tx_buff[1] = 0x00;
	spi_tx_buff[2] = 0x01;
	size = 3;
	MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, spi_rx_buff, 1 );
	MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	if (spi_rx_buff[0] != 0x95){
			return 1;
	}

	HAL_Time_Sleep_MicroSeconds(100);

	// Read signature
	spi_tx_buff[0] = 0x30;
	spi_tx_buff[1] = 0x00;
	spi_tx_buff[2] = 0x02;
	size = 3;
	MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, spi_rx_buff, 1 );
	MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	if (spi_rx_buff[0] != 0x0F){
			return 1;
	}

	HAL_Time_Sleep_MicroSeconds(100);


	// Erasing chip
	spi_tx_buff[0] = 0xAC;
	spi_tx_buff[1] = 0x80;
	spi_tx_buff[2] = 0x00;
	spi_tx_buff[3] = 0x00;
	size = 4;
	MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
	MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, 0, 0 );
	MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

	HAL_Time_Sleep_MicroSeconds(20000);

	burning_location = 0;
	reading_location = 0;

	// writing the memory
	while (reading_location < binarySize){
		if ( (reading_location + eNVM_read_size) < binarySize){
			// reading eNVM_read_size bytes from eNVM

			MSS_NVM_read((uint8_t *)address + reading_location, eNVM_read_buff, eNVM_read_size);
		} else {
			for (j=0; j<eNVM_read_size; j++){
				eNVM_read_buff[j] = 0xff;
			}
			// reading eNVM_read_size bytes from eNVM

			MSS_NVM_read((uint8_t *)address + reading_location, eNVM_read_buff, binarySize-reading_location);
		}

		// programming eNVM_read_size bytes to the remote processor EEPROM
		for (i=0; i<eNVM_read_size; i=i+2){

			// write Mem Page Low
			spi_tx_buff[0] = 0x40;
			spi_tx_buff[1] = (burning_location & 0xFF00 ) >> 8;
			spi_tx_buff[2] = burning_location & 0xFF;
			spi_tx_buff[3] = eNVM_read_buff[i];
			size = 4;
			MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
			MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, 0, 0 );
			MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

			HAL_Time_Sleep_MicroSeconds(100);

			// write Mem Page High
			spi_tx_buff[0] = 0x48;
			spi_tx_buff[1] = (burning_location & 0xFF00 ) >> 8;
			spi_tx_buff[2] = burning_location & 0xFF;
			spi_tx_buff[3] = eNVM_read_buff[i+1];
			size = 4;
			MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
			MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, 0, 0 );
			MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

			HAL_Time_Sleep_MicroSeconds(400);

			burning_location = burning_location + 1;
		}

		burn_command_location = burning_location - target_page_size;

		// write program memory page
		spi_tx_buff[0] = 0x4C;
		spi_tx_buff[1] = (burn_command_location & 0xFF00 ) >> 8;
		spi_tx_buff[2] = burn_command_location & 0xFF;
		spi_tx_buff[3] = 0x00;
		size = 4;
		MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
		MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, 0, 0 );
		MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );

		HAL_Time_Sleep_MicroSeconds(5000);

		reading_location = reading_location + eNVM_read_size;
	}

	burning_location = 0;
	reading_location = 0;

	// verifying the memory
	while (reading_location < binarySize){
		if ( (reading_location + eNVM_read_size) < binarySize){
			// reading eNVM_read_size bytes from eNVM

			MSS_NVM_read((uint8_t *)address + reading_location, eNVM_read_buff, eNVM_read_size);
		} else {
			for (j=0; j<eNVM_read_size; j++){
				eNVM_read_buff[j] = 0xff;
			}
			// reading eNVM_read_size bytes from eNVM

			MSS_NVM_read((uint8_t *)address + reading_location, eNVM_read_buff, binarySize-reading_location);
		}

		// programming eNVM_read_size bytes to the remote processor EEPROM
		for (i=0; i<eNVM_read_size; i=i+2){

			// Read Mem Page Low
			spi_tx_buff[0] = 0x20;
			spi_tx_buff[1] = (burning_location & 0xFF00 ) >> 8;
			spi_tx_buff[2] = burning_location & 0xFF;
			size = 3;
			MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
			MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, spi_rx_buff, 1 );
			MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
			if (spi_rx_buff[0] != eNVM_read_buff[i]){
				return 1;
			}

			// Read Mem Page High
			spi_tx_buff[0] = 0x28;
			spi_tx_buff[1] = (burning_location & 0xFF00 ) >> 8;
			spi_tx_buff[2] = burning_location & 0xFF;
			size = 3;
			MSS_SPI_set_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
			MSS_SPI_transfer_block(&g_mss_spi0, spi_tx_buff, size, spi_rx_buff, 1 );
			MSS_SPI_clear_slave_select( &g_mss_spi0, MSS_SPI_SLAVE_0 );
			if (spi_rx_buff[0] != eNVM_read_buff[i+1]){
				return 1;
			}

			burning_location = burning_location + 1;
		}

		reading_location = reading_location + eNVM_read_size;
	}


	hal_printf("Arduino successfully programmed.\r\n");
	return 0;
}

