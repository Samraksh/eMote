/*
 * netmf_sdio.cpp
 *
 *  Created on: May 16, 2013
 *      Author: Nived
 */

#include <tinyhal.h>
#include "netmf_sdio.h"

#include "../intc/SF2.h"


SDIO_Driver g_SDIODriver;

void SDIO_Driver::GPIOClockEnable()
{
	/*!< GPIOC and GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | SD_DETECT_GPIO_CLK, ENABLE);
}

void SDIO_Driver::GPIOInit()
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

void SDIO_Driver::SDIOClockEnable()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
}


void SDIO_Driver::DMAClockEnable()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}


DeviceStatus SDIO_Driver::Initialize(SDIOStatusFuncPtrType sdCallbackFunction)
{
	GPIOClockEnable();

	GPIOInit();

	SDIOClockEnable();

	DMAClockEnable();

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 37, FALSE);

	CPU_GPIO_SetPinState((GPIO_PIN) 37, TRUE);

	HAL_Time_Sleep_MicroSeconds(500);

	this->sdCallbackFunction = sdCallbackFunction;

	if( !CPU_INTC_ActivateInterrupt(STM32_AITC::c_IRQ_INDEX_SDIO, SDIO_Driver::SDIO_HANDLER, NULL) )
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

DeviceStatus SDIO_Driver::EraseBlock(UINT32 startaddr, UINT32 endaddr)
{
	if(SD_Erase(startaddr,endaddr) != SD_OK)
		return DS_Fail;



	return DS_Success;
}

DeviceStatus SDIO_Driver::WriteBlock(UINT8 *writeBuff, UINT32 WriteAddr, UINT16 BlockSize)
{
	if(SD_WriteBlock(writeBuff, WriteAddr, BlockSize) != SD_OK)
		return DS_Fail;

	return DS_Success;
}

DeviceStatus SDIO_Driver::ReadBlock(UINT8 *readBuff, UINT32 WriteAddr, UINT16 BlockSize)
{
	if(SD_ReadBlock(readBuff, WriteAddr, BlockSize) != SD_OK)
		return DS_Fail;

	return DS_Success;
}

void SDIO_Driver::SDIO_HANDLER( void* Param )
{
	if(SDIO_GetFlagStatus(SDIO_IT_DATAEND))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data Transfer Complete\n");
		SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
		SDIO_ITConfig(SDIO_IT_DATAEND, DISABLE);
		if(g_SDIODriver.sdCallbackFunction)
			g_SDIODriver.sdCallbackFunction(DS_Success);

	}
	if(SDIO_GetFlagStatus(SDIO_IT_DCRCFAIL))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data CRC Check Failed\n");
		SDIO_ClearITPendingBit(SDIO_IT_DCRCFAIL);
		SDIO_ITConfig(SDIO_IT_DCRCFAIL, DISABLE);
		if(g_SDIODriver.sdCallbackFunction)
			g_SDIODriver.sdCallbackFunction(DS_Fail);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_DTIMEOUT))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data Path State Machine Time out\n");
		SDIO_ClearITPendingBit(SDIO_IT_DTIMEOUT);
		SDIO_ITConfig(SDIO_IT_DTIMEOUT, DISABLE);
		if(g_SDIODriver.sdCallbackFunction)
			g_SDIODriver.sdCallbackFunction(DS_Fail);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_TXUNDERR))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] TX FIFO underrun\n");
		SDIO_ClearITPendingBit(SDIO_IT_TXUNDERR);
		SDIO_ITConfig(SDIO_IT_TXUNDERR, DISABLE);
		if(g_SDIODriver.sdCallbackFunction)
			g_SDIODriver.sdCallbackFunction(DS_Fail);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_STBITERR))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Start bit error\n");
		SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
		SDIO_ITConfig(SDIO_IT_STBITERR, DISABLE);
		if(g_SDIODriver.sdCallbackFunction)
			g_SDIODriver.sdCallbackFunction(DS_Fail);
	}

}
