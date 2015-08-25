////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_bl.h"
#include "netmf_bl_nor.h"

//--//
#define CR_PER_Reset             ((uint32_t)0x00001FFD)

STM32F10x_blDriver_nor g_STM32F10x_blDriver_nor;

BOOL STM32F10x_blDriver::InitializeDevice( void* context )
{
	BOOL retStatus = false;
	retStatus = g_STM32F10x_blDriver_nor.InitializeDevice(context);
	return retStatus;
    //return TRUE;
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
	UINT16* addressPtr    = (UINT16* )address;	// 16-bit writing
	UINT16* buffPtr       = (UINT16* )pSectorBuff;
	//if(ReadModifyWrite) {						// CLR is asking us to take care of the erasing
	// Nived.Sivadas - VS assumes flash is erased before writing into flash
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

	UINT16* addressPtr    = (UINT16* )Address;	// 16-bit writing
	// @todo this method sucks - make better
	if(*addressPtr ==  0xFFFF) {
		return TRUE;
	} else {
#if 0
		FLASH_Unlock();
		while(FLASH->SR2 & FLASH_SR_BSY);
		FLASH->KEYR2 = 0x45670123;			// Key1
		FLASH->KEYR2 = 0xCDEF89AB;
		FLASH->CR2   = FLASH_CR_MER;
		//FLASH->AR2	= address;
		FLASH->CR2	= FLASH_CR_STRT;
		while(FLASH->SR2 & FLASH_SR_BSY);
		FLASH->CR2 &= CR_PER_Reset;
		if(*addressPtr ==  0xFFFF) {
			return TRUE;
		}
		else{
			return FALSE;
		}
#endif
		return FALSE;

	}
}

#if 0
BOOL STM32F10x_blDriver::EraseBlock(void* context, ByteAddress Page_Address)
{
	  FLASH_Status status = FLASH_COMPLETE;
	  /* Check the parameters */
	  assert_param(IS_FLASH_ADDRESS(Page_Address));

#ifdef STM32F10X_XL
	  if(Page_Address < FLASH_BANK1_END_ADDRESS)
	  {
	    /* Wait for last operation to be completed */
	    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
	    if(status == FLASH_COMPLETE)
	    {
	      /* if the previous operation is completed, proceed to erase the page */
	      FLASH->CR = FLASH_CR_PER;
	      FLASH->AR = Page_Address;
	      FLASH->CR = FLASH_CR_STRT;

	      /* Wait for last operation to be completed */
	      status = FLASH_WaitForLastBank1Operation(EraseTimeout);
	      if(status != FLASH_TIMEOUT)
	      {
	        /* if the erase operation is completed, disable the PER Bit */
	        FLASH->CR &= CR_PER_Reset;
	      }
	    }
	  }
	  else
	  {
	    /* Wait for last operation to be completed */
	    status = FLASH_WaitForLastBank2Operation(EraseTimeout);
	    if(status == FLASH_COMPLETE)
	    {
	      /* if the previous operation is completed, proceed to erase the page */
	      FLASH->CR2 = FLASH_CR_PER;
	      FLASH->AR2 = Page_Address;
	      FLASH->CR2 |= FLASH_CR_STRT;

	      /* Wait for last operation to be completed */
	      status = FLASH_WaitForLastBank2Operation(EraseTimeout);
	      if(status != FLASH_TIMEOUT)
	      {
	        /* if the erase operation is completed, disable the PER Bit */
	        FLASH->CR2 &= CR_PER_Reset;
	      }
	    }
	  }
#else
	  /* Wait for last operation to be completed */
	  status = FLASH_WaitForLastOperation(EraseTimeout);

	  if(status == FLASH_COMPLETE)
	  {
	    /* if the previous operation is completed, proceed to erase the page */
	    FLASH->CR|= CR_PER_Set;
	    FLASH->AR = Page_Address;
	    FLASH->CR|= CR_STRT_Set;

	    /* Wait for last operation to be completed */
	    status = FLASH_WaitForLastOperation(EraseTimeout);
	    if(status != FLASH_TIMEOUT)
	    {
	      /* if the erase operation is completed, disable the PER Bit */
	      FLASH->CR &= CR_PER_Reset;
	    }
	  }
#endif /* STM32F10X_XL */

	  UINT16* addressPtr    = (UINT16* )Page_Address;	// 16-bit writing
	  if(*addressPtr != 0xffff)
	  	{
	  			hal_printf("EraseBlock Failed 0x%08x", (UINT32)addressPtr);
	  			return FALSE;
	  	}

	  /* Return the Erase Status */
	  return status;

}
#endif



// I don't know why this code was made so painful. I'll probably rediscover.
// But you definitely can't do a global lock on a flash erase...
// --NPS 2014-06-26
BOOL STM32F10x_blDriver::EraseBlock( void* context, ByteAddress address )
{
	FLASH_Status stat;

	FLASH_Unlock();

	stat = FLASH_ErasePage(address);

	while(stat != FLASH_COMPLETE) {
		stat = FLASH_ErasePage(address);
	}

	return TRUE;

	/*
	GLOBAL_LOCK(irq);

	FLASH_Unlock();

	UINT16* addressPtr    = (UINT16* )address;	// 16-bit writing
	//if(*addressPtr == 0xffff)
	//	return TRUE;

	if(address < FLASH_BANK1_END_ADDRESS)
	{
		while(FLASH->SR & FLASH_SR_BSY);	// Wait until flash is ready
		FLASH->KEYR = 0x45670123;			// Key1
		FLASH->KEYR = 0xCDEF89AB;			// Key2
		FLASH->CR   = FLASH_CR_PER;			// Set Page Erase Bit
		FLASH->AR	= address;				// Set arbitrary address in page
		FLASH->CR	= FLASH_CR_PER | FLASH_CR_STRT;
		FLASH->CR	= FLASH_CR_PER | FLASH_CR_STRT;
		while(FLASH->SR & FLASH_SR_BSY);	// Wait until flash is ready
		 FLASH->CR = FLASH_CR_LOCK;
	}
	else
	{
		while(FLASH->SR2 & FLASH_SR_BSY);
		FLASH->KEYR2 = 0x45670123;			// Key1
		FLASH->KEYR2 = 0xCDEF89AB;
		FLASH->CR2   = FLASH_CR_PER;
		FLASH->AR2	= address;
		FLASH->CR2	= FLASH_CR_PER | FLASH_CR_STRT;
		FLASH->CR2	= FLASH_CR_PER | FLASH_CR_STRT;
		//FLASH->CR = FLASH_CR_PER | FLASH_CR_STRT;
		while(FLASH->SR2 & FLASH_SR_BSY);
	    FLASH->CR2 = FLASH_CR_LOCK;

	}
	if(*addressPtr != 0xffff)
	{
			hal_printf("EraseBlock Failed 0x%08x", (UINT32)addressPtr);
			return FALSE;
	}
	//FLASH_ErasePage(address);
    return TRUE;
	*/
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


