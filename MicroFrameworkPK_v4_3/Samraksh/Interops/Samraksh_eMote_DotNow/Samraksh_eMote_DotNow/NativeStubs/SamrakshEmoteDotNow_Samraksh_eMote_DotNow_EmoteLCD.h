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


#ifndef _SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_EMOTELCD_H_
#define _SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_EMOTELCD_H_

namespace Samraksh
{
    namespace eMote
    {
        namespace DotNow
        {
            struct EmoteLCD
            {
                // Helper Functions to access fields of managed object
                static INT32& Get_currentColumn1( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__currentColumn1 ); }

                static INT32& Get_currentColumn2( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__currentColumn2 ); }

                static INT32& Get_currentColumn3( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__currentColumn3 ); }

                static INT32& Get_currentColumn4( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__currentColumn4 ); }

                static INT8& Get_DP1( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__DP1 ); }

                static INT8& Get_DP2( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__DP2 ); }

                static INT8& Get_DP3( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__DP3 ); }

                static INT8& Get_DP4( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_EmoteLCD::FIELD__DP4 ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT8 Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 Uninitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static INT8 Write( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr );
                static INT8 SetDP( CLR_RT_HeapBlock* pMngObj, INT8 param0, INT8 param1, INT8 param2, INT8 param3, HRESULT &hr );
                static INT8 WriteN( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, HRESULT &hr );
                static INT8 WriteRawBytes( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr );
                static INT8 Blink( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
                static INT8 Clear( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_EMOTELCD_H_
