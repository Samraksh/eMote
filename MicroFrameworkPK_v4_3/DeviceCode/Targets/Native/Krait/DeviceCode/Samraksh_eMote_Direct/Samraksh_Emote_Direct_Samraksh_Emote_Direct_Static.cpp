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


#include "Samraksh_Emote_Direct.h"
#include "Samraksh_Emote_Direct_Samraksh_Emote_Direct_Static.h"

using namespace Samraksh_Emote_Direct;

unsigned int jumpVal = 0x0;

UINT32 Static::SetJump( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr )
{
    //TODO: boundary checking.
    jumpVal = param0;
    UINT32 retVal = jumpVal; 
    return retVal;
}

void Static::RunNative( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    typedef void (*funptr_void_foo_void)(void);
    funptr_void_foo_void nativeFunction;
    nativeFunction = (funptr_void_foo_void)jumpVal;
    nativeFunction();
}

