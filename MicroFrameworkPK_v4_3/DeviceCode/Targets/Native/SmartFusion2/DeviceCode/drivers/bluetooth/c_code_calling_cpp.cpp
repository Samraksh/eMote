#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include <stdint.h>
#include <tinyhal_types.h>
#include <stdarg.h>
#include <Samraksh\BluetoothMac_Functions.h>


#define BT_COM_PORT 1
#define RX_BUFF 1

#ifdef __cplusplus
extern "C" {
#endif

#include "btcore\btstack_uart_block.h"	
#include "btcore\bluetooth.h"	
#include "sf2\btstack_port.h"	
#include "sf2\hal_tick.h"
#include "sf2\btmain.h"

// ***** temp storage in RAM ******
// ***** use keystore instead or at least FLASH
bd_addr_t temp_bd_addr;
link_key_t temp_link_key;
link_key_type_t temp_link_key_type;

// uart config
static const btstack_uart_config_t * uart_config;

void call_btstack_scheduler_loop(){
	hal_btstack_run_loop_execute_once();
}

void debugBT_printf(const char* format, va_list argptr){
	hal_vprintf(format, argptr);
	hal_printf("\r\n");
}

void dubgBT_printchar(char c){
	hal_printf("%c",c);
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

void SetBTTimerInterrupt(int ticks){
	//VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, ticks, FALSE, TRUE, (TIMER_CALLBACK_FPN)callbackFunction, ADVTIMER_32BIT);
	VirtTimer_SetOrChangeTimer(VIRT_TIMER_BLUETOOTH_TICK, 0, 10000, FALSE, TRUE, (TIMER_CALLBACK_FPN)call_btstack_scheduler_loop, ADVTIMER_32BIT);
	VirtTimer_Start(VIRT_TIMER_BLUETOOTH_TICK);
	//VirtTimer_SetOrChangeTimer(39, 0, 5000000, FALSE, TRUE, (TIMER_CALLBACK_FPN)sendPacket, ADVTIMER_32BIT);
	//VirtTimer_Start(39);
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
	int byteCount = USART_BytesInManagedBuffer(BT_COM_PORT, RX_BUFF);
	//if  (byteCount != 0)
	//	hal_printf("read avail: %d\r\n", byteCount);
	return byteCount;
}

int btUartRead(uint8_t * rx_buff_ptr, int num_rx_bytes){
	int numBytesRead = USART_Managed_Read(BT_COM_PORT, (char*)rx_buff_ptr, num_rx_bytes);
	//hal_printf("uart read: %d\r\n", numBytesRead);
	return numBytesRead;
}

int btUartNumWriteAvail(){
	return PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE;
}

int btUartWrite(uint8_t* tx_buff_ptr, int num_tx_bytes){
	bool pinState = CPU_GPIO_GetPinState(7);
	if (pinState == true){
		hal_printf("*** error sending data when BT module not ready\r\n");
		return 0;
	} else {
		// should change to USART_Write but need to initialize UART normally first
		//hal_printf("sending %d bytes to BT\r\n", num_tx_bytes);
		//CPU_USART_WriteStringToTxBuffer(BT_COM_PORT, (char*)tx_buff_ptr, num_tx_bytes);
		USART_Write(BT_COM_PORT, (char*)tx_buff_ptr, num_tx_bytes);
	}
	return num_tx_bytes;
}

/*void btRxHandler((char*) &rx_buff, (size_t)rx_size)
{

}*/

void btConnectedFunc(int number, int connectionType){
	CPU_Bluetooth_Connected(number, connectionType);
}

void btDisconnectedFunc(int number, int connectionType){
	CPU_Bluetooth_Disconnected(number, connectionType);
}

void CTS_Handler(GPIO_PIN Pin, BOOL PinState, void* Param){
	bool pinState = CPU_GPIO_GetPinState(7);
	if (pinState == true)
		hal_printf("BT module ready to receive\r\n");
}

int btUartInit(const btstack_uart_config_t * config){
	hal_printf("uart init: baud: %d\r\n", config->baudrate);
	uart_config = config;
	//CPU_USART_Initialize(BT_COM_PORT, config->baudrate, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
	USART_Initialize(BT_COM_PORT, config->baudrate, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );

	// initializing and setting RTS high (telling BT module we are not ready to get data)
	CPU_GPIO_EnableOutputPin(9, FALSE);
	CPU_Timer_Sleep_MicroSeconds(10000,ADVTIMER_32BIT);
	CPU_GPIO_SetPinState(9, TRUE);
	CPU_Timer_Sleep_MicroSeconds(10000,ADVTIMER_32BIT);

	// initializing CTS from BT module
	//CPU_GPIO_EnableInputPin(7, FALSE, CTS_Handler, GPIO_INT_EDGE_LOW, RESISTOR_DISABLED);

	//CPU_USART_set_rx_handler_override(&btRxHandler);
	
	
	// ***** temp link key RAM storage *****
	int i;
	for (i=0; i<BD_ADDR_LEN; i++){
		temp_bd_addr[i] = 0;
	}
	for (i=0; i<LINK_KEY_LEN; i++){
		temp_link_key[i] = 0;
	}
	temp_link_key_type = DEBUG_COMBINATION_KEY;

	return 0;
}

int btUartOpen(){
	hal_printf("uart opened\r\n");
	// set baud
	// baud = uart_config->baudrate;
	
	// setting RTS low (telling BT module we are ready to receive data)
	CPU_GPIO_SetPinState(9, FALSE);

	return 0;
}

int btUartClose(){
	hal_printf("uart closed\r\n");

	// setting RTS high (telling BT module we are not ready to get data)
	CPU_GPIO_SetPinState(9, TRUE);
	return 0;
}

void DisableBTUart(){
	hal_printf("*** incomplete *** DisableBTUart\r\n");
	USART_Uninitialize(BT_COM_PORT);
}

void btCallReceive(uint16_t source, uint8_t *buffer, uint16_t buffer_size){
	Bluetooth_Receive_Data(source, buffer, buffer_size);
}

void sendBTPacket(UINT16 dest, uint8_t* data, uint8_t length){
	sendDataPacket(dest, data, length);
}



void storeBtLinkKey(bd_addr_t bd_addr, link_key_t link_key, link_key_type_t link_key_type){
	hal_printf("store link key: ");
	int i;
	for (i=0; i<LINK_KEY_LEN; i++){
		hal_printf("%x ", link_key[i]);
	}
	hal_printf(" for bd_addr: ");
	for (i=0; i<BD_ADDR_LEN; i++){
		hal_printf("%x ", bd_addr[i]);
	}
	hal_printf(" of type: %d\r\n",link_key_type);
	
	memcpy(temp_bd_addr, bd_addr, BD_ADDR_LEN);
	memcpy(temp_link_key, link_key, LINK_KEY_LEN);
}

void deleteBtLinkKey(bd_addr_t bd_addr){
	hal_printf("delete link key");
	int i;
	
	hal_printf(" for bd_addr: ");
	for (i=0; i<BD_ADDR_LEN; i++){
		hal_printf("%x ", bd_addr[i]);
	}
	hal_printf("\r\n");

	for (i=0; i<BD_ADDR_LEN; i++){
		temp_bd_addr[i] = 0;
	}
	for (i=0; i<LINK_KEY_LEN; i++){
		temp_link_key[i] = 0;
	}
}

int getBtLinkKey(bd_addr_t bd_addr, link_key_t link_key, link_key_type_t *link_key_type){
	hal_printf("get link key: ");
	int i;
	for (i=0; i<LINK_KEY_LEN; i++){
		hal_printf("%x ", link_key[i]);
	}
	hal_printf(" for bd_addr: ");
	for (i=0; i<BD_ADDR_LEN; i++){
		hal_printf("%x ", bd_addr[i]);
	}
	hal_printf("\r\n");

	if (memcmp(bd_addr, temp_bd_addr, 6) == 0){
		// matched bd_addr
		memcpy(link_key, temp_link_key, LINK_KEY_LEN);
		*link_key_type = temp_link_key_type;
		return 1;
	}

	return 0;
}

#ifdef __cplusplus
}
#endif

