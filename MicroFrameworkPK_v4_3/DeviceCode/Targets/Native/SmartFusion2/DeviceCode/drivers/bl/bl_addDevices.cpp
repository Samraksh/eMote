////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

extern struct BlockStorageDevice  SF2_CM3_BlockStorageDevice_InternalFlash;
extern struct IBlockStorageDevice SF2_CM3_IBlockStorageDevice_InternalFlash;
extern struct BLOCK_CONFIG        SF2_CM3_blConfig_InternalFlash;

//extern struct BlockStorageDevice SF2_CM3_BlockStorageDevice_ExternalFlash_NOR;
//extern struct IBlockStorageDevice SF2_CM3_IBlockStorageDevice_ExternalFlash_NOR;
//extern struct BLOCK_CONFIG        SF2_CM3_blConfig_ExternalFlash_NOR;

void BlockStorage_AddDevices()
{
    BlockStorageList::AddDevice( &SF2_CM3_BlockStorageDevice_InternalFlash, &SF2_CM3_IBlockStorageDevice_InternalFlash, &SF2_CM3_blConfig_InternalFlash, FALSE );
    //BlockStorageList::AddDevice( &SF2_CM3_BlockStorageDevice_ExternalFlash_NOR, &SF2_CM3_IBlockStorageDevice_ExternalFlash_NOR, &SF2_CM3_blConfig_ExternalFlash_NOR, FALSE );
}

