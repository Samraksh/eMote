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

#include <cores\arm\include\cpu.h>

//--//


void HAL_AssertEx()
{
    // cause an abort and let the abort handler take over
    *((char*)0xFFFFFFFF) = 'a';
}


BOOL CPU_Initialize()
{
    return AT91_SAM_Driver::Initialize();
}

void CPU_Pause()
{
    AT91_SAM_Driver::Pause();
}

void CPU_DisplayProcessorInformation()
{
    AT91_SAM_Driver::DisplayProcessorInformation();
}

//--//

#pragma arm section code


void CPU_Sleep()
{
    AT91_SAM_Driver::Sleep();
}

void CPU_Halt()
{
    AT91_SAM_Driver::Halt();
}

BOOL CPU_PowerOnReset()
{
    return AT91_SAM_Driver::PowerOnReset();
}

void CPU_Shutdown()
{
    AT91_SAM_Driver::Shutdown();
}

void CPU_Hibernate()
{
    AT91_SAM_Driver::Hibernate();
}

void CPU_Reset()
{
    AT91_SAM_Driver::Reset();
}

BOOL CPU_IsSoftRebootSupported ()
{
    return TRUE;
}

