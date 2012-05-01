#include <cores\arm\include\cpu.h>
#include "netmf_pwr.h"

BOOL CPU_Initialize() {
    return STM32F1x_Power_Driver::Initialize();
}

void CPU_ChangePowerLevel(POWER_LEVEL level) {
    switch(level)
    {
        case POWER_LEVEL__MID_POWER:
			STM32F1x_Power_Driver::High_Power();
            break;
			
        case POWER_LEVEL__LOW_POWER:
			STM32F1x_Power_Driver::Low_Power();
            break;
			
        case POWER_LEVEL__HIGH_POWER:
			STM32F1x_Power_Driver::High_Power();
			break;
			
        default:
			STM32F1x_Power_Driver::High_Power();
            break;
    }
}

// I don't actually know how OFF and DEEP_SLEEP get called
// SLEEP gets called when the MF has no threads ready
void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents ) {
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            STM32F1x_Power_Driver::Hibernate();
            break;
        case SLEEP_LEVEL__OFF:
            STM32F1x_Power_Driver::Shutdown();
            break;
        case SLEEP_LEVEL__SLEEP:
        default:
            STM32F1x_Power_Driver::Sleep();
            break;
    }
}

void CPU_Halt() {
    STM32F1x_Power_Driver::Halt();
}

void CPU_Reset() {
    STM32F1x_Power_Driver::Reset();
}

/*
Indicates whether a soft reboot is supported by the underlying hardware. 
*/
BOOL CPU_IsSoftRebootSupported() {
    return FALSE;
}

