/*
 * netmf_sdio.cpp
 *
 *  Created on: May 16, 2013
 *      Author: Nived
 */


#include "netmf_sdio.h"

extern "C"
{
extern void ManagedSDCallback(DeviceStatus status);
}

SDIO_Driver g_SDIODriver;

static UINT8 convert_from_bytes_to_power_of_two(uint16_t NumberOfBytes)
{
  UINT8 count = 0;

  while (NumberOfBytes != 1)
  {
    NumberOfBytes >>= 1;
    count++;
  }
  return(count);
}

SD_Error SDIO_Driver::CmdResp3Error(void)
{
	SD_Error errorstatus = SD_OK;
	  UINT32 status;

	  status = SDIO->STA;

	  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
	  {
	    status = SDIO->STA;
	  }

	  if (status & SDIO_FLAG_CTIMEOUT)
	  {
	    errorstatus = SD_CMD_RSP_TIMEOUT;
	    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
	    return(errorstatus);
	  }
	  /*!< Clear all the static flags */
	  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

	  return(errorstatus);
}

SD_Error SDIO_Driver::CmdResp2Error(void)
{
  SD_Error errorstatus = SD_OK;
  UINT32 status;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}


SD_Error SDIO_Driver::CmdResp1Error(UINT8 cmd)
{
  while (!(SDIO->STA & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
  }

  SDIO->ICR = SDIO_STATIC_FLAGS;

  return (SD_Error)(SDIO->RESP1 &  SD_OCR_ERRORBITS);
}

void SDIO_Driver::GPIOClockEnable()
{
	/*!< GPIOC and GPIOD Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | SD_DETECT_GPIO_CLK, ENABLE);
}

void SDIO_Driver::GPIOInit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*!< Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	CPU_GPIO_SetPinState((GPIO_PIN) 37, TRUE);


	for(UINT16 i = 0; i < 10000; i++);

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


SD_Error SDIO_Driver::GetStatus()
{
	SD_Error status = SD_OK;

	//UINT32 timeout = SDIO_CMD0TIMEOUT;

	UINT32 timeout = 0x100;

	while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET))
	{
	    timeout--;
	}

	if (timeout == 0)
	{
	    status = SD_CMD_RSP_TIMEOUT;
	    return status;
	}

	  /*!< Clear all the static flags */
	  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

	  return (status);
}

SD_Error SDIO_Driver::CmdResp7Error()
{
	SD_Error errorstatus = SD_OK;
	UINT32 status;

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
	    errorstatus = SD_CMD_RSP_TIMEOUT;
	    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
	    return errorstatus;
	  }

	  if (status & SDIO_FLAG_CMDREND)
	  {
	    /*!< Card is SD V2.0 compliant */
	    errorstatus = SD_OK;
	    SDIO_ClearFlag(SDIO_FLAG_CMDREND);
	    return (errorstatus);
	  }
	  return(errorstatus);
}

DeviceStatus SDIO_Driver::PowerOn()
{
	SD_Error status = SD_OK;

	UINT32 response = 0, count = 0, validvoltage = 0;
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

	if(status != SD_OK)
	{
		return DS_Fail;
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

	 if(status == SD_OK)
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

		   if (status != SD_OK)
		   {
		       return DS_Fail;
		   }

		   SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_SD | SDType;
		   SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_OP_COND;
		   SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
		   SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
		   SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
		   SDIO_SendCommand(&SDIO_CmdInitStructure);

		   status = CmdResp3Error();

		   if (status != SD_OK)
		   {
		      return DS_Fail;
		   }

		   SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
		   SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);

		   response = SDIO_GetResponse(SDIO_RESP1);
		   validvoltage = (((response >> 31) == 1) ? 1 : 0);


		   //SDIO_DEBUG_SETPINSTATE(29, TRUE);
		   //SDIO_DEBUG_SETPINSTATE(29, FALSE);
		   CPU_GPIO_SetPinState(29, TRUE);
		   CPU_GPIO_SetPinState(29, FALSE);


		   count++;
		}
		if (count >= SD_MAX_VOLT_TRIAL)
		{
		    status = SD_INVALID_VOLTRANGE;
		    return DS_Fail;
		}

		if (response &= SD_HIGH_CAPACITY)
		{
		     CardType = SDIO_HIGH_CAPACITY_SD_CARD;
		}



    }/*!< else MMC Card */

	return DS_Success;

}


SD_Error SDIO_Driver::CmdResp6Error(UINT8 cmd, UINT16 *prca)
{
  SD_Error errorstatus = SD_OK;
  UINT32 status;
  UINT32 response_r1;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  /*!< Check response received is of desired command */
  if (SDIO_GetCommandResponse() != cmd)
  {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  /*!< We have received response, retrieve it.  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)))
  {
    *prca = (uint16_t) (response_r1 >> 16);
    return(errorstatus);
  }

  if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_R6_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_R6_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  return(errorstatus);
}


DeviceStatus SDIO_Driver::InitializeCards()
{
	SD_Error errorstatus = SD_OK;
	UINT16 rca = 0x01;

	if (SDIO_GetPowerState() == SDIO_PowerState_OFF)
	{
	    errorstatus = SD_REQUEST_NOT_APPLICABLE;
	    return	DS_Fail;
	}

	if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
	  {
	    /*!< Send CMD2 ALL_SEND_CID */
	    SDIO_CmdInitStructure.SDIO_Argument = 0x0;
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ALL_SEND_CID;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp2Error();

	    if (SD_OK != errorstatus)
	    {
	      return DS_Fail;
	    }

	    CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
	    CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
	    CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
	    CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
	  }
	  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) ||  (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||  (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType)
	      ||  (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
	  {
	    /*!< Send CMD3 SET_REL_ADDR with argument 0 */
	    /*!< SD Card publishes its RCA. */
	    SDIO_CmdInitStructure.SDIO_Argument = 0x00;
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp6Error(SD_CMD_SET_REL_ADDR, &rca);

	    if (SD_OK != errorstatus)
	    {
	      return DS_Fail;
	    }
	  }

	  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
	  {
	    RCA = rca;

	    /*!< Send CMD9 SEND_CSD with argument as card's RCA */
	    SDIO_CmdInitStructure.SDIO_Argument = (UINT32)(rca << 16);
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_CSD;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp2Error();

	    if (SD_OK != errorstatus)
	    {
	      return DS_Fail;
	    }

	    CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
	    CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
	    CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
	    CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
	  }

	  errorstatus = SD_OK; /*!< All cards get intialized */

	  return (DS_Success);
}

SD_Error SDIO_Driver::IsCardProgramming(uint8_t *pstatus)
{
  SD_Error errorstatus = SD_OK;
  __IO UINT32 respR1 = 0, status = 0;

  SDIO_CmdInitStructure.SDIO_Argument = (UINT32) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  status = SDIO->STA;
  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  status = (UINT32)SDIO_GetCommandResponse();

  /*!< Check response received is of desired command */
  if (status != SD_CMD_SEND_STATUS)
  {
    errorstatus = SD_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);


  /*!< We have received response, retrieve it for analysis  */
  respR1 = SDIO_GetResponse(SDIO_RESP1);

  /*!< Find out card status */
  *pstatus = (uint8_t) ((respR1 >> 9) & 0x0000000F);

  if ((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  {
    return(errorstatus);
  }

  if (respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SD_ADDR_OUT_OF_RANGE);
  }

  if (respR1 & SD_OCR_ADDR_MISALIGNED)
  {
    return(SD_ADDR_MISALIGNED);
  }

  if (respR1 & SD_OCR_BLOCK_LEN_ERR)
  {
    return(SD_BLOCK_LEN_ERR);
  }

  if (respR1 & SD_OCR_ERASE_SEQ_ERR)
  {
    return(SD_ERASE_SEQ_ERR);
  }

  if (respR1 & SD_OCR_BAD_ERASE_PARAM)
  {
    return(SD_BAD_ERASE_PARAM);
  }

  if (respR1 & SD_OCR_WRITE_PROT_VIOLATION)
  {
    return(SD_WRITE_PROT_VIOLATION);
  }

  if (respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SD_LOCK_UNLOCK_FAILED);
  }

  if (respR1 & SD_OCR_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  if (respR1 & SD_OCR_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (respR1 & SD_OCR_CARD_ECC_FAILED)
  {
    return(SD_CARD_ECC_FAILED);
  }

  if (respR1 & SD_OCR_CC_ERROR)
  {
    return(SD_CC_ERROR);
  }

  if (respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (respR1 & SD_OCR_STREAM_READ_UNDERRUN)
  {
    return(SD_STREAM_READ_UNDERRUN);
  }

  if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SD_STREAM_WRITE_OVERRUN);
  }

  if (respR1 & SD_OCR_CID_CSD_OVERWRIETE)
  {
    return(SD_CID_CSD_OVERWRITE);
  }

  if (respR1 & SD_OCR_WP_ERASE_SKIP)
  {
    return(SD_WP_ERASE_SKIP);
  }

  if (respR1 & SD_OCR_CARD_ECC_DISABLED)
  {
    return(SD_CARD_ECC_DISABLED);
  }

  if (respR1 & SD_OCR_ERASE_RESET)
  {
    return(SD_ERASE_RESET);
  }

  if (respR1 & SD_OCR_AKE_SEQ_ERROR)
  {
    return(SD_AKE_SEQ_ERROR);
  }

  return(errorstatus);
}


DeviceStatus SDIO_Driver::SD_GetSDCardInfo()
{
  SD_Error errorstatus = SD_OK;
  uint8_t tmp = 0;

  SDCardInfo.CardType = (uint8_t)CardType;
  SDCardInfo.RCA = (uint16_t)RCA;

  /*!< Byte 0 */
  tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
  SDCardInfo.SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  SDCardInfo.SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  SDCardInfo.SD_csd.Reserved1 = tmp & 0x03;

  /*!< Byte 1 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
  SDCardInfo.SD_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
  SDCardInfo.SD_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
  SDCardInfo.SD_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
  SDCardInfo.SD_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
  SDCardInfo.SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  SDCardInfo.SD_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
  SDCardInfo.SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  SDCardInfo.SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  SDCardInfo.SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  SDCardInfo.SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  SDCardInfo.SD_csd.Reserved2 = 0; /*!< Reserved */

  if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
    SDCardInfo.SD_csd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    SDCardInfo.SD_csd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
    SDCardInfo.SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    SDCardInfo.SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    SDCardInfo.SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
    SDCardInfo.SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    SDCardInfo.SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    SDCardInfo.SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    SDCardInfo.SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

    SDCardInfo.CardCapacity = (SDCardInfo.SD_csd.DeviceSize + 1) ;
    SDCardInfo.CardCapacity *= (1 << (SDCardInfo.SD_csd.DeviceSizeMul + 2));
    SDCardInfo.CardBlockSize = 1 << (SDCardInfo.SD_csd.RdBlockLen);
    SDCardInfo.CardCapacity *= SDCardInfo.CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    SDCardInfo.SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

    SDCardInfo.SD_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

    SDCardInfo.SD_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);

    SDCardInfo.CardCapacity = (SDCardInfo.SD_csd.DeviceSize + 1) * 512 * 1024;
    SDCardInfo.CardBlockSize = 512;
  }


  SDCardInfo.SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  SDCardInfo.SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
  SDCardInfo.SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  SDCardInfo.SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
  SDCardInfo.SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  SDCardInfo.SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  SDCardInfo.SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  SDCardInfo.SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
  SDCardInfo.SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  SDCardInfo.SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  SDCardInfo.SD_csd.Reserved3 = 0;
  SDCardInfo.SD_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
  SDCardInfo.SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  SDCardInfo.SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  SDCardInfo.SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  SDCardInfo.SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  SDCardInfo.SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  SDCardInfo.SD_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
  SDCardInfo.SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  SDCardInfo.SD_csd.Reserved4 = 1;


  /*!< Byte 0 */
  tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  SDCardInfo.SD_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  SDCardInfo.SD_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  SDCardInfo.SD_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  SDCardInfo.SD_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  SDCardInfo.SD_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  SDCardInfo.SD_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  SDCardInfo.SD_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  SDCardInfo.SD_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  SDCardInfo.SD_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  SDCardInfo.SD_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  SDCardInfo.SD_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  SDCardInfo.SD_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  SDCardInfo.SD_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  SDCardInfo.SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  SDCardInfo.SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  SDCardInfo.SD_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  SDCardInfo.SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  SDCardInfo.SD_cid.Reserved2 = 1;

  if(errorstatus != SD_OK)
	  return DS_Fail;
  else
	  return DS_Success;

}

DeviceStatus SDIO_Driver::SD_SelectDeselect(UINT32 addr)
{
	 SD_Error errorstatus = SD_OK;

	  /*!< Send CMD7 SDIO_SEL_DESEL_CARD */
	  SDIO_CmdInitStructure.SDIO_Argument =  addr;
	  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEL_DESEL_CARD;
	  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	  SDIO_SendCommand(&SDIO_CmdInitStructure);

	  errorstatus = CmdResp1Error(SD_CMD_SEL_DESEL_CARD);

	  if(errorstatus == SD_OK)
		  return DS_Success;
	  else
		  return DS_Fail;
}

SD_Error SDIO_Driver::FindSCR(UINT16 rca, UINT32 *pscr)
{
  UINT32 index = 0;
  SD_Error errorstatus = SD_OK;
  UINT32 tempscr[2] = {0, 0};

  /*!< Set Block Size To 8 Bytes */
  /*!< Send CMD55 APP_CMD with argument as card's RCA */
  SDIO_CmdInitStructure.SDIO_Argument = (UINT32)8;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  /*!< Send CMD55 APP_CMD with argument as card's RCA */
  SDIO_CmdInitStructure.SDIO_Argument = (UINT32) RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 8;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_8b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);


  /*!< Send ACMD51 SD_APP_SEND_SCR with argument as 0 */
  SDIO_CmdInitStructure.SDIO_Argument = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_SEND_SCR;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(&SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SD_APP_SEND_SCR);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  while (!(SDIO->STA & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
    {
      *(tempscr + index) = SDIO_ReadData();
      index++;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SD_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SD_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SD_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SD_START_BIT_ERR;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  *(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

  *(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

  return(errorstatus);
}


SD_Error SDIO_Driver::SDEnWideBus(FunctionalState NewState)
{
  SD_Error errorstatus = SD_OK;

  UINT32 scr[2] = {0, 0};

  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
  {
    errorstatus = SD_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  /*!< Get SCR Register */
  errorstatus = FindSCR(RCA, scr);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  /*!< If wide bus operation to be enabled */
  if (NewState == ENABLE)
  {
    /*!< If requested card supports wide bus operation */
    if ((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
      SDIO_CmdInitStructure.SDIO_Argument = (UINT32) RCA << 16;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
      SDIO_CmdInitStructure.SDIO_Argument = 0x2;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }
      return(errorstatus);
    }
    else
    {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }   /*!< If wide bus operation to be disabled */
  else
  {
    /*!< If requested card supports 1 bit mode operation */
    if ((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
      SDIO_CmdInitStructure.SDIO_Argument = (UINT32) RCA << 16;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);


      errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
      SDIO_CmdInitStructure.SDIO_Argument = 0x00;
      SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_SD_SET_BUSWIDTH;
      SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(&SDIO_CmdInitStructure);

      errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);

      if (errorstatus != SD_OK)
      {
        return(errorstatus);
      }

      return(errorstatus);
    }
    else
    {
      errorstatus = SD_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }
}


DeviceStatus SDIO_Driver::SD_EnableWideBusOperation(UINT32 WideMode)
{
  SD_Error errorstatus = SD_OK;

  /*!< MMC Card doesn't support this feature */
  if (SDIO_MULTIMEDIA_CARD == CardType)
  {
    errorstatus = SD_UNSUPPORTED_FEATURE;
    return DS_Fail;
  }
  else if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    if (SDIO_BusWide_8b == WideMode)
    {
      errorstatus = SD_UNSUPPORTED_FEATURE;
      return DS_Fail;
    }
    else if (SDIO_BusWide_4b == WideMode)
    {
      errorstatus = SDEnWideBus(ENABLE);

      if (SD_OK == errorstatus)
      {
        /*!< Configure the SDIO peripheral */
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
    else
    {
      errorstatus = SDEnWideBus(DISABLE);

      if (SD_OK == errorstatus)
      {
        /*!< Configure the SDIO peripheral */
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
  }

  if(SD_OK == errorstatus)
	  return DS_Success;
  else
	  return DS_Fail;

}

DeviceStatus SDIO_Driver::EraseBlock(UINT32 startaddr, UINT32 endaddr)
{
	SD_Error errorstatus = SD_OK;
	UINT32 delay = 0;
	__IO UINT32 maxdelay = 0;
	UINT8 cardstate = 0;

	/*!< Check if the card coomnd class supports erase command */
	if (((CSD_Tab[1] >> 20) & SD_CCCC_ERASE) == 0)
	{
	    errorstatus = SD_REQUEST_NOT_APPLICABLE;
	    return DS_Fail;
	}

	maxdelay = 120000 / ((SDIO->CLKCR & 0xFF) + 2);

	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
	  {
	    errorstatus = SD_LOCK_UNLOCK_FAILED;
	    return DS_Fail;
	  }

	  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
	  {
	    startaddr /= 512;
	    endaddr /= 512;
	  }

	  /*!< According to sd-card spec 1.0 ERASE_GROUP_START (CMD32) and erase_group_end(CMD33) */
	  if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
	  {
	    /*!< Send CMD32 SD_ERASE_GRP_START with argument as addr  */
	    SDIO_CmdInitStructure.SDIO_Argument = startaddr;
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_START;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_START);
	    if (errorstatus != SD_OK)
	    {
	      return DS_Fail;
	    }

	    /*!< Send CMD33 SD_ERASE_GRP_END with argument as addr  */
	    SDIO_CmdInitStructure.SDIO_Argument = endaddr;
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_END;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_END);
	    if (errorstatus != SD_OK)
	    {
	    	return DS_Fail;
	    }
	  }

	  /*!< Send CMD38 ERASE */
	  SDIO_CmdInitStructure.SDIO_Argument = 0;
	  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ERASE;
	  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	  SDIO_SendCommand(&SDIO_CmdInitStructure);

	  errorstatus = CmdResp1Error(SD_CMD_ERASE);

	  if (errorstatus != SD_OK)
	  {
	    return DS_Fail;
	  }

	  for (delay = 0; delay < maxdelay; delay++)
	  {}

	  /*!< Wait till the card is in programming state */
	  errorstatus = IsCardProgramming(&cardstate);

	  while ((errorstatus == SD_OK) && ((SD_CARD_PROGRAMMING == cardstate) || (SD_CARD_RECEIVING == cardstate)))
	  {
	    errorstatus = IsCardProgramming(&cardstate);
	  }

	  return DS_Success;

}

DeviceStatus SDIO_Driver::WriteMultiBlocks(UINT8* writebuff, UINT32 WriteAddr,UINT16 BlockSize, UINT32 NumberOfBlocks)
{

}

DeviceStatus SDIO_Driver::ReadBlock(UINT8 *readBuff, UINT32 ReadAddr, UINT16 BlockSize )
{

	SD_Error errorstatus = SD_OK;
	UINT32 count = 0, *tempbuff = (UINT32 *)readBuff;
	UINT8 power = 0;

	if(readBuff == NULL)
		return DS_Fail;

	TransferError = SD_OK;
	TransferEnd = 0;
	TotalNumberOfBytes = 0;

	/*!< Clear all DPSM configuration */
	SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
	SDIO_DataInitStructure.SDIO_DataLength = 0;
	SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
	SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
	SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
	SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Disable;
	SDIO_DataConfig(&SDIO_DataInitStructure);
	SDIO_DMACmd(DISABLE);

	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver]  SD Card Unlock Failed \n");
		return DS_Fail;
	}


	if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
	{
	    BlockSize = 512;
	    ReadAddr /= 512;
	}


	if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
	{
	    power = convert_from_bytes_to_power_of_two(BlockSize);

	    /*!< Set Block Size for Card */
	    SDIO_CmdInitStructure.SDIO_Argument = (UINT32) BlockSize;
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

	    if (SD_OK != errorstatus)
	    {
	    	return DS_Fail;
	    }
	}
	else
	{
	    errorstatus = SD_INVALID_PARAMETER;
	    return DS_Fail;
	}

	SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
	SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
	SDIO_DataInitStructure.SDIO_DataBlockSize = (UINT32) power << 4;
	SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
	SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
	SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
	SDIO_DataConfig(&SDIO_DataInitStructure);

	TotalNumberOfBytes = BlockSize;
	StopCondition = 0;
	DestBuffer = (UINT32 *)readBuff;

	   /*!< Send CMD17 READ_SINGLE_BLOCK */
	SDIO_CmdInitStructure.SDIO_Argument = (UINT32)ReadAddr;
	SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_SINGLE_BLOCK;
	SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	SDIO_SendCommand(&SDIO_CmdInitStructure);

	errorstatus = CmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);

	if (errorstatus != SD_OK)
	{
		return DS_Fail;
	}

	SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
	SDIO_DMACmd(ENABLE);
	SD_DMA_RxConfig((UINT32 *)readBuff, BlockSize);
	while ((DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK))
	{}

	if (TransferError != SD_OK)
	{
	     return DS_Fail;
	}

	return DS_Success;
}

SD_Error SDIO_Driver::SD_WaitReadOperation(void)
{
  SD_Error errorstatus = SD_OK;

  while ((DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK))
  {}

  if (TransferError != SD_OK)
  {
    return(TransferError);
  }

  return(errorstatus);
}

void SDIO_Driver::SD_DMA_RxConfig(UINT32 *BufferDST, UINT32 BufferSize)
{
	  DMA_InitTypeDef DMA_InitStructure;

	  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

	  /*!< DMA2 Channel4 disable */
	  DMA_Cmd(DMA2_Channel4, DISABLE);

	  /*!< DMA2 Channel4 Config */
	  DMA_InitStructure.DMA_PeripheralBaseAddr = (UINT32)SDIO_FIFO_ADDRESS;
	  DMA_InitStructure.DMA_MemoryBaseAddr = (UINT32)BufferDST;
	  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
	  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

	  /*!< DMA2 Channel4 enable */
	  DMA_Cmd(DMA2_Channel4, ENABLE);
}

void SDIO_Driver::SD_DMA_TxConfig(UINT32 *BufferSRC, UINT32 BufferSize)
{

  DMA_InitTypeDef DMA_InitStructure;

  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

  /*!< DMA2 Channel4 disable */
  DMA_Cmd(DMA2_Channel4, DISABLE);

  /*!< DMA2 Channel4 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (UINT32)SDIO_FIFO_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (UINT32)BufferSRC;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  /*!< DMA2 Channel4 enable */
  DMA_Cmd(DMA2_Channel4, ENABLE);
}

UINT32 SDIO_Driver::DMAEndOfTransferStatus(void)
{
  return (UINT32)DMA_GetFlagStatus(DMA2_FLAG_TC4);
}

UINT8 SDIO_Driver::SD_Detect(void)
{
  __IO uint8_t status = SD_PRESENT;

  /*!< Check GPIO to detect SD */
  if (CPU_GPIO_GetPinState((GPIO_PIN) 37) == FALSE)
  {
    status = SD_NOT_PRESENT;
  }
  return status;
}


SD_Error SDIO_Driver::SD_SendStatus(UINT32 *pcardstatus)
{
  SD_Error errorstatus = SD_OK;

  SDIO->ARG = (UINT32) RCA << 16;
  SDIO->CMD = 0x44D;

  errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);

  if (errorstatus != SD_OK)
  {
    return(errorstatus);
  }

  *pcardstatus = SDIO->RESP1;
  return(errorstatus);
}



SDCardState SDIO_Driver::SD_GetState(void)
{
  UINT32 resp1 = 0;

  if(SD_Detect()== SD_PRESENT)
  {
    if (SD_SendStatus(&resp1) != SD_OK)
    {
      return SD_CARD_ERROR;
    }
    else
    {
      return (SDCardState)((resp1 >> 9) & 0x0F);
    }
  }
  else
  {
    return SD_CARD_ERROR;
  }
}


SDTransferState SDIO_Driver::GetTransferStatus(void)
{
  SDCardState cardstate =  SD_CARD_TRANSFER;

  cardstate = SD_GetState();

  if (cardstate == SD_CARD_TRANSFER)
  {
    return(SD_TRANSFER_OK);
  }
  else if(cardstate == SD_CARD_ERROR)
  {
    return (SD_TRANSFER_ERROR);
  }
  else
  {
    return(SD_TRANSFER_BUSY);
  }
}



SD_Error SDIO_Driver::WaitWriteOperation(void)
{
	SD_Error errorstatus = SD_OK;

	  while ((DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK))
	  {}

	  if (TransferError != SD_OK)
	  {
	    return(TransferError);
	  }

	  /*!< Clear all the static flags */
	  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

	  return(errorstatus);
}

DeviceStatus SDIO_Driver::WriteBlock(UINT8 *writebuff, UINT32 WriteAddr,UINT16 BlockSize)
{
	SD_Error errorstatus = SD_OK;
	  uint8_t  power = 0, cardstate = 0;
	  UINT32 timeout = 0, bytestransferred = 0;
	  UINT32 cardstatus = 0, count = 0, restwords = 0;
	  UINT32 *tempbuff = (UINT32 *)writebuff;

	  if (writebuff == NULL)
	  {
	    SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver]  Attempting to write a Null Buffer");
	    return DS_Fail;
	  }

	  TransferError = SD_OK;
	  TransferEnd = 0;
	  TotalNumberOfBytes = 0;

	  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
	  SDIO_DataInitStructure.SDIO_DataLength = 0;
	  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
	  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
	  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
	  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Disable;
	  SDIO_DataConfig(&SDIO_DataInitStructure);
	  SDIO_DMACmd(DISABLE);

	  if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
	  {
	    SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver]  Card Unlock Failed");
	    return DS_Fail;
	  }

	  if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
	  {
	    BlockSize = 512;
	    WriteAddr /= 512;
	  }

	  /*!< Set the block size, both on controller and card */
	  if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
	  {
	    power = convert_from_bytes_to_power_of_two(BlockSize);

	    SDIO_CmdInitStructure.SDIO_Argument = (UINT32) BlockSize;
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

	    if (errorstatus != SD_OK)
	    {
	      return DS_Fail;
	    }
	  }
	  else
	  {
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver]  Invalid Block Size");
	    errorstatus = SD_INVALID_PARAMETER;
	    return DS_Fail;
	  }

	  /*!< Wait till card is ready for data Added */
	  SDIO_CmdInitStructure.SDIO_Argument = (UINT32) (RCA << 16);
	  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
	  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	  SDIO_SendCommand(&SDIO_CmdInitStructure);

	  errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);

	  if (errorstatus != SD_OK)
	  {
	    return DS_Fail;
	  }

	  cardstatus = SDIO_GetResponse(SDIO_RESP1);

	  timeout = SD_DATATIMEOUT;

	  while (((cardstatus & 0x00000100) == 0) && (timeout > 0))
	  {
	    timeout--;
	    SDIO_CmdInitStructure.SDIO_Argument = (UINT32) (RCA << 16);
	    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_STATUS;
	    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	    SDIO_SendCommand(&SDIO_CmdInitStructure);

	    errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);

	    if (errorstatus != SD_OK)
	    {
	      return DS_Fail;
	    }
	    cardstatus = SDIO_GetResponse(SDIO_RESP1);
	  }

	  if (timeout == 0)
	  {
	    return DS_Timeout;
	  }

	  /*!< Send CMD24 WRITE_SINGLE_BLOCK */
	  SDIO_CmdInitStructure.SDIO_Argument = WriteAddr;
	  SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_SINGLE_BLOCK;
	  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
	  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
	  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
	  SDIO_SendCommand(&SDIO_CmdInitStructure);

	  errorstatus = CmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);

	  if (errorstatus != SD_OK)
	  {
	    return DS_Fail;
	  }

	  TotalNumberOfBytes = BlockSize;
	  StopCondition = 0;
	  SrcBuffer = (UINT32 *)writebuff;

	  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
	  SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
	  SDIO_DataInitStructure.SDIO_DataBlockSize = (UINT32) power << 4;
	  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
	  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
	  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
	  SDIO_DataConfig(&SDIO_DataInitStructure);


	  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_TXUNDERR | SDIO_IT_STBITERR, ENABLE);
	  SD_DMA_TxConfig((UINT32 *)writebuff, BlockSize);
	  SDIO_DMACmd(ENABLE);
	  while ((DMAEndOfTransferStatus() == RESET) && (TransferEnd == 0) && (TransferError == SD_OK))
	  {}

	  if (TransferError != SD_OK)
	  {
	     return DS_Fail;
	  }

	  /*!< Clear all the static flags */
	  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

	    /*!< Wait till the card is in programming state */
	  errorstatus = IsCardProgramming(&cardstate);

	  while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
	  {
	    errorstatus = IsCardProgramming(&cardstate);
	  }

	  if(errorstatus == SD_OK)
		  return DS_Success;
	  else
		  return DS_Fail;
}

DeviceStatus SDIO_Driver::Initialize()
{
	GLOBAL_LOCK(irq);

	GPIOClockEnable();

	GPIOInit();

	SDIOClockEnable();

	DMAClockEnable();

	if(PowerOn() != DS_Success)
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] SD Card Power On Failed\n");
		return DS_Fail;
	}

	HAL_Time_Sleep_MicroSeconds(200);

	if(InitializeCards() != DS_Success)
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Initialize Cards Failed\n");
		return DS_Fail;
	}

	HAL_Time_Sleep_MicroSeconds(200);

	 SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
	 SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
	 SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
	 SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
	 SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
	 SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
	 SDIO_Init(&SDIO_InitStructure);

	 HAL_Time_Sleep_MicroSeconds(200);

	 if(SD_GetSDCardInfo() != DS_Success)
	 {
		 SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Getting Card Info Failed\n");
		 return DS_Fail;
	 }

	 HAL_Time_Sleep_MicroSeconds(200);

	 if(SD_SelectDeselect((UINT32) (SDCardInfo.RCA << 16)) != DS_Success)
	 {
		 SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Select Deselect Failed\n");
		 return DS_Fail;
	 }

	 HAL_Time_Sleep_MicroSeconds(200);

     SD_EnableWideBusOperation(SDIO_BusWide_4b);

     TransferError = SD_OK;
     StopCondition = 0;
     TransferEnd = 0;

     HAL_Time_Sleep_MicroSeconds(200);

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

#if 0
     // Initialize the transmit and recieve buffer
     for(UINT32 i = 0; i < SD_DMA_TRANSMIT_BUFFER_SIZE; i++)
     {
    	 dmaTransmitBuffer[i] = 0;
    	 dmaRecieveBuffer[i] = 0;
     }
#endif

     memset(dmaTransmitBuffer, 0, SD_DMA_TRANSMIT_BUFFER_SIZE);
     memset(dmaRecieveBuffer, 0, SD_DMA_TRANSMIT_BUFFER_SIZE);

     dmaTransmitBlockSize = 0;
     dmaRecieveBlockSize = 0;

     CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, FALSE);

     return DS_Success;

}

void SDIO_Driver::SDIO_HANDLER( void* Param )
{
	if(StopCondition == 1)
	{
		SDIO->ARG = 0x0;
		SDIO->CMD = 0x44C;

		TransferError = g_SDIODriver.CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
    }
	else
	 {
	    TransferError = SD_OK;
	  }

	if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data Timeout\n");
		SDIO_ClearITPendingBit(SDIO_FLAG_DTIMEOUT);
		ManagedSDCallback(DS_Fail);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_TXFIFOHE) != RESET)
	{
		g_SDIODriver.SD_DMA_TxConfig((UINT32*)g_SDIODriver.dmaTransmitBuffer, g_SDIODriver.dmaTransmitBlockSize);
		SDIO_DMACmd(ENABLE);
		/*
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] CNT Register Interrupt Handler 1 %d\n", SDIO->FIFOCNT);
		SDIO->FIFO = 1;
		SDIO->FIFO = 2;
		SDIO->FIFO = 3;
		SDIO->FIFO = 4;
		SDIO->FIFO = 5;
		SDIO->FIFO = 6;
		SDIO->FIFO = 7;
		SDIO->FIFO = 8;
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] CNT Register Interrupt Handler 2 %d\n", SDIO->FIFOCNT);
		*/
		SDIO_ClearITPendingBit(SDIO_IT_TXFIFOHE);
		SDIO_ITConfig(SDIO_IT_TXFIFOHE, DISABLE);
	}
	if(SDIO_GetFlagStatus(SDIO_IT_TXUNDERR) != RESET)
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] FIFO Underrun error\n");
		SDIO_ClearITPendingBit(SDIO_IT_TXUNDERR);
		SDIO_ITConfig(SDIO_IT_TXUNDERR, DISABLE);
		ManagedSDCallback(DS_Fail);
	}

	if(SDIO_GetFlagStatus(SDIO_IT_DATAEND))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data Transfer Complete\n");
		SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
		SDIO_ITConfig(SDIO_IT_DATAEND, DISABLE);
		ManagedSDCallback(DS_Success);
		TransferEnd = 1;
	}

	if(SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL))
	{
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] DMA CNT Register %d\n", DMA2_Channel4->CNDTR);
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] CNT Register Interrupt Handler 2 %d\n", SDIO->FIFOCNT);
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] Data CNT Register Interrupt Handler 2 %d\n", SDIO->DCOUNT);
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data CRC failed\n");
		SDIO_ClearITPendingBit(SDIO_FLAG_DCRCFAIL);
		SDIO_ITConfig(SDIO_FLAG_DCRCFAIL, DISABLE);
		ManagedSDCallback(DS_Fail);
		TransferEnd = 1;

	}

	if(SDIO_GetFlagStatus(SDIO_FLAG_STBITERR))
	{
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] DMA CNT Register %d\n", DMA2_Channel4->CNDTR);
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] CNT Register Interrupt Handler 2 %d\n", SDIO->FIFOCNT);
		SDIO_DEBUG_PRINTF1("[NATIVE] [SDIO Driver] Data CNT Register Interrupt Handler 2 %d\n", SDIO->DCOUNT);
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] STBIT Error\n");
		SDIO_ClearITPendingBit(SDIO_FLAG_STBITERR);
		SDIO_ITConfig(SDIO_FLAG_STBITERR, DISABLE);
		ManagedSDCallback(DS_Fail);

	}

	if(SDIO_GetFlagStatus(SDIO_FLAG_DBCKEND))
	{
		SDIO_DEBUG_PRINTF("[NATIVE] [SDIO Driver] Data Transfer Complete crc passed\n");
		SDIO_ClearITPendingBit(SDIO_FLAG_DBCKEND);
		SDIO_ITConfig(SDIO_FLAG_DBCKEND, DISABLE);
		TransferEnd = 1;
	}


}

