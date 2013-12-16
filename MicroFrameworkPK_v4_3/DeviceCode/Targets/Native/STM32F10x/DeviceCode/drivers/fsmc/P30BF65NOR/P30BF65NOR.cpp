/*
 *
 *
 *
 *
 *
 *
 *
 */

#include <tinyhal.h>
#include <gpio\stm32f10x_gpio.h>
#include <rcc\stm32f10x_rcc.h>
#include <fsmc\stm32f10x_fsmc.h>
#include "P30BF65NOR.h"

#define CHECK_BIT(var, pos)	((var) & (1<<(pos)))

P30BF65NOR_Driver gNORDriver;

// Initializes the FSMC, the address and data lines and enables the clocks, also turns on FLASH_RESET pin  for the emoteDotNow
// Returns DS_Fail in the event we are unable to read the manufacture id at the end of the initialization else returns success
DeviceStatus P30BF65NOR_Driver::Initialize(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
	                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

	// Turn on the flash reset pin
	CPU_GPIO_EnableOutputPin((GPIO_PIN) FLASH_RESET, TRUE);

	/*-- GPIO Configuration ------------------------------------------------------*/
	  /*!< NOR Data lines configuration */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
	                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
	                                GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

	  /*!< NOR Address lines configuration */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
	                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 |
	                                GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_Init(GPIOF, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
	                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	  GPIO_Init(GPIOG, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

	  /*!< NOE and NWE configuration */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  /*!< NE2 configuration */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	  GPIO_Init(GPIOG, &GPIO_InitStructure);

	  /*!< Configure PD6 for NOR memory Ready/Busy signal */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  /*-- FSMC Configuration ----------------------------------------------------*/
	  p.FSMC_AddressSetupTime = 0x02;
	  p.FSMC_AddressHoldTime = 0x00;
	  p.FSMC_DataSetupTime = 0x05;
	  p.FSMC_BusTurnAroundDuration = 0x00;
	  p.FSMC_CLKDivision = 0x00;
	  p.FSMC_DataLatency = 0x00;
	  p.FSMC_AccessMode = FSMC_AccessMode_B;

	  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
	  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	  //FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

	  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	  /*!< Enable FSMC Bank1_NOR Bank */
	  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);

	  // Read the details from the flash
	  ReadID();

#if defined(NOR_DEBUGGING_ENABLED)
	  CPU_GPIO_EnableOutputPin((GPIO_PIN) ERASE_PROFILE_PIN, FALSE);
	  CPU_GPIO_EnableOutputPin((GPIO_PIN) WRITE_PROFILE_PIN, FALSE);
	  CPU_GPIO_EnableOutputPin((GPIO_PIN) READ_PROFILE_PIN, FALSE);
#endif

	  if(norId.Manufacturer_Code != MANUFACTURE_ID)
		  return DS_Fail;

	  ReturnToReadMode();

	  return DS_Success;

}

// This function checks if the block in which address resides is locked and returns true if that is the case
// or false if the block  is not locked. This function internally derives the base block address from the address input
BOOL P30BF65NOR_Driver::IsBlockLocked(UINT32 address)
{
	// Obtain the base block address given any address with in the block
	UINT32 baseAddress = ((address) & BASE_BLOCK_ADDRESS_MASK);

	// Read device id or read configuration command
	NOR_WRITE(ADDR_SHIFT(baseAddress, 0), 0x90);

	// Read the block lock status register
	UINT16 blockLockStatus = *(__IO UINT16 *) ADDR_SHIFT(baseAddress , 0x02);

	// Check the block lock status
	if(blockLockStatus & 1 == 0x01)
		return TRUE;
	else
		return FALSE;


}


// This function reads the manufacture and device id of the flash device. This function is used by Initialize mainly as a sanity
// check to ensure that initialization was successful and the device is accessible
DeviceStatus P30BF65NOR_Driver::ReadID()
{
	// Issue read device id/read configuration register command
	NOR_WRITE(ADDR_SHIFT(0, 0), 0x90);

	// Read the manufacture id
	norId.Manufacturer_Code = *(__IO UINT16 *) ADDR_SHIFT(0x0000, 0);

	// Read the device code
	norId.Device_Code1 = *(__IO UINT16 *) ADDR_SHIFT(0x0, 0x1);

	return DS_Success;

}


// Returns the device code to the client
UINT16 P30BF65NOR_Driver::GetDeviceId()
{
	return norId.Device_Code1;
}

// Returns the manufacture id to the client
UINT16 P30BF65NOR_Driver::GetManufactureId()
{
	return norId.Manufacturer_Code;
}

// This function erases the block given block address and returns DS_Busy if the flash is busy doing something else or DS_Fail if the
// function was unable to erase the block
DeviceStatus P30BF65NOR_Driver::EraseBlock(UINT32 BlockAddr)
{

	DeviceStatus status;
	NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Inside EraseBlock\n");

	// Check if the block address is within the range of the flash device
	if((BlockAddr + BOOT_BLOCK_OFFSET) > FLASH_LIMIT)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Address out of range of flash\n");
		return DS_Fail;
	}

	// Check if flash is busy doing something
	if(GetStatus() != DS_Success)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Flash is busy\n");
		return DS_Busy;
	}

	// Clear the status register before doing anything
	ClearStatusRegister();

	// Check if block is locked and unlock it
	if(IsBlockLocked(BlockAddr))
	{
		if(BlockUnlock(BlockAddr) != DS_Success)
			return DS_Fail;
	}

#if defined(NOR_DEBUGGING_ENABLED)
	CPU_GPIO_SetPinState((GPIO_PIN) ERASE_PROFILE_PIN, TRUE);
#endif

	// Issue the erase command on the block
	NOR_WRITE(ADDR_SHIFT(BlockAddr, 0), 0x20);
	NOR_WRITE(ADDR_SHIFT(BlockAddr, 0), 0xD0);



	// Wait with an 8s timeout for the erase to complete
	status = GetStatus(8 * 1000 * 1000);

#if defined(NOR_DEBUGGING_ENABLED)
	CPU_GPIO_SetPinState((GPIO_PIN) ERASE_PROFILE_PIN, FALSE);
#endif


	// Return the flash to read mode at the end of erase
	ReturnToReadMode();

	// Check to see if the erase worked by reading the first address in the block
	// This is not a comprehensive check, but checking every address is too expensive and does not really buy us anything
	if(ReadHalfWord(BlockAddr)  != 0xffff)
		return DS_Fail;

	return status;
}

// This NOR flash does not support complete chip erase, so this has to be simulated with a block by block erase.
// Extremely slow process, use only if ship is sinking :)
DeviceStatus P30BF65NOR_Driver::EraseChip()
{
	for(UINT32 i = FLASH_START_ADDRESS; i <= FLASH_END_ADDRESS; i = i + BLOCK_SIZE)
	{
		if(EraseBlock(i) != DS_Success)
			return DS_Fail;
	}

	return DS_Success;
}

// This function is designed to unlock a block given an address of the location with in the block
DeviceStatus P30BF65NOR_Driver::BlockUnlock(UINT32 address)
{

	// Derive the base address of the block given an address with in the block
	UINT32 baseAddress = ((address) & BASE_BLOCK_ADDRESS_MASK);

	// Check if the address is out of range of the current flash
	if((address + BOOT_BLOCK_OFFSET) > FLASH_LIMIT)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Address out of range of flash\n");
		return DS_Fail;
	}

	// Issue the block unlock command
	NOR_WRITE(ADDR_SHIFT(baseAddress, 0), 0x60);
	NOR_WRITE(ADDR_SHIFT(baseAddress, 0), 0xD0);

	// Check if the block is locked
	NOR_WRITE(ADDR_SHIFT(0,0), 0x90);
	UINT16 blockLockStatus = *(__IO UINT16 *) ADDR_SHIFT(baseAddress , 0x02);

	// Move back to read array mode
	NOR_WRITE(ADDR_SHIFT(0,0), 0xFF);

	// check the status of lock after unlocking
	if((blockLockStatus & 1) != 0)
	{
		return DS_Fail;
	}

	return DS_Success;
}

// This function is use to clear the status register
BOOL P30BF65NOR_Driver::ClearStatusRegister()
{
	NOR_WRITE(ADDR_SHIFT(0, 0), 0x50);

	return TRUE;
}

// This function writes 16bit word to the address specified by WriteAddr
// Returns DeviceStatus DS_Busy - if the flash is busy doing something else
//		                dS_Fail - if write operation failed
DeviceStatus P30BF65NOR_Driver::WriteHalfWord(UINT32 WriteAddr, UINT16 data)
{

	DeviceStatus status;

	// Check if memory is busy
	if(GetStatus() != DS_Success)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Flash is busy\n");
		return DS_Busy;
	}

	// Check to see if you are writing to an unerased location, return failure if that is true
	// The 4th bit should be zero in incoming "data" and should be one (1000) at writeAddr.
	// Raise error if above condition is not met.
	if(ReadHalfWord(WriteAddr) != 0xffff)
	{
		if((CHECK_BIT(data,4)) != 0)
		{
			UINT16 readHalfWordValue = ReadHalfWord(WriteAddr);
			if((CHECK_BIT(readHalfWordValue,4)) != (1<<4))
			{
				NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Attempting to write to a non erased location\n");
				hal_printf("%x\n", WriteAddr);
				return DS_Fail;
			}
		}
	}

	/*if(((data & (1<<4)) != 0) && ((ReadHalfWord(WriteAddr) & (1<<4)) != 1) )
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Attempting to write to a non erased location\n");
		return DS_Fail;
	}*/

	// Clear the status register before doing anything
	ClearStatusRegister();

	// Check if block is locked and unlock it
	if(IsBlockLocked(WriteAddr))
	{
		if(BlockUnlock(WriteAddr) != DS_Success)
			return DS_Fail;
	}

#if defined(NOR_DEBUGGING_ENABLED)
	CPU_GPIO_SetPinState((GPIO_PIN) WRITE_PROFILE_PIN, TRUE);
#endif

	// Issue write data command
	NOR_WRITE(ADDR_SHIFT(WriteAddr, 0x0), 0x40);
	//NOR_WRITE((Bank1_NOR2_ADDR + BOOT_BLOCK_OFFSET + WriteAddr), 0x40);
	NOR_WRITE((Bank1_NOR2_ADDR + BOOT_BLOCK_OFFSET + WriteAddr), data);


	// Wait for write to finish with a 10ms timeout, manual claims writes completion in 175us
	status = GetStatus(10000);

#if defined(NOR_DEBUGGING_ENABLED)
	CPU_GPIO_SetPinState((GPIO_PIN) WRITE_PROFILE_PIN, FALSE);
#endif

	// Return to read mode
	ReturnToReadMode();

	// Give a 500us timeout, the manual claims the write operation is complete in 175us
	if(status != DS_Success)
	{
		return DS_Fail;
	}


	return DS_Success;

}

// Internally calls write half word
DeviceStatus P30BF65NOR_Driver::WriteBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite)
{
	//pBuffer looks something like this - 00 10 00 00 06 00 00 00 ff ff ff ff. pBuffer[0] has 4096 (10 00). 1000 in hex is converted to 4096 in decimal.
	//pBuffer[1] has 00 00. pBuffer[2] has 00 06.
	for(UINT32 i = 0; i < NumHalfWordToWrite; i++)
	{
		if(WriteHalfWord(WriteAddr + i * 0x2, pBuffer[i]) != DS_Success)
			return DS_Fail;
	}

	return DS_Success;
}

// This function will eventually support buffered writes but at this point is not supported
DeviceStatus P30BF65NOR_Driver::ProgramBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite)
{
	return DS_Fail;
}

UINT16 P30BF65NOR_Driver::ReadHalfWord(UINT32 ReadAddr)
{
	UINT16 data = 0;
#if defined(NOR_DEBUGGING_ENABLED)
	CPU_GPIO_SetPinState((GPIO_PIN) READ_PROFILE_PIN, TRUE);
#endif
	NOR_WRITE(ADDR_SHIFT(0,0), 0xFF);
	data =  *(__IO UINT16 *) ADDR_SHIFT(ReadAddr, 0);

#if defined(NOR_DEBUGGING_ENABLED)
	CPU_GPIO_SetPinState((GPIO_PIN) READ_PROFILE_PIN, FALSE);
#endif

	return data;
}

// This function internally calls read half word, responsibility of client to ensure pBuffer is large enough to hold
// all the data that is to be read
DeviceStatus P30BF65NOR_Driver::ReadBuffer(UINT16* pBuffer, UINT32 ReadAddr, UINT32 NumHalfWordToRead)
{
	for(UINT32 i = 0; i < NumHalfWordToRead; i++)
	{
		pBuffer[i] = ReadHalfWord(ReadAddr + i * 0x2);
	}

	return DS_Success;
}

// This function returns the flash to read mode
DeviceStatus P30BF65NOR_Driver::ReturnToReadMode(void)
{
	// Go back to read mode
	NOR_WRITE(ADDR_SHIFT(0,0), 0xFF);

	return DS_Success;
}

// Reset the NOR flash
DeviceStatus P30BF65NOR_Driver::Reset(void)
{
	CPU_GPIO_EnableOutputPin((GPIO_PIN) FLASH_RESET, FALSE);
	HAL_Time_Sleep_MicroSeconds(10);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) FLASH_RESET, TRUE);
	HAL_Time_Sleep_MicroSeconds(10);

	return DS_Success;
}

// Returns the value of the status register
UINT16 P30BF65NOR_Driver::GetStatusRegister()
{
	NOR_WRITE(ADDR_SHIFT(0x0000, 0), 0x70);

	NOR_SR =  *(__IO UINT16 *) ADDR_SHIFT(0x0000, 0);

	return NOR_SR;
}

// Read status register with a timeout
DeviceStatus  P30BF65NOR_Driver::GetStatus(UINT32 Timeout)
{

	UINT64 timeoutTicks = CPU_MicrosecondsToTicks(Timeout);

	UINT64 timeoutAt = timeoutTicks + HAL_Time_CurrentTicks();

	// Wait until device ready or timeout
	while((INT64) (timeoutAt - HAL_Time_CurrentTicks()) > 0)
	{
		NOR_WRITE(ADDR_SHIFT(0x0000, 0), 0x70);

		NOR_SR =  *(__IO UINT16 *) ADDR_SHIFT(0x0000,  0);

		if(NOR_SR & DEVICE_READY_STATUS_BIT)
			return DS_Success;
	}

	// If NOR_DEBUGGING_ENABLED macro is enabled, print out the exact reason for the failure
#if defined(NOR_DEBUGGING_ENABLED)
		if(NOR_SR & BLOCK_LOCKED_STATUS_BIT)
			NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] The Block is locked during program or erase\n");

		if(NOR_SR & ERASE_STATUS_BIT)
			NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Erase Error\n");

		if(NOR_SR & PROGRAM_STATUS_BIT)
			NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Program Error\n");
#endif


	return DS_Fail;

}

// Read the status bit in the device status register and return the value of the status register
DeviceStatus P30BF65NOR_Driver::GetStatus()
{
	NOR_WRITE(ADDR_SHIFT(0x0000, 0x0), 0x70);

	NOR_SR =  *(__IO UINT16 *) ADDR_SHIFT(0x0000,0x0);

	if(NOR_SR & DEVICE_READY_STATUS_BIT)
	{
		return DS_Success;
	}
	else
	{
		return DS_Busy;
	}
}
