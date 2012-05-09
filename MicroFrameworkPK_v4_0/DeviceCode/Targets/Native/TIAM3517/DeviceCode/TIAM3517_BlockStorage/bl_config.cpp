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
	    { BlockRange::BLOCKTYPE_CODE                    , 12,3583 },
	    { BlockRange::BLOCKTYPE_DEPLOYMENT              ,3584,3784 },// 0x80700000
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,3784,3785 },
	    { BlockRange::BLOCKTYPE_STORAGE_B       ,3786,3786 },
	    { BlockRange::BLOCKTYPE_CONFIG          ,3787,3787 }
};

//--//

//--//

const BlockRegionInfo  STM32F10x_BlockRegionInfo_InternalFlash[1] =
{

		0x80000000,
			131072,
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

