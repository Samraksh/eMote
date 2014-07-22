/*
 * Krait_ADC.h
 *
 *  Created on: Feb 18, 2014
 *      Author: Christopher Morse
 */

#ifndef KRAIT_ADC_H
#define KRAIT_ADC_H

#define FPGA_SCLK_PIN 79
#define FPGA_MISO_PIN 82
#define FPGA_MOSI_PIN 77
#define FPGA_CS_PIN   78

#define FPGA_CS(x) (gpio_set(FPGA_CS_PIN, (x) ? 2 : 0))

#ifdef FPGA_O3_BUILD
#define SAM_FPGA_SPI_CLK_A() __asm volatile ("nop")
#define SAM_FPGA_SPI_CLK_B() __asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n")
#else
#define SAM_FPGA_SPI_CLK_A() __asm volatile ("nop")
#define SAM_FPGA_SPI_CLK_B() __asm volatile ("nop")
#endif

// TODO: Get rid of timer and GPIO stuff
#define MSM_TMR_BASE        0x0200A000
#define MSM_DGT_BASE        (MSM_TMR_BASE + 0x24)
#define DGT_REG(off)        (MSM_DGT_BASE + (off))
#define DGT_ENABLE           DGT_REG(0x0008)
#define DGT_CLK_CTL          DGT_REG(0x0010)
#define DGT_MATCH_VAL        DGT_REG(0x0000)
#define DGT_CLEAR            DGT_REG(0x000C)
#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

// GPIO TLMM: Direction
#define GPIO_INPUT      0
#define GPIO_OUTPUT     1

// GPIO TLMM: Pullup/Pulldown
#define GPIO_NO_PULL    0
#define GPIO_PULL_DOWN  1
#define GPIO_KEEPER     2
#define GPIO_PULL_UP    3

// GPIO TLMM: Drive Strength
#define GPIO_2MA        0
#define GPIO_4MA        1
#define GPIO_6MA        2
#define GPIO_8MA        3
#define GPIO_10MA       4
#define GPIO_12MA       5
#define GPIO_14MA       6
#define GPIO_16MA       7

// GPIO TLMM: Status
#define GPIO_ENABLE     1
#define GPIO_DISABLE    0

INT8 fpga_init();

void fpga_gpio_set(UINT32 gpio, UINT32 dir);

// ADC REG COMMANDS

// ADC configuration register.
INT8 fpga_adc_conf(UINT8 incc, UINT8 ref, UINT8 bw);

// ADC command register
INT8 fpga_adc_cmd(UINT8 chan, UINT8 cmd);

// ADC status register
INT8 fpga_adc_get_stat(void);

// Sets sample rate in increments of 13.33us (256 clocks @ 19.2 MHz).
INT8 fpga_adc_sample_rate(UINT8 sample_rate_ticks);

// Returns the last ADC immediate mode sample stored
UINT16 fpga_adc_get_sample(void);

// Bit vector for enabling ADC inputs 0-7. Auto mode only.
INT8 fpga_adc_set_chan(UINT8 chan);

// raw access mode. write_mode = 0. 'offset' is the offset of adc register from module base.
UINT8 fpga_adc_raw(UINT8 offset, UINT8 rw, UINT8 data);

// Get continuous mode samples (multiple). NOTE: Undocumented, making guesses, may change
// Return: Number of samples available (including what was returned)
// buffer: array to place samples (in event of multiple channels, user must sort out).
// toRead: Maximum number of samples to place into buffer (e.g. buffer size)
UINT16 fpga_adc_cont_get(UINT16 *buffer, UINT8 toRead);

// ADC HIGHER LEVEL COMMANDS (composed of above for the user/speed).
// Grab one sample on one channel and blocks until returned.
UINT16 fpga_adc_now(UINT8 chan);

// Setup and start continuous mode.
UINT8 fpga_adc_cont_start(UINT8 chan, UINT8 sample_rate_ticks);

// Stop continuous mode.
// A particular case of fpga_adc_cmd(), only provided for convenience and to match cont_start.
INT8 fpga_adc_cont_stop(void);

INT32 fpga_read_batch(UINT16 samples[], int channel, UINT32 NumSamples, UINT32 SamplingTime);

UINT32 fpga_adc_get_bounds(void);

#endif

