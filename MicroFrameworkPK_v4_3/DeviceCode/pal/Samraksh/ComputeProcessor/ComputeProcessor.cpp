// This file handles the interaction with an external compute processor.
// When using an external processor we are not expected to run a C# app.
//
// ***** Note: all messages to and from the Compute Processor assume '\n' as the last character of the message

#include <tinyhal.h>
#include "ComputeProcessor.h"
#include <drivers/mss_spi/mss_spi.h>
#include <cmsis/m2sxxx.h>
#include <grenadeFunction/grenadeFunction.h>
#include <loadTarget/loadTarget.h>
#include <COM_Manager_decl.h>
#include <PKCS11/CryptokiPAL.h>
#include <Samraksh\MAC_decl.h>
#include <Samraksh/VirtualTimer.h>
#include <../../DSM/ComManager/ComManager.h>

static bool CP_currently_present = FALSE;
const int buffSize = 128;
static uint8_t parseBuffer[buffSize];
static int parseBuffPosition = 0;
volatile int statusCheckGood = 0;
int binaryFixed = 0;
int pinState;
static HAL_CONTINUATION failSafeContinuation;

void failSafe(void *arg){
	hal_printf("Error with Compute Processor. Verifying binary is correct.\r\n");

	//hal_printf("**** enable verify check\r\n");
	int retVal = verifyArduinoSPI((uint8_t*)NVM_CP_BINARY_LOCATION,NVM_CP_BINARY_SIZE);
//int retVal = 0;
	if (retVal == 0){
		hal_printf("Compute Processor binary is correct. Restarting Compute Processor.\r\n");
		CPU_GPIO_SetPinState(GPIO_CP_RESET, TRUE);
		// take Arduino out of reset
		CPU_GPIO_SetPinState(GPIO_CP_RESET, FALSE);
		hal_printf("Sending Compute Processor crash error report to gateway.\r\n");

		hal_printf("Requesting Compute Processor binary update\r\n");
		RequestNewBinary();
		VirtTimer_Stop(VIRT_TIMER_PERIODIC_CP_STATUS);
	} else {
		hal_printf("Compute Processor binary was corrupted. Reloading.\r\n");
		hal_printf("Sending Compute Processor corruption report to gateway.\r\n");
	}
}

void CP_SendMsgToCP(uint8_t* msg, int size){
	int buffSize = 128;
	int sendPos = 0;
	CPU_GPIO_SetPinState(1, TRUE);
	HAL_Time_Sleep_MicroSeconds(1000);

	MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
	while (sendPos < size){
		MSS_SPI_transfer_block(&g_mss_spi1, &msg[sendPos], 1, 0, 0 );
		HAL_Time_Sleep_MicroSeconds(1000);
		sendPos++;
	}
	MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
	HAL_Time_Sleep_MicroSeconds(1000);
	CPU_GPIO_SetPinState(1, FALSE);
}

//returns last compare position if match, otherwise 0
int strCompare(uint8_t* msg, const char* str, int* position){
	(*position) = 0;
	for (int i = 0; i < buffSize; i++){
		if (str[i] == '\n'){
			(*position) = i;
			return i;
		}
		if (msg[i] != str[i])
			return 0;
	}
	return 0;
}

int getParameter(uint8_t* msg, uint8_t* param){
	int msgPos = 0;
	int paramPos = 0;
	
	//searching until we see a ' ' char
	while ( (msgPos<buffSize) && (msg[msgPos] != ' ') && (msg[msgPos] != '\n') )
	{
		msgPos++;
	}
	
	// if there are no parameters we return here
	if (msg[msgPos] ==  '\n')
			return msgPos  + 1;

	// currently at space between command and parameter
	msgPos++;
	
	while ( (msgPos<buffSize) && (msg[msgPos] != '\n') )
	{
		param[paramPos] = msg[msgPos];
		msgPos++;
		paramPos++;
	}

	// currently at '\n', going to return the next character
	if (msgPos < (buffSize - 1) )
		return msgPos + 1;
	else
		return msgPos;
}

int getNumFromASCII(uint8_t asciiNum){
	if ( (int)asciiNum < '0' || (int)asciiNum > '9')
		return 0;
	else
		return ( (int) (asciiNum - '0'));
}

int getNumFromStr(uint8_t* msg){
	int finalNum = 0;

	// num string could be 1-3 char (1, 12, 123)
	if (msg[1] == ' ')
		finalNum = getNumFromASCII(msg[0]);
	else if (msg[2] == ' ')
		finalNum = getNumFromASCII(msg[0]) * 10 + getNumFromASCII(msg[1]);
	else
		finalNum = getNumFromASCII(msg[0]) * 100 + getNumFromASCII(msg[1]) * 10 + getNumFromASCII(msg[2]);

	return finalNum;
}

int get2Parameter(uint8_t* msg, int* dataSize, uint8_t* param){
	int msgPos = 0;
	int paramPos = 0;
	int dataCount = 0;
	uint8_t str[buffSize];
	
	//searching until we see a ' ' char
	while ( (msgPos<buffSize) && (msg[msgPos] != ' ') )
	{
		msgPos++;
	}

	// currently at space between command and data count
	msgPos++;

	*dataSize = getNumFromStr(&msg[msgPos]);
	
	//searching until we see a ' ' char
	while ( (msgPos<buffSize) && (msg[msgPos] != ' ') && (msg[msgPos] != '\n') )
	{
		msgPos++;
	}

	// currently at space between command and data count
	msgPos++;

	dataCount = 0;
	paramPos = 0;
	while ( (msgPos < buffSize) && (dataCount < *dataSize) )
	{
		param[paramPos] = msg[msgPos];
		msgPos++;
		paramPos++;
		dataCount++;
	}

	return msgPos;
}

void shiftBuffer(uint8_t* buff, int shiftAmnt){
	if (shiftAmnt > buffSize - 1 ) shiftAmnt = buffSize - 1;
	for (int i = 0; i < buffSize; i++){
		if (i < buffSize - shiftAmnt - 1){
			buff[i] = buff[i+shiftAmnt];
		} else {
			buff[i] = 0;
		}
	}
	parseBuffPosition -= shiftAmnt;
}

static int parseCPCommand(uint8_t* msg){
	uint8_t parameter1[buffSize];
	int dataSize = 0;
	int parameterEnd;
	int comparePos;

	//hal_printf("parseCPCmd\r\n");

	for (int k = 0; k < buffSize; k++){
		parameter1[k] = 0;
	}

	if (strCompare(msg, "SetLocalName\n", &comparePos)){
		parameterEnd = getParameter(msg, parameter1);
		//hal_printf("set local name %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "SetClassOfDevice\n", &comparePos)){
		parameterEnd = getParameter(msg, parameter1);
		//hal_printf("set class %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "PINCodeResponse\n", &comparePos)){
		parameterEnd = getParameter(msg, parameter1);
		//hal_printf("pin code %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "SetDiscoverabilityMode\n", &comparePos)){
		parameterEnd = getParameter(msg, parameter1);
		//hal_printf("disc mode %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "SetConnectabilityMode\n", &comparePos)){
		parameterEnd = getParameter(msg, parameter1);
		//hal_printf("set conn mode %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "Inquiry\n", &comparePos)){
		//hal_printf("Inquire %d\r\n", comparePos);
		shiftBuffer(msg, comparePos + 1);
		return 1;
	} else if (strCompare(msg, "GetInquiryList\n", &comparePos)){
		//hal_printf("inquiry list %d\r\n", comparePos);
		shiftBuffer(msg, comparePos + 1);
		return 1;
	} else if (strCompare(msg, "CloseConnection\n", &comparePos)){
		//hal_printf("close conn %d\r\n", comparePos);
		shiftBuffer(msg, comparePos + 1);
		return 1;
	} else if (strCompare(msg, "DataWrite\n", &comparePos)){
		parameterEnd = get2Parameter(msg, &dataSize, parameter1);
		//hal_printf("data write (%d) %s\r\n", dataSize, parameter1);
		Message_Receive_From_CP(parameter1, (uint16_t)dataSize);
		shiftBuffer(msg, parameterEnd);
		return 1;
	}
	return 0;
}

int parseBuffContainsCmdCheck(uint8_t* msg){
	for (int i = 0; i < buffSize; i++){
		if (msg[i] == '\n')
			return 1;
	}
	return 0;
}


static void CP_WantsTransaction(GPIO_PIN Pin, BOOL PinState, void* Param)
{
	uint8_t spi_rx_buff[buffSize];
	int sendPos = 0;

	MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
	while ((sendPos < buffSize) && (CPU_GPIO_GetPinState(COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM) == 1)){
		MSS_SPI_transfer_block(&g_mss_spi1, 0, 0, &spi_rx_buff[sendPos], 1 );	
		HAL_Time_Sleep_MicroSeconds(COMPUTE_PROCESSOR_COMM_WAIT_TIME);
		parseBuffer[parseBuffPosition] = spi_rx_buff[sendPos];
		//hal_printf("%02x ",parseBuffer[parseBuffPosition]);
		if (parseBuffPosition < buffSize)
			parseBuffPosition++;	
		if (sendPos < buffSize)
			sendPos++;
	}
	MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
	//hal_printf("\r\n");

	while (parseBuffContainsCmdCheck(parseBuffer)){
		/*hal_printf("pb: ");
		for (int j=0; j < buffSize; j++){
			hal_printf("%c", parseBuffer[j]);
		}
		hal_printf("\r\n\r\n");*/

		// check if parsed, if not we clean
		if (parseCPCommand(parseBuffer) == 0){
			// clean parseBuffer
			for (int i = 0; i < buffSize; i++){
				parseBuffer[i] = 0;
			}
			parseBuffPosition = 0;
		}
		//hal_printf("pbp: %d\r\n", parseBuffPosition);
	}
	
	/*hal_printf("pb: ");
	for (int j=0; j < buffSize; j++){
		hal_printf("%c", parseBuffer[j]);
	}
	hal_printf("\r\n\r\n");*/

	/*hal_printf("rx: ");
	for (int j=0; j < sendPos; j++){
		hal_printf("%c", spi_rx_buff[j]);
	}
	hal_printf("\r\n");}*/
}

static void CP_UserBtnHigh(GPIO_PIN Pin, BOOL PinState, void* Param){
	int buffer_size = 2;
	uint8_t buffer[10];
	buffer[0] = 'B';
	if (CPU_GPIO_GetPinState(COMPUTE_PROCESSOR_USER_BUTTON_PUSH) == true){
		hal_printf("user button HIGH\r\n");
		buffer[1] = 'D';
		statusCheckGood = 1;
	} else {
		hal_printf("user button LOW\r\n");
		buffer[1] = 'U';
		SendDetectMessage();
		statusCheckGood = 0;
	}
	CP_SendMsgToCP(buffer, buffer_size);
}

void PeriodicStatusCheck(void * param){
	CPU_GPIO_SetPinState(11, pinState);
	if (pinState == 0) pinState = 1;
	else pinState = 0;
	CPU_GPIO_SetPinState(14, pinState);
	if ((statusCheckGood == 0) && (binaryFixed == 0)){
		hal_printf("error! status check failed\r\n");
		failSafeContinuation.Enqueue();
	} else {
		int buffer_size = 2;
		uint8_t buffer[10];
		buffer[0] = 'S';
		buffer[1] = 'T';
		CP_SendMsgToCP(buffer, buffer_size);
		//statusCheckGood = 0;
	}
}

void CP_Reload(void){
	hal_printf("Reloading CP\r\n");
	// external power enabled
	CPU_GPIO_EnableOutputPin(4, TRUE);
	//hal_printf("***** arduino binary moved for debug....move back to 0xe000! *****\r\n");
	loadArduinoSPI((uint8_t*)NVM_CP_BINARY_LOCATION,NVM_CP_BINARY_SIZE);
	HAL_Time_Sleep_MicroSeconds(50000);

	// ********** change me back	
	verifyArduinoSPI((uint8_t*)NVM_CP_BINARY_LOCATION,NVM_CP_BINARY_SIZE);
	//hal_printf("**** disabled arduino loading ****\r\n");	
	hal_printf("bring CP out of reset\r\n");
	//CPU_GPIO_EnableOutputPin(0, TRUE);
	HAL_Time_Sleep_MicroSeconds(50000);
	CPU_GPIO_SetPinState(0, FALSE);

	binaryFixed = 1;
	VirtTimer_Start(VIRT_TIMER_PERIODIC_CP_STATUS);
}

// returns TRUE if successful
bool CP_Init(void){
	failSafeContinuation.InitializeCallback(failSafe, NULL);
	statusCheckGood = 1;
	binaryFixed = 0;
	//hal_printf("load arduino commented out\r\n");
	CPU_GPIO_EnableOutputPin(0, TRUE);
	CPU_GPIO_EnableOutputPin(1, FALSE);

	pinState = 0;
	CPU_GPIO_EnableOutputPin(11, FALSE);
	CPU_GPIO_EnableOutputPin(14, FALSE);

	// external power enabled
	CPU_GPIO_EnableOutputPin(4, TRUE);
	//hal_printf("***** arduino binary moved for debug....move back to 0xe000! *****\r\n");
	loadArduinoSPI((uint8_t*)NVM_CP_BINARY_LOCATION,NVM_CP_BINARY_SIZE);
	HAL_Time_Sleep_MicroSeconds(50000);

	// ********** change me back	
	verifyArduinoSPI((uint8_t*)NVM_CP_BINARY_LOCATION,NVM_CP_BINARY_SIZE);
	//hal_printf("**** disabled arduino loading ****\r\n");	
	hal_printf("bring CP out of reset\r\n");
	//CPU_GPIO_EnableOutputPin(0, TRUE);
	HAL_Time_Sleep_MicroSeconds(50000);
	CPU_GPIO_SetPinState(0, FALSE);
	
	hal_printf("Compute processor init\r\n");
	CPU_GPIO_EnableInputPin( COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM, FALSE, CP_WantsTransaction, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	if (CPU_GPIO_GetPinState(COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM) == true){
		CP_WantsTransaction(NULL, FALSE, NULL);
	}

	CPU_GPIO_EnableInputPin( COMPUTE_PROCESSOR_USER_BUTTON_PUSH, FALSE, CP_UserBtnHigh, GPIO_INT_EDGE_BOTH, RESISTOR_DISABLED);
	for (int i = 0; i < buffSize; i++){
		parseBuffer[i] = 0;
	}

	VirtTimer_SetTimer(VIRT_TIMER_PERIODIC_CP_STATUS, 0, 10000000, FALSE, TRUE, (TIMER_CALLBACK_FPN)PeriodicStatusCheck, ADVTIMER_32BIT);
	VirtTimer_Start(VIRT_TIMER_PERIODIC_CP_STATUS);
	return TRUE;
}

bool CP_Present(void){
	return CP_currently_present;
}

bool CP_UnInit(void){
	return TRUE;
}

void PrintHex(uint8_t* sig, int size){
	for (int j=0;j<size; j++){
		hal_printf("%c , ",sig[j]);
		if ((j%16)==0) hal_printf("\r\n");
	}
	hal_printf("\r\n");
}

void Message_Receive_From_CP( uint8_t *buffer, uint16_t buffer_size){
	//PrintHex(buffer, buffer_size);
	if ((buffer[0] == 'B') & (buffer[1] == 'U')){
		//hal_printf("msg rx cp bu\r\n");
	} else if ((buffer[0] == 'B') & (buffer[1] == 'D')){
		//hal_printf("msg rx cp bd\r\n");
	} else if ((buffer[0] == 'S') & (buffer[1] == 'G')){
		//hal_printf("+");
		//statusCheckGood = 1;
	}
/*	CK_BYTE encryptedData[128];

		hal_printf("sending data: ");
		//PrintHex(buffer,buffer_size);

	//if (currentMac == BLUETOOTHMAC){
		//if (link_encrypted == true){
			hal_printf("encrypted link\r\n");
			int encryptSize = Message_Encrypt_Data(buffer, buffer_size, encryptedData);
			MAC_Send(ENCRYPTED_DATA_CHANNEL, NULL, encryptedData, encryptSize);
		//} else {
		//	MAC_Send(UNENCRYPTED_DATA_CHANNEL, NULL, buffer, buffer_size);
		//}
	//}*/
}
