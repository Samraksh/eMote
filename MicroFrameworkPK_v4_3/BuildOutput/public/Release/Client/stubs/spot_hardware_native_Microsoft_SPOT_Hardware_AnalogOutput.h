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


#ifndef _SPOT_HARDWARE_NATIVE_MICROSOFT_SPOT_HARDWARE_ANALOGOUTPUT_H_
#define _SPOT_HARDWARE_NATIVE_MICROSOFT_SPOT_HARDWARE_ANALOGOUTPUT_H_

namespace Microsoft
{
    namespace SPOT
    {
        namespace Hardware
        {
            struct AnalogOutput
            {
                // Helper Functions to access fields of managed object
                static INT32& Get_m_pin( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_native_Microsoft_SPOT_Hardware_AnalogOutput::FIELD__m_pin ); }

                static INT32& Get_m_channel( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_native_Microsoft_SPOT_Hardware_AnalogOutput::FIELD__m_channel ); }

                static double& Get_m_scale( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_double( pMngObj, Library_spot_hardware_native_Microsoft_SPOT_Hardware_AnalogOutput::FIELD__m_scale ); }

                static double& Get_m_offset( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_double( pMngObj, Library_spot_hardware_native_Microsoft_SPOT_Hardware_AnalogOutput::FIELD__m_offset ); }

                static INT32& Get_m_precision( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_spot_hardware_native_Microsoft_SPOT_Hardware_AnalogOutput::FIELD__m_precision ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static void WriteRaw( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                static void Initialize( INT32 param0, INT32 param1, HRESULT &hr );
                static void Uninitialize( INT32 param0, HRESULT &hr );
            };
        }
    }
}
#endif  //_SPOT_HARDWARE_NATIVE_MICROSOFT_SPOT_HARDWARE_ANALOGOUTPUT_H_
