/*
 * 	Name 		:		netmf_sdio.h
 *
 *  Author      :       nived.sivadas@samraksh.com
 *
 *  Description :
 *
 *
 *
 */

#ifndef _NETMF_SDIO_H_
#define _NETMF_SDIO_H_

#include <tinyhal.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>
#include <Samraksh/Hal_util.h>
#include "stm32f10x_sdio.h"


#define SD_DETECT_PIN                    GPIO_Pin_11                 /* PF.11 */
#define SD_DETECT_GPIO_PORT              GPIOF                       /* GPIOF */
#define SD_DETECT_GPIO_CLK               RCC_APB2Periph_GPIOF

#define SDIO_STATIC_FLAGS               ((UINT32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((UINT32)0x00010000)

/**
  * @brief  Mask for errors Card Status R1 (OCR Register)
  */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((UINT32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((UINT32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((UINT32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((UINT32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((UINT32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((UINT32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((UINT32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((UINT32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((UINT32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((UINT32)0x00200000)
#define SD_OCR_CC_ERROR                 ((UINT32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((UINT32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((UINT32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((UINT32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((UINT32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((UINT32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((UINT32)0x00004000)
#define SD_OCR_ERASE_RESET              ((UINT32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((UINT32)0x00000008)
#define SD_OCR_ERRORBITS                ((UINT32)0xFDFFE008)

/**
  * @brief  Masks for R6 Response
  */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((UINT32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((UINT32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((UINT32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((UINT32)0x80100000)
#define SD_HIGH_CAPACITY                ((UINT32)0x40000000)
#define SD_STD_CAPACITY                 ((UINT32)0x00000000)
#define SD_CHECK_PATTERN                ((UINT32)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((UINT32)0x0000FFFF)
#define SD_ALLZERO                      ((UINT32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((UINT32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((UINT32)0x00010000)
#define SD_CARD_LOCKED                  ((UINT32)0x02000000)

#define SD_DATATIMEOUT                  ((UINT32)0xFFFFFFFF)
#define SD_0TO7BITS                     ((UINT32)0x000000FF)
#define SD_8TO15BITS                    ((UINT32)0x0000FF00)
#define SD_16TO23BITS                   ((UINT32)0x00FF0000)
#define SD_24TO31BITS                   ((UINT32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((UINT32)0x01FFFFFF)

#define SD_HALFFIFO                     ((UINT32)0x00000008)
#define SD_HALFFIFOBYTES                ((UINT32)0x00000020)

/**
  * @brief  Command Class Supported
  */
#define SD_CCCC_LOCK_UNLOCK             ((UINT32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((UINT32)0x00000040)
#define SD_CCCC_ERASE                   ((UINT32)0x00000020)

/**
  * @brief  Following commands are SD Card Specific commands.
  *         SDIO_APP_CMD should be sent before sending these commands.
  */
#define SDIO_SEND_IF_COND               ((UINT32)0x00000008)

#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((UINT32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((UINT32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((UINT32)0x00000002)

typedef struct
{
  __IO UINT8  CSDStruct;            /*!< CSD structure */
  __IO UINT8  SysSpecVersion;       /*!< System specification version */
  __IO UINT8  Reserved1;            /*!< Reserved */
  __IO UINT8  TAAC;                 /*!< Data read access-time 1 */
  __IO UINT8  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO UINT8  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO UINT16 CardComdClasses;      /*!< Card command classes */
  __IO UINT8  RdBlockLen;           /*!< Max. read data block length */
  __IO UINT8  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO UINT8  WrBlockMisalign;      /*!< Write block misalignment */
  __IO UINT8  RdBlockMisalign;      /*!< Read block misalignment */
  __IO UINT8  DSRImpl;              /*!< DSR implemented */
  __IO UINT8  Reserved2;            /*!< Reserved */
  __IO UINT32 DeviceSize;           /*!< Device Size */
  __IO UINT8  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO UINT8  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO UINT8  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO UINT8  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO UINT8  DeviceSizeMul;        /*!< Device size multiplier */
  __IO UINT8  EraseGrSize;          /*!< Erase group size */
  __IO UINT8  EraseGrMul;           /*!< Erase group size multiplier */
  __IO UINT8  WrProtectGrSize;      /*!< Write protect group size */
  __IO UINT8  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO UINT8  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO UINT8  WrSpeedFact;          /*!< Write speed factor */
  __IO UINT8  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO UINT8  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO UINT8  Reserved3;            /*!< Reserded */
  __IO UINT8  ContentProtectAppli;  /*!< Content protection application */
  __IO UINT8  FileFormatGrouop;     /*!< File format group */
  __IO UINT8  CopyFlag;             /*!< Copy flag (OTP) */
  __IO UINT8  PermWrProtect;        /*!< Permanent write protection */
  __IO UINT8  TempWrProtect;        /*!< Temporary write protection */
  __IO UINT8  FileFormat;           /*!< File Format */
  __IO UINT8  ECC;                  /*!< ECC code */
  __IO UINT8  CSD_CRC;              /*!< CSD CRC */
  __IO UINT8  Reserved4;            /*!< always 1*/
} SD_CSD;

typedef struct
{
  __IO UINT8  ManufacturerID;       /*!< ManufacturerID */
  __IO UINT16 OEM_AppliID;          /*!< OEM/Application ID */
  __IO UINT32 ProdName1;            /*!< Product Name part1 */
  __IO UINT8  ProdName2;            /*!< Product Name part2*/
  __IO UINT8  ProdRev;              /*!< Product Revision */
  __IO UINT32 ProdSN;               /*!< Product Serial Number */
  __IO UINT8  Reserved1;            /*!< Reserved1 */
  __IO UINT16 ManufactDate;         /*!< Manufacturing Date */
  __IO UINT8  CID_CRC;              /*!< CID CRC */
  __IO UINT8  Reserved2;            /*!< always 1 */
} SD_CID;

typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  UINT32 CardCapacity;  /*!< Card Capacity */
  UINT32 CardBlockSize; /*!< Card Block Size */
  UINT16 RCA;
  UINT8 CardType;
} SD_CardInfo;



class SDIO_Driver
{

	UINT32 CardType;



	SDIO_InitTypeDef SDIO_InitStructure;
	SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
	SDIO_DataInitTypeDef SDIO_DataInitStructure;

	void GPIOClockEnable();

	void GPIOInit();

	void SDIOClockEnable();

	// Unsure why dma is being initialized
	void DMAClockEnable();

	DeviceStatus PowerOn();

	DeviceStatus GetStatus();

	DeviceStatus CmdResp7Error();

public:

	DeviceStatus Initialize();



};


#endif
