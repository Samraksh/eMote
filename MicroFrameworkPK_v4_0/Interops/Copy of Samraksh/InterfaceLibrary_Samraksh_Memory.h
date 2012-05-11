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


#ifndef _INTERFACELIBRARY_SAMRAKSH_MEMORY_H_
#define _INTERFACELIBRARY_SAMRAKSH_MEMORY_H_

namespace Samraksh
{
    struct Memory
    {
        // Helper Functions to access fields of managed object
        static INT32& Get_recordId( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_InterfaceLibrary_Samraksh_Memory::FIELD__recordId ); }

        static UNSUPPORTED_TYPE& Get_buffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_InterfaceLibrary_Samraksh_Memory::FIELD__buffer ); }

        // Declaration of stubs. These functions are implemented by Interop code developers
        static INT8 write( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
        static INT8 constructNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, INT32 param0, UINT32 param1, HRESULT &hr );
        static INT8 disposeNativeMemoryPointer( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr );
        static INT8 read( CLR_RT_HeapBlock* pMngObj, INT32 param0, CLR_RT_TypedArray_UINT8 param1, HRESULT &hr );
    };
}
#endif  //_INTERFACELIBRARY_SAMRAKSH_MEMORY_H_
