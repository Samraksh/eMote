/*
 * Krait_FPGA.h
 *
 *  Created on: Feb 18, 2014
 *      Author: Christopher Morse
 */

#ifndef KRAIT_FPGA_H
#define KRAIT_FPGA_H

#define FPGA_CS(x) (gpio_set(78, (x) ? 2 : 0))

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

UINT8 fpga_init();

// ADC REG COMMANDS

// ADC configuration register.
void fpga_adc_conf(UINT8 incc, UINT8 ref, UINT8 bw);

// ADC command register
void fpga_adc_cmd(UINT8 chan, UINT8 cmd);

// ADC status register
UINT8 fpga_adc_get_stat(void);

// Sets sample rate in increments of 13.33us (256 clocks @ 19.2 MHz).
void fpga_adc_sample_rate(UINT8 sample_rate_ticks);

// Returns the last ADC immediate mode sample stored
UINT16 fpga_adc_get_sample(void);

// Bit vector for enabling ADC inputs 0-7. Auto mode only.
void fpga_adc_set_chan(UINT8 chan);

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
void fpga_adc_cont_stop(void);


#endif

