////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "netmf_bl.h"

const BlockRange STM32F2xx_BlockRange_16KB_InternalFlash[] =
{
	    { BlockRange::BLOCKTYPE_CONFIG          ,0,0 },   //1 Sector, 16KB for Config
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,1,3 }  //3 blocks, 48K for Storage A
};

const BlockRange STM32F2xx_BlockRange_64KB_InternalFlash[] =
{
		{ BlockRange::BLOCKTYPE_DEPLOYMENT      ,0,0 }  //1 block, for 64K for appliation,  address 0x80A2000
};

const BlockRange STM32F2xx_BlockRange_128KB_InternalFlash[] =
{
	    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 0 }, 	//1 blocks, 128K for bootloader
	    { BlockRange::BLOCKTYPE_CODE  		    , 1,5 },	//5 blocks, 640KB for CLR
	    { BlockRange::BLOCKTYPE_STORAGE_B       , 6,6 }  //1 blocks, 128K for Storage B
};

/*
const BlockRange STM32F2xx_BlockRange_InternalFlash[3] =
{
	{
		{ BlockRange::BLOCKTYPE_CONFIG          ,0,0 },   //1 Sector, 16KB for Config
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,1,3 }  //3 blocks, 48K for Storage A
	},
	{
		{ BlockRange::BLOCKTYPE_DEPLOYMENT      ,0,0 }  //1 block, for 64K for appliation,  address 0x80A2000
	},
	{
	    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0, 0 }, 	//1 blocks, 128K for bootloader
	    { BlockRange::BLOCKTYPE_CODE  		    , 1,5 },	//5 blocks, 640KB for CLR
	    { BlockRange::BLOCKTYPE_STORAGE_B       , 6,6 }  //1 blocks, 128K for Storage B
	}
};
*/
//The F2xx has 3 different size sectors.
// 4 16KB sectors, 1 64KB sector and 7 128KB sectors
const BlockRegionInfo  STM32F2xx_BlockRegionInfo_InternalFlash[3] =
{
    {
    	0x08000000,		// ByteAddress     Address;            // Start address
    	4,			// UINT32          NumBlocks;          // total number of blocks in this region
    	0x4000,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    	ARRAYSIZE_CONST_EXPR(STM32F2xx_BlockRange_16KB_InternalFlash),
    	STM32F2xx_BlockRange_16KB_InternalFlash,
    },
    {
      	0x08010000,		// ByteAddress     Address;            // Start address
       	1,			// UINT32          NumBlocks;          // total number of blocks in this region
       	0x10000,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

       	ARRAYSIZE_CONST_EXPR(STM32F2xx_BlockRange_64KB_InternalFlash),
       	STM32F2xx_BlockRange_64KB_InternalFlash,
    },
    {
       	0x08020000,		// ByteAddress     Address;            // Start address
       	7,			// UINT32          NumBlocks;          // total number of blocks in this region
       	0x20000,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

       	ARRAYSIZE_CONST_EXPR(STM32F2xx_BlockRange_128KB_InternalFlash),
       	STM32F2xx_BlockRange_128KB_InternalFlash,
    }


};


BlockDeviceInfo STM32F2xx_BlockDeviceInfo_InternalFlash =
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

    3,											// UINT32 NumRegions;
    STM32F2xx_BlockRegionInfo_InternalFlash,	// const BlockRegionInfo* pRegions;
};

BLOCK_CONFIG STM32F2xx_blConfig_InternalFlash =
{
	GPIO_PIN_NONE,
	FALSE,
    &STM32F2xx_BlockDeviceInfo_InternalFlash		// BlockDeviceinfo
};

//--//

struct BlockStorageDevice STM32F2xx_BlockStorageDevice_InternalFlash;


//--//

