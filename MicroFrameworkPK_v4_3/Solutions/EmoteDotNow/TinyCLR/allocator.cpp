////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <tinyhal.h>
#include <TinyCLR_Runtime.h>
#include "../platform_selector.h"

////////////////////////////////////////////////////////////////////////////////

#if !defined(SAM_CPPALLOC)
#define SAM_CPPALLOC 0
#endif

#if defined(SAM_CPPALLOC_DEBUG)
#define SAM_CPPALLOC_WRAP(fun) ( void* _res = (fun); _res ? _res : RuntimeFault("__FILE__ out of memory."))
#else
#define SAM_CPPALLOC_WRAP(fun) (fun)
#endif

static void RuntimeFault( const char* szText )
{
    lcd_printf("\014ERROR:\r\n%s\r\n", szText );
    debug_printf( "ERROR: %s\r\n", szText );

    HARD_BREAKPOINT();
}


void *operator new(size_t len)
{
#if SAM_CPPALLOC
	return SAM_CPPALLOC_WRAP( CLR_RT_Memory::Allocate((unsigned int)len, CLR_RT_HeapBlock::HB_InitializeToZero) );
#else
#warning "INFO: void *operator new(size_t) not implemented."
    RuntimeFault( "new(size_t)" );

    return NULL;
#endif
}

void *operator new[](size_t len)
{
#if SAM_CPPALLOC
	return SAM_CPPALLOC_WRAP( CLR_RT_Memory::Allocate((unsigned int)len, CLR_RT_HeapBlock::HB_InitializeToZero) );
#else
#warning "INFO: void *operator new[](size_t) not implemented."
    RuntimeFault( "new[](size_t)" );

    return NULL;
#endif
}

void operator delete (void* ptr)
{
#if SAM_CPPALLOC
	CLR_RT_Memory::Release(ptr);
#else
#warning "INFO: void *operator delete (void*) not implemented."
    RuntimeFault( "delete(void*)" );
#endif
}

void operator delete[] (void* ptr)
{
#if SAM_CPPALLOC
	CLR_RT_Memory::Release(ptr);
#else
#warning "INFO: void *operator delete[] (void*) not implemented."
    RuntimeFault( "delete[](void*)" );
#endif
}

////////////////////////////////////////////////////////////////////////////////
