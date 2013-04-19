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


#ifndef _SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_MAC_CSMA_H_
#define _SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_MAC_CSMA_H_

#include <Samraksh/Mac_decl.h>
#include <Samraksh/Message.h>

HRESULT Initialize_CSMA_MAC_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData );
HRESULT EnableDisable_CSMA_MAC_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable );
HRESULT Cleanup_CSMA_MAC_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext );


namespace Samraksh
{
    namespace SPOT
    {
        namespace Net
        {
            namespace Mac
            {
                struct CSMA
                {
                    // Helper Functions to access fields of managed object
                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT32 Configure( CLR_RT_HeapBlock* pMngObj, UNSUPPORTED_TYPE param0, HRESULT &hr );
                    static INT32 UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, UINT16 param3, HRESULT &hr );
                    static UINT16 GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr );
                    static UINT8 GetID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static UINT8 GetBufferSize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static UINT8 GetPendingPacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 RemovePacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                    static INT8 GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
                    static INT32 SetTxPower( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                    static INT32 SetChannel( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                    static INT32 GetChannel( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                    static INT32 GetTxPower( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                    static INT32 TurnOnRadio( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 SleepRadio( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                    static INT32 SetCCA( INT8 param0, HRESULT &hr );
                    static INT32 SetNumberOfRetries( UINT8 param0, HRESULT &hr );
                    static INT32 SetCCASenseTime( UINT8 param0, HRESULT &hr );
                    static INT32 SetBufferSize( UINT8 param0, HRESULT &hr );
                    static INT32 SetRadioID( UINT8 param0, HRESULT &hr );
                    static INT8 GetCCA( HRESULT &hr );
                    static UINT8 GetNumberOfRetries( HRESULT &hr );
                    static UINT8 GetCCASenseTime( HRESULT &hr );
                    static UINT8 GetRadioID( HRESULT &hr );
                    static INT32 ReConfigure( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                    static INT32 InternalInitialize( UNSUPPORTED_TYPE param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );

                    //All aline above is auto generated
                    //Lines below are added by Mukundan
                    static MacEventHandler_t CSMA_Event_Handler;
                    static UINT8 MacID;
                    static UINT8 MyAppID;
                    static Message_15_4_t SendMsg, RcvMsg;
                    static Message_15_4_t *SendMsgPtr, *RcvMsgPtr;
                    static CLR_RT_HeapBlock_NativeEventDispatcher *ne_Context;
                    static UINT64 ne_userData;
					static CLR_RT_HeapBlock_Array *pHeapBlockMsgArray;
                    static UINT8 *managedCSMAMsg;
                    static INT8 UnMarshalNeighbor( CLR_RT_TypedArray_UINT8 nbr, Neighbor_t *nativeNbr);
                };
            }
        }
    }
}
#endif  //_SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_MAC_CSMA_H_
