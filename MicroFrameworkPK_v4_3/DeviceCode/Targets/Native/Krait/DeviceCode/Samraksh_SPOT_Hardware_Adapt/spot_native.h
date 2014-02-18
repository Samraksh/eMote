//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#ifndef _SPOT_NATIVE_H_
#define _SPOT_NATIVE_H_

#include <TinyCLR_Interop.h>
struct Library_spot_native_Samraksh_SPOT_Hardware_Adapt_Accelerometer
{
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_Init___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_Reset___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_SelfTest___BOOLEAN);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_GetX___I2);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_GetY___I2);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_GetZ___I2);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_GetTemperature___I2);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_Raw_Read___U1__U1);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_Raw_Write___BOOLEAN__U1__U1);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_advconfig___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_SetPowerMode___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_SetBandwidth___BOOLEAN__U4);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_SetRange___BOOLEAN__U1);
    TINYCLR_NATIVE_DECLARE(ADAPT_Accel_GetAll___BOOLEAN__SZARRAY_I2);

    //--//

};



extern const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_SPOT_Hardware_Adapt;

#endif  //_SPOT_NATIVE_H_
