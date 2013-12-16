#include <tinyhal.h>
#include "netmf_bl_nor.h"
#include "platform_selector.h"

extern P30BF65NOR_Driver gNORDriver;

BOOL isErased = TRUE;
//--//
BOOL STM32F10x_blDriver_nor::InitializeDevice( void* context )
{
	gNORDriver.Initialize();

	/*for(UINT32 i = 0 ; i < 0x80; i = i + 0x10000)
	{
	   gNORDriver.EraseBlock(i);
	   for(volatile UINT16 j = 0; j < 65000; j++ );
	}*/

}

BOOL STM32F10x_blDriver_nor::UninitializeDevice( void* context )
{
	return TRUE;
}

const BlockDeviceInfo* STM32F10x_blDriver_nor::GetDeviceInfo( void* context )
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG *)context;	// Type case so we can return
    return config->BlockDeviceInformation;
}

BOOL STM32F10x_blDriver_nor::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff )
{
	UINT32 translAddress = Address - DATASTORE_START_ADDRESS;

	UINT32 NumHalfWords = NumBytes / 2;

	gNORDriver.ReadBuffer((UINT16 *) pSectorBuff, translAddress, NumHalfWords);

	return TRUE;
}

BOOL STM32F10x_blDriver_nor::Write( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
	DeviceStatus status;

	UINT32 translAddress = address - DATASTORE_START_ADDRESS;
	UINT32 numberOfHalfWordsToWrite = numBytes / 2;

	status = gNORDriver.WriteBuffer((UINT16 *) pSectorBuff, translAddress, numberOfHalfWordsToWrite);

	if(status == DS_Success )
	{
		isErased = false;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL STM32F10x_blDriver_nor::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
{
	DeviceStatus status;

	UINT32 translAddress = address - DATASTORE_START_ADDRESS;
	uint16_t *buffer;
	uint16_t tempData = (Data << 8);
	tempData |= Data;

	for(int i = 0; i < numBytes / 2; i++)
		buffer[i] = tempData;

	status = gNORDriver.WriteBuffer(buffer, address, (numBytes)/ 2);

}

BOOL STM32F10x_blDriver_nor::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL STM32F10x_blDriver_nor::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL STM32F10x_blDriver_nor::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
	if(gNORDriver.ReadHalfWord(Address) != 0xffff)
		return FALSE;

	return TRUE;

}

BOOL STM32F10x_blDriver_nor::EraseBlock( void* context, ByteAddress address )
{
	UINT32 translAddress = address - DATASTORE_START_ADDRESS;

	if(gNORDriver.EraseBlock(translAddress) != DS_Success)
		return FALSE;

	return TRUE;
}

void  STM32F10x_blDriver_nor::SetPowerState( void* context, UINT32 State )
{
}

UINT32 STM32F10x_blDriver_nor::MaxSectorWrite_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}


UINT32 STM32F10x_blDriver_nor::MaxBlockErase_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

//--//

#pragma arm section code
struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_ExternalFlash_NOR =
{
    &STM32F10x_blDriver_nor::InitializeDevice,
    &STM32F10x_blDriver_nor::UninitializeDevice,
    &STM32F10x_blDriver_nor::GetDeviceInfo,
    &STM32F10x_blDriver_nor::Read,
    &STM32F10x_blDriver_nor::Write,
    &STM32F10x_blDriver_nor::Memset,
    &STM32F10x_blDriver_nor::GetSectorMetadata,
    &STM32F10x_blDriver_nor::SetSectorMetadata,
    &STM32F10x_blDriver_nor::IsBlockErased,
    &STM32F10x_blDriver_nor::EraseBlock,
    &STM32F10x_blDriver_nor::SetPowerState,
    &STM32F10x_blDriver_nor::MaxSectorWrite_uSec,
    &STM32F10x_blDriver_nor::MaxBlockErase_uSec,
};
