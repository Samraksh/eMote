#include <tinyhal.h>
#include "SimplePower.h"

BOOL CPU_Initialize() {
	PowerInit();
}

BOOL CPU_JTAG_Attached(){
	return JTAG_Attached();
}

// TinyBooter never calls this, C# only
void CPU_ChangePowerLevel(POWER_LEVEL level) {
	return;
	/*
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
	*/
}

// Don't sleep so we have easy JTAG grab.
void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents ) {

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

