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


#ifndef _SAMRAKSHEMOTEDOTLAURA_SAMRAKSH_EMOTE_DOTLAURA_LCD_H_
#define _SAMRAKSHEMOTEDOTLAURA_SAMRAKSH_EMOTE_DOTLAURA_LCD_H_

namespace Samraksh
{
    namespace eMote
    {
        namespace DotLaura
        {
            struct LCD
            {
                // Helper Functions to access fields of managed object
                static INT8& Get_LCD_Initialized( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT8( pMngObj, Library_SamrakshEmoteDotLaura_Samraksh_eMote_DotLaura_LCD::FIELD__LCD_Initialized ); }

                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT8 Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
                static void WriteString( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, INT32 param1, INT32 param2, INT32 param3, HRESULT &hr );
                static void Clear( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSHEMOTEDOTLAURA_SAMRAKSH_EMOTE_DOTLAURA_LCD_H_
