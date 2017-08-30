////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//--//

#include <max3263x.h>

#define FLASH_START_ADDRESS	MXC_FLASH_MEM_BASE
#define FLASH_FULL_SIZE MXC_FLASH_FULL_MEM_SIZE
#define FLASH_END_ADDRESS   ((uint32_t)FLASH_START_ADDRESS+FLASH_FULL_SIZE)

#define CR_PG_Set                ((uint32_t)0x00000001)
#define EraseTimeout          ((uint32_t)0x000B0000)

struct Max3263x_blDriver
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
