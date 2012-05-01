////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>
//#include <Tests.h>
#include <lcd_basic/stm32f10x_lcd_basic.h>
#include <rtc/int_time.h>
#include <exti/stm32f10x_exti.h>
#include <led/stm32f10x_led.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>
#include <misc/misc.h>
#include <CPU_SPI_decl.h>
#include <radio/radio.h>
#include <tim/netmf_timers.h>
#include <test_native_drivers/timers_test/main.h>
#include <test_native_drivers/timers_test/stm32f10x_it.h>
#include <pal/COM/usb/usb.h>

////////////////////////////////////////////////////////////////////////////////

void Keep_Linker_Happy() {
	BootstrapCode();
	BootEntry();
	RTC_IRQHandler();
	TIM1_UP_IRQHandler();
	EXTI9_5_IRQHandler();
	EXTI0_IRQHandler();
	TIM2_IRQHandler();
	TIM3_IRQHandler();
}

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

BOOL Execute( UINT32 m_timerDuration, UINT32 m_displayInterval )
{

    //Log& log = Log::InitializeLog( Stream, "Timers" );

    debug_printf( "\r\n" );

    debug_printf( "\r\n" );

    for(UINT32 seconds=0; seconds<m_timerDuration; seconds++)
    {

        //lcd_printf( "\r        %c",thorp[seconds % 4] );

        //hal_printf( "\r        %c",thorp[seconds % 4] );

        if((seconds % m_displayInterval) == 0)
        {					
            debug_printf( "        %2d",seconds );
			STM_EVAL_LEDToggle((Led_TypeDef)0);
            debug_printf( "        %2d",seconds );
        }

        Events_WaitForEvents ( 0, 1000 );
    }
    debug_printf( "\r\n" );

    debug_printf( "\r\n" );

    return TRUE;
}

void CompletionCallback(void * context)
{
    //callBackCount++;
	LED_GREEN();
    Events_Set(SYSTEM_EVENT_FLAG_UNUSED_0x80000000);
}

BOOL TimedEventsExecute()
{
    UINT32 count    = 100;
    UINT32 signaled = FALSE;
    
    HAL_COMPLETION compObj;

    //Log& log = Log::InitializeLog( Stream, "TimedEvent" );

    //callBackCount = 0;

    compObj.InitializeForISR( CompletionCallback, NULL );

    while(count--)
    {
        compObj.EnqueueDelta( 150 );

        signaled = Events_WaitForEvents( SYSTEM_EVENT_FLAG_UNUSED_0x80000000, 50 );

        if(signaled != SYSTEM_EVENT_FLAG_UNUSED_0x80000000)
        {
            //log.CloseLog( FALSE, "Wrong order" );
			LED_RED();
            return FALSE; 
        }
    }

    //log.SetState( callBackCount );
    //log.LogState  ( "%2d callbacks" );
    //log.CloseLog  ( TRUE, NULL );

    return TRUE; 
}

void ApplicationEntryPoint()
{
	BOOL result;
	STM3210E_LCD_Init();
	LCD_Clear(White);
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(White);

    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 100;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;
	
	//for (int i = 0; i < 1000; i++)
	//{
		//debug_printf("In App Entry point!\n\r");	
	//}
	//while(1)
	///{	
	//}
	
	ClrStartup( clrSettings );
	
	// while(1)
	// {	
		// debug_printf("In App Entry point!\n\r");
		// debug_printf("Printing from USB 1\n\r");
		// debug_printf("Printing from USB 2\n\r");
		// debug_printf("Printing from USB 3\n\r");
	// }
	
	//const char* Data = "Hello From USB";
	
	//USB_Configure( ConvertCOM_UsbController(HalSystemConfig.DebugTextPort), NULL );
    //USB_Initialize( ConvertCOM_UsbController(HalSystemConfig.DebugTextPort) );	
    //USB_OpenStream( ConvertCOM_UsbStream(HalSystemConfig.DebugTextPort), USB_DEBUG_EP_WRITE, USB_DEBUG_EP_READ );
	//for(int i =0; i < 100000; i++);
	//while(1)
	//{
	//	USB_Write( 0, Data, 15 );
	//}
	//while(1)
	//{
	//	USB_Write( 0, Data, 15 );
	//}

    //test_timers();
	//test_usart();
	
	//uint8_t sys_clk = RCC_GetSYSCLKSource();

	//Timer_Driver::Timer_Test();

	//radio_hal_init();
	//RCC_ClocksTypeDef RCC_Clocks;
	//RCC_GetClocksFreq(&RCC_Clocks);
	
	//CPU_INTC_Initialize();
	
	//test_timers();    
	
	//rtc_test();
	
	//TimedEvents eventsTest;	
	
	//do
    //{
        //result = Execute(1000, 5);	
		//result = TimedEventsExecute();
	//} while(FALSE);
	
	//while(1)
	//{}

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

