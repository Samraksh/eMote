/*  Name : Krait_FPGA.cpp
 *
 *  Author : Christopher Morse
 *
 *  Description :
 *
 */
#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait_FPGA.h"

// FPGA GENERAL
void fpga_init(){
}

// ADC REG COMMANDS

// ADC configuration register.
void fpga_adc_conf(uint8_t incc, uint8_t ref, uint8_t bw){
}

// ADC command register
void fpga_adc_cmd(uint8_t chan, uint8_t cmd){
}

// ADC status register
uint8_t fpga_adc_get_stat(void){
}

// Sets sample rate in increments of 13.33us (256 clocks @ 19.2 MHz).
void fpga_adc_sample_rate(uint8_t sample_rate_ticks){
}

// Returns the last ADC immediate mode sample stored
uint16_t fpga_adc_get_sample(void){
}

// Bit vector for enabling ADC inputs 0-7. Auto mode only.
void fpga_adc_set_chan(uint8_t chan){
}

// raw access mode. write_mode = 0. 'offset' is the offset of adc register from module base.
uint8_t fpga_adc_raw(uint8_t offset, uint8_t rw, uint8_t data){
}

// Get continuous mode samples (multiple). NOTE: Undocumented, making guesses, may change
// Return: Number of samples available (including what was returned)
// buffer: array to place samples (in event of multiple channels, user must sort out).
// toRead: Maximum number of samples to place into buffer (e.g. buffer size)
uint16_t fpga_adc_cont_get(uint16_t *buffer, uint8_t toRead){
}

// ADC HIGHER LEVEL COMMANDS (composed of above for the user/speed).
// Grab one sample on one channel and blocks until returned.
uint16_t fpga_adc_now(uint8_t chan){
}

// Setup and start continuous mode.
uint8_t fpga_adc_cont_start(uint8_t chan, uint8_t sample_rate_ticks){
}

// Stop continuous mode.
// A particular case of fpga_adc_cmd(), only provided for convenience and to match cont_start.
void fpga_adc_cont_stop(void){
}

