/*
 *  Name : bl_nor_config.cpp
 *
 *  Author : Nived.Sivadas@samraksh.com
 *
 *  Description : Holds the configuration for the external nor flash for both the emote and the stm dev board
 */

#include <tinyhal.h>
#include <netmf_bl_nor.h>

const BlockRange STM32F10x_BlockRange_ExternalFlash_NOR[] =
{
#ifdef EMOTE
    { BlockRange::BLOCKTYPE_FILESYSTEM          ,0,255 }
#else
    { BlockRange::BLOCKTYPE_FILESYSTEM          ,0,127 }
#endif
};


const BlockRegionInfo  STM32F10x_BlockRegionInfo_ExternalFlash_NOR[1] =
{

    0x64000000,		// ByteAddress     Address;            // Start address
#ifdef EMOTE
    256,
    0x20000,
#else
    128,			// UINT32          NumBlocks;          // total number of blocks in this region
    0x20000,			// UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)
#endif
    ARRAYSIZE_CONST_EXPR(STM32F10x_BlockRange_ExternalFlash_NOR),
    STM32F10x_BlockRange_ExternalFlash_NOR,
};



BlockDeviceInfo STM32F10x_BlockDeviceInfo_ExternalFlash_NOR =
{
    {
        FALSE,									// BOOL Removable;
        TRUE,									// BOOL SupportsXIP;
        TRUE,									// BOOL WriteProtected
    },

    16,										    // UINT32 Duration_Max_WordWrite_uSec;
												// The below information does not correspond to the Nor Flash
    4800,										// UINT32 Duration_Max_SectorErase_uSec; @todo
    0x4,										// BytesPerSector;

    // The emote has a 256Mb Flash and the dev board has a 128 Mb Flash
#ifdef EMOTE
    0x2000000,
#else
    0x1000000,									// UINT32 Size;
#endif
    1,											// UINT32 NumRegions;
    STM32F10x_BlockRegionInfo_ExternalFlash_NOR,	// const BlockRegionInfo* pRegions;
};


BLOCK_CONFIG STM32F10x_blConfig_ExternalFlash_NOR =
{
	GPIO_PIN_NONE,
	FALSE,
    &STM32F10x_BlockDeviceInfo_ExternalFlash_NOR		// BlockDeviceinfo
};


struct BlockStorageDevice STM32F10x_BlockStorageDevice_ExternalFlash_NOR;
