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


#include "SamrakshEmoteAdapt.h"
#include "SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Accelerometer.h"
#include "..\Krait_Accelerometer\Krait_Accelerometer.h"

using namespace Samraksh::eMote::Adapt;

CLR_RT_HeapBlock_NativeEventDispatcher *g_adcContext = NULL;
static UINT64 g_adcUserData = 0;
static BOOL g_adcInterruptEnabled = TRUE;

INT8 Accelerometer::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	INT8 retVal = ADAPT_Accel_Init();
	return retVal;
}

INT8 Accelerometer::Reset( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	INT8 retVal = ADAPT_Accel_Reset();
	return retVal;
}

INT8 Accelerometer::SelfTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = ADAPT_Accel_SelfTest(); 
    return retVal;
}

INT16 Accelerometer::GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	INT16 retVal = ADAPT_Accel_GetX();
	return retVal;
}

INT16 Accelerometer::GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	INT16 retVal = ADAPT_Accel_GetY();
	return retVal;
}

INT16 Accelerometer::GetZ( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	INT16 retVal = ADAPT_Accel_GetZ();
	return retVal;
}

INT16 Accelerometer::GetTemperature( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	INT16 retVal = ADAPT_Accel_GetTemperature();
	return retVal;
}

UINT8 Accelerometer::ReadRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr )
{
    UINT8 retVal = ADAPT_Accel_Raw_Read(param0); 
    return retVal;
}

INT8 Accelerometer::WriteRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, HRESULT &hr )
{
    INT8 retVal = ADAPT_Accel_Raw_Write(param0, param1); 
    return retVal;
}

INT8 Accelerometer::AdvancedConfiguration( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = ADAPT_Accel_advconfig(param0); 
    return retVal;
}

INT8 Accelerometer::SetPowerMode( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = ADAPT_Accel_SetPowerMode(param0); 
    return retVal;
}

INT8 Accelerometer::SetBandwidth( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = ADAPT_Accel_SetBandwidth(param0); 
    return retVal;
}

INT8 Accelerometer::SetRange( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr )
{
    INT8 retVal = ADAPT_Accel_SetRange(param0); 
    return retVal;
}

INT8 Accelerometer::GetAllData( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 param0, HRESULT &hr )
{
    INT8 retVal = ADAPT_Accel_GetAll(param0.GetBuffer()); 
    return retVal;
}

static HRESULT InitializeADCDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_adcContext  = pContext;
   g_adcUserData = userData;
   return S_OK;
}

static HRESULT EnableDisableADCDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
   g_adcInterruptEnabled = fEnable;
   return S_OK;
}

static HRESULT CleanupADCDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    g_adcContext = NULL;
    g_adcUserData = 0;
    CleanupNativeEventsFromHALQueue( pContext );
    return S_OK;
}

static const CLR_RT_DriverInterruptMethods g_AdcInteropDriverMethods =
{
  InitializeADCDriver,
  EnableDisableADCDriver,
  CleanupADCDriver
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_ADC =
{
    "ADCCallback",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_AdcInteropDriverMethods
};
