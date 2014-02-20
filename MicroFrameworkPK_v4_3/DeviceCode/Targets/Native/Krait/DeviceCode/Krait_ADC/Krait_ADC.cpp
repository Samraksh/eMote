/*  Name : Krait_ADC.cpp
 *
 *  Author : Christopher Morse
 *
 *  Description :
 *
 */
#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait_ADC.h"

// Globals
uint8_t fpga_init_done;

// Statics

static void udelay(unsigned usecs)
{
	usecs = (usecs * 33 + 1000 - 33) / 1000;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < usecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}

static void mdelay(unsigned int msecs)
{
	msecs *= 33;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < msecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}

static void gpio_set(UINT32 gpio, UINT32 dir)
{
	UINT32 *addr = (UINT32 *)GPIO_IN_OUT_ADDR(gpio);
	writel(dir, addr); // second bit controls GPIO
	return;
}

static uint8_t sam_fpga_spi(uint8_t data) {
    uint8_t bit = 0x80;
    uint8_t ret = 0;
    const uint8_t d = 10;
    const uint8_t miso_pin = 82;
    const uint8_t clk_pin = 79;
    const uint8_t mosi_pin = 77;

    // MISO input
    unsigned int *addr = (unsigned int *)GPIO_IN_OUT_ADDR(miso_pin);

    while (bit) {
        gpio_set(mosi_pin, (data&bit) ? 2 : 0);     // Set output
        udelay(d);
        ret |= ( readl(addr)&1 ); // Latch input
        if(bit>1)
            ret = ret << 1;
        bit = bit >> 1;
        gpio_set(clk_pin, 2); // clock high
        udelay(d);
        gpio_set(clk_pin, 0); // clock low
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

// CMD52, read a register.
static uint8_t fpga_cmd52_r(uint8_t func, uint8_t reg) {

	uint8_t resp, timeout, extra;
	uint8_t spi_send[6] = {0x74, 0x04, 0x00, 0x01, 0x00, 0xFF};
	
	// Function area, 0 or 1
	spi_send[1] |= (func << 4);
	
	// Register address, largest is 0x66 so we only need to worry about bottom byte
	spi_send[3] |= (reg << 1);
	
	FPGA_CS(0);
	mdelay(1);
	for(int i=0; i<6; i++) {
		timeout=0xF;
		sam_fpga_spi(spi_send[i]);
	}
	//dprintf(INFO, "waiting for response\n");
	do {
		resp = sam_fpga_spi(0xFF);
		timeout--;
	} while(resp == 0xFF && timeout > 0);

	// Get returned data
	extra = sam_fpga_spi(0xFF);

	sam_fpga_spi(0xFF); // cleanup

	//dprintf(INFO, "Got CMD52 resp 0x%X timeout=%d\n", resp, timeout);
	//dprintf(INFO, "Got CMD52 data 0x%X\n", extra);
	FPGA_CS(1);
	
	return extra;
}

// CMD52, write to a register.
static uint8_t fpga_cmd52_w(uint8_t func, uint8_t reg, uint8_t data) {

	uint8_t resp, timeout, extra;
	uint8_t spi_send[6] = {0x74, 0x84, 0x00, 0x01, 0x00, 0xFF};
	
	// Data to write
	spi_send[4] |= data;
	
	// Function area, 0 or 1
	spi_send[1] |= (func << 4);
	
	// Register address, largest is 0x66 so we only need to worry about bottom byte
	spi_send[3] |= (reg << 1);
	
	FPGA_CS(0);
	mdelay(1);
	for(int i=0; i<6; i++) {
		timeout=0xF;
		sam_fpga_spi(spi_send[i]);
	}
	//dprintf(INFO, "waiting for response\n");
	do {
		resp = sam_fpga_spi(0xFF);
		timeout--;
	} while(resp == 0xFF && timeout > 0);

	// Get returned data
	extra = sam_fpga_spi(0xFF);

	sam_fpga_spi(0xFF); // cleanup

	//dprintf(INFO, "Got CMD52 resp 0x%X timeout=%d\n", resp, timeout);
	//dprintf(INFO, "Got CMD52 data 0x%X\n", extra);
	FPGA_CS(1);
	
	return extra;
}

// FPGA GENERAL
// Functions
INT8 fpga_init() {
	if (fpga_init_done) return 0; // Already started
	fpga_init_done = 1;
	
	// FPGA power state signaling pins
	gpio_tlmm_config(90, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);
	gpio_tlmm_config(91, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE);

	gpio_set(90, 2); // ON/OFF, set ON (1)
	gpio_set(91, 0); // Asleep/Awake, set Awake (0)
	//dprintf(INFO, "Pwr Cfg pins set\n");

	// Set SDIO pins into SPI comm mode
	gpio_tlmm_config(82, 0, GPIO_INPUT,  GPIO_NO_PULL, GPIO_2MA, GPIO_DISABLE); // MISO
	gpio_tlmm_config(78, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE); // CS
	gpio_tlmm_config(79, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE); // SCLK
	gpio_tlmm_config(77, 0, GPIO_OUTPUT, GPIO_NO_PULL, GPIO_2MA, GPIO_ENABLE); // MOSI
	gpio_set(79, 0);
	gpio_set(78, 2);
	
	FPGA_CS(1); // unassert at startup
	
	for(int i=0; i<10; i++) { sam_fpga_spi(0xFF); }
	mdelay(10);
	
	
	{ // Send CMD0
		uint8_t resp, timeout;
		uint8_t spi_send[6] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
		//dprintf(INFO, "sending CMD0\n");
		FPGA_CS(0); // assert
		mdelay(1);
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

		//dprintf(INFO, "Got CMD0 resp 0x%X timeout=%d\n", resp, timeout);
		FPGA_CS(1);
		
		if (timeout == 0)
			return 1;
	}
	
	
	{ // CMD59, disable CRC
		uint8_t resp, timeout;
		uint8_t spi_send[6] = {0x7B, 0x00, 0x00, 0x00, 0x00, 0x8B};
		//dprintf(INFO, "sending CMD59\n");
		FPGA_CS(0); // assert
		mdelay(1);
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

		//dprintf(INFO, "Got CMD59 resp 0x%X timeout=%d\n", resp, timeout);
		FPGA_CS(1);
		
		if (timeout == 0)
			return 1;
	}
	
        
    { // CMD5, 4 extra bytes
        uint8_t resp, timeout, extra[4];
        uint8_t spi_send[6] = {0x45, 0x00, 0x00, 0x00, 0x00, 0x87};
        //dprintf(INFO, "sending CMD5\n");
        FPGA_CS(0);
        mdelay(1);
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

        //dprintf(INFO, "Got CMD5 resp 0x%X timeout=%d\n", resp, timeout);
        //for(int i=0; i<4; i++)
            //dprintf(INFO, "%X", extra[i]);
        //dprintf(INFO, "\n");
        FPGA_CS(1);
		
		if (timeout == 0)
			return 1;
    }
	
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
	
	UINT8 data = (0x07 & chan) + 0xF0;
	fpga_cmd52_w(1, 0, data); // set channel and ADC_CONF register
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

	UINT16 ret;
	UINT8 cmd = (chan & 0x03) + (2 << 5);
	fpga_cmd52_w(1, 0x61, cmd); // Use immediate mode
	
	while(fpga_cmd52_r(1, 0x62) & 0x1 == 1) { ; } // block waiting for sample
	
	ret  =  fpga_cmd52_r(1, 0x64);
	ret |= (fpga_cmd52_r(1, 0x65) << 8);
	
	return ret;
}

// Setup and start continuous mode.
UINT8 fpga_adc_cont_start(UINT8 chan, UINT8 sample_rate_ticks){
}

// Stop continuous mode.
// A particular case of fpga_adc_cmd(), only provided for convenience and to match cont_start.
INT8 fpga_adc_cont_stop(void){
}

// Read a set number of samples
INT32 fpga_read_batch(UINT16 samples[], int channel, UINT32 NumSamples, UINT32 SamplingTime){
	
	int i;
	uint8_t chan = (UINT8)(channel & 0xFF);
	
	do {
	samples[i++] = fpga_adc_now(chan);
	udelay(SamplingTime);
	} while (i<NumSamples);

	return 0;
}

// Returns max sample rate in Hz. 1000 for now to be conservative.
UINT32 fpga_adc_get_bounds(void){
	return 1000;
}
