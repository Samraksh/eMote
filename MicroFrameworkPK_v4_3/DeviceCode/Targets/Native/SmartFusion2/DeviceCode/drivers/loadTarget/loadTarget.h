#ifndef __NETMFSPI_H__
#define __NETMFSPI_H__

#include <tinyhal.h>
#include <cmsis/m2sxxx.h>
#include <drivers/mss_spi/mss_spi.h>

int loadArduinoSPI( uint8_t* address, uint16_t binarySize);

#endif
