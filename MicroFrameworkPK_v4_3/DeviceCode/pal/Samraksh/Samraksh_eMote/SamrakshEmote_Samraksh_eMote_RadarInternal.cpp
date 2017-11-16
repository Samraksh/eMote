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

UINT16 *g_radarUserBufferChannel1Ptr = NULL;
UINT16 *g_radarUserBufferChannel2Ptr = NULL;
UINT32 g_radarBufferSize = 0;

void Radar_Handler(GPIO_PIN Pin, BOOL PinState, void* Param)
	{
		FlagStatus status;
		int bytesToRead = 768;
		UINT8 rxData[bytesToRead];

		if (radarGarbagePurged == 0){
			radarGarbagePurged = 1;			
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
		} else 	if (radarGarbagePurged == 1){
			radarGarbagePurged = 2;			
			// read two garbage bytes
			CPU_GPIO_SetPinState(25, TRUE);
			HAL_Time_Sleep_MicroSeconds(10);
			CPU_SPI_WriteByte(config, 0x5a);
			HAL_Time_Sleep_MicroSeconds(10);
			CPU_SPI_WriteByte(config, 0x5a);
			HAL_Time_Sleep_MicroSeconds(10);
			CPU_GPIO_SetPinState(25, FALSE);
		}

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
		int tmpPos;
		for (i=0;i<bytesToRead/6;i++){
			tmpPos = i*6;
			g_radarUserBufferChannel1Ptr[i] = (UINT16)(((UINT16)(rxData[tmpPos+2]) << 4) | (((UINT16)(rxData[tmpPos+1])&0xf0) >> 4));
			g_radarUserBufferChannel2Ptr[i] = (UINT16)((((UINT16)(rxData[(tmpPos)+1])&0x0f) << 8) | ((UINT16)(rxData[(tmpPos)])));

			// debug count
			//g_radarUserBufferChannel2Ptr[i] = (UINT16)(((UINT16)((rxData[tmpPos+4]) << 8 | rxData[tmpPos+3])));
		}
		if ((rxData[5] & 0x80) != 0) {
			hal_printf("--- fpga detection ---\r\n");
		}
		/*for (i=0; i<bytesToRead;i=i+6){
			hal_printf("%03d %03x %03x %03x %03x %03x %03x\r\n", i/6, rxData[i], rxData[i+1], rxData[i+2], rxData[i+3], rxData[i+4], rxData[i+5]);
		}*/

		GLOBAL_LOCK(irq);

		g_radarUserData = HAL_Time_CurrentTicks();

		SaveNativeEventToHALQueue( g_radarContext, UINT32(g_radarUserData >> 32), UINT32(g_radarUserData & 0xFFFFFFFF) );
	}

INT8 RadarInternal::ConfigureFPGADetectionPrivate( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, UINT32 param2, HRESULT &hr )
{
    INT8 retVal = 0; 
	g_Radar_Driver_Initialized = TRUE;
	radarGarbagePurged = false;

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

	CPU_GPIO_EnableInputPin(0, FALSE, Radar_Handler, GPIO_INT_EDGE_HIGH, RESISTOR_DISABLED);
	// setup chip select pin
	CPU_GPIO_EnableOutputPin(25,FALSE);
	// toggle reset line to FPGA
	CPU_GPIO_EnableOutputPin(24,FALSE);
	HAL_Time_Sleep_MicroSeconds(300000);
	CPU_GPIO_SetPinState(24, TRUE);
	
    return retVal;
}

INT32 RadarInternal::Init( INT32 param0, HRESULT &hr )
{
    INT32 retVal = 0; 
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

