#include <cores\arm\include\cpu.h>
#include "netmf_pwr.h"

BOOL CPU_Initialize() {

	// A total hack to determine if we're in TinyCLR or TinyBooter.
	// There is probably a preprocessor define somewhere but I wasted 15 minutes on it.
	// If you know it please fix me. -- NPS
	BOOL (*myaddr)() = &CPU_Initialize;
	
	// Use low-power mode to measure HSI, but only from TinyCLR.
	// Then go back to HSI (for now).
	if ((uint32_t)myaddr > 0x8020000) {
		STM32F1x_Power_Driver::Low_Power();
		STM32F1x_Power_Driver::CalibrateHSI();
		STM32F1x_Power_Driver::High_Power();
	}
	else { // Use High-power mode for TinyBooter for now. Probably can use 8 MHz but not sure we care.
		STM32F1x_Power_Driver::High_Power();
	}
}

// Default to Low (8 MHz)
void CPU_ChangePowerLevel(POWER_LEVEL level) {
    switch(level)
    {
        case POWER_LEVEL__MID_POWER:
			STM32F1x_Power_Driver::Low_Power();
            break;
			
        case POWER_LEVEL__LOW_POWER:
			STM32F1x_Power_Driver::Low_Power();
            break;
			
        case POWER_LEVEL__HIGH_POWER:
			STM32F1x_Power_Driver::High_Power();
			break;
			
        default:
			STM32F1x_Power_Driver::Low_Power();
            break;
    }
}

// I don't actually know how OFF and DEEP_SLEEP get called
// SLEEP gets called when the MF has no threads ready
void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents ) {
#if 0
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
#endif
}

void CPU_Halt() {
    STM32F1x_Power_Driver::Halt();
}

void CPU_Reset() {
	CPU_GPIO_SetPinState((GPIO_PIN) 2, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 2, FALSE);

	//STM32F1x_Power_Driver::Reset();
	// This function is supported by the watchdog module but placing the implementation here temporarily
	__disable_irq();
	NVIC_SystemReset();
	while(1);
}

/*
Indicates whether a soft reboot is supported by the underlying hardware. 
*/
BOOL CPU_IsSoftRebootSupported() {
    return FALSE;
}

