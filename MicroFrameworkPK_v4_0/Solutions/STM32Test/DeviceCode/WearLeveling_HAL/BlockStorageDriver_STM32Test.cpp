////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "BlockStorageDriver_STM32Test.h"

//--//

BOOL STM32Test_BlockStorage_Driver::ChipInitialize( void* context )
{
    return TRUE;
}

BOOL STM32Test_BlockStorage_Driver::ChipUnInitialize( void* context )
{
    return TRUE;
}

const BlockDeviceInfo* STM32Test_BlockStorage_Driver::GetDeviceInfo( void* context )
{
    return NULL;    
}

BOOL  STM32Test_BlockStorage_Driver::ChipReadOnly( void* context, BOOL On, UINT32 ProtectionKey )
{
    return TRUE;
}


BOOL  STM32Test_BlockStorage_Driver::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff)
{
    return TRUE;

}


BOOL STM32Test_BlockStorage_Driver::Write(void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite)
{
    return TRUE;
}

BOOL STM32Test_BlockStorage_Driver::Memset(void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes)
{
    return TRUE;
}

BOOL STM32Test_BlockStorage_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}

BOOL STM32Test_BlockStorage_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return TRUE;
}



BOOL STM32Test_BlockStorage_Driver::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
    return TRUE;
}


BOOL STM32Test_BlockStorage_Driver::EraseBlock( void* context, ByteAddress Sector )
{
    return TRUE;
}

void  STM32Test_BlockStorage_Driver::SetPowerState( void* context, UINT32 State )
{
    return;
}

//--//

// Public functions

BOOL STM32Test_BlockStorage_Driver::ReadProductID( void* context, FLASH_WORD& ManufacturerCode, FLASH_WORD& DeviceCode )
{
    return TRUE;
}



//--// ---------------------------------------------------

#if defined(arm) || defined(__arm)
#pragma arm section code = "SectionForFlashOperations"
#endif

UINT32 STM32Test_BlockStorage_Driver::MaxSectorWrite_uSec( void* context )
{
    return 1;
}


UINT32 STM32Test_BlockStorage_Driver::MaxBlockErase_uSec( void* context )
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
    &STM32Test_BlockStorage_Driver::ChipInitialize,
    &STM32Test_BlockStorage_Driver::ChipUnInitialize,
    &STM32Test_BlockStorage_Driver::GetDeviceInfo,
    &STM32Test_BlockStorage_Driver::Read,
    &STM32Test_BlockStorage_Driver::Write,
    &STM32Test_BlockStorage_Driver::Memset,
    &STM32Test_BlockStorage_Driver::GetSectorMetadata,
    &STM32Test_BlockStorage_Driver::SetSectorMetadata,
    &STM32Test_BlockStorage_Driver::IsBlockErased,
    &STM32Test_BlockStorage_Driver::EraseBlock,
    &STM32Test_BlockStorage_Driver::SetPowerState,
    &STM32Test_BlockStorage_Driver::MaxSectorWrite_uSec,
    &STM32Test_BlockStorage_Driver::MaxBlockErase_uSec,    
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata 
#endif 



