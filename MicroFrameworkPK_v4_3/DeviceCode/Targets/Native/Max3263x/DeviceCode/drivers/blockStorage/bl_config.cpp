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

	    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 15 },  // 32 blocks, 256K for Booter,     address 0x0000_0000
	    { BlockRange::BLOCKTYPE_CODE            , 16,176 },  //161 blocks, 1288K for CLR,        address 0x0004 0000
	    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,177,226 },  // 50 blocks, 400K for Deployment, address 0x0018 2000
#if defined(SAMRAKSH_UPDATE_EXT )
	    { BlockRange::BLOCKTYPE_UPDATE          ,227,253 },  // 27 blocks, 216K for Updates,    address 0x001C 2000
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,254,255 },  //  2 blocks,   16K for Storage A,  address 0x001F 8000
#else
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,227,253 },  // 29 blocks, 232K for Storage A,  address 0x001F 8000
#endif
	    //{ BlockRange::BLOCKTYPE_CONFIG          ,254,255 }   //  2 blocks,   16K for Config,     address 0x001F C000


};

//--//

//--//

const BlockRegionInfo  Max3263x_BlockRegionInfo_InternalFlash[1] =
{

    0x00000000,		// ByteAddress     Address;            // Start address
    256,			// UINT32          NumBlocks;          // total number of blocks in this region
    0x2000,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

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


BLOCK_CONFIG Max3263x_blConfig_InternalFlash =
{
	GPIO_PIN_NONE,
	FALSE,
    &Max3263x_BlockDeviceInfo_InternalFlash		// BlockDeviceinfo
};

//--//

struct BlockStorageDevice Max3263x_BlockStorageDevice_InternalFlash;


//--//

