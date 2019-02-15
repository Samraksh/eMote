#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>

#ifdef __cplusplus
extern "C" {
#endif


void debugBT_printf(const char* message){
	hal_printf("debugBT called: %s\r\n", message);
}

void SetBTTimerInterrupt(int ticks, TIMER_CALLBACK_FPN callbackFunction){
	VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, 50000000, TRUE, TRUE, callbackFunction, ADVTIMER_32BIT);
}

void BTTimerStop(void){
	VirtTimer_Stop(VIRT_TIMER_BLUETOOTH_TICK);
}

#ifdef __cplusplus
}
#endif

