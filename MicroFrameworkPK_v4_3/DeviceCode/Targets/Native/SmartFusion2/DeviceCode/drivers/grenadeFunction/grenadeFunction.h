#ifndef __GRENADEFUNCTION_H__
#define __GRENADEFUNCTION_H__

#include <tinyhal.h>
#include <cmsis/m2sxxx.h>
#include <drivers/mss_spi/mss_spi.h>

int codeIntegrityCheck(uint8_t* memory, uint32_t memorySize);

#endif
