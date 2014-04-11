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


#include "RealTimeTimer.h"
#include "RealTimeTimer_Samraksh_eMote_RealTime_Timer.h"

using namespace Samraksh::eMote::RealTime;

extern void RT_Dispose();
extern BOOL RT_Change(UINT32 dueTime, UINT32 period);

void Timer::Dispose( HRESULT &hr )
{
	RT_Dispose();
}

INT8 Timer::Change( UINT32 param0, UINT32 param1, HRESULT &hr )
{
    return RT_Change(param0, param1);
}

void Timer::GenerateInterrupt( HRESULT &hr )
{
}

