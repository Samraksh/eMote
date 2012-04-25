/*
 *  Name : TIAM3517_GPMC.cpp
 *
 *  Description : contains the core functions of the gpmc driver
 *
 *  History : v0.0 - Seminal Version - nived.sivadas@samraksh.com
 *
 */

#include "TIAM3517_GPMC.h"

static TIAM3517_GPMC &GPMC() { return *(TIAM3517_GPMC *) (size_t) (TIAM3517_GPMC::c_Base);}

BOOL TIAM3517_GPMC_Driver::Initialize()
{
	TIAM3517_GPMC &MC = GPMC();

	volatile UINT32 sysconfig = MC.GPMC_SYSCONFIG;

	//set smart idle mode and automatic l3 clock gating
	sysconfig &= 0x03 << 3;

	sysconfig |= (0x02 << 3) | (1 << 0);

	MC.GPMC_SYSCONFIG = sysconfig;





}
