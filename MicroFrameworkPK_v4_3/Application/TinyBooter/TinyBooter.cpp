////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "TinyBooter.h"
#include "TinyCLR_Version.h"
#include <tinybooterentry.h>
#include "ConfigurationManager.h"

extern bool WaitForTinyBooterUpload( INT32 &timeout_ms );

#if defined(PLATFORM_ARM_EmoteDotNow)
//#include "..\..\Solutions\EmoteDotNow\DeviceCode\LCD_PCF85162_HAL\LCD_PCF85162.h"
//extern LCD_PCF85162_Driver g_LCD_PCF85162_Driver;
#endif
////////////////////////////////////////////////////////////////////////////////

//--//

Loader_Engine g_eng;

//--//

HAL_DECLARE_CUSTOM_HEAP( SimpleHeap_Allocate, SimpleHeap_Release, SimpleHeap_ReAllocate );

//--//

void ApplicationEntryPoint()
{
    INT32 timeout       = 10000; // 5 second timeout
	UINT64 timeoutTicks, curTicks, prevTicks;
    bool  enterBootMode = false;

    // crypto API needs to allocate memory. Initialize simple heap for it. 
    UINT8* BaseAddress;                                 
    UINT32 SizeInBytes;                                 

#if defined(PLATFORM_ARM_EmoteDotNow)
    CPU_GPIO_EnableOutputPin((GPIO_PIN) OUTPUT_REBOOT_PIN, FALSE);
#endif

    HeapLocation         ( BaseAddress, SizeInBytes );  
    SimpleHeap_Initialize( BaseAddress, SizeInBytes );  

    g_eng.Initialize( HalSystemConfig.DebuggerPorts[ 0 ] );

    // internal reset and stop check
    enterBootMode = g_PrimaryConfigManager.IsBootLoaderRequired( timeout );

    // ODM defined method to enter bootloader mode
    if(!enterBootMode)
    {
        enterBootMode = WaitForTinyBooterUpload( timeout );
    }

#if defined (ENABLE_TINYOS_HACK) && (TINYOS_BOOT_MAGIC) && (PLATFORM_ARM_EmoteDotNow) && (TINYOS_BOOT_MAGIC_LOC)
{ // This is basically a condensed version of ::Launch, which is private, and I'm lazy
	int check_tinyos = *(volatile unsigned int *)TINYOS_BOOT_MAGIC_LOC;
	if (check_tinyos == TINYOS_BOOT_MAGIC && !enterBootMode) {
		typedef  void (*pFunction)(void);
		//g_eng.Launch((ApplicationStartAddress)0x08020000); // private, can't do this.
		LCD_Clear();
		//DebuggerPort_Flush( m_port );
		DISABLE_INTERRUPTS();
		LCD_Uninitialize();
		CPU_DisableCaches();
		HAL_Time_Uninitialize();
		int ResetVector = *(volatile UINT32 *) (0x08020000 + 4);  // Cortex-M3 vector table is MSP followed by Reset
		__attribute__((__noreturn__)) pFunction Jump_To_Application = (pFunction) ResetVector; // unconditional jump. never returns.
		// Contract: MSP is set by target application entry.
		Jump_To_Application();
	}
}
#endif

    if(!enterBootMode)   
    {
        if(!g_eng.EnumerateAndLaunch())
        {
            timeout       = -1;
            enterBootMode = true;
        }
    }

#if defined(PLATFORM_ARM_EmoteDotNow)
    //g_LCD_PCF85162_Driver.Initialize();
    //g_LCD_PCF85162_Driver.Write(DISPLAY_CHAR_B, DISPLAY_CHAR_O, DISPLAY_CHAR_O, DISPLAY_CHAR_T);
#endif


    if(enterBootMode)
    {
        LCD_Clear();
        
        hal_fprintf( STREAM_LCD, "TinyBooter v1.0\r\n");
        hal_fprintf( STREAM_LCD, "%s Build Date:\r\n\t%s %s\r\n", HalName, __DATE__, __TIME__ );

        DebuggerPort_Initialize( HalSystemConfig.DebuggerPorts[ 0 ] );

        TinyBooter_OnStateChange( State_EnterBooterMode, NULL );

        DebuggerPort_Flush( HalSystemConfig.DebugTextPort  );
        hal_printf( "TinyBooter v1.0\r\n");
        hal_printf( "Platform: %s\r\n", HalName );
        hal_printf( "Build Date %s\r\n", __DATE__);
#include <Samraksh\githash.h>
#include <Samraksh\teamid.h>
        //hal_printf( "Software ID: %s-%s by: %s\r\n", GIT_HASH_AT_BUILD, GIT_INDEX_STATUS_AT_BUILD, YOU_ARE_AWESOME );  // Software ID may be integrated into the Config region for RTM build.
#if defined(__GNUC__)
        hal_printf("GNU Compiler version %d.%d.%d\r\n", __GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__);
        /*hal_printf("Optimization: %s, %s\r\n",
#if defined(__OPTIMIZE__)
                "speed-optimized"
#else
                "no-speed-optimization"
#endif
                ,
#if defined(__OPTIMIZE_SIZE__)
                "size-optimized"
#else
                "no-size-optimization"
#endif
        );*/
#elif defined(_ARC)
        hal_printf("ARC Compiler version %d\r\n", _ARCVER);
#elif defined(__ADSPBLACKFIN__)
        hal_printf( "Blackfin Compiler version %d\r\n", __VERSIONNUM__ );
#elif defined(__RENESAS__)
        hal_printf( "Renesas Compiler version %d\r\n", __RENESAS_VERSION__ );
#else
        hal_printf( "ARM Compiler version %d\r\n", __ARMCC_VERSION );
#endif
        DebuggerPort_Flush( HalSystemConfig.DebugTextPort );

		for (int i=0; i<1000; i++){}
        //
        // Send "presence" ping.
        //
        {
            CLR_DBG_Commands::Monitor_Ping cmd;

            cmd.m_source = CLR_DBG_Commands::Monitor_Ping::c_Ping_Source_TinyBooter;

            g_eng.m_controller.SendProtocolMessage( CLR_DBG_Commands::c_Monitor_Ping, WP_Flags::c_NonCritical, sizeof(cmd), (UINT8*)&cmd );
        }
    
        prevTicks = HAL_Time_CurrentTicks();
		curTicks = prevTicks;

        //
        // Wait for somebody to press a button; if no button press comes in, lauch the image
        //
        do
        {
            const UINT32 c_EventsMask = SYSTEM_EVENT_FLAG_COM_IN |
                                        SYSTEM_EVENT_FLAG_USB_IN |
                                        SYSTEM_EVENT_FLAG_BUTTON;

            UINT32 events = ::Events_WaitForEvents( c_EventsMask, timeout );

            if(events != 0)
            {
                Events_Clear( events );
            }

            if(events & SYSTEM_EVENT_FLAG_BUTTON)
            {
                TinyBooter_OnStateChange( State_ButtonPress, (void*)&timeout );
            }
            
            if(events & (SYSTEM_EVENT_FLAG_COM_IN | SYSTEM_EVENT_FLAG_USB_IN))
            {
                g_eng.ProcessCommands();
            }

			if (curTicks < prevTicks)
			{
				// timer rolled over, if in the rare case a timeout occurred it should be caught the next time around this loop
				prevTicks = curTicks;
			}
			timeoutTicks = curTicks - prevTicks;	
			

            if(LOADER_ENGINE_ISFLAGSET(&g_eng, Loader_Engine::c_LoaderEngineFlag_ValidConnection))
            {
                LOADER_ENGINE_CLEARFLAG(&g_eng, Loader_Engine::c_LoaderEngineFlag_ValidConnection);
                
                TinyBooter_OnStateChange( State_ValidCommunication, (void*)&timeout );

                prevTicks = curTicks;
				curTicks = HAL_Time_CurrentTicks();
            }
            else if((timeout != -1) && (timeoutTicks > CPU_MillisecondsToTicks((UINT32)timeout)) )
			{
                TinyBooter_OnStateChange( State_Timeout, NULL );
                g_eng.EnumerateAndLaunch();
            }
        } while(true);
    }

    ::CPU_Reset();
}
