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


P30BF65NOR_Driver gNORDriver;

DeviceStatus P30BF65NOR_Driver::Initialize(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	GPIO_InitTypeDef GPIO_InitStructure;


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
	                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);

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

	  if(norId.Manufacturer_Code != MANUFACTURE_ID)
		  return DS_Fail;

	  return DS_Success;

}


BOOL P30BF65NOR_Driver::IsBlockLocked(UINT32 address)
{
	UINT32 baseAddress = ((address + BOOT_BLOCK_OFFSET) & BASE_BLOCK_ADDRESS_MASK);

	NOR_WRITE(ADDR_SHIFT(baseAddress), 0x90);

	UINT16 blockLockStatus = *(__IO UINT16 *) ADDR_SHIFT(baseAddress + 0x02);

	//NOR_WRITE(ADDR_SHIFT(BOOT_BLOCK_OFFSET + address), 0x60);
	//NOR_WRITE(ADDR_SHIFT(BOOT_BLOCK_OFFSET + address), 0xD0);

	//blockLockStatus = *(__IO UINT16 *) ADDR_SHIFT(baseAddress + 0x02);

	if(blockLockStatus & 1 == 0x01)
		return TRUE;
	else
		return FALSE;


}

DeviceStatus P30BF65NOR_Driver::ReadID()
{
	NOR_WRITE(ADDR_SHIFT(0), 0x90);

	norId.Manufacturer_Code = *(__IO UINT16 *) ADDR_SHIFT(0x0000);
	norId.Device_Code1 = *(__IO UINT16 *) ADDR_SHIFT(0x0001);

	return DS_Success;

}


UINT16 P30BF65NOR_Driver::GetDeviceId()
{
	return norId.Device_Code1;
}

UINT16 P30BF65NOR_Driver::GetManufactureId()
{
	return norId.Manufacturer_Code;
}

DeviceStatus P30BF65NOR_Driver::EraseBlock(UINT32 BlockAddr)
{
	if((BlockAddr + BOOT_BLOCK_OFFSET) > 0x7FFFFF)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Address out of range of flash");
		return DS_Fail;
	}

	if(GetStatus() != DS_Success)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Flash is busy");
		return DS_Busy;
	}

	// Clear the status register before doing anything
	ClearStatusRegister();

	NOR_WRITE(ADDR_SHIFT(BOOT_BLOCK_OFFSET + BlockAddr), 0x20);
	NOR_WRITE(ADDR_SHIFT(BOOT_BLOCK_OFFSET + BlockAddr), 0xD0);

	// Wait for 3s for erase to complete
	return GetStatus(3 * 1000 * 1000);
}

DeviceStatus P30BF65NOR_Driver::EraseChip()
{
	for(UINT32 i = FLASH_START_ADDRESS; i <= FLASH_END_ADDRESS; i++)
	{
		if(EraseBlock(i) != DS_Success)
			return DS_Fail;
	}

	return DS_Success;
}

DeviceStatus P30BF65NOR_Driver::BlockUnlock(UINT32 address)
{

	UINT32 baseAddress = ((address + BOOT_BLOCK_OFFSET) & BASE_BLOCK_ADDRESS_MASK);

	if((address + BOOT_BLOCK_OFFSET) > 0x7FFFFF)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Address out of range of flash");
		return DS_Fail;
	}

	NOR_WRITE(ADDR_SHIFT(baseAddress), 0x60);
	NOR_WRITE(ADDR_SHIFT(baseAddress), 0xD0);

	// Check if the block is locked
	NOR_WRITE(ADDR_SHIFT(0), 0x90);
	UINT16 blockLockStatus = *(__IO UINT16 *) ADDR_SHIFT(baseAddress + 0x02);

	// Move back to read array mode
	NOR_WRITE(ADDR_SHIFT(0), 0xFF);

	if((blockLockStatus & 1) != 0)
	{
		return DS_Fail;
	}

	return DS_Success;
}


BOOL P30BF65NOR_Driver::ClearStatusRegister()
{
	NOR_WRITE(ADDR_SHIFT(0x0), 0x50);

	return TRUE;
}

DeviceStatus P30BF65NOR_Driver::WriteHalfWord(UINT32 WriteAddr, UINT16 data)
{
	// Check if memory is busy
	if(GetStatus() != DS_Success)
	{
		NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Flash is busy");
		return DS_Busy;
	}

	// Clear the status register before doing anything
	ClearStatusRegister();

	// Check if block is locked and unlock it
	if(IsBlockLocked(WriteAddr))
	{
		if(BlockUnlock(WriteAddr) != DS_Success)
			return DS_Fail;
	}

	// Write data
	NOR_WRITE(ADDR_SHIFT(BOOT_BLOCK_OFFSET + WriteAddr), 0x40);
	NOR_WRITE((Bank1_NOR2_ADDR + BOOT_BLOCK_OFFSET + WriteAddr), data);

	// Give a 500us timeout, the manual claims the write operation is complete in 175us
	if(GetStatus(500) != DS_Success)
	{
		return DS_Fail;
	}
	else
	{
		// Return to read mode if the write was successful
		NOR_WRITE(ADDR_SHIFT(BOOT_BLOCK_OFFSET + WriteAddr), 0xFF);
	}

	return DS_Success;

}

DeviceStatus P30BF65NOR_Driver::WriteBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite)
{
	for(UINT32 i = 0; i < NumHalfWordToWrite; i++)
	{

	}
}

DeviceStatus P30BF65NOR_Driver::ProgramBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite)
{

}

UINT16 P30BF65NOR_Driver::ReadHalfWord(UINT32 ReadAddr)
{
	NOR_WRITE(ADDR_SHIFT(BOOT_BLOCK_OFFSET + ReadAddr), 0xFF);
	return *(__IO UINT16 *) ADDR_SHIFT(BOOT_BLOCK_OFFSET + ReadAddr);
}

DeviceStatus P30BF65NOR_Driver::ReadBuffer(UINT16* pBuffer, UINT32 ReadAddr, UINT32 NumHalfWordToRead)
{

}

DeviceStatus P30BF65NOR_Driver::ReturnToReadMode(void)
{

}

DeviceStatus P30BF65NOR_Driver::Reset(void)
{

}


UINT16 P30BF65NOR_Driver::GetStatusRegister()
{
	NOR_WRITE(ADDR_SHIFT(0x0000), 0x70);

	NOR_SR =  *(__IO UINT16 *) ADDR_SHIFT(0x0000);

	return NOR_SR;
}

DeviceStatus  P30BF65NOR_Driver::GetStatus(UINT32 Timeout)
{
	UINT64 timeoutTicks = CPU_MicrosecondsToTicks(Timeout);

	UINT64 timeoutAt = timeoutTicks + HAL_Time_CurrentTicks();

	while((INT64) (timeoutAt - HAL_Time_CurrentTicks()) > 0)
	{
		NOR_WRITE(ADDR_SHIFT(0x0000), 0x70);

		NOR_SR =  *(__IO UINT16 *) ADDR_SHIFT(0x0000);

#if defined(NOR_DEBUGGING_ENABLED)
		if(NOR_SR & BLOCK_LOCKED_STATUS_BIT)
			NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] The Block is locked during program or erase");

		if(NOR_SR & ERASE_STATUS_BIT)
			NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Erase Error");

		if(NOR_SR & PROGRAM_STATUS_BIT)
			NOR_DEBUG_PRINT("[NATIVE] [NOR Driver] Program Error");
#endif

		if((NOR_SR & DEVICE_READY_STATUS_BIT) == 1)
			return DS_Success;
	}

	return DS_Fail;

}

// Read the status bit in the device status register
DeviceStatus P30BF65NOR_Driver::GetStatus()
{
	NOR_WRITE(ADDR_SHIFT(0x0000), 0x70);

	NOR_SR =  *(__IO UINT16 *) ADDR_SHIFT(0x0000);

	if(NOR_SR & DEVICE_READY_STATUS_BIT)
	{
		return DS_Success;
	}
	else
	{
		return DS_Busy;
	}
}
