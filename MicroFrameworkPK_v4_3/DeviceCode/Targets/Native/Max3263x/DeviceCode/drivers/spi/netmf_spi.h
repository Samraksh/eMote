#ifndef _MAX326_SPI_H_
#define _MAX326_SPI_H_

#include <tinyhal.h>
#include <spim.h>
#include <gpio\netmf_gpio.h>


enum SPIPort
{
	SPIPort_M0=0,
	SPIPort_M1=1,
	SPIPort_M2=2,
	SPIPort_B=3,
	SPI_MAX_PORTS=4
};


//This driver will suport a number of slave devices and this will be determined by their GPIO pins
//allocated by platform designer. Platform designer needs to specify the list of supported slave devices.
extern GPIO_PINS* SPI_SLAVES;
extern uint8_t SPI_SLAVE_COUNT;

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

struct Max326_Spi {
	bool driverInit;
};

#endif //_MAX326_SPI_H_
