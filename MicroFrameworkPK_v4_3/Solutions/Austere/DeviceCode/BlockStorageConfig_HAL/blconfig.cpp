////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2007-20015 The Samraksh Company.  All rights reserved.
// External NOR Flash configuration file.
// FIXME: Is this depreciated, or just not finished? - MAM@20150318
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#define FLASH_MANUFACTURER_CODE              0x0089
#define FLASH_DEVICE_CODE                    0x0017
#define FLASH_BLOCK_COUNT                    64
#define FLASH_BLOCK_ERASE_TYPICAL_TIME_USEC  1000000
#define FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC 210
#define FLASH_BLOCK_ERASE_MAX_TIME_USEC      4000000
#define FLASH_SECTOR_WRITE_MAX_TIME_USEC     630
//
// The real max time should be 4,000,000 uSec, but the chip has independent partitions, it's not critical to avoid radio collisions.
//
#define FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC   400000
//--//

// EBIU Information

#define BLCONFIG_EmoteDotNow_CHIP_SELECT       0
#define BLCONFIG_EmoteDotNow_WAIT_STATES       7                    /* New freescale board uses 75 nS parts */
#define BLCONFIG_EmoteDotNow_RELEASE_COUNTS    0
#define BLCONFIG_EmoteDotNow_BIT_WIDTH         (16 | (1 << 16))     /* Data is on D15-D0 on the freescale boards  */
#define BLCONFIG_EmoteDotNow_BASE_ADDRESS      FLASH_MEMORY_Base    /* FIXME: why does this use FLASH_MEMORY_Base which is defined in platform_selector.h as internal flash address */
#define BLCONFIG_EmoteDotNow_SIZE_IN_BYTES     8*1024*1024
#define BLCONFIG_EmoteDotNow_BYTES_PER_SECTOR  2
#define BLCONFIG_EmoteDotNow_BYTES_PER_BLOCK   0x20000
#define BLCONFIG_EmoteDotNow_WP_GPIO_PIN       GPIO_PIN_NONE
#define BLCONFIG_EmoteDotNow_WP_ACTIVE         FALSE



//--//

#define BLCONFIG_EmoteDotNow_FLASH_BOTTOM_PARAMETER 1
#undef  BLCONFIG_EmoteDotNow_FLASH_TOP_PARAMETER



// BlockDeviceInformation

#define BLCONFIG_EmoteDotNow_REMOVEABLE     FALSE
#define BLCONFIG_EmoteDotNow_SUPPORTXIP     TRUE
#define BLCONFIG_EmoteDotNow_WRITEPROTECTED FALSE
#define BLCONFIG_EmoteDotNow_NUMREGIONS     1


//--//
#ifdef MEMORY_BLOCKTYPE_SPECIAL
#undef MEMORY_BLOCKTYPE_SPECIAL
#endif

#ifdef MEMORY_BLOCKTYPE_SPECIAL2
#undef MEMORY_BLOCKTYPE_SPECIAL2
#endif

#ifdef MEMORY_BLOCKTYPE_SPECIAL3
#undef MEMORY_BLOCKTYPE_SPECIAL3
#endif

#if defined(MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE)
#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE 
#endif

#ifdef __GNUC__
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE       BlockRange::BLOCKTYPE_CODE
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP  BlockRange::BLOCKTYPE_BOOTSTRAP
#else
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE       BlockRange::BLOCKTYPE_DEPLOYMENT
#define MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP  BlockRange::BLOCKTYPE_CODE
#endif




//--//

#if defined(BUILD_RTM)
        #define MEMORY_BLOCKTYPE_SPECIAL  BlockRange::BLOCKTYPE_DEPLOYMENT
#else
        #define MEMORY_BLOCKTYPE_SPECIAL  BlockRange::BLOCKTYPE_FILESYSTEM
#endif

//--//


const BlockRange g_BLCONFIG_EmoteDotNow_BlockRange[] =
{
    { BlockRange::BLOCKTYPE_BOOTSTRAP       ,  0,  0 },     // Bootloader
    { MEMORY_BLOCKTYPE_GCC_SPECIAL_BOOTSTRAP,  1,  1 },     // Bootloader
    { BlockRange::BLOCKTYPE_CODE            ,  2,  7 },     // TinyCLR runtime           
    { MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE     ,  8, 10 },     // TinyCLR runtime 
    { BlockRange::BLOCKTYPE_DEPLOYMENT      , 11, 56 },     // VS Deployment
    { MEMORY_BLOCKTYPE_SPECIAL              , 57, 60 },         
    { BlockRange::BLOCKTYPE_STORAGE_A       , 61, 61 },     // data storage - Extended Weak References  
    { BlockRange::BLOCKTYPE_STORAGE_B       , 62, 62 },     // data storage - Extended Weak References
    { BlockRange::BLOCKTYPE_CONFIG          , 63, 63 },     // data storage - Extended Weak References
};


const BlockRegionInfo  g_BlkRegion_EmoteDotNow[BLCONFIG_EmoteDotNow_NUMREGIONS] = 
{
    BLCONFIG_EmoteDotNow_BASE_ADDRESS,     // ByteAddress     Address;         
    FLASH_BLOCK_COUNT,              // UINT32          NumBlocks;          // total number of blocks in this region
    BLCONFIG_EmoteDotNow_BYTES_PER_BLOCK,  // UINT32          BytesPerBlock;      // Total number of bytes per block (MUST be SectorsPerBlock * DataBytesPerSector)

    ARRAYSIZE_CONST_EXPR(g_BLCONFIG_EmoteDotNow_BlockRange),
    g_BLCONFIG_EmoteDotNow_BlockRange,
};

//--//

#undef MEMORY_BLOCKTYPE_GCC_SPECIAL_CODE 

//--//


BlockDeviceInfo g_BLCONFIG_EmoteDotNow_DeviceInfo=
{
    {  
        BLCONFIG_EmoteDotNow_REMOVEABLE,           // BOOL Removable;
        BLCONFIG_EmoteDotNow_SUPPORTXIP,           // BOOL SupportsXIP;
        BLCONFIG_EmoteDotNow_WRITEPROTECTED,
    },
    FLASH_SECTOR_WRITE_TYPICAL_TIME_USEC,   // UINT32 Duration_Max_WordWrite_uSec;
    FLASH_BLOCK_ERASE_ACTUAL_TIME_USEC,     // UINT32 Duration_Max_SectorErase_uSec;
    BLCONFIG_EmoteDotNow_BYTES_PER_SECTOR,         // BytesPerSector; // Bytes Per Sector

    FLASH_MEMORY_Size,                      // UINT32 Size;

    BLCONFIG_EmoteDotNow_NUMREGIONS,               // UINT32 NumRegions;
    g_BlkRegion_EmoteDotNow,                      // const BlockRegionInfo* pRegions;
};


struct MEMORY_MAPPED_NOR_BLOCK_CONFIG g_BS_Config_EmoteDotNow =
{
    { // BLOCK_CONFIG
        {
            BLCONFIG_EmoteDotNow_WP_GPIO_PIN,      // GPIO_PIN             Pin;
            BLCONFIG_EmoteDotNow_WP_ACTIVE,        // BOOL                 ActiveState;
        },

        &g_BLCONFIG_EmoteDotNow_DeviceInfo,        // BlockDeviceinfo
    },

    { // CPU_MEMORY_CONFIG
        BLCONFIG_EmoteDotNow_CHIP_SELECT,          // UINT8  CPU_MEMORY_CONFIG::ChipSelect;
        TRUE,                               // UINT8  CPU_MEMORY_CONFIG::ReadOnly;
        BLCONFIG_EmoteDotNow_WAIT_STATES,          // UINT32 CPU_MEMORY_CONFIG::WaitStates;
        BLCONFIG_EmoteDotNow_RELEASE_COUNTS,       // UINT32 CPU_MEMORY_CONFIG::ReleaseCounts;
        BLCONFIG_EmoteDotNow_BIT_WIDTH,            // UINT32 CPU_MEMORY_CONFIG::BitWidth;
        BLCONFIG_EmoteDotNow_BASE_ADDRESS,         // UINT32 CPU_MEMORY_CONFIG::BaseAddress;
        BLCONFIG_EmoteDotNow_SIZE_IN_BYTES,        // UINT32 CPU_MEMORY_CONFIG::SizeInBytes;
        0,                                  // UINT8                CPU_MEMORY_CONFIG::XREADYEnable 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ByteSignalsForRead 
        0,                                  // UINT8                CPU_MEMORY_CONFIG::ExternalBufferEnable
    },

    0,                                      // UINT32  ChipProtection;
    FLASH_MANUFACTURER_CODE,                // FLASH_WORD ManufacturerCode;
    FLASH_DEVICE_CODE,                      // FLASH_WORD DeviceCode;
};

//--//

