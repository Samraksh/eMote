////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"

//--//

void HeapLocation( UINT8*& BaseAddress, UINT32& SizeInBytes )
{
    NATIVE_PROFILE_PAL_HEAP();
#if defined (PLATFORM_ARM_EmoteDotNow)
    //For stm32xl with 96K ram
    BaseAddress = (UINT8*) 0x2000C000;
    SizeInBytes = 0xBFFF;

    //For stm32 with 64K ram
    //BaseAddress = (UINT8*) 0x20009000;
    //SizeInBytes = 0x7000;

//#if defined (PLATFORM_ARM) || defined(PLATFORM_SH)
#else

     BaseAddress = (UINT8*)                            &HeapBegin;
     SizeInBytes = (UINT32)((size_t)&HeapEnd - (size_t)&HeapBegin + sizeof(HeapEnd));

//#else

  //  BaseAddress = 0;
   // SizeInBytes = 0;
    
#endif
 
}

