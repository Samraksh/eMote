#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>

#ifdef __cplusplus
extern "C" {
#endif

void debugBT_printf(const char* message){
	hal_printf("debugBT: %s\r\n", message);
}

void debugBT_Numprintf(char* message, int num){
	hal_printf("debugBT: %s %d\r\n", message, num);
}

void SetBTTimerInterrupt(int ticks, TIMER_CALLBACK_FPN callbackFunction){
	VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, ticks, FALSE, TRUE, callbackFunction, ADVTIMER_32BIT);
	VirtTimer_Start(VIRT_TIMER_BLUETOOTH_TICK);
}

void BTTimerStop(void){
	VirtTimer_Stop(VIRT_TIMER_BLUETOOTH_TICK);
}

#ifdef __cplusplus
}
#endif

