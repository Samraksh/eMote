////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

//--//


const BlockRange STM32F10x_BlockRange_InternalFlash[] =
{
#if defined(TARGETLOCATION_GMEM)
    //Block size 2K = 352 blocks to play with from 0x2E00_0000 to 0x2E0A_FFFF
	/*{ BlockRange::BLOCKTYPE_BOOTSTRAP       ,   0,   0 },*/ //IMEM base is 0x2E00_0000 /*TODO: don't know if BLOCKTYPE_BOOTSTRAP is required...*/
	{ BlockRange::BLOCKTYPE_CODE            ,   0, 329 }, //IMEM: 0x2E00_0000 to GMEM: 0x2E0A_4FFF (inclusive)
	{ BlockRange::BLOCKTYPE_DEPLOYMENT      , 330, 348 }, //GMEM: 0x2E0A_5000 to GMEM: 0x2E0A_E7FF //DDR: deployment at 0x805E8000
	{ BlockRange::BLOCKTYPE_STORAGE_A       , 349, 349 }, //GMEM: 0x2E0A_E800 to GMEM: 0x2E0A_EFFF
	{ BlockRange::BLOCKTYPE_STORAGE_B       , 350, 350 }, //GMEM: 0x2E0A_F000 to GMEM: 0x2E0A_F7FF
	{ BlockRange::BLOCKTYPE_CONFIG          , 351, 351 }  //GMEM: 0x2E0A_F800 to GMEM: 0x2E0A_FFFF
#else
    //
       { BlockRange::BLOCKTYPE_BOOTSTRAP , 0 , 11 },
           { BlockRange::BLOCKTYPE_CODE                    , 12,1999 },
           { BlockRange::BLOCKTYPE_DEPLOYMENT              ,2000,2300 },// 0x805E8000
           { BlockRange::BLOCKTYPE_STORAGE_A       ,2300,2301 },
           { BlockRange::BLOCKTYPE_STORAGE_B       ,2302,2302 },
           { BlockRange::BLOCKTYPE_CONFIG          ,2303,2303 }
#endif
};

//--//

//--//

const BlockRegionInfo  STM32F10x_BlockRegionInfo_InternalFlash[1] =
{
#if defined(TARGETLOCATION_GMEM)
			0x2e000000/*MIMEMandGMEM*/,//0x80200000/*release*/,0x88F00000/*original dead code?*/,
			352 /*total blocks*/,
			0x800/*2KB block size*/,
#else
               //0x88F00000,
               0x80200000,
                       2304,
                       0x800,
#endif
			ARRAYSIZE_CONST_EXPR(STM32F10x_BlockRange_InternalFlash),
			STM32F10x_BlockRange_InternalFlash,
};

//--//


BlockDeviceInfo STM32F10x_BlockDeviceInfo_InternalFlash =
{
		{
					FALSE,
					TRUE,
					TRUE,
			},
			9600,
			4800,
#if defined(TARGETLOCATION_GMEM)
			0x4/*32-bit sectors*/,
			0xB0000,
#else
                       0x4,
                       0x10000000,
#endif
			1,
			STM32F10x_BlockRegionInfo_InternalFlash,
};

BLOCK_CONFIG STM32F10x_blConfig_InternalFlash =
{
	GPIO_PIN_NONE,
	FALSE,
    &STM32F10x_BlockDeviceInfo_InternalFlash		// BlockDeviceinfo
};

//--//

struct BlockStorageDevice STM32F10x_BlockStorageDevice_InternalFlash;

//--//

