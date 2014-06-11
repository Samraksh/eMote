//-----------------------------------------------------------------------------
//
//    ** DO NOT EDIT THIS FILE! **
//    This file was generated by a tool
//    re-running the tool will overwrite this file.
//
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include "RealTimeTimer.h"
#include <stm32f10x.h>
//#include <tim/netmf_timers.h>
#include <TinyCLR_Hardware.h>
#include <TinyCLR_Runtime.h>
#include "../Include/Samraksh/VirtualTimer.h"

#define TICKS_PER_MICROSECOND SYSTEM_CLOCK_HZ/1000000
//#define RT_HARDWARE_TIMER 3
//#define DEBUG_RT_TIMER

static bool g_RealTimeTimerEnalbed = false;
static CLR_RT_HeapBlock_NativeEventDispatcher *g_Context = NULL;
CLR_RT_ApplicationInterrupt interrupt;
static UINT64 g_UserData = 0;
bool SingleShot = FALSE;
UINT32 RealTimeTimerTicks, RollOverTicks;
UINT32 ManagedTimerPeriodMicroSeconds;
UINT32 ManagedTimerDueTimeMicroSeconds;
UINT32 ManagedTimerTicks;
UINT64 RealTimeCount=0;
UINT32 RollOverCount=0;
UINT32 RollOver=0;

UINT32 maxTicks = 0;

void ISR_REALTIME_TIMER (void* Param);
void ISR_PendSV_Handler (void* Param);

BOOL InitializeTimer (){

	/*if(!VirtTimer_Initialize(VIRT_TIMER_REALTIME, TRUE, 0, 0, ISR_REALTIME_TIMER, NULL))
		return FALSE;*/

	maxTicks = VirtTimer_GetMaxTicks(VIRT_TIMER_REALTIME);

	if(!VirtTimer_SetTimer(VIRT_TIMER_REALTIME, 0, RealTimeTimerTicks, FALSE, TRUE, ISR_REALTIME_TIMER)){ //50 milli sec Timer in micro seconds
		return DS_Fail;
	}

	////VirtTimer_Start( VIRT_TIMER_REALTIME );

	////VirtTimer_SetCounter(VIRT_TIMER_REALTIME, 0);
	////VirtTimer_SetCompare(VIRT_TIMER_REALTIME, (UINT16)(RealTimeTimerTicks));

	/*if (!Timer_Driver :: Initialize (RT_HARDWARE_TIMER, TRUE, 0, 0, ISR_REALTIME_TIMER, NULL))
	{
		return FALSE;
	}
	Timer_Driver::SetCounter(RT_HARDWARE_TIMER,0);
	Timer_Driver::SetCompare(RT_HARDWARE_TIMER, (UINT16)(RealTimeTimerTicks));*/

	return TRUE;
}

BOOL UnInitializeTimer (){

	if(!VirtTimer_UnInitialize())
		return FALSE;

	/*if (Timer_Driver :: Uninitialize (RT_HARDWARE_TIMER))
	{
		return FALSE;
	}*/

	return TRUE;
}

BOOL RT_Dispose (){
	UnInitializeTimer();
	CPU_INTC_DeactivateInterrupt(60);
	g_RealTimeTimerEnalbed=false;
}

BOOL RT_Change(uint dueTime, uint period){
	ManagedTimerPeriodMicroSeconds= period;
	ManagedTimerDueTimeMicroSeconds= dueTime;

	GLOBAL_LOCK(irq);

	UnInitializeTimer();

	if(period==0){
		SingleShot=TRUE;
		ManagedTimerTicks= TICKS_PER_MICROSECOND*ManagedTimerDueTimeMicroSeconds;
	}else {
		ManagedTimerTicks= TICKS_PER_MICROSECOND*ManagedTimerPeriodMicroSeconds;
		SingleShot=FALSE;
	}

	RollOver=0;
	if(maxTicks == 0)
		maxTicks = VirtTimer_GetMaxTicks(VIRT_TIMER_REALTIME);

	if(ManagedTimerTicks < (maxTicks + 1)) {
		RealTimeTimerTicks = ManagedTimerTicks;
		RollOverCount=0;
	}else {
		RealTimeTimerTicks=maxTicks;
		RollOverCount= ManagedTimerTicks/maxTicks;
		RollOverTicks= ManagedTimerTicks - (RollOverCount* maxTicks);
	}
	InitializeTimer();

	return 1;
}

static HRESULT InitializeRealTimeTimerDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, UINT64 userData )
{
   g_Context  = pContext;
   g_UserData = userData;
   ManagedTimerPeriodMicroSeconds= userData;
   ManagedTimerTicks= TICKS_PER_MICROSECOND*ManagedTimerPeriodMicroSeconds;
   if(ManagedTimerTicks < (maxTicks + 1)) {
	   RealTimeTimerTicks = ManagedTimerTicks;
	   RollOverCount=0;
   }else {
	   RealTimeTimerTicks=maxTicks;
	   RollOverCount= ManagedTimerTicks/maxTicks;
	   RollOverTicks= ManagedTimerTicks - (RollOverCount* maxTicks);
   }
   InitializeTimer();

   //Register the PendSV interrupt Handler

   if(!CPU_INTC_ActivateInterrupt(60, ISR_PendSV_Handler, NULL)){
	   int x;
	   debug_printf("Error Registering ISR Realtime hardware handler");
   }

#ifdef DEBUG_RT_TIMER
   //Initialize GPIO pins
   CPU_GPIO_SetPinState( 0, FALSE);
   CPU_GPIO_SetPinState( 0, TRUE);
   CPU_GPIO_SetPinState( 0, FALSE);
   CPU_GPIO_SetPinState( 0, TRUE);

   CPU_GPIO_EnableOutputPin (7, FALSE);
   CPU_GPIO_SetPinState( 7, TRUE);
   CPU_GPIO_SetPinState( 7, FALSE);

   CPU_GPIO_EnableOutputPin (4, FALSE);
   CPU_GPIO_SetPinState( 4, TRUE);
   CPU_GPIO_SetPinState( 4, FALSE);
#endif

   return S_OK;
}


static HRESULT EnableDisableRealTimeTimerDriver( CLR_RT_HeapBlock_NativeEventDispatcher *pContext, bool fEnable )
{
	if(fEnable)
		g_RealTimeTimerEnalbed= fEnable;
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

static void ISR_RealTimeTimerProc( CLR_RT_HeapBlock_NativeEventDispatcher *pContext )

{
    GLOBAL_LOCK(irq);

#if defined(SAMRAKSH_RTOS_EXT)  //Samraksh
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
    HRESULT hr;
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

    /*if(FAILED(hr))
    {
    	//Dispatch has failed what should we do now?? Try dispatch again??
    	hr = g_CLR_HW_Hardware.SpawnRTOSDispatcher(&interrupt);
    }*/
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

    //Events_Set(SYSTEM_EVENT_FLAG_TIMER2);
    //Process the Interrupt Too
    //g_CLR_HW_Hardware.ProcessInterrupts();
#endif

}

void GeneratePendSVInterrupt(){
	//E000ED04
	//*((uint32_t volatile *)0x00000038) = 0x10000000;
	////SCB->ICSR |= SCB_ICSR_PENDSVSET;
	//*((uint32_t volatile *)SCB->ICSR = SCB_ICSR_PENDSVSET;
}

void GenerateInterrupt( HRESULT &hr )
{
    if ( g_Context == NULL )
    {
        // Generates exception if context not set.
        hr = CLR_E_DRIVER_NOT_REGISTERED;
        return;
    }

    ISR_RealTimeTimerProc( g_Context );
}

void ISR_PendSV_Handler (void* Param){
	HRESULT hresult;
	GenerateInterrupt(hresult);
}

void ISR_REALTIME_TIMER (void* Param){
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	/*if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 );
	}*/
	//HRESULT hresult;

	if(RollOverCount==0){
#ifdef DEBUG_RT_TIMER
		CPU_GPIO_SetPinState( 0, FALSE );
#endif
		//Ready to generate interrupt to CLR
		if(!SingleShot) {

			VirtTimer_Change(VIRT_TIMER_REALTIME, 0, (UINT16)( VirtTimer_GetCounter(VIRT_TIMER_REALTIME) )+ RealTimeTimerTicks, FALSE);
			VirtTimer_Start( VIRT_TIMER_REALTIME );

			////VirtTimer_SetCompare(VIRT_TIMER_REALTIME, (UINT16)( VirtTimer_GetCounter(VIRT_TIMER_REALTIME) )+ RealTimeTimerTicks);

			//Timer_Driver::SetCompare( RT_HARDWARE_TIMER, (UINT16)(Timer_Driver::GetCounter(RT_HARDWARE_TIMER))+ RealTimeTimerTicks);
		}
		else {
			UnInitializeTimer();
		}
		GeneratePendSVInterrupt();
		RollOver=0;
	}else {
		if(RollOver==RollOverCount){
#ifdef DEBUG_RT_TIMER
				CPU_GPIO_SetPinState( 0, FALSE );
#endif
				//Ready to generate interrupt to CLR
				if(!SingleShot) {
					VirtTimer_Change(VIRT_TIMER_REALTIME, 0, maxTicks, FALSE);
					VirtTimer_Start( VIRT_TIMER_REALTIME );

					////VirtTimer_SetCounter(VIRT_TIMER_REALTIME, 0);
					////VirtTimer_SetCompare(VIRT_TIMER_REALTIME, maxTicks);

					//Timer_Driver::SetCounter(RT_HARDWARE_TIMER,0);
					//Timer_Driver::SetCompare( RT_HARDWARE_TIMER, 65535);
				}else {
					UnInitializeTimer();
				}
				RollOver=0;
				GeneratePendSVInterrupt();

		}else if (RollOver==RollOverCount-1){

				VirtTimer_Change(VIRT_TIMER_REALTIME, 0, RollOverTicks, FALSE);
				VirtTimer_Start( VIRT_TIMER_REALTIME );
				////VirtTimer_SetCompare(VIRT_TIMER_REALTIME, RollOverTicks);
				//Timer_Driver::SetCompare( RT_HARDWARE_TIMER, RollOverTicks);
				RollOver++;
		}else{
				RollOver++;
				VirtTimer_Change(VIRT_TIMER_REALTIME, 0, maxTicks, FALSE);
				VirtTimer_Start( VIRT_TIMER_REALTIME );
				////VirtTimer_SetCompare(VIRT_TIMER_REALTIME, 65535);
				//Timer_Driver::SetCompare( RT_HARDWARE_TIMER, 65535);
		}
	}
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
}

static const CLR_RT_DriverInterruptMethods g_InteropRealTimeTimerDriverMethods =

{ InitializeRealTimeTimerDriver,
  EnableDisableRealTimeTimerDriver,
  CleanupRealTimeTimerDriver
};

static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    Library_RealTimeTimer_Samraksh_eMote_RealTime_Timer::Dispose___STATIC__VOID,
    Library_RealTimeTimer_Samraksh_eMote_RealTime_Timer::Change___STATIC__BOOLEAN__U4__U4,
    Library_RealTimeTimer_Samraksh_eMote_RealTime_Timer::GenerateInterrupt___STATIC__VOID,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Samraksh_eMote_RealTime =
{
    "Samraksh_eMote_RealTime", 
    0x96FD394B,
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
