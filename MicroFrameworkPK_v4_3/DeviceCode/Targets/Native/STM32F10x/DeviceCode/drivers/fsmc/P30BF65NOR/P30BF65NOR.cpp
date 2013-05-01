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

void P30BF65NOR_Driver::Initialize(void)
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

}

void P30BF65NOR_Driver::ReadID()
{
	NOR_WRITE(ADDR_SHIFT(0), 0x90);

	norId.Manufacturer_Code = *(__IO UINT16 *) ADDR_SHIFT(0x0000);
	norId.Device_Code1 = *(__IO UINT16 *) ADDR_SHIFT(0x0001);

}

DeviceStatus P30BF65NOR_Driver::EraseBlock(UINT32 BlockAddr)
{

}

DeviceStatus P30BF65NOR_Driver::EraseChip()
{

}


DeviceStatus P30BF65NOR_DriverWriteHalfWord(UINT32 WriteAddr, UINT16 data)
{

}

DeviceStatus P30BF65NOR_DriverWriteBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite)
{

}

DeviceStatus P30BF65NOR_DriverProgramBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite)
{

}

DeviceStatus P30BF65NOR_DriverReadHalfWord(UINT32 ReadAddr)
{

}

DeviceStatus P30BF65NOR_DriverReadBuffer(UINT16* pBuffer, UINT32 ReadAddr, UINT32 NumHalfWordToRead)
{

}

DeviceStatus P30BF65NOR_DriverReturnToReadMode(void)
{

}

DeviceStatus P30BF65NOR_DriverReset(void)
{

}

DeviceStatus P30BF65NOR_Driver::GetStatus(UINT32 Timeout)
{

}
