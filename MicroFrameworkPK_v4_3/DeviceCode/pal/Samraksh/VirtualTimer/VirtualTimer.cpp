/*
 * Samraksh virtual timer driver
 * Initial Create - Mukundan Sridharan; Ananth Muralidharan
 * 07/24/2014
 *
 */


#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Hal_util.h>


// Assumptions:
// ============
// Assumes that the HAL core hardware timer is running at the same configuration  as the system timer because it uses the CPU_MicrosecondsToTicks
// interface
void VirtualTimerCallback(void *arg);

extern const UINT8 g_CountOfHardwareTimers;
extern const UINT8 g_HardwareTimerIDs[g_CountOfHardwareTimers];
extern VirtualTimer gVirtualTimerObject;

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

	for(int i = 0; i < m_current_timer_id_; i++)
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
//BOOL VirtualTimerMapper<VTCount0>::Initialize(UINT16 temp_HWID, UINT16 temp_countVTimers, UINT16 Timer, BOOL IsOneShot, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	VTM_hardwareTimerId = VTM_countOfVirtualTimers = 0;

	VTM_hardwareTimerId = temp_HWID;
	VTM_countOfVirtualTimers = temp_countVTimers;

	m_lastQueueAdjustmentTime = 0;

	// Start Up Timer
	if(!ISR)
	{
		//if(!CPU_Timer_Initialize(VTM_hardwareTimerId, FALSE, 0, 0, VirtualTimerCallback, NULL))
		if(!CPU_Timer_Initialize(VTM_hardwareTimerId, FALSE, 0, VirtualTimerCallback, NULL))
			return FALSE;
		else
			return TRUE;
	}
	else
	{
		//if(!CPU_Timer_Initialize(Timer, FreeRunning, ClkSource, Prescaler, ISR, ISR_PARAM))
		if(!CPU_Timer_Initialize(Timer, IsOneShot, Prescaler, ISR, ISR_PARAM))
			return FALSE;
		else
			return TRUE;
	}
}


template<>
BOOL VirtualTimerMapper<VTCount0>::UnInitialize(UINT16 temp_HWID)
{
	if(!CPU_Timer_UnInitialize(temp_HWID))
		return FALSE;
	else
		return TRUE;
}


// This function takes a timer id as input and changes its state to not running
// This means that the timer does not go back on the queue once its extracted
template<>
BOOL VirtualTimerMapper<VTCount0>::StopTimer(UINT8 timer_id)
{
	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0)
		return FALSE;

	UINT8 VTimerIndex = 0;
	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound)
		return FALSE;

	g_VirtualTimerInfo[VTimerIndex].set_m_is_running(FALSE);
	return TRUE;
}


// This function takes a timer id and dtime as input and changes the corresponding values of the timer
// Note if the timer is currently in the system when this happens, the values are modified only when the timer
// finishes its current instance in the queue and when its added back in it assumes the new values
template<>
BOOL VirtualTimerMapper<VTCount0>::ChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot)
{
	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0)
		return FALSE;

	UINT8 VTimerIndex = 0;
	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound)
		return FALSE;

	UINT32 ticksPeriod = CPU_MicrosecondsToTicks(period, VTM_hardwareTimerId);
	UINT32 ticksStartDelay = CPU_MicrosecondsToTicks(start_delay, VTM_hardwareTimerId);

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
	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0)
		return FALSE;

	UINT8 VTimerIndex = 0;
	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound)
		return FALSE;

	// check to see if we are already running
	if (g_VirtualTimerInfo[VTimerIndex].get_m_is_running() == TRUE){
		return TRUE;
	}

	g_VirtualTimerInfo[VTimerIndex].set_m_is_running(TRUE);

	// Adjusting all timers in the timerQueue (even ones that are not active and not in the queue are adjusted but that won't affect anything). We could iterate and only adjust active ones, but it is not worth the time to check.
	UINT64 currentTicks = CPU_Timer_CurrentTicks(VTM_hardwareTimerId);
	UINT64 tickElapsed = 0;
	if(currentTicks > get_m_lastQueueAdjustmentTime())
		tickElapsed = currentTicks - get_m_lastQueueAdjustmentTime();
	else
		tickElapsed = currentTicks + (VirtTimer_GetMaxTicks(timer_id) - get_m_lastQueueAdjustmentTime());

	set_m_lastQueueAdjustmentTime(currentTicks);

	for(UINT16 i = 0; i < VTM_countOfVirtualTimers; i++)
	{
		//Adjust only active timers
		if (g_VirtualTimerInfo[i].get_m_is_running() == TRUE)
			g_VirtualTimerInfo[i].set_m_ticksTillExpire(g_VirtualTimerInfo[i].get_m_ticksTillExpire() - (INT64) tickElapsed);
	}

	// Initializing timer
	UINT32 ticks;
	ticks = g_VirtualTimerInfo[VTimerIndex].get_m_period();
	g_VirtualTimerInfo[VTimerIndex].set_m_ticksTillExpire(ticks + g_VirtualTimerInfo[VTimerIndex].get_m_start_delay());


	// Insert this into the queue
	// If this happens to be the smallest, do the necessary bookkeeping and
	// set the new value on the comparator
	if( !timerQueue.Insert( &g_VirtualTimerInfo[VTimerIndex] ) )
		return FALSE;

	VirtualTimerInfo *nextTimer = timerQueue.PeekTop();

	// if the nextTimer to fire is the one we just started then we will set the SetCompare function
	if ( nextTimer->get_m_timer_id() == timer_id )
	{
		CPU_Timer_SetCompare(VTM_hardwareTimerId, nextTimer->get_m_ticksTillExpire() );
	}

	return TRUE;
}


template<>
BOOL VirtualTimerMapper<VTCount0>::SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	UINT32 ticksPeriod = 0, ticksStartDelay = 0;
	// Can not accept anymore timers
	if(m_current_timer_id_ > VTM_countOfVirtualTimers)
		return FALSE;

	//Timer 0 is reserved for keeping time and timer 1 for events
	if (timer_id < 0)
		return FALSE;

	UINT8 VTimerIndex = 0;

	BOOL timerFound = VirtTimerIndexMapper(timer_id, VTimerIndex);

	if(!timerFound)
		VTimerIndex = m_current_timer_id_;

	if(g_VirtualTimerInfo[VTimerIndex].get_m_reserved())
		return FALSE;

	if(period == 0xFFFFFFFF)
		ticksPeriod =  period;
	else
		ticksPeriod = CPU_MicrosecondsToTicks(period, VTM_hardwareTimerId);

	if(start_delay == 0xFFFFFFFF)
		ticksStartDelay =  start_delay;
	else
		ticksStartDelay = CPU_MicrosecondsToTicks(start_delay, VTM_hardwareTimerId);

	//UINT64 ticksPeriod = CPU_MicrosecondsToTicks((UINT64)period, VTM_hardwareTimerId);
	//UINT32 ticksStartDelay = CPU_MicrosecondsToTicks(start_delay, VTM_hardwareTimerId);
	//UINT64 ticksStartDelay = CPU_MicrosecondsToTicks((UINT64)start_delay, VTM_hardwareTimerId);

	g_VirtualTimerInfo[m_current_timer_id_].set_m_callBack(callback);
	g_VirtualTimerInfo[m_current_timer_id_].set_m_period(ticksPeriod);
	g_VirtualTimerInfo[m_current_timer_id_].set_m_is_one_shot(is_one_shot);
	g_VirtualTimerInfo[m_current_timer_id_].set_m_is_running(FALSE);
	g_VirtualTimerInfo[m_current_timer_id_].set_m_reserved(_isreserved);
	g_VirtualTimerInfo[m_current_timer_id_].set_m_start_delay(ticksStartDelay);
	g_VirtualTimerInfo[m_current_timer_id_].set_m_timer_id(timer_id);
	g_VirtualTimerInfo[m_current_timer_id_].set_m_ticksTillExpire(ticksPeriod + ticksStartDelay);

	m_current_timer_id_++;

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

/* Algorithm for the callback:
 * Overall idea is that, one timer controls CPU_Timer_SetCompare (which sets the interrupt to be fired after a certain amount of time),
 * and rest of the timers have their timers adjusted based on time spent between and during call-backs.
 * 1. Extract the top timer and execute its callback function.
 * 2. Find out time spent in callback.
 * 3. Subtract time spent in callback from topTimer's period and set that value to "ticksTillExpire" (if it is a repeating timer).
 * 4. Insert it back into the heap.
 * 5. For all the other timers in the heap, go through each of them and subtract topTimer's period and time spent in callback from their ticksTillExpire value.
 * 6. For the top timer, set the interrupt to go off after the "ticksTillExpire" value. */
void VirtualTimerCallback(void *arg)
{
	UINT32 ticks = 0, startDelay = 0;
	UINT16 i = 0;
	UINT64 currentTicks = 0, startTicks = 0, endTicks = 0;
	UINT64 beginExecTime = 0, endExecTime = 0, totalExecTime = 0;
	UINT64 tickElapsed = 0;
	INT64 ticksTillExpire = 0;

	UINT16 currentHardwareTimerId = gVirtualTimerObject.VT_hardwareTimerId;
	UINT8 currentHardwareTimerIndex = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(currentHardwareTimerId, currentHardwareTimerIndex);
	if(currentHardwareTimerIndex == -1)
		return;

	if(currentHardwareTimerIndex == 0)
	{
		UINT16 currentVirtualTimerCount = gVirtualTimerObject.virtualTimerMapper_0.m_current_timer_id_;
		VirtualTimerInfo* topTimer = gVirtualTimerObject.virtualTimerMapper_0.timerQueue.PeekTop();

		if(!topTimer)
			return;

		if(topTimer->get_m_timer_id() == 1)
			int k = 0;

		startTicks = CPU_Timer_CurrentTicks(g_HardwareTimerIDs[currentHardwareTimerIndex]);

		if(topTimer->get_m_is_running())
			(topTimer->get_m_callback())(NULL);

		endTicks = CPU_Timer_CurrentTicks(g_HardwareTimerIDs[currentHardwareTimerIndex]);

		/*
		// Measure time elapsed since last interrupt fired
		// This is necessary because the hardware timer can have other users and not just the virtual timer layer
		// So if some spurious user set a compare value, it can not be assumed that the minimum timer in the queue
		// just fired. This creates a possibility that the ticks till expire would be negative
		// as timeElapsed can be greater than time set in the compare value
		// is timeElapsed can never be negative which has been proved through testing of the HAL_Time_CurrentTicks
		UINT64 currentTime = CPU_Timer_CurrentTicks(topTimer->get_m_timer_id());
		UINT64 timeElapsed = currentTime - gVirtualTimerObject.virtualTimerMapper[k].get_m_lastQueueAdjustmentTime();
		gVirtualTimerObject.virtualTimerMapper[k].set_m_lastQueueAdjustmentTime(currentTime);

		// Adjusting all timers in the timerQueue (even ones that are not active and not in the queue are adjusted but that won't affect anything). We could iterate and only adjust active ones, but it is not worth the time to check.
		//TimerHelperFunctions::AdjustVirtTimers();
		for(UINT16 j = 0; j < gVirtualTimerObject.virtualTimerMapper[k].m_current_timer_id_; j++)
		{
			gVirtualTimerObject.virtualTimerMapper[k].g_VirtualTimerInfo[j].set_m_ticksTillExpire(gVirtualTimerObject.virtualTimerMapper[k].g_VirtualTimerInfo[j].get_m_ticksTillExpire() - (INT64) timeElapsed);
		}
		*/

		// if the timer is a one shot we don't place it back on the timer Queue
		if (topTimer->get_m_is_one_shot()){
			topTimer->set_m_is_running(FALSE);
			VirtualTimerInfo* tempTopTimer = gVirtualTimerObject.virtualTimerMapper_0.timerQueue.ExtractTop();
		} else {
			//ticks = topTimer->get_m_period();
			//topTimer->set_m_ticksTillExpire(ticks);

			//Subtract time spent in callback while setting ticksTillExpire. Assuming that CurrentTicks does not roll-over (endExecTime - beginExecTime)
			if(endTicks > startTicks)
				tickElapsed = endTicks - startTicks;
			else
				tickElapsed = endTicks + (VirtTimer_GetMaxTicks(i) - startTicks);

			//ticksTillExpire = gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_ticksTillExpire();
			ticks = topTimer->get_m_period();

			topTimer->set_m_ticksTillExpire(ticks - tickElapsed);
		}

		for(i = 0; i < gVirtualTimerObject.virtualTimerMapper_0.VTM_countOfVirtualTimers; i++)
		{
			//AnanthAtSamraksh - a minor optimization. Adjust the ticks only for those timers which are active (get_m_is_running is true)
			if(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_is_running() == TRUE)
			{
				// adjusting m_ticksTillExpire again to take into account time spent in callback
				//Don't adjust the topTimer which we adjusted above
				if(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_timer_id() != topTimer->get_m_timer_id())
				{
					currentTicks = CPU_Timer_CurrentTicks(g_HardwareTimerIDs[currentHardwareTimerIndex]);
					UINT64 tickElapsed = 0;
					UINT64 lastQueueAdjTick = gVirtualTimerObject.virtualTimerMapper_0.get_m_lastQueueAdjustmentTime();
					if(currentTicks > lastQueueAdjTick)
						tickElapsed = currentTicks - lastQueueAdjTick;
					else
						tickElapsed = currentTicks + (VirtTimer_GetMaxTicks(i) - lastQueueAdjTick);

					ticksTillExpire = gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_ticksTillExpire();
					if(ticksTillExpire > tickElapsed)
						int mma1 = 0;
					else
						int mma2 = 0;
					INT64 ticksRemaining = ticksTillExpire - tickElapsed;
					gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].set_m_ticksTillExpire(ticksTillExpire - tickElapsed);
				}
			}
		}

		for(i = 0; i < gVirtualTimerObject.virtualTimerMapper_0.VTM_countOfVirtualTimers; i++)
		{
			if(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_is_running() == TRUE)
			{
				// Callback all timers that need to be served at this point
				// At 48MHz it takes about 1500 ticks or 30us to pass, which is about the time it takes to setup and service a timer;
				// so instead of doing all that we just service the callback now
				if(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_ticksTillExpire() <= 1500)
				{
					startTicks = CPU_Timer_CurrentTicks(g_HardwareTimerIDs[currentHardwareTimerIndex]);
					(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_callback())(NULL);
					endTicks = CPU_Timer_CurrentTicks(g_HardwareTimerIDs[currentHardwareTimerIndex]);

					if(endTicks > startTicks)
						tickElapsed = endTicks - startTicks;
					else
						tickElapsed = endTicks + (VirtTimer_GetMaxTicks(i) - startTicks);

					if(gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_is_one_shot())
						gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].set_m_is_running(FALSE);

					ticks = gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].get_m_period();

					gVirtualTimerObject.virtualTimerMapper_0.g_VirtualTimerInfo[i].set_m_ticksTillExpire(ticks - tickElapsed);
				}
			}
		}

		//heapify
		gVirtualTimerObject.virtualTimerMapper_0.timerQueue.heapify(0);

		topTimer = gVirtualTimerObject.virtualTimerMapper_0.timerQueue.PeekTop();

		// if there is a timer in the timerqueue still we will set the advanced timer to interrupt at the correct time
		if(topTimer)
		{
			CPU_Timer_SetCompare(g_HardwareTimerIDs[currentHardwareTimerIndex], (UINT32)topTimer->get_m_ticksTillExpire() );
		}
		currentTicks = CPU_Timer_CurrentTicks(g_HardwareTimerIDs[currentHardwareTimerIndex]);
		gVirtualTimerObject.virtualTimerMapper_0.set_m_lastQueueAdjustmentTime(currentTicks);

	}

	else if(currentHardwareTimerIndex == 1)
	{

	}
}



