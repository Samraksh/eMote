////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
#include <EVAL_LED/Eval_stm322xg_led.h>
extern STM322xG_LED g_STM322xG_LED;

////////////////////////////////////////////////////////////////////////////////
void BlinkLEDs(){

	g_STM322xG_LED.STM_EVAL_LEDInit(LED1);
	g_STM322xG_LED.STM_EVAL_LEDInit(LED2);
	g_STM322xG_LED.STM_EVAL_LEDInit(LED3);
	g_STM322xG_LED.STM_EVAL_LEDInit(LED4);

	for(int i=0; i<10000; i++);

	while(1){
		g_STM322xG_LED.STM_EVAL_LEDToggle(LED1);
		g_STM322xG_LED.STM_EVAL_LEDToggle(LED2);
		g_STM322xG_LED.STM_EVAL_LEDToggle(LED3);
		g_STM322xG_LED.STM_EVAL_LEDToggle(LED4);
		for(int i=0; i<1000000; i++);
	}
}

void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    CPU_GPIO_EnableOutputPin((GPIO_PIN) 3, FALSE);
    CPU_GPIO_EnableOutputPin((GPIO_PIN) 2, FALSE);

    CPU_GPIO_EnableOutputPin((GPIO_PIN) 4, FALSE);


    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;
#if 0
    CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
    UINT8 k = 0;

    while(true)
    {
    	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
    	for(UINT8 i = 0; i < 255; i++)
    		k = i;
    	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
    }
#endif
    BlinkLEDs();

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

