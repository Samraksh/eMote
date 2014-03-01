#ifndef __NETMFSPI_H__
#define __NETMFSPI_H__


/*
REVISION LOG
------------

11/03/2010 Kartik Natarajan (KN) - Initial Create

*/

#include <tinyhal.h>
#include <stm32f2xx.h>
#include <rcc/stm32f2xx_rcc.h>
#include <gpio/stm32f2xx_gpio.h>
#include <spi/stm32f2xx_spi.h>

void GPIO_Config(const SPI_CONFIGURATION& Configuration);
void RCC_Config();
void NVIC_Config();
void SPI_StructInit(const SPI_CONFIGURATION& Configuration);


#endif
