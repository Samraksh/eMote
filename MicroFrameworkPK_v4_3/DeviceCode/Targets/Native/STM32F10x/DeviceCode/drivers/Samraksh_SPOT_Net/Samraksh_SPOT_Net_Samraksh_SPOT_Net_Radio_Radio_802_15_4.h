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
                    static UNSUPPORTED_TYPE& Get_MyReceiveCallback( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Radio_Radio_802_15_4::FIELD__MyReceiveCallback ); }

                    static UNSUPPORTED_TYPE& Get_dataBuffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Radio_Radio_802_15_4::FIELD__dataBuffer ); }

                    static UNSUPPORTED_TYPE& Get_message( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Radio_Radio_802_15_4::FIELD__message ); }

                    static UNSUPPORTED_TYPE& Get_config( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Radio_Radio_802_15_4::FIELD__config ); }

                    static UNSUPPORTED_TYPE& Get_marshalBuffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_SPOT_Net_Samraksh_SPOT_Net_Radio_Radio_802_15_4::FIELD__marshalBuffer ); }

                    // Declaration of stubs. These functions are implemented by Interop code developers
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
                    static INT32 GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                    static INT32 InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                    static INT32 SetTxPower( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                    static INT32 SetChannel( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                    static INT32 GetTxPower( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 GetChannel( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                    static INT32 ReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
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
