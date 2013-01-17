/*
 * 	hal_dac_driver.h
 *
 *  Modified on: Sep 15, 2011
 *	Author: Sandip Bapat
 */

#ifndef HAL_DAC_DRIVER_H_
#define HAL_DAC_DRIVER_H_

#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include <misc/misc.h>
#include "stm32f10x_dac.h"


void hal_dac_init();
uint8_t hal_dac_set(uint16_t val);

#endif /* HAL_DAC_DRIVER_H_ */
