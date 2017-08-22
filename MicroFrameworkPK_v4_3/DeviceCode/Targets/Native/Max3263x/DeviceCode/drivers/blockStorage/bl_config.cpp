////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2007-2015 The Samraksh Company.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

//--//


const BlockRange Max3263x_BlockRange_InternalFlash[] =
{
        // DANGER! Coordinate with applications (TinyBooter and MFUpdate).
        // Always update comments, fix scatterfile targets, and tell everyone on the team before changing the map.
        // It should be obvious that block ranges are inclusive, and different regions have exclusive ranges.
        // TODO: check if User Storage B region is needed for EWR Extended Weak References to work (eg, for wear-leveling).

	    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 63 },  // 64 blocks, 128K for Booter,     address 0x0800_0000
	    { BlockRange::BLOCKTYPE_CODE            , 64,385 },  //322 blocks, 644K for CLR,        address 0x0802_0000
	    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,386,449 },  // 64 blocks, 128K for Deployment, address 0x080C_1000
#if defined(SAMRAKSH_UPDATE_EXT )
	    { BlockRange::BLOCKTYPE_UPDATE          ,450,505 },  // 56 blocks, 112K for Updates,    address 0x080E_1000
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,506,509 },  //  4 blocks,   8K for Storage A,  address 0x080F_9000
#else
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,450,509 },  // 60 blocks, 120K for Storage A,  address 0x080E_1000
#endif
	    { BlockRange::BLOCKTYPE_CONFIG          ,510,511 }   //  2 blocks,   4K for Config,     address 0x080F_F000


};

//--//

//--//

const BlockRegionInfo  Max3263x_BlockRegionInfo_InternalFlash[1] =
{

    0x08000000,		// ByteAddress     Address;            // Start address
    512,			// UINT32          NumBlocks;          // total number of blocks in this region
    0x800,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(Max3263x_BlockRange_InternalFlash),
    Max3263x_BlockRange_InternalFlash,
};

//--//

BlockDeviceInfo Max3263x_BlockDeviceInfo_InternalFlash =
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
    Max3263x_BlockRegionInfo_InternalFlash,	// pointer to an array (NumRegions long) of region information
};

/*
BlockDeviceInfo Max3263x_BlockDeviceInfo_InternalFlash =
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
    Max3263x_BlockRegionInfo_InternalFlash,	// const BlockRegionInfo* pRegions;
};
*/

BLOCK_CONFIG Max3263x_blConfig_InternalFlash =
{
	GPIO_PIN_NONE,
	FALSE,
    &Max3263x_BlockDeviceInfo_InternalFlash		// BlockDeviceinfo
};

//--//

struct BlockStorageDevice Max3263x_BlockStorageDevice_InternalFlash;


//--//

