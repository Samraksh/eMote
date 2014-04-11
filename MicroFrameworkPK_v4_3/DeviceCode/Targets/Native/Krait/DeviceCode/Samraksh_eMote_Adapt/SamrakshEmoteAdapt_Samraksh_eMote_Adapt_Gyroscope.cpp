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
#include "SamrakshEmoteAdapt_Samraksh_eMote_Adapt_Gyroscope.h"
#include "..\Krait_Gyroscope\Krait_Gyroscope.h"

using namespace Samraksh::eMote::Adapt;

INT8 Gyroscope::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = gyro_init(); 
    return retVal;
}

INT8 Gyroscope::Reset( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = gyro_reset(); 
    return retVal;
}

INT8 Gyroscope::SelfTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = gyro_self_test(); 
    return retVal;
}

INT16 Gyroscope::GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal = gyro_get_X(); 
    return retVal;
}

INT16 Gyroscope::GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal = gyro_get_Y(); 
    return retVal;
}

INT16 Gyroscope::GetZ( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal = gyro_get_Z(); 
    return retVal;
}

INT16 Gyroscope::GetTemperature( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT16 retVal = gyro_get_temp(); 
    return retVal;
}

UINT8 Gyroscope::ReadRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr )
{
    UINT8 retVal = gyro_raw_read(param0); 
    return retVal;
}

INT8 Gyroscope::WriteRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, HRESULT &hr )
{
    INT8 retVal = gyro_raw_write(param0, param1); 
    return retVal;
}

INT8 Gyroscope::AdvancedConfiguration( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = gyro_config(param0); 
    return retVal;
}

INT8 Gyroscope::SetPowerMode( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    UINT8 retVal = gyro_sample_config(param0); 
    return retVal;
}

INT8 Gyroscope::SetOffsets( CLR_RT_HeapBlock* pMngObj, INT16 param0, INT16 param1, INT16 param2, HRESULT &hr )
{
    INT8 retVal = gyro_offsets(param0, param1, param2); 
    return retVal;
}

INT8 Gyroscope::SampleConfiguration( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    INT8 retVal = gyro_power(param0 ); 
    return retVal;
}

INT8 Gyroscope::GetAllData( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 param0, HRESULT &hr )
{
    INT8 retVal = gyro_get_XYZ(param0.GetBuffer()); 
    return retVal;
}

