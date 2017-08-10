////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "netmf_bl.h"

extern struct BlockStorageDevice  STM32F10x_BlockStorageDevice_InternalFlash;
//extern struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_InternalFlash;
extern struct BLOCK_CONFIG        STM32F10x_blConfig_InternalFlash;

struct STM32F10x_blDriver g_temp_bl_driver;

struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_InternalFlash;
/*
=
{
    &STM32F10x_blDriver::InitializeDevice,
    &STM32F10x_blDriver::UninitializeDevice,
    &STM32F10x_blDriver::GetDeviceInfo,
    &STM32F10x_blDriver::Read,
    &STM32F10x_blDriver::Write,
    &STM32F10x_blDriver::Memset,
    &STM32F10x_blDriver::GetSectorMetadata,
    &STM32F10x_blDriver::SetSectorMetadata,
    &STM32F10x_blDriver::IsBlockErased,
    &STM32F10x_blDriver::EraseBlock,
    &STM32F10x_blDriver::SetPowerState,
    &STM32F10x_blDriver::MaxSectorWrite_uSec,
    &STM32F10x_blDriver::MaxBlockErase_uSec,
};
*/

void BlockStorage_AddDevices()
{
	UINT32 x = 20;
	STM32F10x_IBlockStorageDevice_InternalFlash.InitializeDevice = &STM32F10x_blDriver::InitializeDevice;
	STM32F10x_IBlockStorageDevice_InternalFlash.UninitializeDevice = &STM32F10x_blDriver::UninitializeDevice;
	STM32F10x_IBlockStorageDevice_InternalFlash.GetDeviceInfo = &STM32F10x_blDriver::GetDeviceInfo;
	STM32F10x_IBlockStorageDevice_InternalFlash.Read = &STM32F10x_blDriver::Read;
	STM32F10x_IBlockStorageDevice_InternalFlash.Write = &STM32F10x_blDriver::Write;
	STM32F10x_IBlockStorageDevice_InternalFlash.Memset = &STM32F10x_blDriver::Memset;
	STM32F10x_IBlockStorageDevice_InternalFlash.GetSectorMetadata = &STM32F10x_blDriver::GetSectorMetadata;
	STM32F10x_IBlockStorageDevice_InternalFlash.SetSectorMetadata = &STM32F10x_blDriver::SetSectorMetadata;
	STM32F10x_IBlockStorageDevice_InternalFlash.IsBlockErased = &STM32F10x_blDriver::IsBlockErased;
	STM32F10x_IBlockStorageDevice_InternalFlash.EraseBlock = &STM32F10x_blDriver::EraseBlock;
	STM32F10x_IBlockStorageDevice_InternalFlash.SetPowerState = &STM32F10x_blDriver::SetPowerState;
	STM32F10x_IBlockStorageDevice_InternalFlash.MaxSectorWrite_uSec = &STM32F10x_blDriver::MaxSectorWrite_uSec;

	/*
	    &STM32F10x_blDriver::GetDeviceInfo,
	    &STM32F10x_blDriver::Read,
	    &STM32F10x_blDriver::Write,
	    &STM32F10x_blDriver::Memset,
	    &STM32F10x_blDriver::GetSectorMetadata,
	    &STM32F10x_blDriver::SetSectorMetadata,
	    &STM32F10x_blDriver::IsBlockErased,
	    &STM32F10x_blDriver::EraseBlock,
	    &STM32F10x_blDriver::SetPowerState,
	    &STM32F10x_blDriver::MaxSectorWrite_uSec,
	    &STM32F10x_blDriver::MaxBlockErase_uSec,
	    */
    BlockStorageList::AddDevice( &STM32F10x_BlockStorageDevice_InternalFlash, &STM32F10x_IBlockStorageDevice_InternalFlash, &STM32F10x_blConfig_InternalFlash, FALSE );
}

