/*
 * netmf_sdio.cpp
 *
 *  Created on: May 16, 2013
 *      Author: Nived
 */

#include <tinyhal.h>
#include "netmf_bl_sdio.h"
#include "../INTC/STM32.h"


STM32F10x_blDriver_SDIO g_STM32F10x_blDriver_SDIO;

void STM32F10x_blDriver_SDIO::GPIOClockEnable()
{
	/*!< GPIOC and GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | SD_DETECT_GPIO_CLK, ENABLE);
}

void STM32F10x_blDriver_SDIO::GPIOInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_ConfigurePin(GPIOC, GPIO_Pin_5, GPIO_Mode_Out_PP, GPIO_Speed_50MHz);

	/*!< Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_ConfigurePin(GPIOC, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

	CPU_GPIO_SetPinState((GPIO_PIN) 37, TRUE);


	for(UINT16 i = 0; i < 10000; i++);

	/*!< Configure PD.02 CMD line */
	GPIO_ConfigurePin(GPIOD, GPIO_Pin_2, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

	/*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
	GPIO_ConfigurePin(GPIOC, SD_DETECT_PIN, GPIO_Mode_IPU, GPIO_Speed_50MHz);

}

void STM32F10x_blDriver_SDIO::SDIOClockEnable()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
}


void STM32F10x_blDriver_SDIO::DMAClockEnable()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}


//DeviceStatus STM32F10x_blDriver_SDIO::Initialize(SDIOStatusFuncPtrType sdCallbackFunction)
DeviceStatus STM32F10x_blDriver_SDIO::Initialize()
{
	GPIOClockEnable();
	GPIOInit();
	SDIOClockEnable();
	DMAClockEnable();

	//CPU_GPIO_EnableOutputPin((GPIO_PIN) 37, FALSE);
	//CPU_GPIO_SetPinState((GPIO_PIN) 37, TRUE);

	HAL_Time_Sleep_MicroSeconds(500);

	//this->sdCallbackFunction = sdCallbackFunction;

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_SDIO, STM32F10x_blDriver_SDIO::SDIO_HANDLER, NULL) )
	{
	   	 SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Could not initialize interrupt handler\n");
	   	 return DS_Fail;
	}

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_SDIO))
	{
	  	 SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Could not enable interrupt handler\n");
	   	 return DS_Fail;
	}

	if(SD_Init() != SD_OK)
		return DS_Fail;

	return DS_Success;
}

//------------------------------------------------------//

BOOL STM32F10x_blDriver_SDIO::InitializeDevice( void* context )
{
	/*this->GPIOClockEnable();
	this->GPIOInit();
	this->SDIOClockEnable();
	this->DMAClockEnable();

	HAL_Time_Sleep_MicroSeconds(500);*/

	//this->sdCallbackFunction = sdCallbackFunction;

	/*if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_SDIO, STM32F10x_blDriver_SDIO::SDIO_HANDLER, NULL) )
	{
	   	 SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Could not initialize interrupt handler\n");
	   	 return FALSE;
	}

	if(!CPU_INTC_InterruptEnable(STM32_AITC::c_IRQ_INDEX_SDIO))
	{
	  	 SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Could not enable interrupt handler\n");
	   	 return FALSE;
	}*/

	bool retVal = Initialize();
	if(retVal != DS_Success)
		return FALSE;
	//if(SD_Init() != SD_OK)
		//return FALSE;

	return TRUE;
}

BOOL STM32F10x_blDriver_SDIO::UninitializeDevice( void* context )
{
	return TRUE;
}

const BlockDeviceInfo* STM32F10x_blDriver_SDIO::GetDeviceInfo( void* context )
{
    BLOCK_CONFIG* config = (BLOCK_CONFIG *)context;	// Type case so we can return
    const BlockDeviceInfo* bInfo = config->BlockDeviceInformation;
    return bInfo;
    //return config->BlockDeviceInformation;
}

BOOL STM32F10x_blDriver_SDIO::Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff )
{
	SD_Error status;

	SD_CardInfo SDCardInfo;
	status = SD_GetCardInfo(&SDCardInfo);
	//status = SD_ReadBlock(pSectorBuff, Address, SDCardInfo.CardBlockSize);
	status = SD_ReadBlock(pSectorBuff, Address, NumBytes);
	return TRUE;
}

BOOL STM32F10x_blDriver_SDIO::Write( void* context, ByteAddress address, UINT32 numBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite )
{
	//GLOBAL_LOCK(irq);
	SD_Error status;

	SD_CardInfo SDCardInfo;
	status = SD_GetCardInfo(&SDCardInfo);
	status = SD_WriteBlock(pSectorBuff, address, SDCardInfo.CardBlockSize);

	if(status == SD_OK )
	{
		return TRUE;
	}
	else
		return FALSE;
}

BOOL STM32F10x_blDriver_SDIO::Memset(void* context, ByteAddress address, UINT8 Data, UINT32 numBytes)
{
	SD_Error status;

	SD_CardInfo *cardinfo;
	status = SD_GetCardInfo(cardinfo);

	//TODO: to be modified
	//uint8_t *buffer = (uint8_t*)private_malloc(numBytes);

	/*for(int i = 0; i < numBytes; i++) {
		buffer[i] = Data;
	}*/

	//status = SD_WriteBlock(buffer, address, cardinfo->CardBlockSize);
}

BOOL STM32F10x_blDriver_SDIO::GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL STM32F10x_blDriver_SDIO::SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata)
{
    return FALSE;
}

BOOL STM32F10x_blDriver_SDIO::IsBlockErased( void* context, ByteAddress Address, UINT32 BlockLength )
{
	//if(gNORDriver.ReadHalfWord(Address) != 0xffff)
		//return FALSE;

	return TRUE;

}

BOOL STM32F10x_blDriver_SDIO::EraseBlock( void* context, ByteAddress startaddr )
{
	//if(SD_Erase(startaddr,endaddr) != SD_OK)
		//return FALSE;

	return TRUE;
}

void  STM32F10x_blDriver_SDIO::SetPowerState( void* context, UINT32 State )
{
}

UINT32 STM32F10x_blDriver_SDIO::MaxSectorWrite_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxSectorWrite_uSec;
}


UINT32 STM32F10x_blDriver_SDIO::MaxBlockErase_uSec( void* context )
{
	BLOCK_CONFIG* config = (BLOCK_CONFIG*)context;
	return config->BlockDeviceInformation->MaxBlockErase_uSec;
}

//------------------------------------------------------//

/*DeviceStatus STM32F10x_blDriver_SDIO::EraseBlock(UINT32 startaddr, UINT32 endaddr)
{
	if(SD_Erase(startaddr,endaddr) != SD_OK)
		return DS_Fail;

	return DS_Success;
}*/

DeviceStatus STM32F10x_blDriver_SDIO::WriteBlock(UINT8 *writeBuff, UINT32 WriteAddr, UINT16 BlockSize)
{
	if(SD_WriteBlock(writeBuff, WriteAddr, BlockSize) != SD_OK)
		return DS_Fail;

	return DS_Success;
}

DeviceStatus STM32F10x_blDriver_SDIO::ReadBlock(UINT8 *readBuff, UINT32 WriteAddr, UINT16 BlockSize)
{
	if(SD_ReadBlock(readBuff, WriteAddr, BlockSize) != SD_OK)
		return DS_Fail;

	return DS_Success;
}

void STM32F10x_blDriver_SDIO::SDIO_HANDLER( void* Param )
{
	if(SDIO_GetFlagStatus(SDIO_IT_DATAEND))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data Transfer Complete\n");
		SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
		SDIO_ITConfig(SDIO_IT_DATAEND, DISABLE);
		if(g_STM32F10x_blDriver_SDIO.sdCallbackFunction)
			g_STM32F10x_blDriver_SDIO.sdCallbackFunction(DS_Success);

	}
	if(SDIO_GetFlagStatus(SDIO_IT_DCRCFAIL))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data CRC Check Failed\n");
		SDIO_ClearITPendingBit(SDIO_IT_DCRCFAIL);
		SDIO_ITConfig(SDIO_IT_DCRCFAIL, DISABLE);
		if(g_STM32F10x_blDriver_SDIO.sdCallbackFunction)
			g_STM32F10x_blDriver_SDIO.sdCallbackFunction(DS_Fail);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_DTIMEOUT))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data Path State Machine Time out\n");
		SDIO_ClearITPendingBit(SDIO_IT_DTIMEOUT);
		SDIO_ITConfig(SDIO_IT_DTIMEOUT, DISABLE);
		if(g_STM32F10x_blDriver_SDIO.sdCallbackFunction)
			g_STM32F10x_blDriver_SDIO.sdCallbackFunction(DS_Fail);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_TXUNDERR))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] TX FIFO underrun\n");
		SDIO_ClearITPendingBit(SDIO_IT_TXUNDERR);
		SDIO_ITConfig(SDIO_IT_TXUNDERR, DISABLE);
		if(g_STM32F10x_blDriver_SDIO.sdCallbackFunction)
			g_STM32F10x_blDriver_SDIO.sdCallbackFunction(DS_Fail);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_STBITERR))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Start bit error\n");
		SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
		SDIO_ITConfig(SDIO_IT_STBITERR, DISABLE);
		if(g_STM32F10x_blDriver_SDIO.sdCallbackFunction)
			g_STM32F10x_blDriver_SDIO.sdCallbackFunction(DS_Fail);
	}

}


#pragma arm section code
struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_SDCARD =
{
    &STM32F10x_blDriver_SDIO::InitializeDevice,
    &STM32F10x_blDriver_SDIO::UninitializeDevice,
    &STM32F10x_blDriver_SDIO::GetDeviceInfo,
    &STM32F10x_blDriver_SDIO::Read,
	&STM32F10x_blDriver_SDIO::Write,
	&STM32F10x_blDriver_SDIO::Memset,
    &STM32F10x_blDriver_SDIO::GetSectorMetadata,
    &STM32F10x_blDriver_SDIO::SetSectorMetadata,
    &STM32F10x_blDriver_SDIO::IsBlockErased,
    &STM32F10x_blDriver_SDIO::EraseBlock,
    &STM32F10x_blDriver_SDIO::SetPowerState,
    &STM32F10x_blDriver_SDIO::MaxSectorWrite_uSec,
    &STM32F10x_blDriver_SDIO::MaxBlockErase_uSec,
};


