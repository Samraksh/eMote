////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
<<<<<<< .mine
#include <rs232/stm32f10x_rs232.h>
#include <led/stm32f10x_led.h>
#include <lcd_basic/stm32f10x_lcd_basic.h>
=======
#include <rtc/int_time.h>
>>>>>>> .r110


////////////////////////////////////////////////////////////////////////////////
void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
	RTC_IRQHandler();
	TIM1_UP_IRQHandler();
}

void ApplicationEntryPoint()
{
      STM3210E_LCD_Init();
	  LCD_Clear(White); 
	  LCD_SetBackColor(Blue);
	  LCD_SetTextColor(White);
	 
	  
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

//LED_RED();
	
}


BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

