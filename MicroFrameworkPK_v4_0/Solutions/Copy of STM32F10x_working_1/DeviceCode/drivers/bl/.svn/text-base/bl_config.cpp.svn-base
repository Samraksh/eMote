////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

//--//


const BlockRange STM32F10x_BlockRange_InternalFlash[] =
{
    //
    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 11 },
    { BlockRange::BLOCKTYPE_CODE			, 12,180 },
    { BlockRange::BLOCKTYPE_DEPLOYMENT		,181,252 },
    { BlockRange::BLOCKTYPE_STORAGE_A       ,253,253 },
    { BlockRange::BLOCKTYPE_STORAGE_B       ,254,254 },
    { BlockRange::BLOCKTYPE_CONFIG          ,255,255 }
};

//--//

//--//

const BlockRegionInfo  STM32F10x_BlockRegionInfo_InternalFlash[1] = 
{
    
    0x08000000,		// ByteAddress     Address;            // Start address
    256,			// UINT32          NumBlocks;          // total number of blocks in this region
    0x800,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(STM32F10x_BlockRange_InternalFlash),
    STM32F10x_BlockRange_InternalFlash,
};

//--//


BlockDeviceInfo STM32F10x_BlockDeviceInfo_InternalFlash =
{
    {
        FALSE,									// BOOL Removable;
        TRUE,									// BOOL SupportsXIP;
        TRUE,									// BOOL WriteProtected
    },
    
    9600,										// UINT32 Duration_Max_WordWrite_uSec; @todo
    4800,										// UINT32 Duration_Max_SectorErase_uSec; @todo
    0x4,										// BytesPerSector;

    0x80000,									// UINT32 Size;

    1,											// UINT32 NumRegions;
    STM32F10x_BlockRegionInfo_InternalFlash,	// const BlockRegionInfo* pRegions;
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

