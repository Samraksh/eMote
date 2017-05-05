////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2007-2015 The Samraksh Company.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

//--//


const BlockRange STM32F10x_BlockRange_InternalFlash[] =
{
        // DANGER! Coordinate with applications (TinyBooter and MFUpdate).
        // Always update comments, fix scatterfile targets, and tell everyone on the team before changing the map.
        // It should be obvious that block ranges are inclusive, and different regions have exclusive ranges.
        // TODO: check if User Storage B region is needed for EWR Extended Weak References to work (eg, for wear-leveling).

	   // { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 63 },  // 64 blocks, 128K for Booter,     address 0x0000_0000
	    { BlockRange::BLOCKTYPE_CODE            , 0,200 },  //201 blocks, 400K for CLR,        address 0x0000_0000
	    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,201,253 },  // 53 blocks, 108K for Deployment, address 0x06_4000
	    { BlockRange::BLOCKTYPE_CONFIG          ,254,255 }   //  2 blocks,   4K for Config,     address 0x07_F000
	    //Flash ends at address 0x07_FFFF
};

//--//

//--//

const BlockRegionInfo  STM32F10x_BlockRegionInfo_InternalFlash[1] =
{

    0x60000000,		// ByteAddress     Address;            // Start address
    256,			// UINT32          NumBlocks;          // total number of blocks in this region
    0x800,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(STM32F10x_BlockRange_InternalFlash),
    STM32F10x_BlockRange_InternalFlash,
};

//--//

BlockDeviceInfo STM32F10x_BlockDeviceInfo_InternalFlash =
{
    {											// Attributes
        FALSE,									// BOOL Removable;
        TRUE,									// BOOL SupportsXIP;
        TRUE,									// BOOL WriteProtected
    },
    140,										// MaxSectorWrite_uSec (70us * 2)
    40000,										// MaxBlockErase_uSec (40ms)
    0x4,										// BytesPerSector
    0x80000,									// Size;
    1,											// NumRegions;
    STM32F10x_BlockRegionInfo_InternalFlash,	// pointer to an array (NumRegions long) of region information
};

/*
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
*/

BLOCK_CONFIG STM32F10x_blConfig_InternalFlash =
{
	GPIO_PIN_NONE,
	FALSE,
    &STM32F10x_BlockDeviceInfo_InternalFlash		// BlockDeviceinfo
};

//--//

struct BlockStorageDevice STM32F10x_BlockStorageDevice_InternalFlash;


//--//

