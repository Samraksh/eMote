/*
 *  Name : bl_nor_config.cpp
 *
 *  Author : Nived.Sivadas@samraksh.com
 *
 *  Description : Holds the configuration for the external nor flash for both the PLATFORM_ARM_EmoteDotNow and the stm dev board
 */

#include <tinyhal.h>


const BlockRange STM32F10x_BlockRange_SDCARD[] =
{
#ifdef PLATFORM_ARM_EmoteDotNow
    { BlockRange::BLOCKTYPE_FILESYSTEM          ,0,0x200000 }
#else
    { BlockRange::BLOCKTYPE_FILESYSTEM          ,0,127 }
#endif
};


const BlockRegionInfo  STM32F10x_BlockRegionInfo_SDCARD[1] =
{

	0x0,		// ByteAddress     Address;            // Start address
#ifdef PLATFORM_ARM_EmoteDotNow
    //0x1000,
    //0x1000000,
	0x200000,		// UINT32          NumBlocks;          // total number of blocks in this region
	0x1000,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)
					//Don't use 0x200 for BytesPerBlock
#else
    128,			// UINT32          NumBlocks;          // total number of blocks in this region
    0x800,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)
#endif
    ARRAYSIZE_CONST_EXPR(STM32F10x_BlockRange_SDCARD),
    STM32F10x_BlockRange_SDCARD,
};



BlockDeviceInfo STM32F10x_BlockDeviceInfo_SDCARD =
{
    {
        FALSE,									// BOOL Removable;
        TRUE,									// BOOL SupportsXIP;
        TRUE,									// BOOL WriteProtected
    },

    34,										    // UINT32 Duration_Max_WordWrite_uSec;

    14800,										// UINT32 Duration_Max_SectorErase_uSec;
    0x1000,										// BytesPerSector;

    // The PLATFORM_ARM_EmoteDotNow has a 16MB Flash and the dev board has a 128 Mb Flash
#ifdef PLATFORM_ARM_EmoteDotNow
    0x200000000,
#else
    0x1000000,									// UINT32 Size;
#endif
    1,											// UINT32 NumRegions;
    STM32F10x_BlockRegionInfo_SDCARD,	// const BlockRegionInfo* pRegions;
};


BLOCK_CONFIG STM32F10x_blConfig_SDCARD =
{
	GPIO_PIN_NONE,
	FALSE,
    &STM32F10x_BlockDeviceInfo_SDCARD		// BlockDeviceinfo
};


//--//

//struct BlockStorageDevice STM32F10x_BlockStorageDevice_SDCARD;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "STM32F10x_BlockStorageDevice_SDCARD"
#endif

struct BlockStorageDevice STM32F10x_BlockStorageDevice_SDCARD;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata
#endif

//--//


//#pragma arm section code
//struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_SDCARD;
