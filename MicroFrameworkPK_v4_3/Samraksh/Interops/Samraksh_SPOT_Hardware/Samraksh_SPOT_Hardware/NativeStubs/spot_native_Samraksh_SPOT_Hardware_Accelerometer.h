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


#ifndef _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ACCELEROMETER_H_
#define _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ACCELEROMETER_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace Hardware
        {
            struct Accelerometer
            {
                // Helper Functions to access fields of managed object
                static UINT8& Get_currentRange( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT8( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer::FIELD__currentRange ); }

                static UINT8& Get_currentResolution( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT8( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer::FIELD__currentResolution ); }

                static UNSUPPORTED_TYPE& Get_sensorData( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer::FIELD__sensorData ); }

                static INT32& Get_refreshRate( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer::FIELD__refreshRate ); }

                static UNSUPPORTED_TYPE& Get_sensorSamplingTimer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer::FIELD__sensorSamplingTimer ); }

                static UNSUPPORTED_TYPE& Get_userCallbackFn( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer::FIELD__userCallbackFn ); }

                static UNSUPPORTED_TYPE& Get_accelerometerDataBuffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_spot_native_Samraksh_SPOT_Hardware_Accelerometer::FIELD__accelerometerDataBuffer ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT8 Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 Reset( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 SelfTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetZ( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT16 GetTemperature( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 ReadRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT8 WriteRegister( CLR_RT_HeapBlock* pMngObj, UINT8 param0, UINT8 param1, HRESULT &hr );
                static INT8 AdvancedConfig( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 SetPowerMode( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 SetBandwidth( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
                static INT8 SetRange( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT8 GetAllData( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 param0, HRESULT &hr );
            };
        }
    }
}
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ACCELEROMETER_H_
