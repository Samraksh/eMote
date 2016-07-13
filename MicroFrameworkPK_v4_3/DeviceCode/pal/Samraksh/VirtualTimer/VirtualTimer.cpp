/*
 * Samraksh virtual timer driver
 * Initial Create - Mukundan Sridharan; Ananth Muralidharan
 * 07/24/2014
 *
 */

#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Hal_util.h>

#if defined(DEBUG_VT)
#define DEBUG_VT_ASSERT_ANAL(x) ASSERT(x)
#else
#define DEBUG_VT_ASSERT_ANAL(x)
#endif
// Assumptions:
// ============
// Assumes that the HAL core hardware timer is running at the same configuration  as the system timer because it uses the CPU_MicrosecondsToTicks
// interface
void VirtualTimerCallback(void *arg);

extern const UINT8 g_CountOfHardwareTimers;
extern const UINT8 g_HardwareTimerIDs[g_CountOfHardwareTimers];
extern VirtualTimer gVirtualTimerObject;
static const UINT64 cTimerMax64Value = 0x0000FFFFFFFFFFFFull; //TODO: use better name.
static const UINT32 cTimerMax32Value = 0xFFFFFFFFul;          //TODO: use better name or use UINT32_MAX.

inline BOOL VirtualTimerMapper::VirtTimerIndexMapper(UINT8 timer_id, UINT8 &VTimerIndex)
{
	BOOL timerFound = FALSE;

	for(int i = 0; i < m_current_timer_cnt_; i++)
	{
		if(g_VirtualTimerInfo[i].get_m_timer_id() == timer_id)
		{
			VTimerIndex = i;
			timerFound = TRUE;
			break;
		}
	}

	return timerFound;
}

// All active timers are placed in a timerQueue. Every time one is inserted, the list is adjusted in order with the timer that will go off soonest on top and the latest on bottom
// Every time the list is changed the m_ticksTillExpire must be adjusted to reflect the current number of ticks before the topmost timer needs to have its callback called.
// After this adjustment the new timer can be inserted where it will be automatically placed in the correct spot in the queue.
// The SetCompare timer then needs to be set appropriately
BOOL VirtualTimerMapper::Initialize(UINT16 temp_HWID)
{
	//CPU_GPIO_EnableOutputPin(VIRTUAL_TIMER_EXCEPTION_CHECK_PIN, FALSE);
	//CPU_GPIO_EnableOutputPin(VTIMER_CALLBACK_LATENCY_PIN, FALSE);
	//CPU_GPIO_SetPinState( VTIMER_CALLBACK_LATENCY_PIN, FALSE );

	//CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
#ifdef DEBUG_VT
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
#endif
	VTM_hardwareTimerId = temp_HWID;

	for (UINT16 j = 0; j < m_current_timer_cnt_; j++)
	{
		g_VirtualTimerInfo[j].set_m_timer_id(0);
	}
	

	if(!CPU_Timer_Initialize(VTM_hardwareTimerId, FALSE, 0, VirtualTimerCallback)) {
		ASSERT(0);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

BOOL VirtualTimerMapper::UnInitialize(UINT16 temp_HWID)
{
	// TODO: uninitialize entry in the timer mapper and decrement count of timers
	if(!CPU_Timer_UnInitialize(temp_HWID)) {
		ASSERT(0);
		return FALSE;
	}
	else {
		m_current_timer_cnt_ = 0;
		//FIXME: UnInitialize other VirtualTimerMapper variables.
		return TRUE;
	}
}

BOOL VirtualTimerMapper::SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	UINT32 ticksPeriod = 0, ticksStartDelay = 0;

	UINT8 VTimerIndex = 0;

	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound) {
		// Can not accept anymore timers
		if(m_current_timer_cnt_ >= g_VirtualTimerPerHardwareTimer) {
			ASSERT(0);
			return FALSE;
		}
		else {
			VTimerIndex = m_current_timer_cnt_;
			m_current_timer_cnt_++;
		}
	}

	if(g_VirtualTimerInfo[VTimerIndex].get_m_reserved()) {
		ASSERT(0);
		return FALSE;
	}
	
	DEBUG_VT_ASSERT_ANAL(g_VirtualTimerInfo[VTimerIndex].get_m_is_running() == FALSE);

	ticksPeriod     = CPU_MicrosecondsToTicks(period,      VTM_hardwareTimerId);
	ticksStartDelay = CPU_MicrosecondsToTicks(start_delay, VTM_hardwareTimerId);

	g_VirtualTimerInfo[VTimerIndex].set_m_callBack(callback);
	g_VirtualTimerInfo[VTimerIndex].set_m_period(ticksPeriod);
	g_VirtualTimerInfo[VTimerIndex].set_m_is_one_shot(is_one_shot);
	g_VirtualTimerInfo[VTimerIndex].set_m_is_running(FALSE);
	g_VirtualTimerInfo[VTimerIndex].set_m_reserved(_isreserved);
	g_VirtualTimerInfo[VTimerIndex].set_m_start_delay(ticksStartDelay);
	g_VirtualTimerInfo[VTimerIndex].set_m_timer_id(timer_id);
	g_VirtualTimerInfo[VTimerIndex].set_m_ticks_when_match_(HAL_Time_CurrentTicks() + ticksPeriod + ticksStartDelay);

	return TRUE;
}


// This function takes a timer id and dtime as input and changes the corresponding values of the timer
// Note if the timer is currently in the system when this happens, the values are modified only when the timer
// finishes its current instance in the queue and when it's added back in it assumes the new values
BOOL VirtualTimerMapper::ChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot)
{
	UINT32 ticksPeriod = 0, ticksStartDelay = 0;

	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0) {
		ASSERT(0);
		return FALSE;
	}

	UINT8 VTimerIndex = 0;
	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound) {
		// no assert; Caller may fall back to SetTimer to add an additional timer copy.
		return FALSE;
	}

	ticksPeriod = CPU_MicrosecondsToTicks(period, VTM_hardwareTimerId);
	ticksStartDelay = CPU_MicrosecondsToTicks(start_delay, VTM_hardwareTimerId);

	g_VirtualTimerInfo[VTimerIndex].set_m_start_delay(ticksStartDelay);
	g_VirtualTimerInfo[VTimerIndex].set_m_period(ticksPeriod);
	g_VirtualTimerInfo[VTimerIndex].set_m_is_one_shot(is_one_shot);

	return TRUE;
}


// This function takes a timer id as input and changes the state to running or returns false
// if the timer you specified does not exist
BOOL VirtualTimerMapper::StartTimer(UINT8 timer_id)
{
	UINT64 currentTicks;
	UINT64 tickElapsed;
#ifdef DEBUG_VT
	CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
#endif
	UINT16 i;
	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0) {
		ASSERT(0);
		return FALSE;
	}

	UINT8 VTimerIndex = 0;
	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound) {
		ASSERT(0);
		return FALSE;
	}

	// check to see if we are already running
	if (g_VirtualTimerInfo[VTimerIndex].get_m_is_running() == TRUE) {
		DEBUG_VT_ASSERT_ANAL(0);
		return TRUE;
	}

	// Initializing timer
	g_VirtualTimerInfo[VTimerIndex].set_m_ticks_when_match_(VirtTimer_GetTicks(timer_id)  + g_VirtualTimerInfo[VTimerIndex].get_m_period() + g_VirtualTimerInfo[VTimerIndex].get_m_start_delay());
	g_VirtualTimerInfo[VTimerIndex].set_m_is_running(TRUE);

		// looking to see which timer will be called the earliest
		UINT16 nextTimer = 0;
		UINT64 smallestTicks = cTimerMax64Value;
		bool timerInQueue = false;
		for(i = 0; i < m_current_timer_cnt_; i++)
		{
			if(g_VirtualTimerInfo[i].get_m_is_running() == TRUE)
			{
				if(g_VirtualTimerInfo[i].get_m_ticks_when_match_() <= smallestTicks)
				{
					smallestTicks = g_VirtualTimerInfo[i].get_m_ticks_when_match_();
					nextTimer = i;
					timerInQueue = true;
				}
			}
		}
	
		// if there is a timer in the timerqueue still we will set the advanced timer to interrupt at the correct time
		if(timerInQueue)
		{
			// for now we have to check that we are setting the compare at some time in the future. This check is also made in the setcompare function itself but there it is limited (for now) to 32-bits
			// when we are fully 64-bit compatible with our timers then we will need only the check in the setcompare function itself
			if (g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() < VirtTimer_GetTicks(timer_id) ) {
				CPU_Timer_SetCompare(VTM_hardwareTimerId, VirtTimer_GetTicks(timer_id));
			}
			else {
				CPU_Timer_SetCompare(VTM_hardwareTimerId, g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() );
			}
			m_current_timer_running_ = nextTimer;
		}
#ifdef DEBUG_VT
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
#endif
	return TRUE;
}


// This function takes a timer id as input and changes its state to not running
// This means that the timer does not go back on the queue once its extracted
BOOL VirtualTimerMapper::StopTimer(UINT8 timer_id)
{
	//Timer 0 is reserved for keeping time and timer 1 for events

	UINT8 VTimerIndex = 0;
	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound) {
		SOFT_BREAKPOINT();
		return FALSE;
	}

	g_VirtualTimerInfo[VTimerIndex].set_m_is_running(FALSE);
	g_VirtualTimerInfo[VTimerIndex].set_m_reserved(FALSE);
	return TRUE;
}


namespace VirtTimerHelperFunctions
{
	// used to take the timer (RTC_32BIT, ADVTIMER_32BIT) and get the correct VT mapper
	void HardwareVirtTimerMapper(UINT32 hardwareTimerId, UINT8 &currentVTMapper)
	{
		currentVTMapper = -1;
		for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
		{
			if(hardwareTimerId == g_HardwareTimerIDs[i])
			{
				currentVTMapper = i;
				return;
			}
		}
	}
}


#define VT_CALLBACK_CONTINUATION_MAX 8
static HAL_CONTINUATION vtCallbackContinuationArray[VT_CALLBACK_CONTINUATION_MAX];

bool queueVTCallback(VirtualTimerInfo* runningTimer){
	int i;
	
	for (i=0; i<VT_CALLBACK_CONTINUATION_MAX; i++){
		if (!vtCallbackContinuationArray[i].IsLinked())
		{
			vtCallbackContinuationArray[i].InitializeCallback((HAL_CALLBACK_FPN) (runningTimer->get_m_callback()),NULL);   
			vtCallbackContinuationArray[i].Enqueue();
			return true;
		}
	}

	return false;
}

// Algorithm for the callback:
// All system timers (except C# user timers) will run through the Virtual timer. Each timer keeps track of the time at which it will fire
// The timer that will fire soonest has its time set in the timer comparator and upon the timer matching, this callback will be called.
// The timer's callback will be called and its time to fire set to a future time if it is not a one-shot timer.
// Finally we look for which timer needs to fire soonest and place that in the timer comparator.
void VirtualTimerCallback(void *arg)
{
	UINT32 ticks = 0, startDelay = 0;
	UINT16 i = 0;

	// the timer used (RTC_32BIT, ADVTIMER_32BIT) is passed as the argument
	UINT32 currentHardwareTimerId = *(UINT32*)arg;
	UINT8 currentVTMapper = 0;
	// getting the correct VT Mapper (currentVTMapper)
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(currentHardwareTimerId, currentVTMapper);
	if(currentVTMapper == -1) {
		ASSERT(0);
		return;
	}

	UINT16 currentVirtualTimerCount = gVirtualTimerObject.virtualTimerMapper[currentVTMapper].m_current_timer_cnt_;
	VirtualTimerInfo* runningTimer = &gVirtualTimerObject.virtualTimerMapper[currentVTMapper].g_VirtualTimerInfo[gVirtualTimerObject.virtualTimerMapper[currentVTMapper].m_current_timer_running_];

	// calling the timer callback that just fired
	if (runningTimer->get_m_is_running()){
		if ( runningTimer->get_m_timer_id() <= VIRT_TIMER_INTERRUPT_CONTEXT_MARKER){
			(runningTimer->get_m_callback())(NULL);
		} else {
			//void * userData = NULL;
			queueVTCallback(runningTimer);
		}
	}

	// if the timer is a one shot we don't place it back on the timer Queue
	if (runningTimer->get_m_is_one_shot()){
		runningTimer->set_m_is_running(FALSE);
	} else {
		// calculating the next time this timer will fire
		runningTimer->set_m_ticks_when_match_(VirtTimer_GetTicks(runningTimer->get_m_timer_id()) + runningTimer->get_m_period());
	}

	// we look for the running timer that needs to be fired the earliest and set the timer comparator
	UINT16 nextTimer = 0;
	UINT64 smallestTicks = cTimerMax64Value;
	bool timerInQueue = false;
	for(i = 0; i < currentVirtualTimerCount; i++)
	{
		if(gVirtualTimerObject.virtualTimerMapper[currentVTMapper].g_VirtualTimerInfo[i].get_m_is_running() == TRUE)
		{
			if(gVirtualTimerObject.virtualTimerMapper[currentVTMapper].g_VirtualTimerInfo[i].get_m_ticks_when_match_() <= smallestTicks)
			{
					smallestTicks = gVirtualTimerObject.virtualTimerMapper[currentVTMapper].g_VirtualTimerInfo[i].get_m_ticks_when_match_();
					nextTimer = i;
					timerInQueue = true;
			}
		}
	}
	
	// if there is a timer in the timerqueue still we will set the advanced timer to interrupt at the correct time
	if(timerInQueue)
	{
		// for now we have to check that we are setting the compare at some time in the future. This check is also made in the setcompare function itself but there it is limited (for now) to 32-bits
		// when we are fully 64-bit compatible with our timers then we will need only the check in the setcompare function itself
		if (gVirtualTimerObject.virtualTimerMapper[currentVTMapper].g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() < VirtTimer_GetTicks(runningTimer->get_m_timer_id()) ){
			CPU_Timer_SetCompare(g_HardwareTimerIDs[currentVTMapper], VirtTimer_GetTicks(runningTimer->get_m_timer_id()));
		} else {
			CPU_Timer_SetCompare(g_HardwareTimerIDs[currentVTMapper], gVirtualTimerObject.virtualTimerMapper[currentVTMapper].g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() );
		}		
		gVirtualTimerObject.virtualTimerMapper[currentVTMapper].m_current_timer_running_ = nextTimer;
	}

}
