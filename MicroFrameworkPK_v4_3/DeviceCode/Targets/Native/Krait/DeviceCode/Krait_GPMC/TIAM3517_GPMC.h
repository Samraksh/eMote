/*
 *  Name : TIAM3517_GPMC.h
 *
 *  Author : nived.sivadas@samraksh.com
 *
 *  Description : This file contains the register mapping for the gpmc for the soc8200 board running the TIAM3517 processor.
 *
 *  History     : v0.0 - Seminal Version by nived.sivadas
 */


#ifndef _TIAM3517_GPMC_H_
#define _TIAM3517_GPMC_H_

#include "..\TIAM3517.h"

#define GPMC_SYSCONFIG_SOFTRESET (1 << 1)
#define GPMC_SYSSTATUS_RESETCOMPLETE 0x1
#define GPMC_SYSSTATUS_RESETONGOING 0x0

// defines the start and end of the gpmc address space
#define GPMC_MEM_START		0x00000000
#define GPMC_MEM_END		0x3FFFFFFF
#define GPMC_CONFIG7_CSVALID		(1 << 6)
#define GPMC_MAX_REG	7

// leave about 1MB of the address space for Boot ROM
#define BOOT_ROM_SPACE		0x100000
#define NUM_MEM_REGIONS 2

#define STNOR_GPMC_CONFIG1    0x00001210
#define STNOR_GPMC_CONFIG2    0x00101001
#define STNOR_GPMC_CONFIG3    0x00020201
#define STNOR_GPMC_CONFIG4    0x0f031003
#define STNOR_GPMC_CONFIG5    0x000f1111
#define STNOR_GPMC_CONFIG6    0x0f030080

#define GPMC_SIZE_16M	0xF

// This structures is mimicked from linux, seems like the node of a memory tree at this point
struct resource{

	UINT32 start;
	UINT32 end;
	const char* name;
	unsigned long flags;
	struct resource *parent, *sibling, *child;

};

static const UINT32 gpmc_nor[GPMC_MAX_REG] = {
		STNOR_GPMC_CONFIG1,
		STNOR_GPMC_CONFIG2,
		STNOR_GPMC_CONFIG3,
		STNOR_GPMC_CONFIG4,
		STNOR_GPMC_CONFIG5,
		STNOR_GPMC_CONFIG6, 0

};


struct GPMC_CS_CONFIG
{
	volatile UINT32 GPMC_CONFIG1;
	volatile UINT32 GPMC_CONFIG2;
	volatile UINT32 GPMC_CONFIG3;
	volatile UINT32 GPMC_CONFIG4;
	volatile UINT32 GPMC_CONFIG5;
	volatile UINT32 GPMC_CONFIG6;
	volatile UINT32 GPMC_CONFIG7;
	volatile UINT32 GPMC_NAND_COMMAND;
	volatile UINT32 GPMC_NAND_ADDRESS;
	volatile UINT32 GPMC_NAND_DATA;
	volatile UINT32 temp1[2];
};

struct GPMC_BCH
{
	volatile UINT32 GPMC_BCH_RESULT0;
	volatile UINT32 GPMC_BCH_RESULT1;
	volatile UINT32 GPMC_BCH_RESULT2;
	volatile UINT32 GPMC_BCH_RESULT3;
};

struct TIAM3517_GPMC
{
	static const UINT32 c_Base = 0x6e000000;

	volatile UINT32 GPMC_REVISION;
	volatile UINT32 TEMP1[3];
	volatile UINT32 GPMC_SYSCONFIG;
	volatile UINT32 GPMC_SYSSTATUS;
	volatile UINT32 GPMC_IRQSTATUS;
	volatile UINT32 GPMC_IRQENABLE;
	volatile UINT32 TEMP2[8];
	volatile UINT32 GPMC_TIMEOUT_CONTROL;
	volatile UINT32 GPMC_ERR_ADDRESS;
	volatile UINT32 GPMC_ERR_TYPE;
	volatile UINT32 TEMP3[1];
	volatile UINT32 GPMC_CONFIG;
	volatile UINT32 GPMC_STATUS;
	volatile UINT32 TEMP4[2];
	volatile GPMC_CS_CONFIG CS_CONTEXT[8];
	volatile UINT32 GPMC_PREFETCH_CONFIG1;
	volatile UINT32 GPMC_PREFETCH_CONFIG2;
	volatile UINT32 TEMP5;
	volatile UINT32 GPMC_PREFETCH_CONTROL;
	volatile UINT32 GPMC_PREFETCH_STATUS;
	volatile UINT32 GPMC_ECC_CONFIG;
	volatile UINT32 GPMC_ECC_CONTROL;
	volatile UINT32 GPMC_ECC_SIZE_CONFIG;
	volatile UINT32 GPMC_ECC1_RESULT;
	volatile UINT32 GPMC_ECC2_RESULT;
	volatile UINT32 GPMC_ECC3_RESULT;
	volatile UINT32 GPMC_ECC4_RESULT;
	volatile UINT32 GPMC_ECC5_RESULT;
	volatile UINT32 GPMC_ECC6_RESULT;
	volatile UINT32 GPMC_ECC7_RESULT;
	volatile UINT32 GPMC_ECC8_RESULT;
	volatile UINT32 GPMC_ECC9_RESULT;
	volatile UINT32 TEMP6[7];
	volatile GPMC_BCH GPMC_BCH_RESULT[8];
	volatile UINT32 TEMP7[4];
	volatile UINT32 GPMC_BCH_SWDATA;

};

struct TIAM3517_GPMC_Driver
{
	static BOOL Initialize();



};



#endif