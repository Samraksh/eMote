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
#include <radar\hal_radar_driver.h>

using namespace Samraksh::eMote;

CLR_RT_HeapBlock_NativeEventDispatcher *g_radarContext = NULL;
UINT64 g_radarUserData = 0;
extern BOOL g_Radar_Driver_Initialized = FALSE;
static BOOL g_radarInterruptEnabled = TRUE;

INT8 RadarInternal::ConfigureFPGADetectionPrivate( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, UINT32 param2, HRESULT &hr )
{
	INT8 retVal = FPGA_RadarInit(param0.GetBuffer(), param1.GetBuffer(), param2);
    return retVal;
}

INT8 RadarInternal::GetWindowOverThreshold( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return getWindowOverThreshold();
}

INT8 RadarInternal::CurrentDetectionFinished( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return getDetectionFinished();
}

INT32 RadarInternal::GetNetDisplacement( CLR_RT_HeapBlock* pMngObj, INT32 portion, HRESULT &hr )
{
    return getNetDisplacement(portion);
}

INT32 RadarInternal::GetAbsoluteDisplacement( CLR_RT_HeapBlock* pMngObj, INT32 portion, HRESULT &hr )
{
    return getAbsoluteDisplacement(portion);	
}

INT32 RadarInternal::GetDisplacementRange( CLR_RT_HeapBlock* pMngObj, INT32 portion, HRESULT &hr )
{
    return getDisplacementRange(portion);	
}

INT32 RadarInternal::GetCountOverTarget( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return getCountOverTarget();
}

void RadarInternal::SetProcessingInProgress( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr )
{
	setProcessingInProgress(param0);
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


