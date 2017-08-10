//#include <cores\arm\include\cpu.h>
#include <tinyhal.h>
#include "netmf_pwr.h"

BOOL CPU_Initialize() {
	PowerInit();
	return TRUE;
}

BOOL CPU_JTAG_Attached(){
	return JTAG_Attached();
}

void CPU_ChangePowerLevel(POWER_LEVEL level) {
    switch(level)
    {
        case POWER_LEVEL__HIGH_POWER:
			High_Power();
			break;
		case POWER_LEVEL__MID_POWER:
			Mid_Power();
			break;
		case POWER_LEVEL__LOW_POWER:
        default:
			Low_Power();
            break;
    }
}

void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents ) {
#if defined (DISABLE_SLEEP) || defined (SAM_APP_TINYBOOTER) // To make grabbing it with the JTAG easier.
	return;
#else
    switch(level)
    {
		case SLEEP_LEVEL__OFF:
			Shutdown();
			break;
		case SLEEP_LEVEL__DEEP_SLEEP:
		case SLEEP_LEVEL__SELECTIVE_OFF:
		case SLEEP_LEVEL__AWAKE:
		case SLEEP_LEVEL__SLEEP:
		default:
			Sleep();
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

