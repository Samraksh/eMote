//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_Samraksh_rDAC.h"

using namespace Samraksh;


HRESULT Library_InterfaceLibrary_Samraksh_rDAC::Init___STATIC__VOID( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        rDAC::Init( hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_InterfaceLibrary_Samraksh_rDAC::setValue___STATIC__I4__U2( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UINT16 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT16( stack, 0, param0 ) );

        INT32 retVal = rDAC::setValue( param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
