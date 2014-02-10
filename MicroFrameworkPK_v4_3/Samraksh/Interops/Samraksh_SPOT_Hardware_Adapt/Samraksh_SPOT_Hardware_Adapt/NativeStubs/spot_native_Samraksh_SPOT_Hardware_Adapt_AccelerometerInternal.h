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


#ifndef _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ACCELEROMETERINTERNAL_H_
#define _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ACCELEROMETERINTERNAL_H_

namespace Samraksh_SPOT_Hardware_Adapt
{
    struct AccelerometerInternal
    {
        // Helper Functions to access fields of managed object
        // Declaration of stubs. These functions are implemented by Interop code developers
        static INT16 ADAPT_Accel_Init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT16 ADAPT_Accel_GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT16 ADAPT_Accel_GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT16 ADAPT_Accel_GetZ( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT16 ADAPT_Accel_GetTemperature( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT16 ADAPT_Accel_reset( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT16 ADAPT_Accel_advconfig( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
        static INT16 ADAPT_Accel_SelfTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        static INT16 ADAPT_Accel_SetPowerMode( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
        static INT16 ADAPT_Accel_SetBandwidth( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
        static INT16 ADAPT_Accel_SetRange( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr );
        static UINT16 ADAPT_Accel_Raw_Read( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr );
        static INT16 ADAPT_Accel_Raw_Write( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT16 param1, HRESULT &hr );
    };
}
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ACCELEROMETERINTERNAL_H_
