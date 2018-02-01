
#pragma once

int spiflash_init(int id, int args);
int spiflash_chip_erase(int id, int args);
int spiflash_isBusy(int id, int args);
int spiflash_write_page(const uint8_t *data, unsigned size, unsigned addr);
int spiflash_read(uint8_t *data, unsigned size, unsigned addr);
