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


#include "SamrakshEmoteRealtime.h"
#include "SamrakshEmoteRealtime_Samraksh_eMote_RealTime_Timer.h"

using namespace Samraksh::eMote::RealTime;

extern void RT_Dispose();
extern BOOL RT_Change(UINT32 dueTime, UINT32 period);
extern void GenerateSoftwareInterrupt(void);

void Timer::Dispose( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	RT_Dispose();
}

INT8 Timer::Change( CLR_RT_HeapBlock* pMngObj, UINT32 param0, UINT32 param1, HRESULT &hr )
{
    return RT_Change(param0, param1);
}

void Timer::GenerateInterrupt( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	GenerateSoftwareInterrupt();
}

