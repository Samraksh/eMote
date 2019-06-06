#include <tinyhal.h>
#include <Samraksh/VirtualTimer.h>
#include <stdint.h>
#include <tinyhal_types.h>
#include <stdarg.h>
#include <..\..\..\..\..\..\pal\PKCS11\CryptokiPAL.h>
#include <crypto.h>

#define BT_COM_PORT 1
#define RX_BUFF 1

#ifdef __cplusplus
extern "C" {
#endif

#include "btcore\btstack_uart_block.h"	
#include "sf2\btstack_port.h"	
#include "sf2\hal_tick.h"
#include "sf2\btmain.h"


const int hmacSize=32;

const CK_BYTE key1[hmacSize] = {
		0xC6, 0x29, 0x73, 0xE3, 0xC8, 0xD4, 0xFC, 0xB6,
        0x89, 0x36, 0x46, 0xF9, 0x58, 0xE5, 0xF5, 0xE5,
        0x25, 0xC2, 0xE4, 0x1E, 0xCC, 0xA8, 0xC3, 0xEF,
        0xA2, 0x8D, 0x24, 0xDE, 0xFD, 0x19, 0xDA, 0x08
};

const CK_BYTE hmac1[hmacSize] = {
		0, 103, 74, 155, 97, 125, 27, 130,
		83, 111, 216, 226, 156, 45, 100, 50,
		59, 61, 228, 144, 127, 39, 150, 29,
		253, 74, 92, 188, 247, 200, 88, 195
};

CK_BYTE data[128] = {
	0, 103, 74, 155, 97, 125, 27, 130,
	83, 111, 216, 226, 156, 45, 100, 50,
	59, 61, 228, 144, 127, 39, 150, 29,
	253, 74, 92, 188, 247, 200, 88, 195,
	0, 103, 74, 155, 97, 125, 27, 130,
	83, 111, 216, 226, 156, 45, 100, 50,
	59, 61, 228, 144, 127, 39, 150, 29,
	253, 74, 92, 188, 247, 200, 88, 195,
	0, 103, 74, 155, 97, 125, 27, 130,
	83, 111, 216, 226, 156, 45, 100, 50,
	59, 61, 228, 144, 127, 39, 150, 29,
	253, 74, 92, 188, 247, 200, 88, 195,
	0, 103, 74, 155, 97, 125, 27, 130,
	83, 111, 216, 226, 156, 45, 100, 50,
	59, 61, 228, 144, 127, 39, 150, 29,
	253, 74, 92, 188, 247, 200, 88, 195,
};

CK_BYTE ddata[128];

	//CK_BYTE data[128];
	CK_BYTE digest[32];
	CK_BYTE IV[48];
	CK_BYTE_PTR  pData;
	CK_ULONG ulDataLen;
	CK_BYTE  pCryptText[128];
	CK_ULONG ulCryptLen;
	CK_BYTE_PTR pDigest;
	CK_MECHANISM_TYPE mtype;
	CK_KEY_TYPE kt;
	CK_BYTE_PTR pkey;

void PrintHex(CK_BYTE_PTR sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("0x%.2X , ",sig[j]);
	}
	hal_printf("\n");
}

// uart config
static const btstack_uart_config_t * uart_config;

void call_btstack_scheduler_loop(){
	hal_btstack_run_loop_execute_once();
}

void sendPacket(){
	sendDataPacket();
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
	VirtTimer_SetOrChangeTimer(39, 0, 5000000, FALSE, TRUE, (TIMER_CALLBACK_FPN)sendPacket, ADVTIMER_32BIT);
	VirtTimer_Start(39);
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
	memset(pDigest,0,hmacSize);
	//memcpy(data,"Samraksh eMote Cryptoki HMAC Example; Plus the wolf is great, but the fox is grey. The lamb is prey, but its a mountain pro!",124);
	pData=data;
	ulDataLen=128;
	ulCryptLen=128;
	Crypto_GetRandomBytes(IV, 48);
	hal_printf("IV : ");
	PrintHex(IV,48);
	pDigest=digest;
	mtype=CKM_SHA256_HMAC;
	pkey=(CK_BYTE_PTR)key1;
	kt= CKK_GENERIC_SECRET;

	hal_printf("Original Text: ");PrintHex(pData,ulDataLen);
	bool ret= Crypto_Encrypt(pkey,32,IV, 48, pData, ulDataLen, pCryptText, ulCryptLen);
	if(!ret){hal_printf("Encryption Failed\n");}
	hal_printf("Encrypted Text: ");PrintHex(pCryptText,ulCryptLen);
	ret= Crypto_Decrypt(pkey,32,IV, 48, pCryptText, ulCryptLen, ddata, ulDataLen);
	if(!ret){hal_printf("Decryption Failed\n");}
	hal_printf("Decrypted Text: ");PrintHex(pData,ulDataLen);
	hal_printf("\n\n  ");

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

#ifdef __cplusplus
}
#endif

