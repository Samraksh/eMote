////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_bl.h"

//--//

BOOL STM32F10x_blDriver::InitializeDevice( void* context )
{
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
	FLASH_Status status;
	UINT8 pages 	= (numBytes / 0x800) + 1;
	UINT8 startPage = address / 0x800;
	if(ReadModifyWrite) {						// CLR is asking us to take care of the erasing
		UINT8 ii;
		for(ii = 0; ii < pages; ii++) {
			EraseBlock(context, (startPage + ii)*0x800);
		}
	}
	UINT32 startAddress   = address;
	UINT32 endAddress     = address + numBytes;
	UINT16* addressPtr    = (UINT16* )address;	// 16-bit writing
	UINT16* buffPtr       = (UINT16* )pSectorBuff;
	UINT16* endAddressPtr = (UINT16*)endAddress;
	FLASH_Unlock();
	while(addressPtr < endAddressPtr) {
		if(address < FLASH_BANK1_END_ADDRESS)
		{
			while(FLASH->SR & FLASH_SR_BSY);		// Wait until flash is ready
			FLASH->CR = FLASH_CR_PG;				// Set Programming Bit
			*addressPtr++ = *buffPtr++;
		}
		else
		{
			while(FLASH->SR2 & FLASH_SR_BSY);		// Wait until flash is ready
			FLASH->CR2 = FLASH_CR_PG;				// Set Programming Bit
			*addressPtr++ = *buffPtr++;
			//FLASH_ProgramWord(startAddress++, 0);
		}


	}
	// @todo check programming
	return TRUE;
}

BOOL STM32F10x_blDriver::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
{
	UINT8 pages 	= (numBytes / 0x800) + 1;
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
	FLASH_Unlock();
	if(address < FLASH_BANK1_END_ADDRESS)
	{
		while(FLASH->SR & FLASH_SR_BSY);	// Wait until flash is ready
		FLASH->KEYR = 0x45670123;			// Key1
		FLASH->KEYR = 0xCDEF89AB;			// Key2
		FLASH->CR   = FLASH_CR_PER;			// Set Page Erase Bit
		FLASH->AR	= address;				// Set arbitrary address in page
		FLASH->CR	= FLASH_CR_STRT;		// Set Start Bit
		while(FLASH->SR & FLASH_SR_BSY);	// Wait until flash is ready
	}
	else
	{
		while(FLASH->SR2 & FLASH_SR_BSY);
		FLASH->KEYR2 = 0x45670123;			// Key1
		FLASH->KEYR2 = 0xCDEF89AB;
		FLASH->CR2   = FLASH_CR_PER;
		FLASH->AR2	= address;
		FLASH->CR2	= FLASH_CR_STRT;
		while(FLASH->SR2 & FLASH_SR_BSY);

	}

	//FLASH_ErasePage(address);
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

#pragma arm section code
struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_InternalFlash =
{
    &STM32F10x_blDriver::InitializeDevice,
    &STM32F10x_blDriver::UninitializeDevice,
    &STM32F10x_blDriver::GetDeviceInfo,
    &STM32F10x_blDriver::Read,
    &STM32F10x_blDriver::Write,
    &STM32F10x_blDriver::Memset,
    &STM32F10x_blDriver::GetSectorMetadata,
    &STM32F10x_blDriver::SetSectorMetadata,
    &STM32F10x_blDriver::IsBlockErased,
    &STM32F10x_blDriver::EraseBlock,
    &STM32F10x_blDriver::SetPowerState,
    &STM32F10x_blDriver::MaxSectorWrite_uSec,
    &STM32F10x_blDriver::MaxBlockErase_uSec,
};


