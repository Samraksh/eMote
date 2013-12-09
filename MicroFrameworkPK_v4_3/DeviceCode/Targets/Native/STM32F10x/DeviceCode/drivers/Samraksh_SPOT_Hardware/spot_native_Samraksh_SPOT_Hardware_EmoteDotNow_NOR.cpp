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
#include "spot_native_Samraksh_SPOT_Hardware_EmoteDotNow_NOR.h"

#include "..\fsmc\P30BF65NOR\P30BF65NOR.h"

extern P30BF65NOR_Driver gNORDriver;

using namespace Samraksh::SPOT::Hardware::EmoteDotNow;

INT8 NOR::InternalInitialize( HRESULT &hr )
{
   return gNORDriver.Initialize();
}

INT32 NOR::InternalWrite( CLR_RT_TypedArray_UINT16 param0, UINT32 param1, UINT16 param2, HRESULT &hr )
{
	if(gNORDriver.ReadHalfWord(param1) != 0xffff)
		gNORDriver.EraseBlock(param1);

    return gNORDriver.WriteBuffer(param0.GetBuffer(), param1, param2);
}

INT32 NOR::InternalWrite( CLR_RT_TypedArray_UINT16 param0, UINT32 param1, UINT16 param2, UINT16 param3, HRESULT &hr )
{
    INT32 retVal = 0; 
    return retVal;
}

INT32 NOR::InternalRead( CLR_RT_TypedArray_UINT16 param0, UINT32 param1, UINT16 param2, HRESULT &hr )
{
	return gNORDriver.ReadBuffer(param0.GetBuffer(), param1, param2);
}

