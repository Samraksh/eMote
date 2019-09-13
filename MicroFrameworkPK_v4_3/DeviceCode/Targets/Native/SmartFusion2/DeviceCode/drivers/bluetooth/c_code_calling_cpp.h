#ifndef C_CODE_CALLING_CPP_H
#define C_CODE_CALLING_CPP_H

#include <stdint.h>
#include <stdarg.h>
#include <tinyhal_types.h>
#include "sf2\btmain.h"
#include "btcore\bluetooth.h"	

#define ENCRYPTED_DATA_CHANNEL ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE
#define UNENCRYPTED_DATA_CHANNEL ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FC_01_VALUE_HANDLE
#define CLOUD_CHANNEL ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FD_01_VALUE_HANDLE

#ifdef __cplusplus
extern "C" {
#endif
#include "btcore\btstack_uart_block.h"	

#define BD_ADDR_LEN 6
typedef uint8_t bd_addr_t[BD_ADDR_LEN];

typedef void (*TIMER_CALLBACK_FPN_C)( void* arg );
// used to debug the C bluetooth code
void debugBT_printf(const char* format, va_list argptr);
void dubgBT_printchar(char c);

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
void btCallReceive(uint16_t source, uint8_t *buffer, uint16_t buffer_size);

void sendBTPacket(UINT16 dest, uint8_t* data, uint8_t length);
void btConnectedFunc(int number, int connectionType);
void btDisconnectedFunc(int number, int connectionType);

void storeBtLinkKey(bd_addr_t bd_addr, link_key_t link_key, link_key_type_t link_key_type);
void deleteBtLinkKey(bd_addr_t bd_addr);
int getBtLinkKey(bd_addr_t bd_addr, link_key_t link_key, link_key_type_t *link_key_type);


#ifdef __cplusplus
}
#endif



#endif // C_CODE_CALLING_CPP_H

