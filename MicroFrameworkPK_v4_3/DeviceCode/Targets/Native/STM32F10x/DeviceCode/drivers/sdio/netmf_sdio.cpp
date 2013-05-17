/*
 * netmf_sdio.cpp
 *
 *  Created on: May 16, 2013
 *      Author: Nived
 */


#include "netmf_sdio.h"


SDIO_Driver g_SDIODriver;

void SDIO_Driver::GPIOClockEnable()
{
	/*!< GPIOC and GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | SD_DETECT_GPIO_CLK, ENABLE);
}

void SDIO_Driver::GPIOInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/*!< Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*!< Configure PD.02 CMD line */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
	GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);

}

void SDIO_Driver::SDIOClockEnable()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
}


void SDIO_Driver::DMAClockEnable()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}


DeviceStatus CmdResp1Error(UINT8 cmd)
{
  while (!(SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
  }

  SDIO->ICR = SDIO_STATIC_FLAGS;

  return (SD_Error)(SDIO->RESP1 &  SD_OCR_ERRORBITS);
}

DeviceStatus SDIO_Driver::GetStatus()
{
	DeviceStatus status = DS_Success;

	UINT32 timeout = SDIO_CMD0TIMEOUT;

	while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET))
	{
	    timeout--;
	}

	if (timeout == 0)
	{
	    status = DS_Timeout;
	    return status;
	}

	  /*!< Clear all the static flags */
	  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

	  return (status);
}

DeviceStatus SDIO_Driver::CmdResp7Error()
{
	DeviceStatus status = DS_Success;

	UINT32 timeout = SDIO_CMD0TIMEOUT;

	status = SDIO->STA;

	 while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) && (timeout > 0))
	 {
	   timeout--;
	   status = SDIO->STA;
	 }

	  if ((timeout == 0) || (status & SDIO_FLAG_CTIMEOUT))
	  {
	    /*!< Card is not V2.0 complient or card does not support the set voltage range */
	    status = DS_Timeout;
	    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
	    return status;
	  }

	  if (status & SDIO_FLAG_CMDREND)
	  {
	    /*!< Card is SD V2.0 compliant */
	    status = DS_Success;
	    SDIO_ClearFlag(SDIO_FLAG_CMDREND);
	    return (status);
	  }
	  return(status);
}

DeviceStatus SDIO_Driver::PowerOn()
{
	DeviceStatus status = DS_Success;

	UINT32 SDType = SD_STD_CAPACITY;

	SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV;
	SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
	SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
	SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
	SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
	SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
	SDIO_Init(&SDIO_InitStructure);

	/*!< Set Power State to ON */
    SDIO_SetPowerState(SDIO_PowerState_ON);

	 /*!< Enable SDIO Clock */
	SDIO_ClockCmd(ENABLE);

	/*!< CMD0: GO_IDLE_STATE ---------------------------------------------------*/
	/*!< No CMD response required */
	SDIO_CmdInitStructure.SDIO_Argument = 0x0;
	SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_GO_IDLE_STATE;
	SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_No;
	SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_CmdInitStructure);

	status = GetStatus();

	if(status != DS_Success)
	{
		return status;
	}

	 /*!< CMD8: SEND_IF_COND ----------------------------------------------------*/
	 /*!< Send CMD8 to verify SD card interface operating condition */
	 /*!< Argument: - [31:12]: Reserved (shall be set to '0')
	               - [11:8]: Supply Voltage (VHS) 0x1 (Range: 2.7-3.6 V)
	               - [7:0]: Check Pattern (recommended 0xAA) */
	 /*!< CMD Response: R7 */
	 SDIO_CmdInitStructure.SDIO_Argument = SD_CHECK_PATTERN;
	 SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_SEND_IF_COND;
	 SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	 SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	 SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	 SDIO_SendCommand(&SDIO_CmdInitStructure);


	 status =  CmdResp7Error();

	 if(status == DS_Success)
	 {
		 CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; /*!< SD Card 2.0 */
		 SDType = SD_HIGH_CAPACITY;
	 }
	 else
	 {
		     /*!< CMD55 */
	     SDIO_CmdInitStructure.SDIO_Argument = 0x00;
	     SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
	     SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	     SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	     SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	     SDIO_SendCommand(&SDIO_CmdInitStructure);
		 status = CmdResp1Error(SD_CMD_APP_CMD);
	  }
		   /*!< CMD55 */
	  SDIO_CmdInitStructure.SDIO_Argument = 0x00;
	  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
	  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	  SDIO_SendCommand(&SDIO_CmdInitStructure);
	  status = CmdResp1Error(SD_CMD_APP_CMD);

		   /*!< If errorstatus is Command TimeOut, it is a MMC card */
		   /*!< If errorstatus is SD_OK it is a SD card: SD card 2.0 (voltage range mismatch)
		      or SD card 1.x */
	  if (status == SD_OK)
	  {
	     /*!< SD CARD */
	     /*!< Send ACMD41 SD_APP_OP_COND with Argument 0x80100000 */
	     while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL))
	     {
	       /*!< SEND CMD55 APP_CMD with RCA as 0 */
	       SDIO_CmdInitStructure.SDIO_Argument = 0x00;
	       SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
	       SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	       SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	       SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	       SDIO_SendCommand(&SDIO_CmdInitStructure);

	       status = CmdResp1Error(SD_CMD_APP_CMD);

		   if (errorstatus != SD_OK)
		   {
		       return(status);
		   }

		   SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_SD | SDType;
		   SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_OP_COND;
		   SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
		   SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
		   SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
		   SDIO_SendCommand(&SDIO_CmdInitStructure);

		   status = CmdResp3Error();

		   if (errorstatus != SD_OK)
		   {
		      return(errorstatus);
		   }

		   response = SDIO_GetResponse(SDIO_RESP1);
		   validvoltage = (((response >> 31) == 1) ? 1 : 0);
		   count++;
		}
		if (count >= SD_MAX_VOLT_TRIAL)
		{
		    status = SD_INVALID_VOLTRANGE;
		    return (status);
		}

		if (response &= SD_HIGH_CAPACITY)
		{
		     CardType = SDIO_HIGH_CAPACITY_SD_CARD;
		}

    }/*!< else MMC Card */

		   return (status);

}

DeviceStatus SDIO_Driver::Initialize()
{
	GPIOClockEnable();

	GPIOInit();

	SDIOClockEnable();

	DMAClockEnable();

}

