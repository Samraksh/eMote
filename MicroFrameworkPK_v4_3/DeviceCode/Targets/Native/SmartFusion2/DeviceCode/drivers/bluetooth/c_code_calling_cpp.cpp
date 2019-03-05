#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include <stdint.h>
#include <tinyhal_types.h>
#include <stdarg.h>

#define BT_COM_PORT 1
#define RX_BUFF 1

#ifdef __cplusplus
extern "C" {
#endif

#include "btcore\btstack_uart_block.h"	
#include "sf2\btstack_port.h"	

// uart config
static const btstack_uart_config_t * uart_config;

void call_btstack_scheduler_loop(){
	hal_btstack_run_loop_execute_once();
}

void debugBT_printf(const char* format, va_list argptr){
	hal_vprintf(format, argptr);
	hal_printf("\r\n");
}

void* mf_private_malloc ( size_t len             ){
	return private_malloc(len);
}

void  mf_private_free   ( void*  ptr             ){
	private_free(ptr);
}

void mf_delay_us(unsigned int usDelay){
	CPU_Timer_Sleep_MicroSeconds(usDelay,ADVTIMER_32BIT);
}

void SetBTTimerInterrupt(int ticks, void* callbackFunction){
	//VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, ticks, FALSE, TRUE, (TIMER_CALLBACK_FPN)callbackFunction, ADVTIMER_32BIT);
	VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, 1000000, FALSE, TRUE, (TIMER_CALLBACK_FPN)call_btstack_scheduler_loop, ADVTIMER_32BIT);
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

int btUartNumReadAvail(){
	USART_BytesInBuffer(BT_COM_PORT, RX_BUFF);
}

int btUartRead(uint8_t * rx_buff_ptr, int num_rx_bytes){
}

int btUartNumWriteAvail(){
	hal_printf("can uartnum avail tx return actual value\r\n");
	return 100;
}

int btUartWrite(uint8_t* tx_buff_ptr, int num_tx_bytes){
	CPU_USART_WriteStringToTxBuffer(BT_COM_PORT, (char*)tx_buff_ptr, num_tx_bytes);
	return num_tx_bytes;
}

/*void btRxHandler((char*) &rx_buff, (size_t)rx_size)
{

}*/

int btUartInit(const btstack_uart_config_t * config){
	hal_printf("uart init: baud: %d\r\n", config->baudrate);
	uart_config = config;
	CPU_USART_Initialize(BT_COM_PORT, config->baudrate, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
	//CPU_USART_set_rx_handler_override(&btRxHandler);
	return 0;
}

int btUartOpen(){
	hal_printf("uart opened\r\n");
	// set baud
	// baud = uart_config->baudrate;
	return 0;
}

int btUartClose(){
	hal_printf("uart closed\r\n");
	return 0;
}

int btUartSend(const uint8_t *buffer, uint16_t len){
	CPU_USART_WriteStringToTxBuffer(BT_COM_PORT, (char*) buffer, len);
}

void DisableBTUart(){
	hal_printf("*** incomplete *** DisableBTUart\r\n");
}

#ifdef __cplusplus
}
#endif

