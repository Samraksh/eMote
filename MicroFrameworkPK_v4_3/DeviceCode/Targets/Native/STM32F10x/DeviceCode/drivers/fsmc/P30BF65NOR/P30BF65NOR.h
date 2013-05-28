/*
 *
 *		Name	: P30BF65NOR.h
 *
 *		Author  : nived.sivadas@samraksh.com
 *
 *      Description  : contains the api for the numonyx axcell p30-65nm flash memory
 *
 */

#include <stm32f10x.h>

#include <Samraksh\HAL_util.h>

#define NOR_DEBUGGING_ENABLED 1

#if defined(NOR_DEBUGGING_ENABLED)
#define NOR_DEBUG_PRINT(x) hal_printf(x);
#define ERASE_PROFILE_PIN 24
#define WRITE_PROFILE_PIN 25
#define READ_PROFILE_PIN  ERASE_PROFILE_PIN
#else
#define NOR_DEBUG_PRINT(x)
#define ERASE_PROFILE_PIN
#define WRITE_PROFILE_PIN
#define READ_PROFILE_PIN
#endif


#define FLASH_RESET 107

#define Bank1_NOR2_ADDR       ((uint32_t)0x64000000)

#define BOOT_BLOCK_OFFSET	0x10000

#define BLOCK_SIZE 64 * 1024

#define FLASH_LIMIT 0x7FFFFF

//#define ADDR_SHIFT(A) (Bank1_NOR2_ADDR + (2 * (A)))
#define ADDR_SHIFT(A,B) (Bank1_NOR2_ADDR + BOOT_BLOCK_OFFSET + A + ( 2 * (B)))
#define NOR_WRITE(Address, Data)  (*(__IO UINT16 *)(Address) = (Data))

#define MANUFACTURE_ID 0x89

#define FLASH_START_ADDRESS (0x10000 - BOOT_BLOCK_OFFSET)
#define FLASH_END_ADDRESS	(0x7F0000 - BOOT_BLOCK_OFFSET)

#define BASE_BLOCK_ADDRESS_MASK (0x7f << 16)

// SR bit map
#define DEVICE_READY_STATUS_BIT (1 << 7)
#define ERASE_SUSPEND_STATUS_BIT (1 << 6)
#define ERASE_STATUS_BIT (1 << 5)
#define PROGRAM_STATUS_BIT (1 << 4)
#define VPP_STATUS_BIT (1 << 3)
#define PROGRAM_SUSPEND_STATUS_BIT (1 << 2)
#define BLOCK_LOCKED_STATUS_BIT (1 << 1)
#define BEFP_WRITE_STATUS_BIT (1 << 0)

// SR Device Ready status
#define DEVICE_BUSY 0
#define DEVICE_READY 1


#define ERASE_SUSPEND_NOT_IN_EFFECT 0
#define ERASE_SUSPEND_IN_EFFECT 1

#define PROGRAM_ERASE_SUCCESS 0
#define PROGRAM_ERROR (1 << 4)
#define ERASE_ERROR   (1 << 3)
#define COMMAND_SEQUENCE_ERROR ((1 << 4) | (1 << 3))

#define VPP_ACCEPTABLE 0
#define VPP_LOW 1

#define PROGRAM_SUSPEND_IN_EFFECT 1
#define PROGRAM_SUSPEND_NOT_IN_EFFECT 0

#define BLOCK_NOT_LOCKED 0
#define BLOCK_LOCKED 1

#define BEFP_COMPLETE 0
#define BEFP_IN_PROGRESS 1

#define DEFAULT_STATE 0x80

typedef struct
{
	UINT16 Manufacturer_Code;
	UINT16 Device_Code1;
	UINT16 Device_Code2;
	UINT16 Device_Code3;
}NOR_IDTypeDef;

struct P30BF65NOR_Driver
{

	NOR_IDTypeDef norId;

	UINT8 NOR_SR;

public:

	UINT16 GetManufactureId();

	UINT16 GetDeviceId();

	DeviceStatus Initialize(void);

	DeviceStatus ReadID();

	DeviceStatus EraseBlock(UINT32 BlockAddress);

	DeviceStatus EraseChip();

	DeviceStatus WriteHalfWord(UINT32 WriteAddr, UINT16 data);

	DeviceStatus WriteBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite);

	DeviceStatus ProgramBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite);

	UINT16 ReadHalfWord(UINT32 ReadAddr);

	DeviceStatus ReadBuffer(UINT16* pBuffer, UINT32 ReadAddr, UINT32 NumHalfWordToRead);

	DeviceStatus ReturnToReadMode(void);

	DeviceStatus Reset(void);

	DeviceStatus GetStatus();

	DeviceStatus BlockUnlock(UINT32 address);

	UINT16 GetStatusRegister();

	DeviceStatus GetStatus(UINT32 Timeout);

	BOOL ClearStatusRegister();

	BOOL IsBlockLocked(UINT32 address);

	BOOL DisplayStats(BOOL result, char* resultParameter1, char* resultParameter2, char* accuracy);

};

