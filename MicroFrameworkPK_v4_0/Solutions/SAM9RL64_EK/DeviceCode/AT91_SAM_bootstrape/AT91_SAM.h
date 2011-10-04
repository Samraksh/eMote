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

#ifndef _AT91SAM_H_
#define _AT91SAM_H_ 1
    
//////////////////////////////////////////////////////////////////////////////
extern "C"
{
    void CPWAIT(int reg);
}

//////////////////////////////////////////////////////////////////////////////
// SAM9 driver
//
struct AT91_SAM_Driver
{
    static const UINT32 c_SystemTime_Timer         = 0;
    static const UINT32 c_PerformanceCounter_Timer = 1;
    BOOL             m_PowerOnReset;

    #if !defined(BUILD_RTM)
        volatile UINT32* m_PerformanceCounter;
    #endif

    //--//
    static BOOL Initialize();
    static void Pause();
    static void Sleep();
    static void Halt();
    static void Reset();
    static BOOL PowerOnReset();
    static void Shutdown();
    static void Hibernate();
    static void DisplayProcessorInformation();
};
extern AT91_SAM_Driver g_AT91SAM_Driver;
//
// SAM9 driver
//////////////////////////////////////////////////////////////////////////////

#endif // _AT91SAM_H_
