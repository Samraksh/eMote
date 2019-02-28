#ifndef C_CODE_CALLING_CPP_H
#define C_CODE_CALLING_CPP_H

#include <stdint.h>
#include <stdarg.h>
#include <tinyhal_types.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*TIMER_CALLBACK_FPN_C)( void* arg );
// used to debug the C bluetooth code
void debugBT_printf(const char* format, va_list argptr);

void* mf_private_malloc ( size_t len             );
void  mf_private_free   ( void*  ptr             );

void  SetBTTimerInterrupt(int ticks, void* callbackFunction);
uint64_t BTGetTicks(void);
void BTTimerStop(void);

void ClearReset(void);
void SetReset(void);
void ConfigureSoftwareFlowCtrl(void);
void ConfigureHardwareFlowCtrl(void);
void FlowOff(void);
void FlowOn(void);
int FlowIsOn(void);

#ifdef __cplusplus
}
#endif



#endif // C_CODE_CALLING_CPP_H

