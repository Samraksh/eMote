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

#include "../DeviceCode/Targets/Native/TIAM3517/DeviceCode/TIAM3517.h"

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

int IS_CONNECTED=0;

INT32 MessageLayer::Init( HRESULT &hr )
{
<<<<<<< .merge_file_a07924
    INT32 retVal = 0;

    main_tinyos();

    return retVal;
=======
    // Set PADCONFs
	MUX_VAL( 0x0190, (IDIS | PTU | EN  | M4) ); // gpio_158 --> radio LED
	MUX_VAL( 0x0192, (IDIS | PTU | EN  | M4) ); // gpio_159 --> radio IS_CONNECTED
	MUX_VAL( 0x217C, (IDIS | PTU | EN  | M0) ); // UART1 TX
	MUX_VAL( 0x2180, (IDIS | PTU | EN  | M0) ); // UART1 RX
	
	TIAM3517_USART_Driver::Initialize(1, 115200, 0, 8, 1, 0) // Init UART @ 115200
	
	return 0;
>>>>>>> .merge_file_a08416
}

INT32 MessageLayer::Send( UINT8 param0, UINT16 param1, CLR_RT_TypedArray_UINT8 param2, UINT16 param3, HRESULT &hr )
{
<<<<<<< .merge_file_a07924
    INT32 retVal = 0;

    CLR_RT_TypedArray_UINT8 dataBuf = param2;

    STMAppMessagingP$STMAppMessaging$Send(param0,param1,dataBuf.GetBuffer(),param3);

    RealMainP$Scheduler$taskLoop();

    return retVal;
=======
	volatile int i;
	
	if (IS_CONNECTED == 0) {
		omap_set_gpio_direction(158, 0); // Set LED as output
		// Strobe LED line to connect
		omap_set_gpio_dataout(158, 1);
		for(i=0; i<32000; i++) { ; } // spin
		omap_set_gpio_dataout(158, 0);
		for(i=0; i<32000; i++) { ; } // spin
		omap_set_gpio_dataout(158, 1);
		for(i=0; i<32000; i++) { ; } // spin
		IS_CONNECTED = 1;
	}
	
	fgdhgjhdgjm
	
	TIAM3517_USART_Driver::WriteCharToTxBuffer( 1, param0 ); // Write the value to the USART
	// Strobe LED to send
	omap_set_gpio_dataout(158, 0);
	for(i=0; i<32000; i++) { ; } // spin
	omap_set_gpio_dataout(158, 1);
	for(i=0; i<32000; i++) { ; } // spin
	
    return 0;
>>>>>>> .merge_file_a08416
}

INT32 MessageLayer::Received( CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
<<<<<<< .merge_file_a07924
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
=======
    return 0;
>>>>>>> .merge_file_a08416
}

INT32 MessageLayer::ConfigureReceiver( UINT8 param0, UNSUPPORTED_TYPE param1, HRESULT &hr )
{
<<<<<<< .merge_file_a07924
    INT32 retVal = 0;
    return retVal;
=======
    return 0;
>>>>>>> .merge_file_a08416
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

