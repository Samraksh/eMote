#include <tinyhal.h>
#include "netmf_bl_nor.h"

BOOL isErased = TRUE;
//--//
BOOL STM32F10x_blDriver_nor::InitializeDevice( void* context )
{
	//NOR_Init();
	//NOR_EraseChip();

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
	UINT32 translAddress = Address - 0x64000000;
    //pSectorBuff = (BYTE *)Address;			// Just move the buffer pointer
	uint16_t *data;
	UINT32 NumHalfWords = NumBytes / 2;

	//NOR_ReadBuffer(data,translAddress,NumHalfWords);

	pSectorBuff = (BYTE *) data;

	return TRUE;
}

BOOL STM32F10x_blDriver_nor::Write( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
	NOR_Status status;
	UINT32 translAddress = address - 0x64000000;

	UINT16* buffPtr       = (UINT16 *) pSectorBuff;

	//status = NOR_WriteBuffer(buffPtr, translAddress, numBytes / 2);

	if(status == NOR_SUCCESS )
	{
		isErased = false;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL STM32F10x_blDriver_nor::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
{
	UINT32 translAddress = address - 0x64000000;
	uint16_t *buffer;
	uint16_t tempData = (Data << 8);
	tempData |= Data;

	for(int i = 0; i < numBytes / 2; i++)
		buffer[i] = tempData;

#if 0
	if(NOR_SUCCESS == NOR_WriteBuffer(buffer, translAddress, numBytes / 2))
			return TRUE;
		else
			return FALSE;
#endif
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
	if(isErased == TRUE)
		return TRUE;
	else
		return FALSE;

}

BOOL STM32F10x_blDriver_nor::EraseBlock( void* context, ByteAddress address )
{
#if 0
	if(NOR_EraseBlock(address))
	{
		isErased = TRUE;
		return TRUE;
	}
	else
		return FALSE;
#endif
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
