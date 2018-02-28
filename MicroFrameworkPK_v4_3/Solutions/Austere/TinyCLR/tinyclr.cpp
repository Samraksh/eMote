////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

#include <austere\austere.h>

// static void rad_power_monitor(GPIO_PIN Pin, BOOL PinState, void* Param) {
	// if (CPU_GPIO_GetPinState(39) == TRUE){
		// hal_printf("*** rad pwr good ***\r\n");
	// } else {
		// hal_printf("*** rad pwr bad ***\r\n");
	// }
// }

void ApplicationEntryPoint()
{
// #ifdef PLATFORM_ARM_AUSTERE // Print power status before starting CLR
	// CPU_GPIO_EnableInputPin( (GPIO_PIN) 39, FALSE, rad_power_monitor, GPIO_INT_EDGE_BOTH, RESISTOR_PULLUP );
	// print_power_supply_status();
// #endif

	print_power_supply_status();

    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

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

