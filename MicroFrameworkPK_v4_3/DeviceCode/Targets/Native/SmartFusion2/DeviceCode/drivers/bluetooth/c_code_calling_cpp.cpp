#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include <stdint.h>
#include <tinyhal_types.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void debugBT_printf(const char* format, va_list argptr){
	//hal_vprintf(format, argptr);
}

void* mf_private_malloc ( size_t len             ){
	return mf_private_malloc(len);
}

void  mf_private_free   ( void*  ptr             ){
	private_free(ptr);
}

void SetBTTimerInterrupt(int ticks, void* callbackFunction){
	VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, ticks, FALSE, TRUE, (TIMER_CALLBACK_FPN)callbackFunction, ADVTIMER_32BIT);
	VirtTimer_Start(VIRT_TIMER_BLUETOOTH_TICK);
}

uint64_t BTGetTicks(void){
	// getting only lower 32-bits
	CPU_Timer_CurrentTicks(DEFAULT_TIMER);
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

