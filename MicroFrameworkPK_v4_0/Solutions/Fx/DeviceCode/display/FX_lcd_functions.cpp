////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"

//--//


BOOL LCD_Initialize()
{
    return TRUE;
}

BOOL LCD_Uninitialize()
{
    return TRUE;
}

void LCD_Clear()
{
}

void LCD_BitBltEx( int x, int y, int width, int height, UINT32 data[] )
{
}

void LCD_BitBlt( int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta )
{
}

void LCD_WriteChar( unsigned char c, int row, int col )
{
}

void LCD_WriteFormattedChar( unsigned char c )
{
}

INT32 LCD_GetWidth()
{
    return 0;
}

INT32 LCD_GetHeight()
{
    return 0;
}

INT32 LCD_GetBitsPerPixel()
{
    return 0;
}

UINT32 LCD_GetPixelClockDivider()
{
    return 0;
}
INT32 LCD_GetOrientation()
{
    return 0;
}
void LCD_PowerSave( BOOL On )
{
}

UINT32* LCD_GetFrameBuffer()
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return NULL;
}

UINT32 LCD_ConvertColor(UINT32 color)
{
    NATIVE_PROFILE_HAL_DRIVERS_DISPLAY();
    return color;
}
//--//


