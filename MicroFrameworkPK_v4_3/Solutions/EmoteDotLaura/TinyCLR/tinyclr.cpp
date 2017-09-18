////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

#include <DeviceCode\MaximLib\DevKitBoard\nhd12832.h>
#include <DeviceCode\MaximLib\DevKitBoard\board.h>
#include <tmr_utils.h>

#include "RequestSendAtTimeInstantTest.h"
///////////////////////////////////////////////////////////////////////////////

//
extern "C" void __cxa_pure_virtual() { while (true); }
void* __dso_handle;
//void* __dso_handle;
void operator delete(void *, unsigned int){
	while (true);
}

/* Monochrome logo for OLED display */
nhd12832_bitmap_t samraksh_logo = {
    128,   32,    0,
    {
    		0x00000000, 0x00000000, 0x00000000, 0x00000000,
    		0x00038000, 0x007FFC00, 0x01FFFF00, 0x03FFFF80,
            0x0FFFFFE0, 0x1FFFFFF0, 0x3FFFFFF8, 0x3FFFFFFC,
            0x7F0003FC, 0xFF0001FE, 0xFF0001FE, 0xFFFFF9FF,
            0xFFFFF9FF, 0xFFFFF9FF, 0xFF3FF1FF, 0xFF1FC1FF,
            0xFF03C7FF, 0xFFC1FFFF, 0xFFE07FFF, 0xFFE01FFF,
            0xFF830FFF, 0xFF0F83FF, 0xFF1FE1FF, 0xFF7FF9FF,
            0xFFFFF9FF, 0xFFFFF9FF, 0xFF0001FE, 0xFF0001FE,
            0x7F0001FC, 0x3FFFFFFC, 0x3FFFFFF8, 0x1FFFFFF0,
            0x0FFFFFE0, 0x03FFFF80, 0x01FFFF00, 0x007FFC00,
            0x00038000, 0x00000000, 0x00000000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
            0x00030000, 0x0FFB3FE0, 0x0FFB3FE0, 0x00000060,
            0x00000060, 0x0FF80060, 0x0FF83FE0, 0x00383FE0,
            0x00180060, 0x00180060, 0x0FF80060, 0x0FF03FE0,
            0x00003FE0, 0x00180000, 0x00180000, 0x0FFF1E60,
            0x0FFF3E60, 0x0C183360, 0x0C183320, 0x03C03B60,
            0x0FF03FE0, 0x0FF83FE0, 0x0CD80000, 0x0CD83020,
            0x0CD838E0, 0x0CF81FE0, 0x0EF00FC0, 0x00000F80,
            0x78E01FE0, 0x7FF03CE0, 0x6FF83020, 0x4F980000,
            0x4D983FE6, 0x6DF83FEE, 0x7DF00004, 0x78780000,
            0x38183FE0, 0x00003FE0, 0x0FF83FE0, 0x0FF00060,
            0x00300060, 0x00380060, 0x00183FE0, 0x0FB03FE0,
            0x0F980060, 0x0D980060, 0x0CD80060, 0x0CD83FE0,
            0x0FF83FE0, 0x0FF00000, 0x00000000, 0x00180000,
            0x00180000, 0x0FFF0000, 0x0FFF0000, 0x0C180000,
            0x0C180000, 0x00000000, 0x0FF00000, 0x0FF80000,
            0x0CD80000, 0x0CD80000, 0x0CD80000, 0x0CF80000,
            0x0EF00000, 0x00000000, 0x03C00000, 0x0FF00000,
            0x0FF00000, 0x0C380000, 0x0C180000, 0x0C180000,
            0x0FFF8000, 0x0FFF8000, 0x0FFF8000, 0x00000000,
            0x00000000, 0x00000000, 0x00000000, 0x00000000,
    }
};


void DevKitLCD_Display(){
	//NHD12832_Init();
	//NHD12832_ShowString((uint8_t*)"Samraksh MAX C# Demo", 0, 4);
	// Display the splash screen for  3 seconds, uses timer 5
	/*TMR_Delay(MXC_TMR5, MSEC(5000));
	NHD12832_Clear(0,0);
	TMR_Delay(MXC_TMR5, MSEC(1500));
	NHD12832_LoadImage(&samraksh_logo);
	NHD12832_PrintScreen();
	TMR_Delay(MXC_TMR5, MSEC(5000));*/
	//NHD12832_Clear(0,0);
	/*while (1){
		TMR_Delay(MXC_TMR5, MSEC(5));
		debug_printf("a");
	}*/
}



////////////////////////////////////////////////////////////////////////////////
void ApplicationEntryPoint()
{
	DevKitLCD_Display();

	CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));


    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

    Test_InitializeAndRun();

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

