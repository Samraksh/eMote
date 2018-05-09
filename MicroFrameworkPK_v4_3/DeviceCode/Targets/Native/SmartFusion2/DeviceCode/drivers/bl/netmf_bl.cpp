////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_bl.h"
#include "../eNVM/eNVM.h"

//--//
#define CR_PER_Reset             ((uint32_t)0x00001FFD)

BOOL SF2_CM3_blDriver::InitializeDevice( void* context )
{
    return TRUE;
}

BOOL SF2_CM3_blDriver::UninitializeDevice( void* context )
{
	return TRUE;
}

const BlockDeviceInfo* SF2_CM3_blDriver::GetDeviceInfo( void* context )
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG *)context;	// Type case so we can return
    return config->BlockDeviceInformation;
}

BOOL SF2_CM3_blDriver::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff )
{
    pSectorBuff = (BYTE *)Address;			// Just move the buffer pointer
	return TRUE;
}

BOOL SF2_CM3_blDriver::Write( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
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
				hal_printf( "SF2_CM3_blDriver::Write failure @ 0x%08x, wrote 0x%08x, read 0x%08x\r\n", (UINT32)addressPtr, *buffPtr, *addressPtr );
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
		       hal_printf( "SF2_CM3_blDriver::Write failure @ 0x%08x, wrote 0x%08x, read 0x%08x\r\n", (UINT32)addressPtr, *buffPtr, *addressPtr );
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

BOOL SF2_CM3_blDriver::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
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

BOOL SF2_CM3_blDriver::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL SF2_CM3_blDriver::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL SF2_CM3_blDriver::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
	/*UINT16* addressPtr    = (UINT16* )Address;	// 16-bit writing
	if(*addressPtr ==  0xFFFF) {
		return TRUE;
	} else {
	  			return FALSE;
	}*/
	return TRUE;
	  	}

BOOL SF2_CM3_blDriver::EraseBlock( void* context, ByteAddress address )
{
	//FLASH_Status stat;

	//FLASH_Unlock();
	//// unlocking deployment section
	//NVM_unlock(0x064000, 110592);

	/*stat = FLASH_ErasePage(address);

	while(stat != FLASH_COMPLETE) {
		stat = FLASH_ErasePage(address);
	}*/

    return TRUE;
}



void  SF2_CM3_blDriver::SetPowerState( void* context, UINT32 State )
{
}

UINT32 SF2_CM3_blDriver::MaxSectorWrite_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}


UINT32 SF2_CM3_blDriver::MaxBlockErase_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

//--//

#pragma arm section code
struct IBlockStorageDevice SF2_CM3_IBlockStorageDevice_InternalFlash =
{
    &SF2_CM3_blDriver::InitializeDevice,
    &SF2_CM3_blDriver::UninitializeDevice,
    &SF2_CM3_blDriver::GetDeviceInfo,
    &SF2_CM3_blDriver::Read,
    &SF2_CM3_blDriver::Write,
    &SF2_CM3_blDriver::Memset,
    &SF2_CM3_blDriver::GetSectorMetadata,
    &SF2_CM3_blDriver::SetSectorMetadata,
    &SF2_CM3_blDriver::IsBlockErased,
    &SF2_CM3_blDriver::EraseBlock,
    &SF2_CM3_blDriver::SetPowerState,
    &SF2_CM3_blDriver::MaxSectorWrite_uSec,
    &SF2_CM3_blDriver::MaxBlockErase_uSec,
};


