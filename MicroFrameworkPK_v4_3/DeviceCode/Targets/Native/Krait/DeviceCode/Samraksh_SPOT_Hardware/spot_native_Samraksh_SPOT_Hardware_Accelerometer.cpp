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


#include "spot_native.h"
#include "spot_native_Samraksh_SPOT_Hardware_Accelerometer.h"
#include "..\Krait_Accelerometer\Krait_Accelerometer.h"

using namespace Samraksh::SPOT::Hardware;

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

INT8 Accelerometer::AdvancedConfig( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
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

