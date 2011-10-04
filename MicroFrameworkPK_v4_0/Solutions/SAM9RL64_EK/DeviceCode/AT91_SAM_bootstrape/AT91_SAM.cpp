////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

///////////////////////////////////////////////////////////////////////////////

#pragma arm section code = "SectionForFlashOperations"

AT91_SAM_Driver g_AT91SAM_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

void __section(SectionForFlashOperations) AT91_SAM_Driver::Sleep(void)
{
#if defined(PLATFORM_ARM_SAM9261_ANY)
    UINT32 reg;
    // ARM926EJ-S Wait For Interrupt

#ifdef __GNUC__
	asm("MOV %0, #1" : "=r" (reg) : "r" (reg));
	asm("MCR p14, 0, %0, c7, c0, 0" :: "r" (reg));
#else
    __asm
    {
        mov     reg,#1                       
        mcr     p14, 0, reg, c7, c0, 0       
    }
#endif
#endif
}

BOOL AT91_SAM_Driver::Initialize()
{
    CPU_INTC_Initialize();
    CPU_GPIO_Initialize();
    return TRUE;
}

void AT91_SAM_Driver::Halt(void)      {}
void AT91_SAM_Driver::Reset(void)     {}
void AT91_SAM_Driver::Pause(void)     {}
BOOL AT91_SAM_Driver::PowerOnReset(void)   {return TRUE;}
void AT91_SAM_Driver::Shutdown(void)  {}
void AT91_SAM_Driver::Hibernate(void) {}
void AT91_SAM_Driver::DisplayProcessorInformation() {}

