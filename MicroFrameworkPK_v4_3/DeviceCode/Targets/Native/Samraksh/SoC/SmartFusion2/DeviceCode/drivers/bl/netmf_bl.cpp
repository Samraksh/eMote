////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_bl.h"
#include "../eNVM/eNVM.h"

//--//
#define CR_PER_Reset             ((uint32_t)0x00001FFD)

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
	// unlocking deployment section
	//NVM_unlock(0x064000, 110592);
return FALSE;

	//NVM_write(address, (const uint8_t *)pSectorBuff, numBytes, NVM_DO_NOT_LOCK_PAGE);
	/*FLASH_Status status;
	UINT8 pages 	= (numBytes / 0x800) + 1;
	UINT8 startPage = address / 0x800;
	UINT16* addressPtr    = (UINT16* )address;	// 16-bit writing
	UINT16* buffPtr       = (UINT16* )pSectorBuff;
	// VS assumes flash is erased before writing into flash
	if(*addressPtr != 0xffff) {						// CLR is asking us to take care of the erasing
		UINT8 ii;
		for(ii = 0; ii < pages; ii++) {
			if(!EraseBlock(context, address + ii * 0x800))
			{
				hal_printf( "STM32F10x_blDriver::Write failure @ 0x%08x, wrote 0x%08x, read 0x%08x\r\n", (UINT32)addressPtr, *buffPtr, *addressPtr );
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
		       hal_printf( "STM32F10x_blDriver::Write failure @ 0x%08x, wrote 0x%08x, read 0x%08x\r\n", (UINT32)addressPtr, *buffPtr, *addressPtr );
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
	}*/
	// @todo check programming
	return TRUE;
}

BOOL STM32F10x_blDriver::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
{
	// unlocking deployment section
	//NVM_unlock(0x064000, 110592);
	uint8_t data_block[numBytes];
	int i;
	for (i=0; i<numBytes; i++){
		data_block[i] = Data;
	}
return FALSE;
	//NVM_write(address, data_block, numBytes, NVM_DO_NOT_LOCK_PAGE);
	/*UINT8 pages 	= (numBytes / 0x800) + 1;
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
	// @todo check programming*/
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
	/*UINT16* addressPtr    = (UINT16* )Address;	// 16-bit writing
	if(*addressPtr ==  0xFFFF) {
		return TRUE;
	} else {
	  			return FALSE;
	}*/
	return TRUE;
	  	}

BOOL STM32F10x_blDriver::EraseBlock( void* context, ByteAddress address )
{
	FLASH_Status stat;

	//FLASH_Unlock();
	//// unlocking deployment section
	//NVM_unlock(0x064000, 110592);

	/*stat = FLASH_ErasePage(address);

	while(stat != FLASH_COMPLETE) {
		stat = FLASH_ErasePage(address);
	}*/

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


