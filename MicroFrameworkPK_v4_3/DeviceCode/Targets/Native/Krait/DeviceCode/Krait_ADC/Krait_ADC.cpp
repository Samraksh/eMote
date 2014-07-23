/*  Name : Krait_ADC.cpp
 *
 *  Author : Christopher Morse
 *
 *  Description :
 *
 */

// zoom
#pragma GCC optimize ("O3")
#define FPGA_O3_BUILD

#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait_ADC.h"

// Globals
uint8_t fpga_init_done;

// Statics, I use my own statics because these need to be optimised along with the ADC
// Since we optimize per subproject and not whole project.
// Once we get the whole thing running in -O3 then please make this sane.

static void gpio_set(UINT32 gpio, UINT32 dir)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	writel(dir, addr); // second bit controls GPIO
	return;
}

static uint8_t sam_fpga_spi(uint8_t data) {
    uint8_t bit = 0x80;
    uint8_t ret = 0;

    // MISO input
    unsigned int *addr = (unsigned int *)GPIO_IN_OUT_ADDR(FPGA_MISO_PIN);

	// Fully optimized we push about 1 MHz

    while (bit) {
        gpio_set(FPGA_MOSI_PIN, (data&bit) ? 2 : 0);     // Set output
		SAM_FPGA_SPI_CLK_A();
        ret |= ( readl(addr)&1 ); // Latch input
        if(bit>1)
            ret = ret << 1;
        bit = bit >> 1;
        gpio_set(FPGA_SCLK_PIN, 2); // clock high
		SAM_FPGA_SPI_CLK_B();
        gpio_set(FPGA_SCLK_PIN, 0); // clock low
    }
    //dprintf(INFO, "sent 0x%X got SPI 0x%X\n", data, ret);
    return ret;
}

static void gpio_tlmm_config(UINT32 gpio, UINT8 func,
		      UINT8 dir, UINT8 pull,
		      UINT8 drvstr, UINT32 enable)
{
	UINT32 val = 0;
	val |= pull;
	val |= func << 2;
	val |= drvstr << 6;
	val |= enable << 9;
	UINT32 *addr = (UINT32 *)GPIO_CONFIG_ADDR(gpio);
	writel(val, addr);
	return;
}


/* Perform a multi-byte read from consecutive memory locations. Note: Seems limited to 4 bytes */
static void fpga_cmd53_r(uint8_t func, uint8_t reg, uint8_t *dest, uint8_t size) {
	uint8_t resp, timeout;
	uint8_t spi_send[6] = {0x75, 0x14, 0x00, 0x00, 0x00, 0xFF};

	// Function area, 0 or 1
	spi_send[1] |= (func << 4);

	// Register address bits;
	spi_send[3] |= (reg << 1);
	spi_send[2] |= (reg >> 7);

	// number of bytes to read
	// As of FPGA v1.6, we only seem to support 4 bytes
	if (size > 4)
		return;
	spi_send[4] = size;

	FPGA_CS(0);
	for(int i=0; i<6; i++) {
		timeout=0xF;
		sam_fpga_spi(spi_send[i]);
	}

	do {
		resp = sam_fpga_spi(0xFF);
		timeout--;
	} while(resp == 0xFF && timeout > 0);

	// Data token, error token, data response ???
	sam_fpga_spi(0);
	sam_fpga_spi(0);
	sam_fpga_spi(0);

	// Data, bytes
	for(int i=0; i<size; i++) {
		dest[i] = sam_fpga_spi(0);
	}

	// 2 CRC bytes, ignored.
	sam_fpga_spi(0);
	sam_fpga_spi(0);

	sam_fpga_spi(0xFF); // cleanup
	FPGA_CS(1);

	return;
}

// Wrapper function for reads larger than 4 bytes (see cmd53_r)
// You MUST use this if you are reading > 4 bytes
static void fpga_cmd53_r_wrap(uint8_t func, uint8_t reg, uint8_t *dest, uint8_t size) {
	for(int i=0; i<size; i+=4)
		if (size - i >= 4)
			fpga_cmd53_r(func, reg+i, dest+i, 4);
		else
			fpga_cmd53_r(func, reg+i, dest+i, size-i);
}

// CMD52, read a register.
static uint8_t fpga_cmd52_r(uint8_t func, uint8_t reg) {

	uint8_t resp, timeout, extra;
	uint8_t spi_send[6] = {0x74, 0x00, 0x00, 0x01, 0x00, 0xFF};
	
	// Function area, 0 or 1
	spi_send[1] |= (func << 4);
	
	// Register address, largest is 0x66 so we only need to worry about bottom byte
	spi_send[3] |= (reg << 1);
	
	FPGA_CS(0);
	for(int i=0; i<6; i++) {
		timeout=0xF;
		sam_fpga_spi(spi_send[i]);
	}

	do {
		resp = sam_fpga_spi(0xFF);
		timeout--;
	} while(resp == 0xFF && timeout > 0);

	// Get returned data
	extra = sam_fpga_spi(0xFF);
	sam_fpga_spi(0xFF); // cleanup
	FPGA_CS(1);
	
	return extra;
}

// CMD52, write to a register.
static uint8_t fpga_cmd52_w(uint8_t func, uint8_t reg, uint8_t data) {

	uint8_t resp, timeout, extra;
	uint8_t spi_send[6] = {0x74, 0x80, 0x00, 0x01, 0x00, 0xFF};

	// Data to write
	spi_send[4] |= data;

	// Function area, 0 or 1
	spi_send[1] |= (func << 4);

	// Register address, largest is 0x66 so we only need to worry about bottom byte
	spi_send[3] |= (reg << 1);

	FPGA_CS(0);
	for(int i=0; i<6; i++) {
		timeout=0xF;
		sam_fpga_spi(spi_send[i]);
	}

	do {
		resp = sam_fpga_spi(0xFF);
		timeout--;
	} while(resp == 0xFF && timeout > 0);

	// Get returned data
	extra = sam_fpga_spi(0xFF);

	sam_fpga_spi(0xFF); // cleanup
	FPGA_CS(1);

	return extra;
}

static const int FPGA_GPIO_BASE = 0x20;
void fpga_gpio_set(UINT32 gpio, UINT32 dir) {
	if (fpga_init_done != 1) {
		fpga_init();
	}

	int my_pin = gpio - 1000;

	if (my_pin > 31) {
		return;
	}

	int reg_offset = my_pin / 8;
	int bit_offset = my_pin % 8;
	unsigned int newval;

	// Current register contents
	newval = fpga_cmd52_r(1, FPGA_GPIO_BASE + reg_offset) | (1 << bit_offset);

	// Set direction to output, '1'
	fpga_cmd52_w(1, FPGA_GPIO_BASE + reg_offset, newval);

	// Set actual output val , '0' or '1'
	newval = fpga_cmd52_r(1, FPGA_GPIO_BASE + reg_offset + 4); // Current register contents.
	if (dir == 0) {
		newval &= ~(1 << bit_offset);
	}
	else {
		newval |= (1 << bit_offset);
	}

	fpga_cmd52_w(1, FPGA_GPIO_BASE + reg_offset + 4, newval);

	return;
}

// FPGA GENERAL
// Functions
INT8 fpga_init() {
	if (fpga_init_done) return 0; // Already started
	fpga_init_done = 1;

	char ver[17]; // FPGA version string, 16 bytes plus null
	const char *expected_version = "ADAPT v1.6 ÿÿÿÿÿ";
	
	// FPGA power state signaling pins
	gpio_tlmm_config(90, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);
	gpio_tlmm_config(91, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);

	// FPGA power state pins default
	gpio_set(90, 2); // ON/OFF, set ON (1)
	gpio_set(91, 0); // Asleep/Awake, set Awake (0)

	// Set SDIO pins into SPI comm mode
	gpio_tlmm_config(FPGA_MISO_PIN, 0, GPIO_INPUT,  GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE);
	gpio_tlmm_config(FPGA_CS_PIN,   0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);
	gpio_tlmm_config(FPGA_SCLK_PIN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);
	gpio_tlmm_config(FPGA_MOSI_PIN, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);
	gpio_set(FPGA_SCLK_PIN, 0);
	gpio_set(FPGA_CS_PIN, 2);

	FPGA_CS(1); // unassert at startup

	for(int i=0; i<10; i++) { sam_fpga_spi(0xFF); }
	HAL_Time_Sleep_MicroSeconds_InterruptEnabled(10000);

	{ // Send CMD0
		uint8_t resp, timeout;
		uint8_t spi_send[6] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
		//dprintf(INFO, "sending CMD0\n");
		FPGA_CS(0); // assert
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1000);
		for(int i=0; i<6; i++) {
			timeout=0xF;
			sam_fpga_spi(spi_send[i]);
		}
		//dprintf(INFO, "waiting for response\n");
		do {
			resp = sam_fpga_spi(0xFF);
			timeout--;
		} while(resp == 0xFF && timeout > 0);
		sam_fpga_spi(0xFF); // cleanup byte

		FPGA_CS(1);

		if (timeout == 0)
			return 1;
	}

	{ // CMD59, disable CRC
		uint8_t resp, timeout;
		uint8_t spi_send[6] = {0x7B, 0x00, 0x00, 0x00, 0x00, 0x8B};
		//dprintf(INFO, "sending CMD59\n");
		FPGA_CS(0); // assert
		HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1000);
		for(int i=0; i<6; i++) {
			timeout=0xF;
			sam_fpga_spi(spi_send[i]);
		}
		//dprintf(INFO, "waiting for response\n");
		do {
			resp = sam_fpga_spi(0xFF);
			timeout--;
		} while(resp == 0xFF && timeout > 0);
		sam_fpga_spi(0xFF); // cleanup

		FPGA_CS(1);

		if (timeout == 0)
			return 1;
	}

    { // CMD5, 4 extra bytes
        uint8_t resp, timeout, extra[4];
        uint8_t spi_send[6] = {0x45, 0x00, 0x00, 0x00, 0x00, 0x87};
        //dprintf(INFO, "sending CMD5\n");
        FPGA_CS(0);
        HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1000);
        for(int i=0; i<6; i++) {
            timeout=0xF;
            sam_fpga_spi(spi_send[i]);
        }
        //dprintf(INFO, "waiting for response\n");
        do {
            resp = sam_fpga_spi(0xFF);
            timeout--;
        } while(resp == 0xFF && timeout > 0);

        // Get extra bytes
        for(int i=0; i<4; i++)
            extra[i] = sam_fpga_spi(0xFF);

        sam_fpga_spi(0xFF); // cleanup

        FPGA_CS(1);
		
		if (timeout == 0)
			return 1;
    }

	// Enable Func 1
	fpga_cmd52_w(0,2,2);

	// FPGA ADC module reset command
	fpga_cmd52_w(1, 0x61, 32); // ADC_CMD reg, cmd: reset

	// Version string should read: "ADAPT v1.6" plus some garbage
	ver[16] = '\0';
	fpga_cmd53_r_wrap(1, 0,  (uint8_t *)(ver)   , 16);

	if ( strcmp(ver, expected_version) != 0 )
		hal_printf("%s FPGA DRIVER not recognized!\r\n%s Expected\r\n", ver, expected_version);
	else
		hal_printf("%s FPGA DRIVER init\r\n", ver);

	// Configure default ADC settings
	// [2:0] INCC, forced to 3'b111 for GND reference, see ADC7689 datasheet
	// [6:4] REF, forced to 3'b000 for internal 2.5v reference.
	fpga_cmd52_w(1, 0x60, 0x07);

	return 0;
}

// ADC REG COMMANDS

// ADC configuration register.
INT8 fpga_adc_conf(UINT8 incc, UINT8 ref, UINT8 bw){
}

// ADC command register
INT8 fpga_adc_cmd(UINT8 chan, UINT8 cmd){
}

// ADC status register
INT8 fpga_adc_get_stat(void){
}

// Sets sample rate in increments of 13.33us (256 clocks @ 19.2 MHz).
INT8 fpga_adc_sample_rate(UINT8 sample_rate_ticks){
}

// Returns the last ADC immediate mode sample stored
UINT16 fpga_adc_get_sample(void){
}

// Bit vector for enabling ADC inputs 0-7. Auto mode only.
INT8 fpga_adc_set_chan(UINT8 chan){
	return -1;
}

// raw access mode. write_mode = 0. 'offset' is the offset of adc register from module base.
UINT8 fpga_adc_raw(UINT8 offset, UINT8 rw, UINT8 data){
}

// Get continuous mode samples (multiple). NOTE: Undocumented, making guesses, may change
// Return: Number of samples available (including what was returned)
// buffer: array to place samples (in event of multiple channels, user must sort out).
// toRead: Maximum number of samples to place into buffer (e.g. buffer size)
UINT16 fpga_adc_cont_get(UINT16 *buffer, UINT8 toRead){
}

// ADC HIGHER LEVEL COMMANDS (composed of above for the user/speed).
// Grab one sample on one channel and blocks until returned.
UINT16 fpga_adc_now(UINT8 chan){
	uint8_t cmd;
	uint8_t ret[4];
	
	GLOBAL_LOCK(irq);

	// [1:0] channel
	// [7:5] command
	// 1,2,3,4
	// reset, get immediate, start auto, stop auto.
	// Start and stop auto not currently supported.
	cmd = (chan & 0x03) + (2 << 5); // ADC_CMD reg
	fpga_cmd52_w(1, 0x61, cmd);

	// Either wait or poll the FPGA
	// Not needed with cmd53 where we pull all at once.
	//HAL_Time_Sleep_MicroSeconds_InterruptEnabled(100);
	//while(fpga_cmd52_r(1, 0x62) & 0x2) { ; } // ADC_STAT busy bit

	// Read two with two commands. Slower but easiest. Not used.
	//ret[0]  =  fpga_cmd52_r(1, 0x64);		// ADC_LAST_LO
	//ret[1]  =  fpga_cmd52_r(1, 0x65);		// ADC_LAST_HI

	// Just checking the two regs does not work. Don't know why.
	//fpga_cmd53_r(1, 0x64, ret, 2);

	// Pull 4 regs at once, check busy bit, throw out middle, use last two (the sample)
	do {
		fpga_cmd53_r(1, 0x62, ret, 4);
	}
	while(ret[0] & 0x2); // Check ADC_BUSY bit
	// In practice, we never need to loop.
	// Well, at -O0 we didn't, maybe we do otherwise
	
	//hal_printf("%d\r\n", ret[2] | (ret[3]<<8));

	return ret[2] | (ret[3]<<8);
}

// Setup and start continuous mode.
UINT8 fpga_adc_cont_start(UINT8 chan, UINT8 sample_rate_ticks){
}

// Stop continuous mode.
// A particular case of fpga_adc_cmd(), only provided for convenience and to match cont_start.
INT8 fpga_adc_cont_stop(void){
}

// Read a set number of samples
// TODO: needs refreshed
INT32 fpga_read_batch(UINT16 samples[], int channel, UINT32 NumSamples, UINT32 SamplingTime){
	int i;
	uint8_t chan = (UINT8)(channel & 0xFF);

	do {
	samples[i++] = fpga_adc_now(chan);
	HAL_Time_Sleep_MicroSeconds_InterruptEnabled(SamplingTime);
	} while (i<NumSamples);

	return 0;
}

// Returns max sample rate in Hz.
UINT32 fpga_adc_get_bounds(void){
	return 1000;
}

#pragma GCC reset_options