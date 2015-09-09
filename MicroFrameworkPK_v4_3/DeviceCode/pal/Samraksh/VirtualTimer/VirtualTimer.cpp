/*
 * Samraksh virtual timer driver
 * Initial Create - Mukundan Sridharan; Ananth Muralidharan
 * 07/24/2014
 *
 */

#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Hal_util.h>

//#define DEBUG_VT 1
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

//For additional virtual timer support, adjust values here as well as in platform_selector.h, VirtualTimer.h (VirtualTimer class)
#ifdef PLATFORM_ARM_EmoteDotNow
	const UINT8 VTCount0 = 8;
#else
	const UINT8 VTCount0 = 8;
#endif

template<>
inline BOOL VirtualTimerMapper<VTCount0>::VirtTimerIndexMapper(UINT8 timer_id, UINT8 &VTimerIndex)
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
template<>
BOOL VirtualTimerMapper<VTCount0>::Initialize(UINT16 temp_HWID, UINT16 temp_countVTimers, UINT16 Timer, BOOL IsOneShot, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
#ifdef DEBUG_VT
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 24, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 25, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 29, TRUE);
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 30, TRUE);
#endif
	VTM_hardwareTimerId = VTM_countOfVirtualTimers = 0;

	VTM_hardwareTimerId = temp_HWID;
	VTM_countOfVirtualTimers = temp_countVTimers;

	m_lastQueueAdjustmentTime = 0;

	// Start Up Timer
	if(!ISR)
	{
		if(!CPU_Timer_Initialize(VTM_hardwareTimerId, FALSE, 0, VirtualTimerCallback, NULL)) {
			ASSERT(0);
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
	else
	{
		if(!CPU_Timer_Initialize(Timer, IsOneShot, Prescaler, ISR, ISR_PARAM)) {
			ASSERT(0);
			return FALSE;
		}
		else {
			return TRUE;
		}
	}
}

template<>
BOOL VirtualTimerMapper<VTCount0>::UnInitialize(UINT16 temp_HWID)
{
	if(!CPU_Timer_UnInitialize(temp_HWID)) {
		ASSERT(0);
		return FALSE;
	}
	else {
		return TRUE;
	}
}

template<>
BOOL VirtualTimerMapper<VTCount0>::SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	UINT32 ticksPeriod = 0, ticksStartDelay = 0;
	// Can not accept anymore timers
	if(m_current_timer_cnt_ >= VTM_countOfVirtualTimers) {
		ASSERT(0);
		return FALSE;
	}

	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0) {
		ASSERT(0);
		return FALSE;
	}

	UINT8 VTimerIndex = 0;

	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound)
		VTimerIndex = m_current_timer_cnt_;

	if(g_VirtualTimerInfo[VTimerIndex].get_m_reserved()) {
		ASSERT(0);
		return FALSE;
	}
	
	ticksPeriod = CPU_MicrosecondsToTicks(period, VTM_hardwareTimerId);
	ticksStartDelay = CPU_MicrosecondsToTicks(start_delay, VTM_hardwareTimerId);

	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_callBack(callback);
	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_period(ticksPeriod);
	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_is_one_shot(is_one_shot);
	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_is_running(FALSE);
	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_reserved(_isreserved);
	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_start_delay(ticksStartDelay);
	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_timer_id(timer_id);
	g_VirtualTimerInfo[m_current_timer_cnt_].set_m_ticks_when_match_(HAL_Time_CurrentTicks() + ticksPeriod + ticksStartDelay);

	m_current_timer_cnt_++;

	return TRUE;
}


// This function takes a timer id and dtime as input and changes the corresponding values of the timer
// Note if the timer is currently in the system when this happens, the values are modified only when the timer
// finishes its current instance in the queue and when it's added back in it assumes the new values
template<>
BOOL VirtualTimerMapper<VTCount0>::ChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot)
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
template<>
BOOL VirtualTimerMapper<VTCount0>::StartTimer(UINT8 timer_id)
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
	g_VirtualTimerInfo[VTimerIndex].set_m_ticks_when_match_(HAL_Time_CurrentTicks()  + g_VirtualTimerInfo[VTimerIndex].get_m_period() + g_VirtualTimerInfo[VTimerIndex].get_m_start_delay());
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
			if (g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() < HAL_Time_CurrentTicks() ) {
				CPU_Timer_SetCompare(VTM_hardwareTimerId, HAL_Time_CurrentTicks());
			}
			else {
				CPU_Timer_SetCompare(VTM_hardwareTimerId, g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() );
			}
			gVirtualTimerObject.virtualTimerMapper_0.m_current_timer_running_ = nextTimer;
		}
#ifdef DEBUG_VT
	CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
#endif
	return TRUE;
}


// This function takes a timer id as input and changes its state to not running
// This means that the timer does not go back on the queue once its extracted
template<>
BOOL VirtualTimerMapper<VTCount0>::StopTimer(UINT8 timer_id)
{
	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0)  {
		ASSERT(0);
		return FALSE;
	}

	UINT8 VTimerIndex = 0;
	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound) {
		ASSERT(0);
		return FALSE;
	}

	g_VirtualTimerInfo[VTimerIndex].set_m_is_running(FALSE);
	g_VirtualTimerInfo[VTimerIndex].set_m_reserved(FALSE);
	return TRUE;
}


namespace VirtTimerHelperFunctions
{
	void HardwareVirtTimerMapper(UINT16 hardwareTimer_id, UINT8 &hardwareTimerIndex)
	{
		hardwareTimerIndex = -1;
		for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
		{
			if(hardwareTimer_id == g_HardwareTimerIDs[i])
			{
				hardwareTimerIndex = i;
				break;
			}
		}
	}
}

HAL_CONTINUATION    vtCallbackContinuation;


// Algorithm for the callback:
// All system timers (except C# user timers) will run through the Virtual timer. Each timer keeps track of the time at which it will fire
// The timer that will fire soonest has its time set in the timer comparator and upon the timer matching, this callback will be called.
// The timer's callback will be called and its time to fire set to a future time if it is not a one-shot timer.
// Finally we look for which timer needs to fire soonest and place that in the timer comparator.
void VirtualTimerCallback(void *arg)
{
	UINT32 ticks = 0, startDelay = 0;
	UINT16 i = 0;

	UINT16 currentHardwareTimerId = gVirtualTimerObject.VT_hardwareTimerId;
	UINT8 currentHardwareTimerIndex = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(currentHardwareTimerId, currentHardwareTimerIndex);
	if(currentHardwareTimerIndex == -1) {
		ASSERT(0);
		return;
	}

	UINT16 currentVirtualTimerCount = gVirtualTimerObject.virtualTimerMapper_0.m_current_timer_cnt_;
	VirtualTimerInfo* runningTimer = &gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[gVirtualTimerObject.virtualTimerMapper_0.m_current_timer_running_];

	// calling the timer callback that just fired
	if (runningTimer->get_m_is_running()){
		//(runningTimer->get_m_callback())(NULL);
		if ( (runningTimer->get_m_timer_id() == VIRT_TIMER_EVENTS) || (runningTimer->get_m_timer_id() == VIRT_TIMER_REALTIME) )
		{
			(runningTimer->get_m_callback())(NULL);
		}
		else {
			void * userData = NULL;
			vtCallbackContinuation.InitializeCallback((HAL_CALLBACK_FPN) (runningTimer->get_m_callback()),NULL);
			if(!vtCallbackContinuation.IsLinked())
			{
				vtCallbackContinuation.Enqueue();
				HAL_CONTINUATION::Dequeue_And_Execute();
			}
		}
	}

	// if the timer is a one shot we don't place it back on the timer Queue
	if (runningTimer->get_m_is_one_shot()){
		runningTimer->set_m_is_running(FALSE);
	} else {
		// calculating the next time this timer will fire
		runningTimer->set_m_ticks_when_match_(HAL_Time_CurrentTicks() + runningTimer->get_m_period());
	}

	// we look for the running timer that needs to be fired the earliest and set the timer comparator
	UINT16 nextTimer = 0;
	UINT64 smallestTicks = cTimerMax64Value;
	bool timerInQueue = false;
	for(i = 0; i < currentVirtualTimerCount; i++)
	{
		if(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_is_running() == TRUE)
		{
			if(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_ticks_when_match_() <= smallestTicks)
			{
					smallestTicks = gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_ticks_when_match_();
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
		if (gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() < HAL_Time_CurrentTicks() ){
			CPU_Timer_SetCompare(g_HardwareTimerIDs[currentHardwareTimerIndex], HAL_Time_CurrentTicks());
		} else {
			CPU_Timer_SetCompare(g_HardwareTimerIDs[currentHardwareTimerIndex], gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[nextTimer].get_m_ticks_when_match_() );
		}		
		gVirtualTimerObject.virtualTimerMapper_0.m_current_timer_running_ = nextTimer;
	}

}
