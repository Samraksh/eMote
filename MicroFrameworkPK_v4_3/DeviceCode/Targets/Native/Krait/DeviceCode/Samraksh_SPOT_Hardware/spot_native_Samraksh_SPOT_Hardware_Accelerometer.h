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

//struct qup_i2c_dev *adapt_i2c_GSBI12;

#define BMA250_ADDR (0x18)

namespace Samraksh
{
    namespace SPOT
    {
        namespace Hardware
        {
            struct Accelerometer
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT8 ADAPT_Accel_Init( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 ADAPT_Accel_GetX( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 ADAPT_Accel_GetY( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            };
        }
    }
}
#endif  //_SPOT_NATIVE_SAMRAKSH_SPOT_HARDWARE_ACCELEROMETER_H_
