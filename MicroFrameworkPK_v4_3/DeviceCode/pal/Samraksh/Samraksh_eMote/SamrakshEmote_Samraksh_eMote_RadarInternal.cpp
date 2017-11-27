//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmote.h"
#include "SamrakshEmote_Samraksh_eMote_RadarInternal.h"

#include <tinyhal.h>
#include <stm32f10x.h>
#include <spi\netmf_spi.h>

CLR_RT_HeapBlock_NativeEventDispatcher *g_radarContext = NULL;
static UINT64 g_radarUserData = 0;
static BOOL g_radarInterruptEnabled = TRUE;
static BOOL g_Radar_Driver_Initialized = FALSE;
static bool windowOverThreshold = false;
static bool detectionFinished = false;
static bool processingInProgress = false;
static int continueToSendCount = 0;
static int unwrapSigned = 0;
static UINT16 countOverTarget = 0;
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

using namespace Samraksh::eMote;

static int radarGarbagePurged = 0;
SPI_CONFIGURATION config;

#define SPIy             SPI2
#define SPIy_CLK         RCC_APB1Periph_SPI2
#define SPIy_GPIO        GPIOB
#define SPIy_GPIO_CLK    RCC_APB2Periph_GPIOB 

#define SPIy_NSS	     GPIO_Pin_12 //nss
#define SPIy_PIN_SCK     GPIO_Pin_13 //sck
#define SPIy_PIN_MISO	 GPIO_Pin_14 //miso
#define SPIy_PIN_MOSI    GPIO_Pin_15 //mosi

enum SAMPLE_WINDOW_PORTION
    {
        SAMPLE_WINDOW_FULL,
        SAMPLE_WINDOW_FIRST_HALF,
        SAMPLE_WINDOW_SECOND_HALF
    };

UINT16 *g_radarUserBufferChannel1Ptr = NULL;
UINT16 *g_radarUserBufferChannel2Ptr = NULL;
UINT32 g_radarBufferSize = 0;

void Radar_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
	{
		UINT16 unwrap = 0;
		FlagStatus status;
		int bytesToRead = 768;
		UINT8 rxData[bytesToRead];

		// checking to make sure we have enough data to pull
		// this could occur if we are currently pulling data (per continuations) and a detection occurs
		if (CPU_GPIO_GetPinState(0) == FALSE){
			hal_printf("detection but not enough data\r\n");
			return;
		}

		// if we are already processing data, we need to wait
		if (processingInProgress == true){
			return;
		}

		GLOBAL_LOCK(irq);
		if (radarGarbagePurged <= 2){
			radarGarbagePurged++;
			// read three garbage bytes
			CPU_GPIO_SetPinState(25, TRUE);
			HAL_Time_Sleep_MicroSeconds(10);
			CPU_SPI_WriteByte(config, 0x5a);
			HAL_Time_Sleep_MicroSeconds(10);
			CPU_SPI_WriteByte(config, 0x5a);
			HAL_Time_Sleep_MicroSeconds(10);
			CPU_SPI_WriteByte(config, 0x5a);
			HAL_Time_Sleep_MicroSeconds(10);
			CPU_GPIO_SetPinState(25, FALSE);
		} else 	if (radarGarbagePurged == 3){
			radarGarbagePurged = 4;			
			// disabling data ready interrupt
			CPU_GPIO_DisablePin(0, RESISTOR_DISABLED,  GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
		}

		// pulling out a block of data
		CPU_GPIO_SetPinState(25, TRUE);
		HAL_Time_Sleep_MicroSeconds(10);
		int i;
		for (i=0;i<bytesToRead;i++){
			CPU_SPI_WriteByte(config, 0x5a);
			status = SPI_I2S_GetFlagStatus(SPIy, SPI_I2S_FLAG_RXNE);
			while (status != SET){
				status = SPI_I2S_GetFlagStatus(SPIy, SPI_I2S_FLAG_RXNE);
			}
			rxData[i] = SPI_I2S_ReceiveData(SPI2);
		}
		CPU_GPIO_SetPinState(25, FALSE);

		int maxDetect = 0;
		int detect = 0;
		int tmpPos;
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

		for (i=0;i<bytesToRead/6;i++){
			tmpPos = i*6;
			g_radarUserBufferChannel1Ptr[i] = (UINT16)(((UINT16)(rxData[tmpPos+2]) << 4) | (((UINT16)(rxData[tmpPos+1])&0xf0) >> 4));
			g_radarUserBufferChannel2Ptr[i] = (UINT16)((((UINT16)(rxData[(tmpPos)+1])&0x0f) << 8) | ((UINT16)(rxData[(tmpPos)])));
			unwrap = (UINT16)((((UINT16)(rxData[(tmpPos)+5])&0x0f) << 4) | (((UINT16)(rxData[(tmpPos)+4])&0xf0)>>4));
			countOverTarget = (UINT16)((((UINT16)(rxData[(tmpPos)+4])&0x0f) << 8) | ((UINT16)(rxData[(tmpPos)+3])));
			detect = (UINT16)(((UINT16)(rxData[(tmpPos)+5])&0xf0) >> 4);
hal_printf("%d %d %x\r\n",i,countOverTarget, detect);
			if (unwrap & 0x80) {
				unwrapSigned = 0 - (256 - unwrap);
			} else {
				unwrapSigned = unwrap;
			}
			if (detect > maxDetect){
				maxDetect = detect;
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
			if (unwrapSigned > maxDisplacementEntire)
				maxDisplacementEntire = unwrapSigned;
		}

		if (radarGarbagePurged != 4) return;

		UINT32 FPGAIQRejection;
		if ((CPU_GPIO_GetPinState(1) == TRUE) | (continueToSendCount > 0)){		
			hal_printf("enabling data pull; cont: %d detect: %x\r\n", continueToSendCount, maxDetect);
			CPU_GPIO_EnableInputPin(0, FALSE, Radar_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);

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
			if ((maxDetect != 0) | (continueToSendCount > 0)) {
					
				if ((maxDetect & 0x8) != 0) {
					hal_printf("--- fpga detection ---\r\n");
				}
				
				g_radarUserData = HAL_Time_CurrentTicks();
				processingInProgress = true;
				SaveNativeEventToHALQueue( g_radarContext, UINT32(g_radarUserData >> 32), UINT32(g_radarUserData & 0xFFFFFFFF) );

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
			}
		}  else {
			// if there is a current detection or we  are pulling out continuation data the we allow the data alert pulse to call this interrupt
			// we need to exit this interrupt after every block of data to allow user processing time
			hal_printf("disabling data pull\r\n");
			CPU_GPIO_DisablePin(0, RESISTOR_DISABLED,  GPIO_Mode_IN_FLOATING, GPIO_ALT_PRIMARY);
			return;
		}
		

		
	}

INT8 RadarInternal::ConfigureFPGADetectionPrivate( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, UINT32 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
	g_Radar_Driver_Initialized = TRUE;
	radarGarbagePurged = 0;

	g_radarUserBufferChannel1Ptr = param0.GetBuffer();
	g_radarUserBufferChannel2Ptr = param1.GetBuffer();
	g_radarBufferSize = param2;

	CPU_SPI_Initialize();
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(SPIy_GPIO_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(SPIy_CLK, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  SPIy_PIN_SCK | SPIy_NSS | SPIy_PIN_MOSI;
	GPIO_ConfigurePin(SPIy_GPIO, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_AF_PP, GPIO_Speed_50MHz);

	// SPI MISO pin
	GPIO_InitStructure.GPIO_Pin =  SPIy_PIN_MISO;
	GPIO_ConfigurePin(GPIOB, GPIO_InitStructure.GPIO_Pin, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

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
	SPI_SSOutputCmd(SPIy, ENABLE);
	SPI_Cmd(SPIy, ENABLE);

	config.SPI_mod				 = SPIBUS2;

	// data alert
	CPU_GPIO_EnableInputPin(0, FALSE, Radar_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	// detection
	CPU_GPIO_EnableInputPin(1, FALSE, Radar_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	// setup chip select pin
	CPU_GPIO_EnableOutputPin(25,FALSE);
	// toggle reset line to FPGA
	CPU_GPIO_EnableOutputPin(24,FALSE);
	HAL_Time_Sleep_MicroSeconds(300000);
	CPU_GPIO_SetPinState(24, TRUE);
	
    return retVal;
}

INT8 RadarInternal::GetWindowOverThreshold( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return windowOverThreshold;
}

INT8 RadarInternal::CurrentDetectionFinished( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return detectionFinished;
}

INT32 RadarInternal::GetNetDisplacement( CLR_RT_HeapBlock* pMngObj, INT32 portion, HRESULT &hr )
{
    if (portion == SAMPLE_WINDOW_FULL){
		return (unwrapSigned);
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return (displacementFirstHalf);
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return (displacementSecondHalf);
	}
}

INT32 RadarInternal::GetAbsoluteDisplacement( CLR_RT_HeapBlock* pMngObj, INT32 portion, HRESULT &hr )
{
    if (portion == SAMPLE_WINDOW_FULL){
		return absoluteDisplEntire;
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return absoluteDisplFirstHalf;
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return absoluteDisplSecondHalf;
	}
}

INT32 RadarInternal::GetDisplacementRange( CLR_RT_HeapBlock* pMngObj, INT32 portion, HRESULT &hr )
{
    if (portion == SAMPLE_WINDOW_FULL){
		return (maxDisplacementEntire - minDisplacementEntire);
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return (maxDisplacementFirstHalf - minDisplacementFirstHalf);
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return (maxDisplacementSecondHalf - minDisplacementSecondHalf);
	}
}

INT32 RadarInternal::GetCountOverTarget( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return countOverTarget;
}

void RadarInternal::SetProcessingInProgress( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr )
{
	processingInProgress = param0;
	hal_printf("PiP set to %d\r\n",processingInProgress);
	if ((param0 == FALSE) & (CPU_GPIO_GetPinState(0) == TRUE)){
		hal_printf("calling data pull\r\n");
		Radar_Handler((GPIO_PIN) 0, TRUE, NULL);
	}
}

INT32 RadarInternal::Init( INT32 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
	g_Radar_Driver_Initialized = TRUE;
    return retVal;
}

INT8 RadarInternal::Uninit( HRESULT &hr )
{
    INT8 retVal = 0; 
	g_Radar_Driver_Initialized = FALSE;
    return retVal;
}

static HRESULT InitializeRadarDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_radarContext  = pContext;
   g_radarUserData = userData;
   return S_OK;
}

static HRESULT EnableDisableRadarDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
   g_radarInterruptEnabled = fEnable;
   return S_OK;
}

static HRESULT CleanuRadarDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
	if (g_Radar_Driver_Initialized == FALSE){
    	g_radarContext = NULL;
    	g_radarUserData = 0;
		//Radar_Uninitialize();
    	CleanupNativeEventsFromHALQueue( pContext );
	}
    return S_OK;
}


static const CLR_RT_DriverInterruptMethods g_RadarInteropDriverMethods =
{
  InitializeRadarDriver,
  EnableDisableRadarDriver,
  CleanuRadarDriver
};


const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Radar =
{
    "RadarCallback",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_RadarInteropDriverMethods
};

