////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
#include <cmsis_gcc.h>
////////////////////////////////////////////////////////////////////////////////
bool IsPrivMode(){
	if (__get_IPSR() || !(__get_CONTROL() & 0x1))
	{
		return true;
	}
	return false;
}

void ChangeExecMode(bool priv){
	if(priv){

	}else {
		UINT32 privMask=0xFFFFFFFE; //clear the CONTROL[0] to enter user mode
		UINT32 con = __get_CONTROL();
		__set_CONTROL(privMask & con);
	}
}

void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;


    if(IsPrivMode()){
    	debug_printf( "Still in Priviledged Mode... Switching to thread mode.\r\n" );
    	ChangeExecMode(false);
    }

    ClrStartup( clrSettings );

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

