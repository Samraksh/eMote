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

#include <stm32f10x.h>
#include "../sdio/stm32_eval_sdio_sd.h"
#include <Samraksh/Hal_util.h>
#include <gpio\netmf_gpio.h>


typedef void (*SDIOStatusFuncPtrType) (DeviceStatus status);

//#define SDIO_DEBUGGING_ENABLED 1

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

class STM32F10x_blDriver_SDIO
{
public:
	SDIOStatusFuncPtrType sdCallbackFunction;
	DeviceStatus Initialize(SDIOStatusFuncPtrType callback);
	static BOOL UninitializeDevice( void* context );

	static BOOL InitializeDevice( void* context );
	static const BlockDeviceInfo* GetDeviceInfo( void* context);
	static BOOL Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff );
	static BOOL Write( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite );
	static BOOL Memset( void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes );
	static BOOL GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
	static BOOL SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
	static BOOL IsBlockErased( void* context, ByteAddress Sector, UINT32 BlockLength );
	//static BOOL EraseBlock( void* context, ByteAddress Sector1, ByteAddress Sector2 );
	static BOOL EraseBlock( void* context, ByteAddress Sector1 );
	static void SetPowerState( void* context, UINT32 State );
	static UINT32 MaxSectorWrite_uSec( void* context );
	static UINT32 MaxBlockErase_uSec( void* context );

	//DeviceStatus EraseBlock(UINT32 startaddr, UINT32 endaddr);
	DeviceStatus WriteBlock(UINT8 *writeBuff, UINT32 WriteAddr, UINT16 BlockSize);
	DeviceStatus ReadBlock(UINT8 *readBuff, UINT32 WriteAddr, UINT16 BlockSize);
	static void SDIO_HANDLER( void* Param );
	void GPIOClockEnable();
	void GPIOInit();
	void SDIOClockEnable();
	void DMAClockEnable();
};


/*struct STM32F10x_blDriver_SDIO
{
    static BOOL InitializeDevice( void* context );
    static BOOL UninitializeDevice( void* context );
    static const BlockDeviceInfo* GetDeviceInfo( void* context);
    static BOOL Read( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff );
    static BOOL Write( void* context, ByteAddress Address, UINT32 NumBytes, BYTE * pSectorBuff, BOOL ReadModifyWrite );
    static BOOL Memset( void* context, ByteAddress Address, UINT8 Data, UINT32 NumBytes );
    static BOOL GetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
    static BOOL SetSectorMetadata(void* context, ByteAddress SectorStart, SectorMetadata* pSectorMetadata);
    static BOOL IsBlockErased( void* context, ByteAddress Sector, UINT32 BlockLength );
    static BOOL EraseBlock( void* context, ByteAddress Sector );
    static void SetPowerState( void* context, UINT32 State );
    static UINT32 MaxSectorWrite_uSec( void* context );
    static UINT32 MaxBlockErase_uSec( void* context );
};*/

#endif
