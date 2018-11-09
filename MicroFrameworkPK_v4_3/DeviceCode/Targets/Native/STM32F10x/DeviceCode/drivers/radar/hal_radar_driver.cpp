#include <tinyhal.h>
#include <stm32f10x.h>
#include <spi\netmf_spi.h>
#include "hal_radar_driver.h"
#include <TinyCLR_Interop.h>

extern CLR_RT_HeapBlock_NativeEventDispatcher *g_radarContext;
extern UINT64 g_radarUserData;


namespace HAL_RADAR_DRIVER{

BOOL g_Radar_Driver_Initialized = FALSE;
static bool windowOverThreshold = false;
static bool detectionFinished = false;
static bool processingInProgress = false;
static int continueToSendCount = 0;
static int unwrapSigned = 0;
static UINT16 countOverTarget = 0;
static int maxCountOverTarget = 0;
static int displacementFirstHalf = 0;
static int displacementSecondHalf = 0;

static int absoluteDisplFirstHalf = 0;
static int absoluteDisplSecondHalf = 0;
static int absoluteDisplEntire = 0;

static int minDisplacementFirstHalf = 0;
static int minDisplacementSecondHalf = 0;
static int minDisplacementEntire = 0;

static int maxDisplacementFirstHalf = 0;
static int maxDisplacementSecondHalf = 0;
static int maxDisplacementEntire = 0;

static bool alertInterruptActive = false;

static int radarGarbagePurged = 0;
SPI_CONFIGURATION config;

static HAL_CONTINUATION radarHandler_continuation;
static bool interruptServiceInProcess = false;

#define SPIy             SPI1
#define SPIy_CLK         RCC_APB2Periph_SPI1
#define SPIy_GPIO        GPIOA
#define SPIy_GPIO_CLK    RCC_APB2Periph_GPIOA 

#define SPIy_NSS	     GPIO_Pin_11 //nss
#define SPIy_PIN_SCK     GPIO_Pin_5 //sck
#define SPIy_PIN_MISO	 GPIO_Pin_6 //miso
#define SPIy_PIN_MOSI    GPIO_Pin_7 //mosi

enum SAMPLE_WINDOW_PORTION
{
	SAMPLE_WINDOW_FULL,
	SAMPLE_WINDOW_FIRST_HALF,
	SAMPLE_WINDOW_SECOND_HALF
};

UINT16 *g_radarUserBufferChannel1Ptr = NULL;
UINT16 *g_radarUserBufferChannel2Ptr = NULL;
UINT32 g_radarBufferSize = 0;

void detectHandler(GPIO_PIN Pin, BOOL PinState, void* Param){
	if (interruptServiceInProcess == true){
		//hal_printf("#detect\r\n");
		return;
	}
	hal_printf("\r\n detect\r\n");
	// checking to make sure we have enough data to pull
	// this could occur if we are currently pulling data (per continuations) and a detection occurs
	if (CPU_GPIO_GetPinState(33) == FALSE){
		//hal_printf("detection but not enough data\r\n");
		return;
	}
	//Radar_Handler((GPIO_PIN) 33, TRUE, NULL);
	interruptServiceInProcess = true;
	radarHandler_continuation.Enqueue();
}

void dataAlertHandler(GPIO_PIN Pin, BOOL PinState, void* Param){
	if (interruptServiceInProcess == true){
		//hal_printf("#alert\r\n");
		return;
	}
	//hal_printf("alert\r\n");
	if (CPU_GPIO_GetPinState(33) == FALSE){
		hal_printf("alert but not enough data\r\n");
		return;
	}
	//Radar_Handler((GPIO_PIN) 33, TRUE, NULL);
	interruptServiceInProcess = true;
	radarHandler_continuation.Enqueue();
}



void Radar_Handler(void *arg)
{
	UINT16 unwrap = 0;
	FlagStatus status;
	int bytesToRead = 768;
	UINT8 rxData[bytesToRead];

	// if we are already processing data, we need to wait
	if (processingInProgress == true){
		//hal_printf("@\r\n");
		interruptServiceInProcess = false;
		return;
	}

	if (radarGarbagePurged <= 2){
		radarGarbagePurged++;
		// read three garbage bytes
		CPU_GPIO_SetPinState(8, TRUE);
		HAL_Time_Sleep_MicroSeconds(10);
		CPU_SPI_WriteByte(config, 0x5a);
		HAL_Time_Sleep_MicroSeconds(10);
		CPU_SPI_WriteByte(config, 0x5a);
		HAL_Time_Sleep_MicroSeconds(10);
		CPU_SPI_WriteByte(config, 0x5a);
		HAL_Time_Sleep_MicroSeconds(10);
		CPU_GPIO_SetPinState(8, FALSE);
	} else 	if (radarGarbagePurged == 3){
		radarGarbagePurged = 4;
		// disabling data ready interrupt
		CPU_GPIO_DisablePin(33, RESISTOR_DISABLED,  GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
		alertInterruptActive = false;
	}

	// pulling out a block of data
	CPU_GPIO_SetPinState(8, TRUE);
	HAL_Time_Sleep_MicroSeconds(10);
	int i;
	for (i=0;i<bytesToRead;i++){
		CPU_SPI_WriteByte(config, 0x5a);
		status = SPI_I2S_GetFlagStatus(SPIy, SPI_I2S_FLAG_RXNE);
		while (status != SET){
			status = SPI_I2S_GetFlagStatus(SPIy, SPI_I2S_FLAG_RXNE);
		}
		rxData[i] = SPI_I2S_ReceiveData(SPIy);
	}
	CPU_GPIO_SetPinState(8, FALSE);
	int maxDetect = 0;
	int detect = 0;
	int tmpPos;

	//hal_printf("Radar_Handler START Clear variables \r\n ");
	maxCountOverTarget = 0;

	displacementFirstHalf = 0;
	displacementSecondHalf = 0;

	absoluteDisplFirstHalf = 0;
	absoluteDisplSecondHalf = 0;
	absoluteDisplEntire = 0;

	minDisplacementFirstHalf = 0;
	minDisplacementSecondHalf = 0;
	minDisplacementEntire = 0;

	maxDisplacementFirstHalf = 0;
	maxDisplacementSecondHalf = 0;
	maxDisplacementEntire = 0;

	//hal_printf("Radar_Handler START Clear variables: maxDisplacementEntire = %d , maxCountOverTarget = %d, maxDetect = %d \r\n"
	//		, maxDisplacementEntire, maxCountOverTarget, maxDetect);


	for (i=0;i<bytesToRead/6;i++){
		tmpPos = i*6;
		g_radarUserBufferChannel1Ptr[i] = (UINT16)(((UINT16)(rxData[tmpPos+2]) << 4) | (((UINT16)(rxData[tmpPos+1])&0xf0) >> 4));
		g_radarUserBufferChannel2Ptr[i] = (UINT16)((((UINT16)(rxData[(tmpPos)+1])&0x0f) << 8) | ((UINT16)(rxData[(tmpPos)])));
		unwrap = (UINT16)((((UINT16)(rxData[(tmpPos)+5])&0x0f) << 4) | (((UINT16)(rxData[(tmpPos)+4])&0xf0)>>4));
		countOverTarget = (UINT16)((((UINT16)(rxData[(tmpPos)+4])&0x0f) << 8) | ((UINT16)(rxData[(tmpPos)+3])));
		detect = (UINT16)(((UINT16)(rxData[(tmpPos)+5])&0xf0) >> 4);
		if (unwrap & 0x80) {
			unwrapSigned = 0 - (256 - unwrap);
		} else {
			unwrapSigned = unwrap;
		}
		//if (radarGarbagePurged == 4) hal_printf("%d %d %x\r\n",unwrapSigned ,countOverTarget, detect);
		//if (radarGarbagePurged == 4) hal_printf("%d %d %d %d %d %x\r\n", i, g_radarUserBufferChannel1Ptr[i], g_radarUserBufferChannel2Ptr[i],unwrapSigned ,countOverTarget, detect);
		if (detect > maxDetect){
			maxDetect = detect;
		}
		if ((int)countOverTarget > maxCountOverTarget){
			maxCountOverTarget = (int)countOverTarget;

		}
		if (i < bytesToRead/12){
			displacementFirstHalf = unwrapSigned;
			absoluteDisplFirstHalf = abs(unwrapSigned);
			if (unwrapSigned < minDisplacementFirstHalf)
				minDisplacementFirstHalf = unwrapSigned;
			if (unwrapSigned > maxDisplacementFirstHalf)
				maxDisplacementFirstHalf = unwrapSigned;
		}
		if (i >= bytesToRead/12){
			displacementSecondHalf = unwrapSigned;
			absoluteDisplSecondHalf = abs(unwrapSigned);
			if (unwrapSigned < minDisplacementFirstHalf)
				minDisplacementSecondHalf = unwrapSigned;
			if (unwrapSigned > maxDisplacementFirstHalf)
				maxDisplacementSecondHalf = unwrapSigned;
		}
		// calculating data for entire window
		absoluteDisplEntire = abs(unwrapSigned);
		if (unwrapSigned < minDisplacementEntire)
			minDisplacementEntire = unwrapSigned;
		if (unwrapSigned > maxDisplacementEntire){
			maxDisplacementEntire = unwrapSigned;
		}

	}

	//hal_printf("Radar_Handler: maxDisplacementEntire = %d , maxCountOverTarget = %d, maxDetect = %d \r\n"
	//		, maxDisplacementEntire, maxCountOverTarget, maxDetect);

	if (radarGarbagePurged != 4) {
		interruptServiceInProcess = false;
		return;
	}

	UINT32 FPGAIQRejection;
	if ((CPU_GPIO_GetPinState(33) == TRUE) | (continueToSendCount > 0)){
		if (alertInterruptActive == false){
			//hal_printf("enabling data pull; cont: %d detect: %x\r\n", continueToSendCount, maxDetect);
			CPU_GPIO_EnableInputPin(33, FALSE, dataAlertHandler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
			alertInterruptActive = true;
		} else {
			//hal_printf("cont: %d detect: %x\r\n", continueToSendCount, maxDetect);
		}

		// we'll send a few more frames to close out the human detector logic
		if ((maxDetect) != 0) {
			continueToSendCount = 6;
			windowOverThreshold = true;
			detectionFinished = false;
		} else {
			if (continueToSendCount > 0)
				continueToSendCount--;
			windowOverThreshold = false;
			detectionFinished = true;
		}
		//if ((maxDetect != 0) | (continueToSendCount > 0)) {

		//if ((maxDetect & 0x8) != 0) {
		//	hal_printf("--- fpga detection ---\r\n");
		//}

		g_radarUserData = HAL_Time_CurrentTicks();
		processingInProgress = true;
				GLOBAL_LOCK(irq);
		SaveNativeEventToHALQueue( g_radarContext, UINT32(g_radarUserData >> 32), UINT32(g_radarUserData & 0xFFFFFFFF) );
				irq.Release();

		/*for (i=0; i<bytesToRead;i=i+6){
					hal_printf("%03d %02x %02x %02x %02x %02x %02x\r\n", i/6, rxData[i], rxData[i+1], rxData[i+2], rxData[i+3], rxData[i+4], rxData[i+5]);
				}*/	
		/*UINT16 adc1, adc2,median,detection,unwrap;
				static UINT16 markerPrimary = 0xa5a5;
				static UINT16 bogus = 0x2031;
				static UINT16 bogus2 = 0x3339;
				static UINT16 end = 0x0a0d;
				USART_Write( 0, (char *)&markerPrimary, 2 );
				for (i=0;i<bytesToRead;i=i+6){
					adc1 = (UINT16)(((UINT16)(rxData[(i)+2]) << 4) | (((UINT16)(rxData[(i)+1])&0xf0) >> 4));
					adc2 = (UINT16)((((UINT16)(rxData[(i)+1])&0x0f) << 8) | ((UINT16)(rxData[(i)])));
					median = (UINT16)((((UINT16)(rxData[(i)+4])&0x0f) << 8) | ((UINT16)(rxData[(i)+3])));
					detection = (UINT16)(((UINT16)(rxData[(i)+5])&0xf0) >> 4);
					unwrap = (UINT16)((((UINT16)(rxData[(i)+5])&0x0f) << 8) | (((UINT16)(rxData[(i)+4])&0xf0)>>4));

					USART_Write( 0, (char *)&adc1, 2 );
					USART_Write( 0, (char *)&adc2, 2 );
				}

				USART_Write( 0, (char *)&bogus, 2 );
				USART_Write( 0, (char *)&bogus, 2 );
				USART_Write( 0, (char *)&bogus, 2 );
				USART_Write( 0, (char *)&bogus, 2 );
				USART_Write( 0, (char *)&bogus2, 2 );
				USART_Write( 0, (char *)&end, 2 );*/
		//}
	}  else {
		// if there is a current detection or we  are pulling out continuation data the we allow the data alert pulse to call this interrupt
		// we need to exit this interrupt after every block of data to allow user processing time
		//hal_printf("disabling data pull\r\n");
		CPU_GPIO_DisablePin(33, RESISTOR_DISABLED,  GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
		alertInterruptActive = false;
		interruptServiceInProcess = false;
		return;
	}
	interruptServiceInProcess = false;
	return;
}

INT8 FPGA_RadarInit(UINT16 *chan1Ptr, UINT16 *chan2Ptr, UINT32 size){
	INT8 retVal = 0; 
	g_Radar_Driver_Initialized = TRUE;
	radarGarbagePurged = 0;

	g_radarUserBufferChannel1Ptr = chan1Ptr;
	g_radarUserBufferChannel2Ptr = chan2Ptr;
	g_radarBufferSize = size;

	CPU_SPI_Initialize();
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(SPIy_GPIO_CLK, ENABLE);
	//RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE); // SPI1 is NOT APB1
	RCC_APB2PeriphClockCmd(SPIy_CLK, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  SPIy_PIN_SCK |  SPIy_PIN_MOSI;
	GPIO_ConfigurePin(SPIy_GPIO, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

	// SPI MISO pin
	GPIO_InitStructure.GPIO_Pin =  SPIy_PIN_MISO;
	GPIO_ConfigurePin(SPIy_GPIO, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	SPI_StructInit(&SPI_InitStructure);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPIy, &SPI_InitStructure);
	//SPI_SSOutputCmd(SPIy, ENABLE); // This is only for "Hard" NSS.
	SPI_Cmd(SPIy, ENABLE);

	config.SPI_mod				 = SPIBUS1;
	// data alert
	CPU_GPIO_EnableInputPin(33, FALSE, dataAlertHandler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	alertInterruptActive = true;
	// detection
	CPU_GPIO_EnableInputPin(0, FALSE, detectHandler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	// setup chip select pin
	CPU_GPIO_EnableOutputPin(8,FALSE);
	// toggle reset line to FPGA
	CPU_GPIO_EnableOutputPin(32,FALSE);
	HAL_Time_Sleep_MicroSeconds(300000);
	CPU_GPIO_SetPinState(32, TRUE);
	interruptServiceInProcess = false;
	radarHandler_continuation.InitializeCallback(Radar_Handler, NULL);
	hal_printf("radar initialized\r\n");

	return 0;
}	

INT8 getWindowOverThreshold(){
	//hal_printf( "getWindowOverThreshold() = %d \r\n", windowOverThreshold );
	return windowOverThreshold;
}

INT8 getDetectionFinished(){
	return detectionFinished;
}

INT32 getNetDisplacement(INT32 portion){
	if (portion == SAMPLE_WINDOW_FULL){
		return (unwrapSigned);
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return (displacementFirstHalf);
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return (displacementSecondHalf);
	}
}

INT32 getAbsoluteDisplacement(INT32 portion){
	if (portion == SAMPLE_WINDOW_FULL){
		if (abs(minDisplacementEntire) > maxDisplacementEntire)
			return abs(maxDisplacementEntire);
		else
			return maxDisplacementEntire;
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		if (abs(minDisplacementFirstHalf) > maxDisplacementFirstHalf)
			return abs(minDisplacementFirstHalf);
		else
			return maxDisplacementFirstHalf;
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		if (abs(minDisplacementSecondHalf) > maxDisplacementSecondHalf)
			return abs(minDisplacementSecondHalf);
		else
			return maxDisplacementSecondHalf;
	}
}

INT32 getDisplacementRange(INT32 portion){
	if (portion == SAMPLE_WINDOW_FULL){
		return (maxDisplacementEntire + abs(minDisplacementEntire));
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return (maxDisplacementFirstHalf + abs(minDisplacementFirstHalf));
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return (maxDisplacementSecondHalf + abs(minDisplacementSecondHalf));
	}
}

INT32 getCountOverTarget(){
	//hal_printf( "hal_radar_driver.cpp getCountOverTarget() = %d \r\n", maxCountOverTarget );
	return maxCountOverTarget;
}

void setProcessingInProgress(int state){
	processingInProgress = state;
	//hal_printf("PiP set to %d\r\n",processingInProgress);
	if ((state == FALSE) & (CPU_GPIO_GetPinState(33) == TRUE)){
		//hal_printf("calling data pull\r\n");
		Radar_Handler(NULL);
	}
}
}
