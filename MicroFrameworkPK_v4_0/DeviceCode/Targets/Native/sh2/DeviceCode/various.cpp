////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include "tinyhal.h"
#include "tinyhal.h"
#include <string.h>
   
////////////////////////////////////////////////////////////////////////////////////////////////////

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif


#if !defined(BUILD_RTM)

void debug_printf(char const* format, ... )
{
}

void lcd_printf(char const * format,...)
{
}

#endif


int hal_strcpy_s(char* strDst, size_t sizeInBytes, const char* strSrc )
{
    strncpy(strDst, strSrc, sizeInBytes);
    return 0;
}

int hal_strncpy_s( char* strDst, size_t sizeInBytes, const char* strSrc, size_t count )
{
    strncpy(strDst, strSrc, __min(sizeInBytes, count));
    return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
void abort(void)
{

}
#endif
void main(void)
{
    HAL_Initialize();
    ApplicationEntryPoint();
}


///////////////////////////////////////////////////////////////////////////////////////////


