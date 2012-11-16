/*
 *  Name : TIAM3517_GPMC.cpp
 *
 *  Description : contains the core functions of the gpmc driver
 *
 *  History : v0.0 - Seminal Version - nived.sivadas@samraksh.com
 *
 */

#include "TIAM3517_GPMC.h"

// creating a memory tree, other external memory controllers can be added to this tree
static struct resource gpmc_mem_root;

static TIAM3517_GPMC &GPMC() { return *(TIAM3517_GPMC *) (size_t) (TIAM3517_GPMC::c_Base);}


void CPU_GPMC_Initialize()
{
	TIAM3517_GPMC_Driver::Initialize();
}

BOOL TIAM3517_GPMC_Driver::Initialize()
{
	UINT32 cs;

	UINT32 boot_rom_space = 0;

	TIAM3517_GPMC &MC = GPMC();

	volatile UINT32 sysconfig = MC.GPMC_SYSCONFIG;

	//set smart idle mode and automatic l3 clock gating
	//sysconfig &= 0x03 << 3;

	//sysconfig |= (0x02 << 3) | (1 << 0);

	//MC.GPMC_SYSCONFIG = sysconfig;
	volatile UINT32 temp = MC.CS_CONTEXT[1].GPMC_CONFIG1;

	MC.CS_CONTEXT[1].GPMC_CONFIG1 = STNOR_GPMC_CONFIG1;

	temp = MC.CS_CONTEXT[1].GPMC_CONFIG1;

	MC.CS_CONTEXT[1].GPMC_CONFIG2 = STNOR_GPMC_CONFIG2;

	temp = MC.CS_CONTEXT[1].GPMC_CONFIG2;

	MC.CS_CONTEXT[1].GPMC_CONFIG3 = STNOR_GPMC_CONFIG3;

	temp = MC.CS_CONTEXT[1].GPMC_CONFIG3;

	MC.CS_CONTEXT[1].GPMC_CONFIG4 = STNOR_GPMC_CONFIG4;

	temp = MC.CS_CONTEXT[1].GPMC_CONFIG4;

	MC.CS_CONTEXT[1].GPMC_CONFIG5 = STNOR_GPMC_CONFIG5;

	temp = MC.CS_CONTEXT[1].GPMC_CONFIG5;

	MC.CS_CONTEXT[1].GPMC_CONFIG6 = STNOR_GPMC_CONFIG6;

	temp = MC.CS_CONTEXT[1].GPMC_CONFIG6;

	// BASE ADDRESS = 0x08000000
	// SIZE = 16M, even though the size of the memory is only 4 i am using 16 since it is the smallest size available
	MC.CS_CONTEXT[1].GPMC_CONFIG7 = (GPMC_SIZE_16M & 0xF) | ((0x08000000 >> 24) & 0x3F) | (1 << 6);


}
