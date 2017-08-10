////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "BlockStorageDriver_SOC_ADAPT.h"

//--//

BOOL SOC_ADAPT_BlockStorage_Driver::ChipInitialize( void* context )
{
    return TRUE;
}

BOOL SOC_ADAPT_BlockStorage_Driver::ChipUnInitialize( void* context )
{
    return TRUE;
}

const BlockDeviceInfo* SOC_ADAPT_BlockStorage_Driver::GetDeviceInfo( void* context )
{
    return NULL;    
}

BOOL  SOC_ADAPT_BlockStorage_Driver::ChipReadOnly( void* context, BOOL On, UINT32 ProtectionKey )
{
    return TRUE;
}


BOOL  SOC_ADAPT_BlockStorage_Driver::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff)
{
    return TRUE;

}


BOOL SOC_ADAPT_BlockStorage_Driver::Write(void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite)
{
    return TRUE;
}

BOOL SOC_ADAPT_BlockStorage_Driver::Memset(void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes)
{
    return TRUE;
}

BOOL SOC_ADAPT_BlockStorage_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL SOC_ADAPT_BlockStorage_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}



BOOL SOC_ADAPT_BlockStorage_Driver::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
    return TRUE;
}


BOOL SOC_ADAPT_BlockStorage_Driver::EraseBlock( void* context, ByteAddress Sector )
{
    return TRUE;
}

void  SOC_ADAPT_BlockStorage_Driver::SetPowerState( void* context, UINT32 State )
{
    return;
}

//--//

// Public functions

BOOL SOC_ADAPT_BlockStorage_Driver::ReadProductID( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode )
{
    return TRUE;
}



//--// ---------------------------------------------------

#if defined(arm) || defined(__arm)
#pragma arm section code = "SectionForFlashOperations"
#endif

UINT32 SOC_ADAPT_BlockStorage_Driver::MaxSectorWrite_uSec( void* context )
{
    return 1;
}


UINT32 SOC_ADAPT_BlockStorage_Driver::MaxBlockErase_uSec( void* context )
{
    return 1;
    
}

#if defined(arm) || defined(__arm)
#pragma arm section code 
#endif


//--// 

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "g_I28F_16_BS_DeviceTable"
#endif

struct IBlockStorageDevice g_I28F_16_BS_DeviceTable = 
{                          
    &SOC_ADAPT_BlockStorage_Driver::ChipInitialize,
    &SOC_ADAPT_BlockStorage_Driver::ChipUnInitialize,
    &SOC_ADAPT_BlockStorage_Driver::GetDeviceInfo,
    &SOC_ADAPT_BlockStorage_Driver::Read,
    &SOC_ADAPT_BlockStorage_Driver::Write,
    &SOC_ADAPT_BlockStorage_Driver::Memset,
    &SOC_ADAPT_BlockStorage_Driver::GetSectorMetadata,
    &SOC_ADAPT_BlockStorage_Driver::SetSectorMetadata,
    &SOC_ADAPT_BlockStorage_Driver::IsBlockErased,
    &SOC_ADAPT_BlockStorage_Driver::EraseBlock,
    &SOC_ADAPT_BlockStorage_Driver::SetPowerState,
    &SOC_ADAPT_BlockStorage_Driver::MaxSectorWrite_uSec,
    &SOC_ADAPT_BlockStorage_Driver::MaxBlockErase_uSec,    
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 



