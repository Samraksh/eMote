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


#ifndef _SAMRAKSH_SPOT_SAMRAKSH_SPOT_GC_H_
#define _SAMRAKSH_SPOT_SAMRAKSH_SPOT_GC_H_

namespace Samraksh
{
    namespace SPOT
    {
        struct GC
        {
            // Helper Functions to access fields of managed object
            // Declaration of stubs. These functions are implemented by Interop code developers
            static float GetUsedKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static float GetFreeKBytes( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 IsGCSamExtensionsEnabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 EnableIncrementalCleaning( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 EnableIncrementalCompaction( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 DisableIncrementalCleaning( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 DisableIncrementalCompaction( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static INT8 CleanBeforeSleep( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
            static INT8 CleanNow( CLR_RT_HeapBlock* pMngObj, UINT32 param0, HRESULT &hr );
            static INT8 CleanNow( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static UINT32 AvgTimeForCleaning( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static UINT32 AvgTimeForCompaction( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
        };
    }
}
#endif  //_SAMRAKSH_SPOT_SAMRAKSH_SPOT_GC_H_
