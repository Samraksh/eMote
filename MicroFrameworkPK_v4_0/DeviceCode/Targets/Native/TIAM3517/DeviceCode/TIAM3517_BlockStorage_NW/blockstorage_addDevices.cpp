/*
 * 	Name : blockstorage_addDevices.cpp
 *
 *  Author : Nived.Sivadas@samraksh.com
 *
 *  Description :
 *
 *
 */

#include <tinyhal.h>

extern struct BlockStorageDevice  TIAM3517_BlockStorageDevice_InternalFlash;
extern struct IBlockStorageDevice TIAM3517_IBlockStorageDevice_InternalFlash;
extern struct BLOCK_CONFIG        TIAM3517_blConfig_InternalFlash;

void BlockStorage_AddDevices()
{
    BlockStorageList::AddDevice( &TIAM3517_BlockStorageDevice_InternalFlash, &TIAM3517_IBlockStorageDevice_InternalFlash, &TIAM3517_blConfig_InternalFlash, TRUE );
}
