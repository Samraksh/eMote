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


#ifndef _SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_MACBASE_H_
#define _SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_MACBASE_H_

namespace Samraksh
{
    namespace SPOT
    {
        namespace Net
        {
            struct MACBase
            {
                // Helper Functions to access fields of managed object
                static UNSUPPORTED_TYPE& Get_message( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_MACBase::FIELD__message ); }

                static UNSUPPORTED_TYPE& Get_ByteNeighbor( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_MACBase::FIELD__ByteNeighbor ); }

                static UNSUPPORTED_TYPE& Get_MarshalBuffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_MACBase::FIELD__MarshalBuffer ); }

                static UNSUPPORTED_TYPE& Get_MyReceiveCallback( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_MACBase::FIELD__MyReceiveCallback ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                static UINT8 GetPendingPacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 GetBufferSize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 GetID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr );
                static UINT16 GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, UINT16 param3, HRESULT &hr );
                static INT32 UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 Configure( CLR_RT_HeapBlock* pMngObj, UNSUPPORTED_TYPE param0, HRESULT &hr );
                static INT32 SetCCA( CLR_RT_HeapBlock* pMngObj, INT8 param0, HRESULT &hr );
                static INT32 SetNumberOfRetries( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT32 SetCCASenseTime( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT32 SetBufferSize( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT32 SetRadioID( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                static INT8 GetCCA( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 GetNumberOfRetries( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 GetCCASenseTime( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 GetRadioID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                static INT8 GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_MACBASE_H_
