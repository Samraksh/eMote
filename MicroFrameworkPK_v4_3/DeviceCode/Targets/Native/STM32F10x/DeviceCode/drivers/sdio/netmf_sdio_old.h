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
#include <intc/STM32.h>
#include <dma/stm32f10x_dma.h>
#include "stm32f10x_sdio.h"

#define SDIO_DEBUGGING_ENABLED 1

#if defined(SDIO_DEBUGGING_ENABLED)
#define SDIO_DEBUG_PRINTF(x) hal_printf(x)
#define SDIO_DEBUG_PRINTF1(x,y) hal_printf(x,y)
#define SDIO_DEBUG_ENABLEOUTPUTPIN(x, y) 	CPU_GPIO_EnableOutputPin((GPIO_PIN)	x, y)
#define SDIO_DEBUG_SETPINSTATE(x,y) CPU_GPIO_SetPinState((GPIO_PIN) x, y)
#else
#define SDIO_DEBUG_PRINTF(x)
#define SDIO_DEBUG_PRINTF1(x,y)
#define SDIO_DEBUG_ENABLEOUTPUTPIN(x, y)
#define SDIO_DEBUG_SETPINSTATE(x,y)
#endif

#define SDIO_FIFO_ADDRESS                ((UINT32)0x40018080)

#define SD_DETECT_PIN                    GPIO_Pin_7                 /* PC.7 */
#define SD_DETECT_GPIO_PORT              GPIOC                       /* GPIOC */
#define SD_DETECT_GPIO_CLK               RCC_APB2Periph_GPIOF

#define SDIO_STATIC_FLAGS               ((UINT32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((UINT32)0x00010000)

#define SDIO_INIT_CLK_DIV                ((UINT8)0xB2)
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

#define SD_CMD_ERASE                               ((UINT8)38)
#define SD_CMD_FAST_IO                             ((UINT8)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((UINT8)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((UINT8)42)
#define SD_CMD_APP_CMD                             ((UINT8)55)
#define SD_CMD_GEN_CMD                             ((UINT8)56)
#define SD_CMD_NO_CMD                              ((UINT8)64)

#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((UINT8)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((UINT8)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((UINT8)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((UINT8)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((UINT8)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((UINT8)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((UINT8)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((UINT8)53) /*!< For SD I/O Card only */

#define SD_CMD_WRITE_SINGLE_BLOCK                  ((UINT8)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((UINT8)25)

#define SD_CMD_READ_SINGLE_BLOCK                   ((UINT8)17)

#define SD_CMD_SD_ERASE_GRP_START                  ((UINT8)32)
#define SD_CMD_SD_ERASE_GRP_END                    ((UINT8)33)

#define SD_CMD_SEND_STATUS                         ((UINT8)13)

#define SD_CMD_STOP_TRANSMISSION                   ((UINT8)12)
/**
  * @brief Supported SD Memory Cards
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((UINT32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((UINT32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((UINT32)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((UINT32)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((UINT32)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((UINT32)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((UINT32)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((UINT32)0x00000007)

#define SD_CMD_ALL_SEND_CID                        ((UINT8)2)
#define SD_CMD_SET_REL_ADDR                        ((UINT8)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SEND_CSD                            ((UINT8)9)


#define SD_CMD_GO_IDLE_STATE                       ((UINT8)0)
#define SDIO_TRANSFER_CLK_DIV            		   ((UINT8)0x00)

#define SD_CMD_SEL_DESEL_CARD                      ((UINT8)7)

#define SD_CMD_SET_BLOCKLEN                        ((UINT8)16)

#define SD_PRESENT                                 ((uint8_t)0x01)
#define SD_NOT_PRESENT                             ((uint8_t)0x00)

#define SD_DMA_TRANSMIT_BUFFER_SIZE					512

typedef enum
{
  SD_TRANSFER_OK  = 0,
  SD_TRANSFER_BUSY = 1,
  SD_TRANSFER_ERROR
} SDTransferState;


typedef enum
{
  SD_CARD_READY                  = ((uint32_t)0x00000001),
  SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SD_CARD_STANDBY                = ((uint32_t)0x00000003),
  SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SD_CARD_SENDING                = ((uint32_t)0x00000005),
  SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;



typedef enum
{
/**
  * @brief  SDIO specific error defines
  */
  SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
  SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
  SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
  SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
  SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
  SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
  SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
  SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
  SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
  SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
  SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
  SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
  SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
  SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
  SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
  SD_CC_ERROR                        = (18), /*!< Internal card controller error */
  SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
  SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
  SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
  SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
  SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
  SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
  SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
  SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
  SD_INVALID_VOLTRANGE               = (27),
  SD_ADDR_OUT_OF_RANGE               = (28),
  SD_SWITCH_ERROR                    = (29),
  SD_SDIO_DISABLED                   = (30),
  SD_SDIO_FUNCTION_BUSY              = (31),
  SD_SDIO_FUNCTION_FAILED            = (32),
  SD_SDIO_UNKNOWN_FUNCTION           = (33),

/**
  * @brief  Standard error defines
  */
  SD_INTERNAL_ERROR,
  SD_NOT_CONFIGURED,
  SD_REQUEST_PENDING,
  SD_REQUEST_NOT_APPLICABLE,
  SD_INVALID_PARAMETER,
  SD_UNSUPPORTED_FEATURE,
  SD_UNSUPPORTED_HW,
  SD_ERROR,
  SD_OK = 0
} SD_Error;




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

	UINT32 CSD_Tab[4];

	UINT32 CID_Tab[4];

	UINT32 RCA;

	SD_CardInfo SDCardInfo;

	UINT32 TotalNumberOfBytes;

	UINT32 *DestBuffer;

	UINT32* SrcBuffer;


	static __IO SD_Error TransferError;
	static __IO UINT32 TransferEnd;
	static __IO UINT32 StopCondition;


	SDIO_InitTypeDef SDIO_InitStructure;
	SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
	SDIO_DataInitTypeDef SDIO_DataInitStructure;

	void GPIOClockEnable();

	void GPIOInit();

	void SDIOClockEnable();

	// Unsure why dma is being initialized
	void DMAClockEnable();

	DeviceStatus PowerOn();

	DeviceStatus PowerOff();

	SD_Error GetStatus();

	SD_Error CmdResp7Error();

	SD_Error CmdResp6Error(UINT8 cmd, UINT16 *prca);

	SD_Error CmdResp1Error(UINT8 cmd);

	SD_Error CmdResp3Error(void);

	SD_Error CmdResp2Error(void);



public:

	UINT32 dmaTransmitBuffer[SD_DMA_TRANSMIT_BUFFER_SIZE];

	UINT32 dmaRecieveBuffer[SD_DMA_TRANSMIT_BUFFER_SIZE];

	UINT32 dmaTransmitBlockSize;

	UINT32 dmaRecieveBlockSize;

	DeviceStatus Initialize();

	DeviceStatus InitializeCards();

	DeviceStatus SD_GetSDCardInfo();

	DeviceStatus SD_SelectDeselect(UINT32 addr);

	DeviceStatus SD_EnableWideBusOperation(UINT32 mode);

	SD_Error SDEnWideBus(FunctionalState NewState);

	SD_Error FindSCR(UINT16 rca, UINT32 *pscr);

	DeviceStatus WriteBlock(UINT8 *writeBuff, UINT32 WriteAddr, UINT16 BlockSize);

	DeviceStatus WriteMultiBlocks(UINT8 *writebuff, UINT32 WriteAddr, UINT16 BlockSize, UINT32 NumberOfBlocks);

	DeviceStatus ReadBlock(UINT8 *readBuff, UINT32 WriteAddr, UINT16 BlockSize);

	DeviceStatus EraseBlock(UINT32 startaddr, UINT32 endaddr);

	SD_Error IsCardProgramming(UINT8 *pstatus);

	static void SDIO_HANDLER( void* Param );

	void SD_DMA_TxConfig(UINT32 *BufferSRC, UINT32 BufferSize);

	void SD_DMA_RxConfig(UINT32 *BufferSRC, UINT32 BufferSize);

	SD_Error WaitWriteOperation();

	UINT32 DMAEndOfTransferStatus(void);

	SDTransferState GetTransferStatus();

	SDCardState SD_GetState();

	UINT8 SD_Detect(void);

	SD_Error SD_SendStatus(UINT32 *pcardstatus);

	SD_Error SD_WaitReadOperation(void);


};

volatile SD_Error SDIO_Driver::TransferError;
volatile UINT32 SDIO_Driver::TransferEnd = 0;
volatile UINT32 SDIO_Driver::StopCondition = 0;

#endif
