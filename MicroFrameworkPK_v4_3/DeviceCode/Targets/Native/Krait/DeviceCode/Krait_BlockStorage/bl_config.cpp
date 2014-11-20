////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

//--//


const BlockRange STM32F10x_BlockRange_InternalFlash[] =
{
    //
	{ BlockRange::BLOCKTYPE_BOOTSTRAP , 0 , 11 },
	    { BlockRange::BLOCKTYPE_CODE                    , 12,1999 },
	    { BlockRange::BLOCKTYPE_DEPLOYMENT              ,2000,2300 },// 0x805E8000
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,2300,2301 },
	    { BlockRange::BLOCKTYPE_STORAGE_B       ,2302,2302 },
	    { BlockRange::BLOCKTYPE_CONFIG          ,2303,2303 }
};

//--//

//--//

const BlockRegionInfo  STM32F10x_BlockRegionInfo_InternalFlash[1] =
{

		//0x88F00000,
		0x90000000,
			2304,
			0x800,
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
			0x4,
			0x10000000,
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

