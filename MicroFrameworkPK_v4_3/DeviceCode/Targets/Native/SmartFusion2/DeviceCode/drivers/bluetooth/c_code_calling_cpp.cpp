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

void ClearReset(void){
	hal_printf("*** clear reset not implemented\r\n");
}

void SetReset(void){
	hal_printf("*** set reset not implemented\r\n");
}

void ConfigureSoftwareFlowCtrl(void){
	hal_printf("*** ConfigureSoftwareFlowCtrl not implemented\r\n");
}

void ConfigureHardwareFlowCtrl(void){
	hal_printf("*** ConfigureHardwareFlowCtrl not implemented\r\n");
}

void FlowOff(void){
	hal_printf("*** FlowOff not implemented\r\n");
}

void FlowOn(void){
	hal_printf("*** FlowOn not implemented\r\n");
}

int FlowIsOn(void){
	hal_printf("*** FlowIsOn not implemented\r\n");
	return 1;
}

int EnableBTUart(){
	return 0;
}

void DisableBTUart(){
	hal_printf("*** incomplete *** DisableBTUart\r\n");
}

#ifdef __cplusplus
}
#endif

