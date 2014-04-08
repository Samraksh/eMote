////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

//--//


const BlockRange STM32F10x_BlockRange_InternalFlash[] =
{
        // DANGER! Coordinate with applications (TinyBooter and MFUpdate).
        // Always update comments, fix scatterfile targets, and tell everyone on the team before changing the map.
        // It should be obvious that block ranges are inclusive, and different regions have exclusive ranges.
	    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 63 },  // 64 blocks, 128K for Booter,     address 0x0800_0000
	    { BlockRange::BLOCKTYPE_CODE            , 64,385 },  //322 blocks, 644K for CLR,        address 0x0802_0000
	    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,386,449 },  // 64 blocks, 128K for Deployment, address 0x080C_1000
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,450,509 },  // 60 blocks, 120K for Storage A,  address 0x080E_1000
	    { BlockRange::BLOCKTYPE_CONFIG          ,510,511 }   //  2 blocks,   4K for Config,     address 0x080F_F000

};

//--//

//--//

const BlockRegionInfo  STM32F10x_BlockRegionInfo_InternalFlash[1] =
{

    0x08000000,		// ByteAddress     Address;            // Start address
    512,			// UINT32          NumBlocks;          // total number of blocks in this region
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

