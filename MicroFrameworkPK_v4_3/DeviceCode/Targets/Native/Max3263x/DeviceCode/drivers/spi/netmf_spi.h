#ifndef _MAX326_SPI_H_
#define _MAX326_SPI_H_

#include <tinyhal.h>
#include <spim.h>
#include <gpio\netmf_gpio.h>

#define SPI_MAX_PORTS 3

enum SPIPort
{
	SPIPort_M0=0,
	SPIPort_M1=1,
	SPIPort_M2A=2, //M2 port Map_A pin mappings
	SPIPort_M2B=3, //M2 port Map_B pin mappings
	//SPIPort_B=3,

};

enum SPIPortWidth {
	PortM0_Width= SPIM_WIDTH_1,
	PortM1_Width= SPIM_WIDTH_1,
	PortM2_Width= SPIM_WIDTH_1,
};


//This driver will suport a number of slave devices and this will be determined by their GPIO pins
//allocated by platform designer. Platform designer needs to specify the list of supported slave devices.


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
