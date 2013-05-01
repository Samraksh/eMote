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

#define FLASH_RESET 107

#define Bank1_NOR2_ADDR       ((uint32_t)0x64000000)

#define ADDR_SHIFT(A) (Bank1_NOR2_ADDR + (2 * (A)))
#define NOR_WRITE(Address, Data)  (*(__IO UINT16 *)(Address) = (Data))

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



public:

	void Initialize(void);

	void ReadID();

	DeviceStatus EraseBlock(UINT32 BlockAddress);

	DeviceStatus EraseChip();

	DeviceStatus WriteHalfWord(UINT32 WriteAddr, UINT16 data);

	DeviceStatus WriteBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite);

	DeviceStatus ProgramBuffer(UINT16* pBuffer, UINT32 WriteAddr, UINT32 NumHalfWordToWrite);

	DeviceStatus ReadHalfWord(UINT32 ReadAddr);

	DeviceStatus ReadBuffer(UINT16* pBuffer, UINT32 ReadAddr, UINT32 NumHalfWordToRead);

	DeviceStatus ReturnToReadMode(void);

	DeviceStatus Reset(void);

	DeviceStatus GetStatus(UINT32 Timeout);

};

