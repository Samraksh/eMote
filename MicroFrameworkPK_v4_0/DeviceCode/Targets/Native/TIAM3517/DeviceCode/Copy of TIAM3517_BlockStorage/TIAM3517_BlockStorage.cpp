/*
 *  Name : TIAM3517_BlockStorage.cpp
 *
 *  Author : Nived.Sivadas@Samraksh.com
 *
 * 	Description :
 */

#include <tinyhal.h>
#include "..\TIAM3517.h"

BOOL TIAM3517_BlockStorage_Driver::InitializeDevice( void* context )
{
    return TRUE;
}

BOOL TIAM3517_BlockStorage_Driver::UninitializeDevice( void* context )
{
	return TRUE;
}

const BlockDeviceInfo* TIAM3517_BlockStorage_Driver::GetDeviceInfo(void * context)
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG *)context;     // Type case so we can return
    return config->BlockDeviceInformation;
}

BOOL TIAM3517_BlockStorage_Driver::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff )
{
    pSectorBuff = (BYTE *)Address;                      // Just move the buffer pointer
        return TRUE;
}

BOOL TIAM3517_BlockStorage_Driver::Write(void * context, ByteAddress address, UINT32 numBytes, BYTE* pSectorBuff, BOOL ReadModifyWrite)
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

BOOL TIAM3517_BlockStorage_Driver::Memset(void * context, ByteAddress address, UINT8 data, UINT32 numBytes)
{

	UINT8* addressPtr = (UINT8*) address;

	for(UINT8 i = 0; i < numBytes; i++)
	{
		*addressPtr++ = 0xff;
	}

	UINT32 endAddress = address + numBytes;
	UINT8* buffPtr = &data;
	UINT8* endAddressPtr = (UINT8 *) endAddress;
	while(addressPtr < endAddressPtr){
		*addressPtr++ = *buffPtr++;
	}

	return TRUE;



}


BOOL TIAM3517_BlockStorage_Driver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
	return FALSE;
}

BOOL TIAM3517_BlockStorage_Driver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL TIAM3517_BlockStorage_Driver::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
        // @todo this method sucks - make better
        if(Address = 0xFFFF) {
                return TRUE;
        } else {
                return FALSE;
        }
}

BOOL TIAM3517_BlockStorage_Driver::EraseBlock( void* context, ByteAddress address )
{
	// Considering blocks are of 2k size, since we are writing in ram directly
	UINT16* addressPtr = (UINT16*) address;
	for(UINT8 i = 0; i < 2048; i++)
	{
		*addressPtr++ = 0xff;
	}

    return TRUE;
}

void  TIAM3517_BlockStorage_Driver::SetPowerState( void* context, UINT32 State )
{
}

UINT32 TIAM3517_BlockStorage_Driver::MaxSectorWrite_uSec( void* context )
{
        BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
        return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}

UINT32 TIAM3517_BlockStorage_Driver::MaxBlockErase_uSec( void* context )
{
        BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
        return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

#pragma arm section code
struct IBlockStorageDevice TIAM3517_IBlockStorageDevice_InternalFlash =
{
	   &TIAM3517_BlockStorage_Driver::InitializeDevice,
	   &TIAM3517_BlockStorage_Driver::UninitializeDevice,
	   &TIAM3517_BlockStorage_Driver::GetDeviceInfo,
	   &TIAM3517_BlockStorage_Driver::Read,
	   &TIAM3517_BlockStorage_Driver::Write,
	   &TIAM3517_BlockStorage_Driver::Memset,
	   &TIAM3517_BlockStorage_Driver::GetSectorMetadata,
	   &TIAM3517_BlockStorage_Driver::SetSectorMetadata,
	   &TIAM3517_BlockStorage_Driver::IsBlockErased,
	   &TIAM3517_BlockStorage_Driver::EraseBlock,
       &TIAM3517_BlockStorage_Driver::SetPowerState,
	   &TIAM3517_BlockStorage_Driver::MaxSectorWrite_uSec,
	   &TIAM3517_BlockStorage_Driver::MaxBlockErase_uSec,


};


//--//

