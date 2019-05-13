// This file handles the interaction with an external compute processor.
// When using an external processor we are not expected to run a C# app.

#include <tinyhal.h>
#include "ComputeProcessor.h"
#include <drivers/mss_spi/mss_spi.h>
#include <cmsis/m2sxxx.h>
#include <string.h>

static bool CP_currently_present = FALSE;
const int buffSize = 128;
static uint8_t parseBuffer[buffSize];
static int parseBuffPosition = 0;

//returns last compare position if match, otherwise 0
int strCompare(uint8_t* msg, const char* str){
	for (int i = 0; i < buffSize; i++){
		if (str[i] == '\n')
			return i;
		if (msg[i] != str[i])
			return 0;
	}
	return 0;
}

int getParameter(uint8_t* msg, uint8_t* param){
	int msgPos = 0;
	int paramPos = 0;
	
	//searching until we see a ' ' char
	while ( (msgPos<buffSize) && (msg[msgPos] != ' ') )
	{
		msgPos++;
	}
	
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


int get2Parameter(uint8_t* msg, int &dataSize, uint8_t* param){
	int msgPos = 0;
	int dataCount = 0;
	int paramPos = 0;
	uint8_t str[buffSize];
	
	//searching until we see a ' ' char
	while ( (msgPos<buffSize) && ((msg[msgPos] != ' ') || (msg[msgPos] != '\n')) )
	{
		msgPos++;
	}

	// currently at space between command and data count
	msgPos++;

	sscanf((const char*)&msg[msgPos], "%d %s", &dataCount, str);
	
	//searching until we see a ' ' char
	while ( (msgPos<buffSize) && ((msg[msgPos] != ' ') || (msg[msgPos] != '\n')) )
	{
		msgPos++;
	}

	dataCount = 0;
	while ( (msgPos<buffSize) && (dataCount < dataSize) )
	{
		param[paramPos] = msg[msgPos];
		msgPos++;
		paramPos++;
		dataCount++;
	}

	// currently at '\n', going to return the next character
	if (msgPos < (buffSize - 1) )
		return msgPos + 1;
	else
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

	for (int k = 0; k < buffSize; k++){
		parameter1[k] = 0;
	}

	if (strCompare(msg, "SetLocalName\n")){
		parameterEnd = getParameter(msg, parameter1);
		hal_printf("set local name %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "SetClassOfDevice\n")){
		parameterEnd = getParameter(msg, parameter1);
		hal_printf("set class %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "PINCodeResponse\n")){
		parameterEnd = getParameter(msg, parameter1);
		hal_printf("pin code %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "SetDiscoverabilityMode\n")){
		parameterEnd = getParameter(msg, parameter1);
		hal_printf("disc mode %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "SetConnectabilityMode\n")){
		parameterEnd = getParameter(msg, parameter1);
		hal_printf("con mode %s\r\n", parameter1);
		shiftBuffer(msg, parameterEnd);
		return 1;
	} else if (strCompare(msg, "Inquiry\n")){
		hal_printf("Inquire %d\r\n", comparePos);
		shiftBuffer(msg, comparePos);
		return 1;
	} else if (strCompare(msg, "GetInquiryList\n")){
		hal_printf("inquiry list %d\r\n", comparePos);
		shiftBuffer(msg, comparePos);
		return 1;
	} else if (strCompare(msg, "CloseConnection\n")){
		hal_printf("close conn %d\r\n", comparePos);
		shiftBuffer(msg, comparePos);
		return 1;
	} else if (strCompare(msg, "DataWrite\n")){
		parameterEnd = get2Parameter(msg, dataSize, parameter1);
		hal_printf("data write (%d) %s\r\n", dataSize, parameter1);
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
	while ((sendPos < buffSize) && (CPU_GPIO_GetPinState(COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM) == false)){
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
	/*	hal_printf("pb: ");
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
		}
		//hal_printf("pbp: %d\r\n", parseBuffPosition);
	}
	
/*	hal_printf("pb: ");
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

void CP_SendMsgToCP(uint8_t* msg, int size){
	int buffSize = 128;
	int sendPos = 0;

	MSS_SPI_set_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
	while (sendPos < size){
		MSS_SPI_transfer_block(&g_mss_spi1, &msg[sendPos], 1, 0, 0 );		
		HAL_Time_Sleep_MicroSeconds(COMPUTE_PROCESSOR_COMM_WAIT_TIME);	
		sendPos++;
	}
	MSS_SPI_clear_slave_select( &g_mss_spi1, MSS_SPI_SLAVE_0 );
}

// returns TRUE if successful
bool CP_Init(void){
	hal_printf("bring CP out of reset\r\n");
	//CPU_GPIO_EnableOutputPin(3, FALSE);
	//CPU_GPIO_SetPinState(3, TRUE);

	CPU_GPIO_EnableOutputPin(0, TRUE);
	HAL_Time_Sleep_MicroSeconds(5000);
	CPU_GPIO_SetPinState(0, FALSE);

	hal_printf("Compute processor init\r\n");
	CPU_GPIO_EnableInputPin( COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM, FALSE, CP_WantsTransaction, GPIO_INT_EDGE_LOW, RESISTOR_DISABLED);
	if (CPU_GPIO_GetPinState(COMPUTE_PROCESSOR_DATA_TO_SEND_GPIO_NUM) == false){
		CP_WantsTransaction(NULL, FALSE, NULL);
	}
	for (int i = 0; i < buffSize; i++){
		parseBuffer[i] = 0;
	}

	uint8_t testSend[6];
	testSend[0] = 'a';
	testSend[1] = 'b';
	testSend[2] = 'c';
	testSend[3] = 'd';
	testSend[4] = 'e';
	testSend[5] = '\n';

	CP_SendMsgToCP(&testSend[0], 6);

	return TRUE;
}

bool CP_Present(void){
	return CP_currently_present;
}

bool CP_UnInit(void){
	return TRUE;
}
