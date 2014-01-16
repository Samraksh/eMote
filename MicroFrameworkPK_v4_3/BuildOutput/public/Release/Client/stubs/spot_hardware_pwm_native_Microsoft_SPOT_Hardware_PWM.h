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


#ifndef _SPOT_HARDWARE_PWM_NATIVE_MICROSOFT_SPOT_HARDWARE_PWM_H_
#define _SPOT_HARDWARE_PWM_NATIVE_MICROSOFT_SPOT_HARDWARE_PWM_H_

namespace Microsoft
{
    namespace SPOT
    {
        namespace Hardware
        {
            struct PWM
            {
                // Helper Functions to access fields of managed object
                static UNSUPPORTED_TYPE& Get_m_pin( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_spot_hardware_pwm_native_Microsoft_SPOT_Hardware_PWM::FIELD__m_pin ); }

                static void& Get_m_channel( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_void( pMngObj, Library_spot_hardware_pwm_native_Microsoft_SPOT_Hardware_PWM::FIELD__m_channel ); }

                static UINT32& Get_m_period( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_spot_hardware_pwm_native_Microsoft_SPOT_Hardware_PWM::FIELD__m_period ); }

                static UINT32& Get_m_duration( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_spot_hardware_pwm_native_Microsoft_SPOT_Hardware_PWM::FIELD__m_duration ); }

                static INT8& Get_m_invert( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_spot_hardware_pwm_native_Microsoft_SPOT_Hardware_PWM::FIELD__m_invert ); }

                static UINT32& Get_m_scale( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_spot_hardware_pwm_native_Microsoft_SPOT_Hardware_PWM::FIELD__m_scale ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static void Start( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static void Stop( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static void Commit( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static void Init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static void Uninit( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static void Start( CLR_RT_TypedArray_UNSUPPORTED_TYPE param0, HRESULT &hr );
                static void Stop( CLR_RT_TypedArray_UNSUPPORTED_TYPE param0, HRESULT &hr );
            };
        }
    }
}
#endif  //_SPOT_HARDWARE_PWM_NATIVE_MICROSOFT_SPOT_HARDWARE_PWM_H_
