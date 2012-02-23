/*
 * 	Name : blockstorage_config.cpp
 *
 *  Author : Nived.Sivadas@samraksh.com
 *
 *  Description :
 *
 */

#include <tinyhal.h>

const BlockRange TIAM3517_BlockRange_InternalFlash[] =
{
		{ BlockRange::BLOCKTYPE_BOOTSTRAP , 0 , 11 },
	    { BlockRange::BLOCKTYPE_CODE                    , 600,3583 },
	    { BlockRange::BLOCKTYPE_DEPLOYMENT              ,3584,3784 },
	    { BlockRange::BLOCKTYPE_STORAGE_A       ,3784,3785 },
	    { BlockRange::BLOCKTYPE_STORAGE_B       ,3786,3786 },
	    { BlockRange::BLOCKTYPE_CONFIG          ,3787,3787 }
};

const BlockRegionInfo TIAM3517_BlockRegionInfo_InternalFlash[1] =
{
		0x80000000,
		131072,
		0x800,
		ARRAYSIZE_CONST_EXPR(TIAM3517_BlockRange_InternalFlash),
		TIAM3517_BlockRange_InternalFlash,

};

BlockDeviceInfo TIAM3517_BlockDeviceInfo_InternalFlash  =
{
		{
				FALSE,
				TRUE,
				TRUE,
		},
		9600,
		4800,
		0x4,
		0x10000000,
		1,
		TIAM3517_BlockRegionInfo_InternalFlash,
};


BLOCK_CONFIG TIAM3517_blConfig_InternalFlash =
{
		GPIO_PIN_NONE,
		FALSE,
		&TIAM3517_BlockDeviceInfo_InternalFlash

};

struct BlockStorageDevice TIAM3517_BlockStorageDevice_InternalFlash;
