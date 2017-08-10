////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_bl.h"

//--//

BOOL myInitializeDevice(void *context)
{
	return TRUE;
}


//#pragma arm section code
BOOL STM32F10x_blDriver::InitializeDevice( void* context )
{
	UINT32 junk = 0;
    return TRUE;
}

BOOL STM32F10x_blDriver::UninitializeDevice( void* context )
{
	return TRUE;
}

const BlockDeviceInfo* STM32F10x_blDriver::GetDeviceInfo( void* context )
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG *)context;	// Type case so we can return
    return config->BlockDeviceInformation;
}

BOOL STM32F10x_blDriver::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff )
{
    pSectorBuff = (BYTE *)Address;			// Just move the buffer pointer
	return TRUE;
}

BOOL STM32F10x_blDriver::Write( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
	UINT16* addressPtr = (UINT16*) address;
		if(ReadModifyWrite)
		{
			for(UINT8 i = 0; i < numBytes; i++)
			{
				*addressPtr++ = 0xff;
			}
		}
		UINT32 endAddress = address + numBytes;
		UINT16* buffPtr = (UINT16 *) pSectorBuff;
		UINT16* endAddressPtr = (UINT16 *) endAddress;
		while(addressPtr < endAddressPtr){
			*addressPtr++ = *buffPtr++;
		}

	return TRUE;
}

BOOL STM32F10x_blDriver::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
{
	UINT8* addressPtr = (UINT8*) address;

		for(UINT8 i = 0; i < numBytes; i++)
		{
			*addressPtr++ = 0xff;
		}

		UINT32 endAddress = address + numBytes;
		UINT8* buffPtr = &Data;
		UINT8* endAddressPtr = (UINT8 *) endAddress;
		while(addressPtr < endAddressPtr){
			*addressPtr++ = *buffPtr++;
		}
	return TRUE;
}

BOOL STM32F10x_blDriver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL STM32F10x_blDriver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL STM32F10x_blDriver::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
	// @todo this method sucks - make better
	if(Address = 0xFFFF) {
		return TRUE;
	} else {
		return FALSE;
	}
}

BOOL STM32F10x_blDriver::EraseBlock( void* context, ByteAddress address )
{
	UINT16* addressPtr = (UINT16*) address;
	for(UINT8 i = 0; i < 2048; i++)
	{
		*addressPtr++ = 0xff;
	}
	return TRUE;
}

void  STM32F10x_blDriver::SetPowerState( void* context, UINT32 State )
{
}

UINT32 STM32F10x_blDriver::MaxSectorWrite_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}


UINT32 STM32F10x_blDriver::MaxBlockErase_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

//--//

//#pragma arm section rodata

//#pragma arm section rodata

