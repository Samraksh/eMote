////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_bl.h"

#include "mxc_config.h"
#include "flc.h"

//--//
#define CR_PER_Reset             ((uint32_t)0x00001FFD)

BOOL Max3263x_blDriver::InitializeDevice( void* context )
{
	FLC_Init();
    return TRUE;
}

BOOL Max3263x_blDriver::UninitializeDevice( void* context )
{
	return TRUE;
}

const BlockDeviceInfo* Max3263x_blDriver::GetDeviceInfo( void* context )
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG *)context;	// Type case so we can return
    return config->BlockDeviceInformation;
}

//This device supports execute in place, since its internal flash. Hence its usually not read using this API.
//The below implementation is not needed.
BOOL Max3263x_blDriver::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff )
{
    //pSectorBuff = (BYTE *)Address;			// Just move the buffer pointer
	//return TRUE;
	return FALSE;
}

BOOL Max3263x_blDriver::Write( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
	if (FLC_Write((uint32_t)address, (const void *)pSectorBuff, numBytes, MXC_V_FLC_FLSH_UNLOCK_KEY)==E_NO_ERROR) return TRUE;
	else return FALSE;
	/*FLASH_Status status;
	UINT8 pages 	= (numBytes / 0x800) + 1;
	UINT8 startPage = address / 0x800;
	UINT16* addressPtr    = (UINT16* )address;	// 16-bit writing
	UINT16* buffPtr       = (UINT16* )pSectorBuff;
	//if(ReadModifyWrite) {						// CLR is asking us to take care of the erasing
	// Nived.Sivadas - VS assumes flash is erased before writing into flash
	if(*addressPtr != 0xffff) {						// CLR is asking us to take care of the erasing
		UINT8 ii;
		for(ii = 0; ii < pages; ii++) {
			if(!EraseBlock(context, address + ii * 0x800))
			{
				hal_printf( "Max3263x_blDriver::Write failure @ 0x%08x, wrote 0x%08x, read 0x%08x\r\n", (UINT32)addressPtr, *buffPtr, *addressPtr );
				return FALSE;
			}
		}
	}

	UINT32 startAddress   = address;
	UINT32 endAddress     = address + numBytes;


	UINT16* endAddressPtr = (UINT16*)endAddress;
	FLASH_Unlock();
	while(addressPtr < endAddressPtr) {
		if(address < FLASH_BANK1_END_ADDRESS)
		{
			while(FLASH->SR & FLASH_SR_BSY);		// Wait until flash is ready
			FLASH->CR = FLASH_CR_PG;				// Set Programming Bit
			*addressPtr = *buffPtr;
			while(FLASH->SR & FLASH_SR_BSY);
		}
		else
		{
			while(FLASH->SR2 & FLASH_SR_BSY);		// Wait until flash is ready
			FLASH->CR2 = FLASH_CR_PG;				// Set Programming Bit
			*addressPtr = *buffPtr;
			while(FLASH->SR2 & FLASH_SR_BSY);
			//FLASH_ProgramWord(startAddress++, 0);
		}
		if (*addressPtr != *buffPtr) {
		       hal_printf( "Max3263x_blDriver::Write failure @ 0x%08x, wrote 0x%08x, read 0x%08x\r\n", (UINT32)addressPtr, *buffPtr, *addressPtr );
		       return FALSE;
		}
		addressPtr++;
		buffPtr++;

	}
	if(address < FLASH_BANK1_END_ADDRESS)
	{
		FLASH->CR = FLASH_CR_LOCK;
	}
	else
	{
		FLASH->CR2 = FLASH_CR_LOCK;
	}
	// @todo check programming
	return TRUE; */
}

BOOL Max3263x_blDriver::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
{
	return FALSE;
/*	UINT8 pages 	= (numBytes / 0x800) + 1;
	UINT8 startPage = address / 0x800;
	UINT8 ii;
	for(ii = 0; ii < pages; ii++) {
		EraseBlock(context, (startPage + ii)*0x800);
	}
	UINT32 endAddress    = address + numBytes;
	UINT8* addressPtr    = (UINT8* )address;	// 8-bit writing
	UINT8* buffPtr       = &Data;
	UINT8* endAddressPtr = (UINT8*)endAddress;
	while(addressPtr < endAddressPtr) {
		while(FLASH->SR & FLASH_SR_BSY);		// Wait until flash is ready
		FLASH->CR = FLASH_CR_PG;				// Set Programming Bit
		*addressPtr++ = *buffPtr++;
	}
	// @todo check programming
	return TRUE;*/
}

BOOL Max3263x_blDriver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL Max3263x_blDriver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL Max3263x_blDriver::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
	UINT16* addressPtr    = (UINT16* )Address;	// 16-bit writing
	if(*addressPtr ==  0xFFFF) {
		return TRUE;
	} else {
	  			return FALSE;
	}
	return TRUE;
}


BOOL Max3263x_blDriver::EraseBlock( void* context, ByteAddress address )
{
	if(FLC_PageErase((uint32_t) address, MXC_V_FLC_ERASE_CODE_PAGE_ERASE, MXC_V_FLC_FLSH_UNLOCK_KEY)==E_NO_ERROR) return TRUE;
	else return FALSE;

	/*FLASH_Status stat;
	FLASH_Unlock();
	stat = FLASH_ErasePage(address);
	while(stat != FLASH_COMPLETE) {
		stat = FLASH_ErasePage(address);
	}
	return TRUE;*/
}



void  Max3263x_blDriver::SetPowerState( void* context, UINT32 State )
{
}

UINT32 Max3263x_blDriver::MaxSectorWrite_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}


UINT32 Max3263x_blDriver::MaxBlockErase_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

//--//

#pragma arm section code
struct IBlockStorageDevice Max3263x_IBlockStorageDevice_InternalFlash =
{
    &Max3263x_blDriver::InitializeDevice,
    &Max3263x_blDriver::UninitializeDevice,
    &Max3263x_blDriver::GetDeviceInfo,
    &Max3263x_blDriver::Read,
    &Max3263x_blDriver::Write,
    &Max3263x_blDriver::Memset,
    &Max3263x_blDriver::GetSectorMetadata,
    &Max3263x_blDriver::SetSectorMetadata,
    &Max3263x_blDriver::IsBlockErased,
    &Max3263x_blDriver::EraseBlock,
    &Max3263x_blDriver::SetPowerState,
    &Max3263x_blDriver::MaxSectorWrite_uSec,
    &Max3263x_blDriver::MaxBlockErase_uSec,
};


