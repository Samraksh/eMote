/*
 * hal_adc_driver.h
 *
 *  Created on: Jan 7, 2011
 *      Author: Sandip
 */

#ifndef HAL_ADC_DRIVER_H_
#define HAL_ADC_DRIVER_H_

#include <gpio/stm32f10x_gpio.h>
#include <rcc/stm32f10x_rcc.h>
#include <misc/misc.h>
#include "stm32f10x_adc.h"

void hal_adc_init(uint8_t sampTime);
uint8_t hal_adc_config();
uint8_t hal_adc_getData(uint16_t *dataBuf, uint8_t startChannel, uint8_t numChannels);
uint8_t hal_adc_getNow();

#endif /* HAL_ADC_DRIVER_H_ */
