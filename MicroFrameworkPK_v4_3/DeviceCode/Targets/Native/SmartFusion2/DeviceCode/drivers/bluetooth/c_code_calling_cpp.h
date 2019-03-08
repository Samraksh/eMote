#ifndef C_CODE_CALLING_CPP_H
#define C_CODE_CALLING_CPP_H

#include <stdint.h>
#include <stdarg.h>
#include <tinyhal_types.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "btcore\btstack_uart_block.h"	

typedef void (*TIMER_CALLBACK_FPN_C)( void* arg );
// used to debug the C bluetooth code
void debugBT_printf(const char* format, va_list argptr);

void* mf_private_malloc ( size_t len             );
void  mf_private_free   ( void*  ptr             );

void  SetBTTimerInterrupt(int ticks);
uint64_t BTGetTicks(void);
void BTTimerStop(void);
void mf_delay_us(unsigned int usDelay);

int btUartInit(const btstack_uart_config_t * config);
int btUartOpen();
int btUartClose();

int btUartNumReadAvail();
int btUartRead(uint8_t * rx_buff_ptr, int num_rx_bytes); 
int btUartNumWriteAvail();
int btUartWrite(uint8_t * tx_buff_ptr, int num_tx_bytes); 

void call_btstack_scheduler_loop();

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

