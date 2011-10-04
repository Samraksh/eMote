//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "spot_io_native.h"
#include "spot_io_native_Microsoft_SPOT_IO_VolumeInfo.h"

using namespace Microsoft::SPOT::IO;


HRESULT Library_spot_io_native_Microsoft_SPOT_IO_VolumeInfo::_ctor___VOID__STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        LPCSTR param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_LPCSTR( stack, 1, param0 ) );

        VolumeInfo::_ctor( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_io_native_Microsoft_SPOT_IO_VolumeInfo::_ctor___VOID__U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        UINT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param0 ) );

        VolumeInfo::_ctor( pMngObj,  param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_io_native_Microsoft_SPOT_IO_VolumeInfo::Refresh___VOID( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        VolumeInfo::Refresh( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_io_native_Microsoft_SPOT_IO_VolumeInfo::FlushAll___VOID( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        CLR_RT_HeapBlock* pMngObj = Interop_Marshal_RetrieveManagedObject( stack );

        FAULT_ON_NULL(pMngObj);

        VolumeInfo::FlushAll( pMngObj,  hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_io_native_Microsoft_SPOT_IO_VolumeInfo::GetVolumes___STATIC__SZARRAY_MicrosoftSPOTIOVolumeInfo( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE retVal = VolumeInfo::GetVolumes( hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_UNSUPPORTED_TYPE( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_io_native_Microsoft_SPOT_IO_VolumeInfo::GetFileSystems___STATIC__SZARRAY_STRING( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        LPCSTR retVal = VolumeInfo::GetFileSystems( hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_LPCSTR( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}
