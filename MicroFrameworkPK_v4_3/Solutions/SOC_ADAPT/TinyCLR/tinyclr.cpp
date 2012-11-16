////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>


////////////////////////////////////////////////////////////////////////////////
void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

#if GPIO_TEST
    CPU_GPIO_EnableOutputPin(58, TRUE);
    CPU_GPIO_EnableOutputPin(55, TRUE);
    CPU_GPIO_EnableOutputPin(53, TRUE);
    CPU_GPIO_EnableOutputPin(52, TRUE);
    CPU_GPIO_EnableOutputPin(51, TRUE);
    CPU_GPIO_EnableOutputPin(50, TRUE);
    CPU_GPIO_EnableOutputPin(70, TRUE);
    CPU_GPIO_EnableOutputPin(71, TRUE);
    CPU_GPIO_EnableOutputPin(72, TRUE);
    CPU_GPIO_EnableOutputPin(68, TRUE);
#endif

    ClrStartup( clrSettings );

#if GPIO_TEST
    while(true)
    {
    	CPU_GPIO_SetPinState(58, TRUE);
    	for(int i = 0;  i < 1000000;  i++);
    	CPU_GPIO_SetPinState(58, FALSE);

    	CPU_GPIO_SetPinState(55, TRUE);
    	for(int i = 0;  i < 200000;  i++);
    	CPU_GPIO_SetPinState(55, FALSE);

    	CPU_GPIO_SetPinState(53, TRUE);
    	for(int i = 0;  i < 1000;  i++);
    	CPU_GPIO_SetPinState(53, FALSE);

    	CPU_GPIO_SetPinState(52, TRUE);
    	for(int i = 0;  i < 1000;  i++);
    	CPU_GPIO_SetPinState(52, FALSE);

    	CPU_GPIO_SetPinState(51, TRUE);
    	for(int i = 0;  i < 1000000;  i++);
    	CPU_GPIO_SetPinState(51, FALSE);

    	CPU_GPIO_SetPinState(50, TRUE);
    	for(int i = 0;  i < 1000;  i++);
    	CPU_GPIO_SetPinState(50, FALSE);

    	CPU_GPIO_SetPinState(70, TRUE);
    	for(int i = 0;  i < 1000;  i++);
    	CPU_GPIO_SetPinState(70, FALSE);

    	CPU_GPIO_SetPinState(71, TRUE);
    	for(int i = 0;  i < 1000;  i++);
    	CPU_GPIO_SetPinState(71, FALSE);
    }
#endif

#if !defined(BUILD_RTM)
    debug_printf( "Exiting.\r\n" );
#else
    ::CPU_Reset();
#endif
}


BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

