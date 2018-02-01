#pragma once

#include <stm32f10x.h>
#include <tinyhal.h>

// Debugging and profiling
#define MX25R6435F_CE_TIMIMG

// Uncomment me to show what is going on
#define SPIFLASH_DRIVER_VERBOSE
#ifdef SPIFLASH_DRIVER_VERBOSE
#define spiflash_printf hal_printf
#else
#define spiflash_printf(...)
#endif

enum {
	ERR_NOERR = 0,
	ERR_NO_INIT = -1,
	ERR_FLASH_BUSY = -2,
	ERR_PROTECTED = -3,
	ERR_UNKNOWN = -4,
	ERR_BAD_ARGS = -5,
	ERR_BAD_ADDR = -6,
};

// Read/Write a byte
static uint8_t spi_byte(uint8_t data) {
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) ; // spin
	SPI_I2S_SendData(SPI1, data);
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET ) ; // spin
	return SPI_I2S_ReceiveData(SPI1);
}

// Read/Write half-word (2 bytes)
static uint16_t spi_halfword(uint16_t data) {
	uint16_t ret = 0;
	ret = spi_byte(data>>8) << 8;
	ret |= spi_byte(data&0xFF);
	return ret;
}

static void spi_stream_out(const uint8_t *data, unsigned size) {
	for (int i=0; i<size; i++) {
		while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) ; // spin
		SPI_I2S_SendData(SPI1, data[i]);
	}
	// Make sure the flag gets cleared
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET ) ;
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET ) { SPI_I2S_ReceiveData(SPI1); }
}

static void spi_stream_in(uint8_t *data, unsigned size) {
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) ; // spin once
	for (int i=0; i<size; i++) {
		SPI_I2S_SendData(SPI1, 0);
		while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET ) ; // spin
		data[i] = SPI_I2S_ReceiveData(SPI1);
	}
}

class mx25r6435f {

	private:
	const static int block32_sz = 32*1024;
	const static int block64_sz = 64*1024;
	const static int max_spi_freq = 33000000;
	const static int dummy_cycles = 8;

	const static int RESET_US = 40;
	const static int DP_WAIT_US = 10+30;
	const static int WAKEUP_TIME = 40;

	const static uint8_t  MANF_ID = 0xC2;
	const static uint16_t DEV_ID  = 0x2817;

	const static uint8_t cmd_RDID = 0x9F; // Read Manf and PN
	const static uint8_t cmd_READ = 0x03; // Read
	const static uint8_t cmd_PP   = 0x02; // Page Program
	const static uint8_t cmd_SE   = 0x20; // Sector Erase
	const static uint8_t cmd_CE   = 0x60; // Chip Erase
	const static uint8_t cmd_RDSR = 0x05; // Read Status Register
	const static uint8_t cmd_RDCR = 0x15; // Read Config Register
	const static uint8_t cmd_DP   = 0xB9; // Deep Power Down
	const static uint8_t cmd_WREN = 0x06; // Write Enable

	int isAsleep;
	void set_asleep(int s) { isAsleep = s; }
	int get_asleep() { return isAsleep; }

	uint8_t  SR; // Status reg shadow
	uint16_t CR; // Configuration reg shadow
	
	int write_enabled()     { return (update_status() & 2); }
	int FlashIsBusy()       { return (update_status() & 1); }

	void cs(int go);
	void wp(int go);
	void reset(int go);
	void deinit();
	void deep_sleep();
	void wakeup();
	uint8_t  update_status();
	uint16_t update_config();
	void write_enable();

	public:
	const static int mem_size = 8*1024*1024; // bytes
	const static int page_size = 256;
	const static int sector_size = 4*1024;
	const static int sys_size = 4*1024;

	int write_page(const uint8_t *data, unsigned size, unsigned addr);
	int read(uint8_t *data, unsigned size, unsigned addr);
	int init();
	int chip_erase();
	int isBusy() { return FlashIsBusy(); }
};