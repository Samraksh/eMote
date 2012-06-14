////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"

//--//

void HeapLocation( UINT8*& BaseAddress, UINT32& SizeInBytes )
{
    NATIVE_PROFILE_PAL_HEAP();
#if defined (PLATFORM_ARM)
     //BaseAddress = (UINT8*)                            &HeapBegin;
     //SizeInBytes = (UINT32)((size_t)&HeapEnd - (size_t)&HeapBegin + sizeof(HeapEnd));
#if defined (PLATFORM_CORTEXM3)
     BaseAddress = (UINT8*) 0x2000A000;
     SizeInBytes = 0xE000;
#else
     BaseAddress = (UINT8*) 0x80800000;
	 SizeInBytes = 0x10000;
#endif

#elif defined(PLATFORM_BLACKFIN)

    BaseAddress = 0;
    SizeInBytes = 0;

#elif defined(PLATFORM_SH)

    BaseAddress = 0;
    SizeInBytes = 0;


#endif

}

