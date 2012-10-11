////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RAMTEST_H_
#define _RAMTEST_H_ 1

#include <tinyhal_types.h>
//--//

BOOL RamWordTest( volatile UINT32* WordToTest );

void RamTest( UINT32 start, UINT32 size, const char* name );

//--//

#endif  // _RAMTEST_H_

