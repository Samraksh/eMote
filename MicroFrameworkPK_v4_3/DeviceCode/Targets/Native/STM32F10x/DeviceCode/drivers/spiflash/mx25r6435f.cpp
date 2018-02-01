
#include "spiflash.h"

static unsigned get_APB1_clock() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return RCC_Clocks.PCLK1_Frequency;
}

static unsigned get_APB2_clock() {
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	return RCC_Clocks.PCLK2_Frequency;
}

static int initSPI(SPI_TypeDef* SPIx) {
	GPIO_InitTypeDef GPIO_InitStructure;
	unsigned int baud;
	unsigned SpiBusClock;
	SPI_InitTypeDef SPI_InitStruct;

	if (SPIx != SPI1) {
		spiflash_printf("Only SPI1 is supported\r\n");
		return -1;
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,	ENABLE);
	SPI_I2S_DeInit(SPI1);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_StructInit(&SPI_InitStruct);
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;

	SpiBusClock = get_APB2_clock();

	switch(SPI_InitStruct.SPI_BaudRatePrescaler) {
		case SPI_BaudRatePrescaler_2:  baud = SpiBusClock / 2  / 1000; break;
		case SPI_BaudRatePrescaler_4:  baud = SpiBusClock / 4  / 1000; break;
		case SPI_BaudRatePrescaler_8:  baud = SpiBusClock / 8  / 1000; break;
		case SPI_BaudRatePrescaler_16: baud = SpiBusClock / 16 / 1000; break;
		default: baud = 0;
	}

	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);

	spiflash_printf("SPI1 up CPOL: %d CPHA: %d Baud: %d kHz (%d kHz bus)\r\n",
		SPI_InitStruct.SPI_CPOL, SPI_InitStruct.SPI_CPHA, baud, SpiBusClock/1000);
	return 0;
}

// Platform dependent code.
// Inits GPIOs (CS, WP, and RST)
static void init_mx25r6435f_pins() {
	GPIO_InitTypeDef GPIO_InitStructure;

	// CS# pin PA2 , RST# PA3, WP# PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_WriteBit(GPIOA ,GPIO_Pin_2, Bit_SET);
	GPIO_WriteBit(GPIOA ,GPIO_Pin_3, Bit_SET);
	GPIO_WriteBit(GPIOA ,GPIO_Pin_4, Bit_SET);
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// Reversed because active low
void mx25r6435f::reset(int go) {
	if (go)
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_RESET);
	else
		GPIO_WriteBit(GPIOA, GPIO_Pin_3, Bit_SET);
}

// Reversed because active low
void mx25r6435f::wp(int go) {
	if (go)
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_RESET);
	else
		GPIO_WriteBit(GPIOA, GPIO_Pin_4, Bit_SET);
}

// Reversed because active low
void mx25r6435f::cs(int go) {
	if (go)
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET);
	else
		GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET);
	__NOP();
}

// Undo all the pins and peripherals we touched
void mx25r6435f::deinit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	SPI_Cmd(SPI1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,	DISABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void mx25r6435f::deep_sleep() {
	cs(1);
	spi_byte(cmd_DP); // Put in deep power down mode
	cs(0);
	HAL_Time_Sleep_MicroSeconds(DP_WAIT_US); // Ensure state fully transitions
	set_asleep(1);
}

void mx25r6435f::wakeup() {
	cs(1);
	HAL_Time_Sleep_MicroSeconds(WAKEUP_TIME);
	cs(0);
	HAL_Time_Sleep_MicroSeconds(WAKEUP_TIME);
	set_asleep(0);
}

void mx25r6435f::write_enable() {
	cs(1);
	spi_byte(cmd_WREN);
	cs(0);
}

uint8_t mx25r6435f::update_status() {
	cs(1);
	spi_byte(cmd_RDSR);
	SR = spi_byte(0);
	cs(0);
	//spiflash_printf("SR: 0x%.4x\r\n");
	return SR;
}

uint16_t mx25r6435f::update_config() {
	cs(1);
	spi_byte(cmd_RDCR);
	CR = spi_halfword(0);
	cs(0);
	//spiflash_printf("CR: 0x%.4x\r\n");
	return CR;
}

// Erases the whole chip
// WARNING: THIS LITERALLY TAKES 2 MINUTES
int mx25r6435f::chip_erase() {
#ifdef MX25R6435F_CE_TIMIMG
	UINT64 now, now2;
	uint32_t time;
#endif
	if (get_asleep()) wakeup();

	if ( FlashIsBusy() )    { return ERR_FLASH_BUSY; }
	write_enable();
	if ( !write_enabled() ) { return ERR_UNKNOWN; }

	cs(1);
	spi_byte(cmd_CE);
	cs(0);
	spiflash_printf("Chip Erase Started... (takes ~100 seconds)\r\n");
#ifdef MX25R6435F_CE_TIMIMG
	now  = HAL_Time_CurrentTicks();
	while (FlashIsBusy() == 1) {HAL_Time_Sleep_MicroSeconds(1000);} // spin
	now2 = HAL_Time_CurrentTicks();
	time = ((now2-now)/10000) & 0xFFFFFFFF;
	spiflash_printf("Chip Erase took %d ms\r\n", time);
#endif
	return 0;
}

int mx25r6435f::read(uint8_t *data, unsigned size, unsigned addr) {
	if (get_asleep()) wakeup();
	if ( FlashIsBusy() )    { return ERR_FLASH_BUSY; }

	cs(1);
	spi_byte(cmd_READ);
	spi_byte((addr>>16) & 0xFF);
	spi_byte((addr>>8)  & 0xFF);
	spi_byte((addr)     & 0xFF);
	// for(int i=0; i<size; i++) {
		// data[i] = spi_byte(0);
	// }
	spi_stream_in(data, size);
	cs(0);
	return ERR_NOERR;
}

int mx25r6435f::write_page(const uint8_t *data, unsigned size, unsigned addr) {
	if (get_asleep()) wakeup();

	if ( FlashIsBusy() )    { return ERR_FLASH_BUSY; }
	write_enable();
	if ( !write_enabled() ) { return ERR_UNKNOWN; }

	spiflash_printf("Programming Page 0x%.6x with %u bytes starting with %c ...\r\n", addr, size, data[0]);

	cs(1);
	spi_byte(cmd_PP);
	// Now send 24-bit address
	spi_byte((addr>>16) & 0xFF);
	spi_byte((addr>>8)  & 0xFF);
	spi_byte((addr)     & 0xFF);
	// Now send the data
	spi_stream_out(data, size);
	cs(0);
	return ERR_NOERR;
}

int mx25r6435f::init() {
	uint8_t manf;
	uint16_t dev;
	int ret;
	init_mx25r6435f_pins();
	ret = initSPI(SPI1);
	if (ret) { goto OUT_FAIL; }
	reset(1);
	HAL_Time_Sleep_MicroSeconds(RESET_US);
	reset(0);
	HAL_Time_Sleep_MicroSeconds(RESET_US);

	// Check RDID to verify
	cs(1);
	spi_byte(cmd_RDID);
	manf = spi_byte(0);
	dev = spi_halfword(0);
	cs(0);

	if (manf == MANF_ID && dev == DEV_ID ) {
		hal_printf("Macronix MX25R6435F 64 Mbit SPI-Flash detected.\r\n");
	} else {
		hal_printf("SPI-FLASH MANF: 0x%.2x DEV: 0x%.4x\r\n", manf, dev);
		hal_printf("Unknown or Bad device detected. Failed to init...\r\n");
		goto OUT_FAIL;
	}
	
	update_status();
	update_config();

	deep_sleep();
	return 0;
	
OUT_FAIL:
	deinit();
	return -1;
}
