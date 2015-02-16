//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include <TinyCLR_Hardware.h>
#include <TinyCLR_Runtime.h>
#include "../Include/Samraksh/VirtualTimer.h"

#include "SamrakshEmoteRealtime.h"

#ifdef PLATFORM_ARM_EmoteDotNow
#include <stm32f10x.h>
#endif

#define PEND_SV_INTERRUPT 60			//For .Now

//#define DEBUG_RT_TIMER

static bool g_RealTimeTimerEnalbed = false;
static CLR_RT_HeapBlock_NativeEventDispatcher *g_Context = NULL;
CLR_RT_ApplicationInterrupt interrupt;
static UINT64 g_UserData = 0;
UINT32 RealTimeTimerTicks;
UINT32 RealTimeTimerMicrosecs;
UINT32 ManagedTimerPeriodMicroSeconds;
UINT32 ManagedTimerDueTimeMicroSeconds;
UINT32 ManagedTimerTicks = 0, ManagedTimerMicrosecs = 0;
UINT64 RealTimeCount=0;

UINT32 maxTicks = 0;
UINT64 maxMicroseconds = 0;

void ISR_REALTIME_TIMER (void* Param);
void ISR_SoftwareInterrupt_Handler (void* Param);
static void EnqueueEventToCLR( CLR_RT_HeapBlock_NativeEventDispatcher *pContext );

BOOL InitializeTimer ()
{
	VirtualTimerReturnMessage retVal = VirtTimer_SetTimer(VIRT_TIMER_REALTIME, 0, RealTimeTimerMicrosecs, FALSE, TRUE, ISR_REALTIME_TIMER);

	// if timer is already reserved we just change it to new value
	if(retVal == TimerReserved){
		VirtTimer_Change(VIRT_TIMER_REALTIME, 0, RealTimeTimerMicrosecs, false);
	}

	VirtTimer_Start( VIRT_TIMER_REALTIME );

	return TRUE;
}

BOOL UnInitializeTimer ()
{
	if(!VirtTimer_UnInitialize())
		return FALSE;

	return TRUE;
}

BOOL RT_Dispose ()
{
	//UnInitializeTimer();
	VirtTimer_Stop( VIRT_TIMER_REALTIME );
#ifdef PLATFORM_ARM_EmoteDotNow
	CPU_INTC_DeactivateInterrupt(PEND_SV_INTERRUPT);
#else
	//Need something similar to above for Adapt
#endif
	g_RealTimeTimerEnalbed = false;
}


BOOL RT_Change(uint dueTime, uint period)
{
	VirtTimer_Change(VIRT_TIMER_REALTIME, dueTime, period, false);

	return 1;
}

static HRESULT InitializeRealTimeTimerDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_Context  = pContext;
   g_UserData = userData;

   RealTimeTimerMicrosecs = userData;
   InitializeTimer();

#ifdef PLATFORM_ARM_EmoteDotNow
   //Register the software interrupt Handler
   //On eMote .Now, it is pendSV interrupt.
   if(!CPU_INTC_ActivateInterrupt(PEND_SV_INTERRUPT, ISR_SoftwareInterrupt_Handler, NULL))
   {
	   int x;
	   debug_printf("Error Registering ISR Realtime hardware handler");
   }
#else
   //Need something similar to above for Adapt
#endif
   return S_OK;
}

void ISR_SoftwareInterrupt_Handler (void* Param)
{
	HRESULT hresult;
	EnqueueEventToCLR( g_Context );
}


void GenerateInterrupt( HRESULT &hr )
{
    EnqueueEventToCLR( g_Context );
}

void GenerateSoftwareInterrupt()
{
#ifdef PLATFORM_ARM_EmoteDotNow
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
#else
	//Below is temporary. Need something similar to above (.Now) for Adapt
	HRESULT hresult;
	GenerateInterrupt(hresult);
#endif
}

static void EnqueueEventToCLR( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    GLOBAL_LOCK(irq);

    HRESULT hr;
    if ( g_Context == NULL )
	{
		// Generates exception if context not set.
		hr = CLR_E_DRIVER_NOT_REGISTERED;
		return;
	}

#ifdef SAMRAKSH_RTOS_EXT  //Samraksh
	//Garbage Collector is running, dont mess with it, return immediately.
	if (g_CLR_RT_ExecutionEngine.m_heapState == g_CLR_RT_ExecutionEngine.c_HeapState_UnderGC) {
		return;
	}

	//CLR_RT_ApplicationInterrupt interrupt;
    interrupt.m_interruptPortInterrupt.m_context = pContext;
    interrupt.m_interruptPortInterrupt.m_time= Time_GetUtcTime();
    interrupt.m_interruptPortInterrupt.m_data1 = UINT32(g_UserData >> 16);
    interrupt.m_interruptPortInterrupt.m_data2 =  UINT32(g_UserData & 0xFFFFFFFF);

    //Dispatch interrupt to RTOSThread
    ////HRESULT hr;
#ifdef DEBUG_RT_TIMER
    CPU_GPIO_SetPinState(4,TRUE);
#endif
    if(g_CLR_RT_ExecutionEngine.m_rtosInterruptThread){
    	g_CLR_RT_ExecutionEngine.m_rtosInterruptThread->m_status =  CLR_RT_Thread::TH_S_Terminated;
    }
    hr = g_CLR_HW_Hardware.SpawnRTOSDispatcher(&interrupt);
#ifdef DEBUG_RT_TIMER
    CPU_GPIO_SetPinState(4,FALSE);
#endif
    RealTimeCount++;

    //Execute Thread

    //if(SUCCEEDED(hr)){
#ifdef DEBUG_RT_TIMER
		CPU_GPIO_SetPinState(7,TRUE);
#endif
		//g_CLR_RT_ExecutionEngine.m_rtosInterruptThread->Execute();
		g_CLR_RT_ExecutionEngine.m_rtosInterruptThread->m_timeQuantumExpired=FALSE;
		g_CLR_RT_ExecutionEngine.m_rtosInterruptThread->m_isRtosThread=TRUE;
		//g_CLR_RT_ExecutionEngine.m_rtosInterruptThread->Execute_Inner();
		g_CLR_RT_ExecutionEngine.m_rtosInterruptThread->RTOS_Thread_Execute();
#ifdef DEBUG_RT_TIMER
		CPU_GPIO_SetPinState(7,FALSE);
#endif
    //}
#else
    //Generate a Software Interrupt and Add to HAL Queue
    SaveNativeEventToHALQueue( pContext, UINT32(g_UserData >> 16), UINT32(g_UserData & 0xFFFFFFFF) );
#endif
}


void ISR_REALTIME_TIMER (void* Param)
{
#ifdef DEBUG_RT_TIMER
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
#endif
		GenerateSoftwareInterrupt();
#ifdef DEBUG_RT_TIMER
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
#endif
}

static HRESULT EnableDisableRealTimeTimerDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
	if(fEnable)
		g_RealTimeTimerEnalbed = fEnable;
	else
		RT_Dispose();

   return S_OK;
}

static HRESULT CleanupRealTimeTimerDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )
{
    g_Context = NULL;
    g_UserData = 0;
    CleanupNativeEventsFromHALQueue( pContext );
    return S_OK;
}

static const CLR_RT_DriverInterruptMethods g_InteropRealTimeTimerDriverMethods =
{
	InitializeRealTimeTimerDriver,
	EnableDisableRealTimeTimerDriver,
	CleanupRealTimeTimerDriver
};

static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    Library_SamrakshEmoteRealtime_Samraksh_eMote_RealTime_Timer::Dispose___STATIC__VOID,
    Library_SamrakshEmoteRealtime_Samraksh_eMote_RealTime_Timer::Change___STATIC__BOOLEAN__U4__U4,
    Library_SamrakshEmoteRealtime_Samraksh_eMote_RealTime_Timer::GenerateInterrupt___STATIC__VOID,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_RealTime =
{
    "Samraksh_eMote_RealTime", 
    0x416CBE60,
    method_lookup
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_RealTimeInteropTimer  =
{
    "RealTimeInteropTimer",
    //0x6848EA00,
    DRIVER_INTERRUPT_METHODS_CHECKSUM,
    &g_InteropRealTimeTimerDriverMethods
    //method_lookup
};
