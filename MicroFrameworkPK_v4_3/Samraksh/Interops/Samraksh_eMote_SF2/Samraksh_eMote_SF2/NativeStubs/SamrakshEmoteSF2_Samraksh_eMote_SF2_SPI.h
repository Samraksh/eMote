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


#ifndef _SAMRAKSHEMOTESF2_SAMRAKSH_EMOTE_SF2_SPI_H_
#define _SAMRAKSHEMOTESF2_SAMRAKSH_EMOTE_SF2_SPI_H_

namespace Samraksh
{
    namespace eMote
    {
        namespace SF2
        {
            struct SPI
            {
                // Helper Functions to access fields of managed object
                // Declaration of stubs. These functions are implemented by Interop code developers
                static INT8 SendData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, HRESULT &hr );
                static INT8 ReceiveData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, INT32 param2, HRESULT &hr );
                static INT8 SendReceiveData( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, CLR_RT_TypedArray_UINT8 param2, INT32 param3, HRESULT &hr );
            };
        }
    }
}
#endif  //_SAMRAKSHEMOTESF2_SAMRAKSH_EMOTE_SF2_SPI_H_
