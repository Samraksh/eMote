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
#include "InterfaceLibrary_Samraksh_MessageLayer.h"

using namespace Samraksh;

typedef uint8_t error_t  ;
extern int main_tinyos(void);
extern error_t STMAppMessagingP$STMAppMessaging$Send(uint8_t appId, uint16_t destId, uint8_t *dataBuf, uint16_t len);
extern void RealMainP$Scheduler$taskLoop(void);

extern uint8_t STMAppMessagingP$rcvdMsg[100];

static bool g_TestInterruptEnabled = false;
CLR_RT_HeapBlock_NativeEventDispatcher *g_Context = NULL;
static UINT64 g_UserData = 0;

uint8_t globalRadioBuffer[100];

INT32 MessageLayer::Init( HRESULT &hr )
{
    INT32 retVal = 0;

    main_tinyos();

    return retVal;
}

INT32 MessageLayer::Send( UINT8 param0, UINT16 param1, CLR_RT_TypedArray_UINT8 param2, UINT16 param3, HRESULT &hr )
{
    INT32 retVal = 0;

    CLR_RT_TypedArray_UINT8 dataBuf = param2;

    STMAppMessagingP$STMAppMessaging$Send(param0,param1,dataBuf.GetBuffer(),param3);

    RealMainP$Scheduler$taskLoop();

    return retVal;
}

INT32 MessageLayer::Received( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
    INT32 retVal = 0;

    UINT32 elemCount = 0;
    UINT32 i = 0;

    CLR_RT_TypedArray_UINT8 dataBuf = param0;

    RealMainP$Scheduler$taskLoop();

    for(; elemCount < dataBuf.GetSize(); elemCount++)
    {
    	globalRadioBuffer[elemCount] = STMAppMessagingP$rcvdMsg[elemCount];
    	dataBuf.SetValue(elemCount, globalRadioBuffer[elemCount]);
    }

    for(; i < 100 ; i++)
    	STMAppMessagingP$rcvdMsg[i] = 0;

    return retVal;
}

INT32 MessageLayer::ConfigureReceiver( UINT8 param0, UNSUPPORTED_TYPE param1, HRESULT &hr )
{
    INT32 retVal = 0;
    return retVal;
}

static HRESULT InitializeTestDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_Context  = pContext;
   g_UserData = userData;
   return S_OK;
}

static HRESULT EnableDisableTestDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
   g_TestInterruptEnabled = fEnable;
   return S_OK;
}

static HRESULT CleanupTestDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
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


static const CLR_RT_DriverInterruptMethods g_InteropSampleDriverMethods =
{
  InitializeTestDriver,
  EnableDisableTestDriver,
  CleanupTestDriver
};


const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Radio =
{
    "RadioCallback",
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_InteropSampleDriverMethods
};

