////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

/**********************************************************************
**
** Function: CustomHeapLocation retrives the buffer to be used for Simple Heap allocations
** PK customers need to implement this function if they want to use it with Simple Heap.
** This function returns the buffer location and the size of the buffer.
** inline void HAL_Init_Custom_Heap() ( in tynihal.h )calls CustomHeapLocation to retrive 
** custom heap location and initialize custom heap.
**
** Arguments: [BaseAddress] - Reference to pointer to buffer for custom heap. Filled by function
**            [SizeInBytes] - Reference to varialbe that receives size of the buffer.
**********************************************************************/
void CustomHeapLocation( UINT8*& BaseAddress, UINT32& SizeInBytes )

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

