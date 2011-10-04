
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


#include "InterfaceLibrary.h"
#include "InterfaceLibrary_Samraksh_TimeSync.h"

using namespace Samraksh;

static bool g_TestInterruptEnalbed = false;
static CLR_RT_HeapBlock_NativeEventDispatcher *g_Context = NULL;
static UINT64 g_UserData = 0;
extern int TimeSyncP$0$BEACON_RATE;

extern int main_tinyos();
extern void RealMainP$Scheduler$taskLoop(void);
extern void /*TimeSyncC.TimeSyncP*/TimeSyncP$0$Timer$fired(void);
extern float /*TimeSyncC.TimeSyncP*/TimeSyncP$0$TimeSyncInfo$getSkew(void);
extern uint16_t /*TimeSyncC.TimeSyncP*/TimeSyncP$0$TimeSyncInfo$getRootID(void);
extern uint32_t /*TimeSyncC.TimeSyncP*/TimeSyncP$0$GlobalTime$getLocalTime(void);

static const CLR_RT_DriverInterruptMethods g_InteropSampleDriverMethods = 

{ InitializeTimeSyncDriver,
  EnableDisableTimeSyncDriver,
  CleanupTimeSyncDriver
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Microsoft_SPOT_InteropSample_DriverProcs =
{
    "TimeSyncDriver", 
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_InteropSampleDriverMethods
};

static HRESULT InitializeTimeSyncDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_Context  = pContext;
   g_UserData = userData;
   return S_OK;
}

static HRESULT EnableDisableTimeSyncDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
   g_TestInterruptEnalbed = fEnable;
   return S_OK;
}

static HRESULT CleanupTimeSyncDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    g_Context = NULL;
    g_UserData = 0;
    CleanupNativeEventsFromHALQueue( pContext );
    return S_OK;
}

void ISR_TestProc( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    GLOBAL_LOCK(irq);
    SaveNativeEventToHALQueue( pContext, UINT32(g_UserData >> 16), UINT32(g_UserData & 0xFFFFFFFF) );
}



void TimeSync::setBeaconRate( INT32 param0, HRESULT &hr )
{
	TimeSyncP$0$BEACON_RATE = param0;
}

void TimeSync::sendTimeSyncMessage( HRESULT &hr )
{
	TimeSyncP$0$Timer$fired();
	RealMainP$Scheduler$taskLoop();
}

INT32 TimeSync::getLocalTime( HRESULT &hr )
{
    INT32 retVal = 0; 
	retVal = TimeSyncP$0$GlobalTime$getLocalTime();
    return retVal;
}

INT32 TimeSync::getGlobalTime( HRESULT &hr )
{
    INT32 retVal = 0; 
	//return TimeSyncP$0$GlobalTime$local2Global(&retVal);
    return retVal;
}

float TimeSync::getSkew( HRESULT &hr )
{
    float retVal = 0; 
	
    return TimeSyncP$0$TimeSyncInfo$getSkew();
}

void TimeSync::local2Global( HRESULT &hr )
{
}

INT16 TimeSync::getRootId( HRESULT &hr )
{
    INT16 retVal = 0; 
    return TimeSyncP$0$TimeSyncInfo$getRootID();
}


void TimeSync::Init( HRESULT &hr )
{
	main_tinyos();
	RealMainP$Scheduler$taskLoop();
}

