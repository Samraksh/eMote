////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
#include <lcd_basic/stm32f10x_lcd_basic.h>
#include <rtc/int_time.h>
//#include <udivdi3.c>

////////////////////////////////////////////////////////////////////////////////
//extern "C" void Default_Handler();

void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
	RTC_IRQHandler();
	TIM1_UP_IRQHandler();
	//Default_Handler();
}

int64_t Test_uint64_t();
//double Test_Double();
//uint64_t Divide(uint64_t dividend, uint64_t divisor);



void ApplicationEntryPoint()
{

	//CPU_GPIO_SetPinState(89, TRUE);
	STM3210E_LCD_Init();
	LCD_Clear(White); 
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line1, (uint8_t*)"Hello...");

    //Time_Initialize(); //Could not find the Time_Initialize call anywhere else
	  
    CLR_SETTINGS clrSettings;
	
	//Test_uint64_t();

	//Test_Double();

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

int64_t Test_uint64_t()
{
	uint64_t a = 600000;
	uint64_t b = 60;	
	//LCD_DisplayStringLine(Line0, (uint8_t *)"Here.. ");
	uint64_t c = a / b;
	//LCD_DisplayStringLine(Line1, (uint8_t *)"SUCCEEDED");
	return c;
}

BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

