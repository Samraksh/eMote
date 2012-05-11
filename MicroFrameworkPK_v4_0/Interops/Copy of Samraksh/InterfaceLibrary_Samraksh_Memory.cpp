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


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_Samraksh_Memory.h"

#include <MemoryPtr.h>

using namespace Samraksh;

INT8 Memory::write( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
    INT8 retVal = 0;

    MemPtr<UINT8> memptr(param0);

    memptr = param1;

    return retVal;
}

INT8 Memory::constructNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, INT32 param0, UINT32 param1, HRESULT &hr )
{
    INT8 retVal = 0;

    // create a record in flash
    MemPtr<UINT8> memptr(param0,param1);

    return retVal;
}

INT8 Memory::disposeNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
    INT8 retVal = 0;

    MemPtr<UINT8> memptr(param0);

    memptr.dispose();

    return retVal;
}

INT8 Memory::read( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr )
{
    INT8 retVal = 0;

    MemPtr<UINT8> memptr(param0);

    UINT8* ptr = memptr.getPointer();

    CLR_RT_TypedArray_UINT8 lbuffer = param1;

    for(UINT32 i = 0; i < lbuffer.GetSize(); i++)
    	lbuffer.SetValue(i, ptr[i]);


    return retVal;
}

