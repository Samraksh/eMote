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

// FPGA GENERAL
INT8 fpga_init(){
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
	return 345;
}

// Setup and start continuous mode.
UINT8 fpga_adc_cont_start(UINT8 chan, UINT8 sample_rate_ticks){
}

// Stop continuous mode.
// A particular case of fpga_adc_cmd(), only provided for convenience and to match cont_start.
INT8 fpga_adc_cont_stop(void){
}

INT32 fpga_read_batch(UINT16 samples[], int channel, UINT32 NumSamples, UINT32 SamplingTime){
	samples[0] = 1;
	samples[1] = 2;
	samples[NumSamples - 1] =  3;

	return 0;
}

UINT32 fpga_adc_get_bounds(void){
	return 150000;
}
