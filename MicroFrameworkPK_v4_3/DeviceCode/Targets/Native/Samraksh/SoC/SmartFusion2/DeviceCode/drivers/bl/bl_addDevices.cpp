////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

extern struct BlockStorageDevice  STM32F10x_BlockStorageDevice_InternalFlash;
extern struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_InternalFlash;
extern struct BLOCK_CONFIG        STM32F10x_blConfig_InternalFlash;

//extern struct BlockStorageDevice STM32F10x_BlockStorageDevice_ExternalFlash_NOR;
//extern struct IBlockStorageDevice STM32F10x_IBlockStorageDevice_ExternalFlash_NOR;
//extern struct BLOCK_CONFIG        STM32F10x_blConfig_ExternalFlash_NOR;

void BlockStorage_AddDevices()
{
    BlockStorageList::AddDevice( &STM32F10x_BlockStorageDevice_InternalFlash, &STM32F10x_IBlockStorageDevice_InternalFlash, &STM32F10x_blConfig_InternalFlash, FALSE );
    //BlockStorageList::AddDevice( &STM32F10x_BlockStorageDevice_ExternalFlash_NOR, &STM32F10x_IBlockStorageDevice_ExternalFlash_NOR, &STM32F10x_blConfig_ExternalFlash_NOR, FALSE );
}

