#ifndef C_CODE_CALLING_CPP_H
#define C_CODE_CALLING_CPP_H



#ifdef __cplusplus
extern "C" {
#endif
typedef void (*TIMER_CALLBACK_FPN_C)( void* arg );
// used to debug the C bluetooth code
void debugBT_printf(char* message);
void debugBT_Numprintf(char* message, int num);

void  SetBTTimerInterrupt(int ticks, TIMER_CALLBACK_FPN_C callbackFunction);
void BTTimerStop(void);

#ifdef __cplusplus
}
#endif



#endif // C_CODE_CALLING_CPP_H

