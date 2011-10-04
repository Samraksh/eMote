//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------


#include "spot_net_native.h"
#include "spot_net_native_Microsoft_SPOT_Net_SocketNative.h"

using namespace Microsoft::SPOT::Net;


HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::socket___STATIC__I4__I4__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        INT32 param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        INT32 retVal = SocketNative::socket( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::bind___STATIC__VOID__OBJECT__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param1 ) );

        SocketNative::bind( param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::connect___STATIC__VOID__OBJECT__SZARRAY_U1__BOOLEAN( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param1 ) );

        INT8 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT8( stack, 2, param2 ) );

        SocketNative::connect( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::send___STATIC__I4__OBJECT__SZARRAY_U1__I4__I4__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        INT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 3, param3 ) );

        INT32 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 4, param4 ) );

        INT32 param5;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 5, param5 ) );

        INT32 retVal = SocketNative::send( param0, param1, param2, param3, param4, param5, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::recv___STATIC__I4__OBJECT__SZARRAY_U1__I4__I4__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        INT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 3, param3 ) );

        INT32 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 4, param4 ) );

        INT32 param5;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 5, param5 ) );

        INT32 retVal = SocketNative::recv( param0, param1, param2, param3, param4, param5, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::close___STATIC__I4__OBJECT( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        INT32 retVal = SocketNative::close( param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::listen___STATIC__VOID__OBJECT__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        SocketNative::listen( param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::accept___STATIC__I4__OBJECT( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        INT32 retVal = SocketNative::accept( param0, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::getaddrinfo___STATIC__VOID__STRING__BYREF_STRING__BYREF_SZARRAY_SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        LPCSTR param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_LPCSTR( stack, 0, param0 ) );

        LPCSTR * param1;
        UINT8 heapblock1[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_LPCSTR_ByRef( stack, heapblock1, 1, param1 ) );

        UNSUPPORTED_TYPE * param2;
        UINT8 heapblock2[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE_ByRef( stack, heapblock2, 2, param2 ) );

        SocketNative::getaddrinfo( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock1, 1 ) );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock2, 2 ) );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::shutdown___STATIC__VOID__OBJECT__I4__BYREF_I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        INT32 * param2;
        UINT8 heapblock2[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32_ByRef( stack, heapblock2, 2, param2 ) );

        SocketNative::shutdown( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock2, 2 ) );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::sendto___STATIC__I4__OBJECT__SZARRAY_U1__I4__I4__I4__I4__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        INT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 3, param3 ) );

        INT32 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 4, param4 ) );

        INT32 param5;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 5, param5 ) );

        CLR_RT_TypedArray_UINT8 param6;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 6, param6 ) );

        INT32 retVal = SocketNative::sendto( param0, param1, param2, param3, param4, param5, param6, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::recvfrom___STATIC__I4__OBJECT__SZARRAY_U1__I4__I4__I4__I4__BYREF_SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        CLR_RT_TypedArray_UINT8 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        INT32 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 3, param3 ) );

        INT32 param4;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 4, param4 ) );

        INT32 param5;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 5, param5 ) );

        UNSUPPORTED_TYPE * param6;
        UINT8 heapblock6[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE_ByRef( stack, heapblock6, 6, param6 ) );

        INT32 retVal = SocketNative::recvfrom( param0, param1, param2, param3, param4, param5, param6, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT32( stack, retVal );

        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock6, 6 ) );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::getpeername___STATIC__VOID__OBJECT__BYREF_SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        UNSUPPORTED_TYPE * param1;
        UINT8 heapblock1[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE_ByRef( stack, heapblock1, 1, param1 ) );

        SocketNative::getpeername( param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock1, 1 ) );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::getsockname___STATIC__VOID__OBJECT__BYREF_SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        UNSUPPORTED_TYPE * param1;
        UINT8 heapblock1[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE_ByRef( stack, heapblock1, 1, param1 ) );

        SocketNative::getsockname( param0, param1, hr );
        TINYCLR_CHECK_HRESULT( hr );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock1, 1 ) );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::getsockopt___STATIC__VOID__OBJECT__I4__I4__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        CLR_RT_TypedArray_UINT8 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 3, param3 ) );

        SocketNative::getsockopt( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::setsockopt___STATIC__VOID__OBJECT__I4__I4__SZARRAY_U1( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        CLR_RT_TypedArray_UINT8 param3;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT8_ARRAY( stack, 3, param3 ) );

        SocketNative::setsockopt( param0, param1, param2, param3, hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::poll___STATIC__BOOLEAN__OBJECT__I4__I4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        INT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 1, param1 ) );

        INT32 param2;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_INT32( stack, 2, param2 ) );

        INT8 retVal = SocketNative::poll( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        SetResult_INT8( stack, retVal );

    }
    TINYCLR_NOCLEANUP();
}

HRESULT Library_spot_net_native_Microsoft_SPOT_Net_SocketNative::ioctl___STATIC__VOID__OBJECT__U4__BYREF_U4( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        UNSUPPORTED_TYPE param0;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UNSUPPORTED_TYPE( stack, 0, param0 ) );

        UINT32 param1;
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32( stack, 1, param1 ) );

        UINT32 * param2;
        UINT8 heapblock2[CLR_RT_HEAP_BLOCK_SIZE];
        TINYCLR_CHECK_HRESULT( Interop_Marshal_UINT32_ByRef( stack, heapblock2, 2, param2 ) );

        SocketNative::ioctl( param0, param1, param2, hr );
        TINYCLR_CHECK_HRESULT( hr );
        TINYCLR_CHECK_HRESULT( Interop_Marshal_StoreRef( stack, heapblock2, 2 ) );
    }
    TINYCLR_NOCLEANUP();
}
