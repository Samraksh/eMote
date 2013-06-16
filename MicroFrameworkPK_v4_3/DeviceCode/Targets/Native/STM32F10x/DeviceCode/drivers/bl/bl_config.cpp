////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

//--//


const BlockRange STM32F10x_BlockRange_InternalFlash[] =
{
    //
#if 0
	    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 63 }, 	//64 blocks, 128K for bootloader
	    { BlockRange::BLOCKTYPE_CODE  		    , 64,319 },		//256 blocks, 512K for CLR
	    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,320,383 },		//32 blocaks, 64K for appliation A,  address 0x80A0000
	    /*{ BlockRange::BLOCKTYPE_DEPLOYMENT      ,320,351 },		//32 blocaks, 64K for appliation A,  address 0x80A0000
	    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,352,383 },	*/	//32 blocaks, 64K for appliation B,  address 0x80B0000
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,384,447 },		//64 blocks, 128K for Storage A
	    { BlockRange::BLOCKTYPE_STORAGE_B       ,448,507 },		//60 blocks, 120K for Storage B
	    { BlockRange::BLOCKTYPE_CONFIG          ,508,511 } 		//4 blocks, 8K for Config
#endif

	    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 63 }, 	//64 blocks, 128K for bootloader
	    { BlockRange::BLOCKTYPE_CODE  		    , 64,324 },		//256 blocks, 512K for CLR
	    { BlockRange::BLOCKTYPE_CONFIG          ,325,327 },   //4 blocks, 8K for Config
	    { BlockRange::BLOCKTYPE_DEPLOYMENT      ,328,387 },  //32 blocaks, 64K for appliation A,  address 0x80A2000
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,388,451 },  //64 blocks, 128K for Storage A
	    { BlockRange::BLOCKTYPE_STORAGE_B       ,452,511 }  //60 blocks, 120K for Storage B

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

