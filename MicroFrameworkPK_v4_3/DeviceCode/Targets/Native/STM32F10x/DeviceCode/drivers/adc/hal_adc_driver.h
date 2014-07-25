/*
 * hal_adc_driver.h
 *
 *  Created on: Jan 7, 2011
 *      Author: Sandip
 */

#ifndef HAL_ADC_DRIVER_H_
#define HAL_ADC_DRIVER_H_

#include <stm32f10x.h>
//#include <gpio/stm32f10x_gpio.h>
//#include <rcc/stm32f10x_rcc.h>
#include <Timer/Timer16Bit/stm32f10x_tim.h>
//#include <dma/stm32f10x_dma.h>
//#include <misc/misc.h>
#include "stm32f10x_adc.h"
#include <intc/STM32.h>

void hal_adc_init(uint8_t sampTime);
uint8_t hal_adc_config();
uint8_t hal_adc_getData(uint16_t *dataBuf, uint8_t startChannel, uint8_t numChannels);
uint8_t hal_adc_getNow();


//#define NATIVE_TEST 1

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

#endif /* HAL_ADC_DRIVER_H_ */
