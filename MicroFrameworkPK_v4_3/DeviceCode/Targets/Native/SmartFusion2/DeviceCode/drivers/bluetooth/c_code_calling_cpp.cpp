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

void BT_VTCallback(void * arg){
	hal_printf("bt_vt callback\r\n");
	//callbackFunction();
}

void SetBTTimerInterrupt(int ticks, TIMER_CALLBACK_FPN callbackFunction){
	hal_printf("starting timer\r\n");
	VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, 5000, TRUE, TRUE, BT_VTCallback, ADVTIMER_32BIT);
	VirtTimer_Start(VIRT_TIMER_BLUETOOTH_TICK);
}

void BTTimerStop(void){
	VirtTimer_Stop(VIRT_TIMER_BLUETOOTH_TICK);
}

#ifdef __cplusplus
}
#endif

