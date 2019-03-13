////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
#include "OMACTest.h"

////////////////////////////////////////////////////////////////////////////////


void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

    ClrStartup( clrSettings );
    /*do
    {
    	//OMACTest omacTest();
    	BOOL ret;
    	ret = gOMACTest.Initialize();
    	ret = gOMACTest.StartTest();

    	//dsTestObject.Execute(TEST_INITIALIZATION);

    } while(false); // run only once!
    */

    while(true){
    	::Events_WaitForEvents(0, 100);
    }

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

