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


#ifndef _SAMRAKSH_EMOTE_NET_SAMRAKSH_EMOTE_NET_MACBASE_H_
#define _SAMRAKSH_EMOTE_NET_SAMRAKSH_EMOTE_NET_MACBASE_H_

#include <Samraksh/MAC_decl.h>
#include <Samraksh/Message.h>

namespace Samraksh
{
    namespace eMote
    {
        namespace Net
        {
            struct MACBase
            {
                // Helper Functions to access fields of managed object
                static UNSUPPORTED_TYPE& Get__neighborListTemp( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___neighborListTemp ); }

                static UNSUPPORTED_TYPE& Get__byteNeighbor( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___byteNeighbor ); }

                static UNSUPPORTED_TYPE& Get__marshalBuffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___marshalBuffer ); }

                static UNSUPPORTED_TYPE& Get_OnReceiveAll( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD__OnReceiveAll ); }

                static UNSUPPORTED_TYPE& Get_OnReceive( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD__OnReceive ); }

                static UNSUPPORTED_TYPE& Get_OnNeighborChange( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD__OnNeighborChange ); }

                static UNSUPPORTED_TYPE& Get_OnTransmitACK( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD__OnTransmitACK ); }
                static INT8& Get__cca( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___cca ); }

                static UINT8& Get__numberOfRetries( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT8( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___numberOfRetries ); }

                static UINT8& Get__ccaSenseTime( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT8( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___ccaSenseTime ); }

                static UINT8& Get__bufferSize( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT8( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___bufferSize ); }

                static UINT32& Get__neighborLivenessDelay( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UINT32( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD___neighborLivenessDelay ); }

                static INT32& Get_MACType( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD__MACType ); }

                static UNSUPPORTED_TYPE& Get_MACRadioObj( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Samraksh_eMote_Net_Samraksh_eMote_Net_MACBase::FIELD__MACRadioObj ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT32 RemovePacket( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 PendingReceivePacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UINT8 PendingSendPacketCount( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, UINT16 param3, HRESULT &hr );
                static INT32 UnInitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT32 InternalInitialize( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, UINT8 param1, HRESULT &hr );
                static INT32 InternalReConfigure( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                static INT32 GetNextPacket( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr );
                static INT32 GetNeighborInternal( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
                static INT32 GetNeighborListInternal( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, HRESULT &hr );
                static INT32 Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, CLR_RT_TypedArray_UINT8 param1, UINT16 param2, UINT16 param3, UINT32 param4, HRESULT &hr );
                static INT32 Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, UINT8 param1, CLR_RT_TypedArray_UINT8 param2, UINT16 param3, UINT16 param4, HRESULT &hr );
                static INT32 Send( CLR_RT_HeapBlock* pMngObj, UINT16 param0, UINT8 param1, CLR_RT_TypedArray_UINT8 param2, UINT16 param3, UINT16 param4, UINT32 param5, HRESULT &hr );
				static UINT8 MyAppID;
                static MACEventHandler_t Event_Handler;
            };
        }
    }
}
#endif  //_SAMRAKSH_EMOTE_NET_SAMRAKSH_EMOTE_NET_MACBASE_H_
