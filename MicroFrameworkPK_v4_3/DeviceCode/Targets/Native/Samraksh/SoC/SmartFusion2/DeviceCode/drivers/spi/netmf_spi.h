#ifndef __NETMFSPI_H__
#define __NETMFSPI_H__




#include <tinyhal.h>
#include <cmsis/m2sxxx.h>
#include <gpio\netmf_gpio.h>


enum SPIBUS
{
	SPIBUS1=1,
	SPIBUS2=2,
};

void GPIO_Config(const SPI_CONFIGURATION& Configuration);
void RCC_Config();
void NVIC_Config();
void SPI_StructInit(const SPI_CONFIGURATION& Configuration);

#if defined(DEBUG_SAM_SPI)
extern volatile UINT64 spiSpinWaitCount;
extern volatile UINT64 spiSpinWaitAvg;
extern volatile UINT64 spiSpinWaitMax;
extern volatile UINT64 spiSpinWaitFailureCount;
#endif
#endif
