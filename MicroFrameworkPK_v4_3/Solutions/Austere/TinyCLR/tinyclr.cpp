////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyclr_application.h>
#include <tinyhal.h>

#include <austere\austere.h>

#ifdef NETMF_RADIO_DEBUG
#include <Samraksh/Radio_decl.h>
static void wait() { while (HAL_CONTINUATION::Dequeue_And_Execute() == TRUE); }
static void print_radio_state() { hal_printf("Radio: %s\r\n", rs_tostring(CPU_Radio_Get_State(SI4468_SPI2))); }
static void wait2() { wait(); wait(); }
#endif

void ApplicationEntryPoint()
{	
	austere_init();
	print_power_supply_status();

#ifdef NETMF_RADIO_DEBUG
	DeviceStatus ret;
	hal_printf("Native Radio Test Running...\r\n");
	print_radio_state();

	ret = CPU_Radio_Set_State(SI4468_SPI2, STATE_START); // STATE_START only valid from OFF and will block until ready
	if (ret == DS_Success) hal_printf("Radio power-on test pass\r\n");
	else { hal_printf("Radio power-on test FAIL\r\n"); goto OUT; }
	wait2();

	ret = CPU_Radio_Initialize(NULL, SI4468_SPI2, 0, 0 );
	if (ret == DS_Success) hal_printf("CPU_Radio_Initialize() test pass\r\n");
	else { hal_printf("CPU_Radio_Initialize() test FAIL\r\n"); goto OUT; }
	wait2();

	// wait 11 seconds then turn OFF
	HAL_Time_Sleep_MicroSeconds(11000000);
	ret = CPU_Radio_Set_State(SI4468_SPI2, STATE_OFF); // Turn OFF the radio
	if (ret == DS_Success) hal_printf("Radio power-off test pass\r\n");
	else { hal_printf("Radio power-off test FAIL\r\n"); goto OUT; }
	wait2();

	// wait 11 seconds then turn ON
	HAL_Time_Sleep_MicroSeconds(11000000);
	ret = CPU_Radio_Set_State(SI4468_SPI2, STATE_START);
	if (ret == DS_Success) hal_printf("Radio power-on test2 pass\r\n");
	else { hal_printf("Radio power-on test2 FAIL\r\n"); goto OUT; }
	wait2();

	// wait 11 seconds then turn OFF
	HAL_Time_Sleep_MicroSeconds(11000000);
	ret = CPU_Radio_Set_State(SI4468_SPI2, STATE_OFF); // Turn OFF the radio
	wait2();

	// wait 11 seconds then turn ON to SLEEP
	HAL_Time_Sleep_MicroSeconds(11000000);
	ret = CPU_Radio_Set_State(SI4468_SPI2, STATE_SLEEP);
	if (ret == DS_Success) hal_printf("Radio off-->sleep test pass\r\n");
	else { hal_printf("Radio off-->sleep test FAIL\r\n"); goto OUT; }
	wait2();

	// wait 11 seconds then SLEEP to RX
	HAL_Time_Sleep_MicroSeconds(11000000);
	ret = CPU_Radio_Set_State(SI4468_SPI2, STATE_RX);
	if (ret == DS_Success) hal_printf("Radio Sleep-->RX test pass\r\n");
	else { hal_printf("Radio Sleep-->RX test FAIL\r\n"); goto OUT; }
	wait2();
	
	// wait 11 seconds then RX to SLEEP
	HAL_Time_Sleep_MicroSeconds(11000000);
	ret = CPU_Radio_Set_State(SI4468_SPI2, STATE_SLEEP);
	if (ret == DS_Success) hal_printf("Radio RX-->sleep test pass\r\n");
	else { hal_printf("Radio RX-->Sleep test FAIL\r\n"); goto OUT; }
	wait2();
	
	hal_printf("All tests PASS\r\n");

OUT:
	hal_printf("DONE\r\n");
	CPU_Sleep(SLEEP_LEVEL__AWAKE, 0);
	hal_printf("wokeup\r\n");
	while(1) { wait(); }
#else
    CLR_SETTINGS clrSettings;

    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

    ClrStartup( clrSettings );
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

