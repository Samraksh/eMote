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


#ifndef _SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_RADIO_RADIO_802_15_4_H_
#define _SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_RADIO_RADIO_802_15_4_H_

#include <Samraksh/Radio_decl.h>
#include <Samraksh/Message.h>
#include "TinyCLR_Runtime.h"

//static HRESULT Initialize_Radio_802_15_4_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData );
//static HRESULT  EnableDisable_Radio_802_15_4_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable );
//static HRESULT Cleanup_Radio_802_15_4_Driver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext );


namespace Samraksh
{
    namespace SPOT
    {
        namespace Net
        {
            namespace Radio
            {
                struct Radio_802_15_4
                {
                    // Helper Functions to access fields of managed object
                    // Declaration of stubs. These functions are implemented by Interop code developers
                    static INT32 Configure( CLR_RT_HeapBlock* pMngObj, UNSUPPORTED_TYPE param0, HRESULT &hr );
                    static INT32 UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static UINT8 GetID( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static UINT16 GetAddress( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 SetAddress( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr );
                    static INT32 TurnOn( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 Sleep( CLR_RT_HeapBlock* pMngObj, UINT8 param0, HRESULT &hr );
                    static INT32 PreLoad( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT16 param1, HRESULT &hr );
                    static INT32 SendStrobe( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 Send( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT16 param1, HRESULT &hr );
                    static INT32 SendTimeStamped( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT16 param1, UINT32 param2, HRESULT &hr );
                    static INT8 ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT8 ClearChannelAssesment( CLR_RT_HeapBlock* pMngObj, UINT16 param0, HRESULT &hr );
                    static INT32 InternalInitialize( UNSUPPORTED_TYPE param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );


                    //All aline above is auto generated
                    //Lines below are added by Mukundan
                    static RadioEventHandler_t Radio_Event_Handler;
                    static UINT8 RadioID;
                    static Message_15_4_t SendMsg, RcvMsg;
                    static Message_15_4_t *SendMsgPtr, *RcvMsgPtr;
                    static CLR_RT_HeapBlock_NativeEventDispatcher *ne_Context;
                    static UINT64 ne_userData;
                    static CLR_RT_HeapBlock_Array *pHeapBlockMsgArray;
                    static UINT8 *managedRadioMsg;
                };
            }
        }
    }
}




#endif  //_SAMRAKSH_SPOT_NET_SAMRAKSH_SPOT_NET_RADIO_RADIO_802_15_4_H_
