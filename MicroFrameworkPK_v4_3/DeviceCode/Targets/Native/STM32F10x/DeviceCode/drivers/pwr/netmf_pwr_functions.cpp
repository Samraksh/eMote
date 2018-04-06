#include <tinyhal.h>
#include "netmf_pwr.h"
#include "netmf_pwr_wakelock.h"

BOOL CPU_Initialize() {
	PowerInit();
	return TRUE;
}

BOOL CPU_JTAG_Attached(){
	return JTAG_Attached();
}

void CPU_ChangePowerLevel(POWER_LEVEL level) {
	GLOBAL_LOCK(irq);
    switch(level)
    {
        case POWER_LEVEL__HIGH_POWER:
			hal_printf("POWER LEVEL: HIGH\r\n");
			High_Power();
			break;
		case POWER_LEVEL__MID_POWER:
			hal_printf("POWER LEVEL: MID \r\n");
			Mid_Power();
			break;
		case POWER_LEVEL__LOW_POWER:
        default:
			hal_printf("POWER LEVEL: LOW \r\n");
			Low_Power();
            break;
    }
}

BOOL CPU_Sleep_WakeLock_Status() {
	return GetWakeLocked();
}

void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents ) {
#if defined (DISABLE_SLEEP) // To make grabbing it with the JTAG easier.
	return;
#else
    switch(level)
    {
		case SLEEP_LEVEL__OFF:
			Shutdown();
			break;
		case SLEEP_LEVEL__DEEP_SLEEP:
		case SLEEP_LEVEL__SELECTIVE_OFF:
			Sleep();
			break;
		case SLEEP_LEVEL__AWAKE:
		case SLEEP_LEVEL__SLEEP:
		default:
			Snooze();
			break;
    }
#endif
}

void CPU_Halt() {
#ifdef NDEBUG
	CPU_Reset();
#else
	Halt();
#endif
}

void CPU_Reset() {
	Reset();
}

/*
Indicates whether a soft reboot is supported by the underlying hardware. 
*/
BOOL CPU_IsSoftRebootSupported() {
    return TRUE;
}

