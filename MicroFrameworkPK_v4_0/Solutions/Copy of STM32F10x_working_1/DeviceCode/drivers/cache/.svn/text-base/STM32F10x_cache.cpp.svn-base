////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

//--//

void CPU_FlushCaches()
{
}

void CPU_DrainWriteBuffers()
{
}

void CPU_InvalidateCaches()
{
}

void CPU_EnableCaches()
{
}

void CPU_DisableCaches()
{
}

//--//

template <typename T> void CPU_InvalidateAddress( T* address )
{
}

template <typename T> void CPU_InvalidateAddress( volatile T* address )
{
}

//--//

/**
 *	Since we are using all close-to-chip memory and the chip doesn't posses
 *	an actual cache we will just use the code location as the execution
 *	location
 **/
size_t CPU_GetCachableAddress( size_t address )
{
    return address;
}

//--//

size_t CPU_GetUncachableAddress( size_t address )
{
    return address;
}

