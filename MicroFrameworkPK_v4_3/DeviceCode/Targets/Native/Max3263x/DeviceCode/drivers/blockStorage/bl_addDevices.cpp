#include <tinyhal.h>

extern struct BlockStorageDevice  Max3263x_BlockStorageDevice_InternalFlash;
extern struct IBlockStorageDevice Max3263x_IBlockStorageDevice_InternalFlash;
extern struct BLOCK_CONFIG        Max3263x_blConfig_InternalFlash;

extern struct BlockStorageDevice Max3263x_BlockStorageDevice_ExternalFlash_NOR;
extern struct IBlockStorageDevice Max3263x_IBlockStorageDevice_ExternalFlash_NOR;

void BlockStorage_AddDevices()
{
    BlockStorageList::AddDevice( &Max3263x_BlockStorageDevice_InternalFlash, &Max3263x_IBlockStorageDevice_InternalFlash, &Max3263x_blConfig_InternalFlash, FALSE );
}

