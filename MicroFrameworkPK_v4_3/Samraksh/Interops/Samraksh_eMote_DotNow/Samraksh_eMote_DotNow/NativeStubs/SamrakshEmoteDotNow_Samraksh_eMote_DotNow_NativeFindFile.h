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


#ifndef _SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_NATIVEFINDFILE_H_
#define _SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_NATIVEFINDFILE_H_

namespace Samraksh
{
    namespace eMote
    {
        namespace DotNow
        {
            struct NativeFindFile
            {
                // Helper Functions to access fields of managed object
                static UNSUPPORTED_TYPE& Get_m_ff( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_SamrakshEmoteDotNow_Samraksh_eMote_DotNow_NativeFindFile::FIELD__m_ff ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static void _ctor( CLR_RT_HeapBlock* pMngObj, LPCSTR param0, LPCSTR param1, HRESULT &hr );
                static UNSUPPORTED_TYPE GetNext( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static void Close( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static UNSUPPORTED_TYPE GetFileInfo( UNSUPPORTED_TYPE param0, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSHEMOTEDOTNOW_SAMRAKSH_EMOTE_DOTNOW_NATIVEFINDFILE_H_
