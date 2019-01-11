////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//--//
#ifndef NETMF_BL_H_
#define NETMF_BL_H_

#include <cmsis/m2sxxx.h>
//#include <stm32f10x.h>

#define FLASH_BANK1_END_ADDRESS   ((uint32_t)0x0007FFFF)
#define CR_PG_Set                ((uint32_t)0x00000001)
#define EraseTimeout          ((uint32_t)0x000B0000)

struct SF2_CM3_blDriver
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
};

void BlockStorage_AddDevices();

#endif // NETMF_BL_H_
