#include <tinyhal.h>

// This is very PAL like but still hardware dependent due to SPI code

#ifdef PLATFORM_EMOTE_AUSTERE
#include "spiflash.h"

static mx25r6435f flash;
static int is_init = 0;

#define SPIFLASH_INIT_CHECK() ( {if (is_init != 1) {SOFT_BREAKPOINT(); return ERR_NO_INIT;}} )


static void print_error(int err) {
	switch(err) {
		case ERR_NOERR: 											break;
		case ERR_NO_INIT:	spiflash_printf("Flash Not Init\r\n");	break;
		case ERR_FLASH_BUSY: spiflash_printf("Flash Busy\r\n"); 	break;
		default: spiflash_printf("Unknown error %d\r\n", err); 		break;
	}
}

// 0 on success
int spiflash_init(int id, int args) {
	int ret;

	if (is_init) return 1; // already up...
	ret = flash.init();
	if (ret) return -1;
	is_init = 1;
	return 0;
}

// return 0 on success
int spiflash_chip_erase(int id, int args) {
	int ret;
	SPIFLASH_INIT_CHECK();
	ret = flash.chip_erase();
	print_error(ret);
	return ret;
}

// 1 --> Busy
// 0 --> Not Busy
// <0 --> Error
int spiflash_isBusy(int id, int args) {
	SPIFLASH_INIT_CHECK();
	return flash.isBusy();
}

// data : pointer to data block to write
// size : number of bytes to write
// addr : address in flash memory in BYTES but must be page aligned.
int spiflash_write_page(const uint8_t *data, unsigned size, unsigned addr) {
	int ret;
	SPIFLASH_INIT_CHECK();
	if (size == 0) return ERR_NOERR; // No-op
	if (size > flash.page_size) {
		spiflash_printf("%s: Bad size. Writes cannot be larger than page size of %d bytes\r\n", __func__, flash.page_size);
		return ERR_BAD_ARGS;
	}
	if (addr+size > flash.mem_size) {
		spiflash_printf("%s: Write address out of memory bounds\r\n", __func__);
		return ERR_BAD_ADDR;
	}
	if (addr % flash.page_size != 0) {
		spiflash_printf("%s: Writes must be page aligned\r\n", __func__);
		return ERR_BAD_ARGS;
	}
	ret = flash.write_page(data, size, addr);
	print_error(ret);
	return ret;
}

int spiflash_read(uint8_t *data, unsigned size, unsigned addr) {
	int ret;
	SPIFLASH_INIT_CHECK();
	if (size == 0) return ERR_NOERR; // No-op
	if (addr+size > flash.mem_size) {
		spiflash_printf("%s: Read goes out of memory bounds\r\n", __func__);
		return ERR_BAD_ADDR;
	}
	ret = flash.read(data, size, addr);
	print_error(ret);
	return ret;
}

#else // stubs
int spiflash_init(int id, int args) { spiflash_printf("No spi-flash supported on this platform\r\n"); return ERR_NO_INIT; }
int spiflash_chip_erase(int id, int args) { return ERR_NO_INIT; }
int spiflash_isBusy(int id, int args) { return ERR_NOERR; }
int spiflash_write_page(const uint8_t *ptr, unsigned size, unsigned addr) { return ERR_NO_INIT; }
int spiflash_read(uint8_t *data, unsigned size, unsigned addr) { return ERR_NO_INIT; }
#endif