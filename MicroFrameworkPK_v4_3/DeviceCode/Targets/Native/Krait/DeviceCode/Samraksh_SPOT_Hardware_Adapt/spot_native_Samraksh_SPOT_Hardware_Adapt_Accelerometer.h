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


#ifndef _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ACCELEROMETER_H_
#define _SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ACCELEROMETER_H_

//struct qup_i2c_dev *adapt_i2c_GSBI12;

#define BMA250_ADDR (0x18)

namespace Samraksh
{
    namespace SPOT
    {
        namespace Hardware
        {
            namespace Adapt
            {
                struct Accelerometer
                {
                    // Helper Functions to access fields of managed object
                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT8 ADAPT_Accel_Init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 ADAPT_Accel_Reset( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 ADAPT_Accel_SelfTest( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT16 ADAPT_Accel_GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT16 ADAPT_Accel_GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT16 ADAPT_Accel_GetZ( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT16 ADAPT_Accel_GetTemperature( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static UINT8 ADAPT_Accel_Raw_Read( CLR_RT_HeapBlock* pMngObj, UINT8 reg, HRESULT &hr );
                    static INT8 ADAPT_Accel_Raw_Write( CLR_RT_HeapBlock* pMngObj, UINT8 reg, UINT8 data, HRESULT &hr );
                    static INT8 ADAPT_Accel_advconfig( CLR_RT_HeapBlock* pMngObj, UINT32 config, HRESULT &hr );
                    static INT8 ADAPT_Accel_SetPowerMode( CLR_RT_HeapBlock* pMngObj, UINT32 mode, HRESULT &hr );
                    static INT8 ADAPT_Accel_SetBandwidth( CLR_RT_HeapBlock* pMngObj, UINT32 bandwidth, HRESULT &hr );
                    static INT8 ADAPT_Accel_SetRange( CLR_RT_HeapBlock* pMngObj, UINT8 range, HRESULT &hr );
                    static INT8 ADAPT_Accel_GetAll( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 data, HRESULT &hr );
                };
            }
        }
    }
}
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ADAPT_ACCELEROMETER_H_
