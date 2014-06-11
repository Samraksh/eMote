
#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Hal_util.h>
////#include <Timer/advancedtim/netmf_advancedtimer.h>
#include <time/netmf_time.h>

// Assumptions:
// ============
// Assumes that the HAL core hardware timer is running at the same configuration  as the system timer becauses it uses the CPU_MicrosecondsToTicks
// interface
void VirtualTimerCallback(void *arg);

extern const UINT8 g_CountOfHardwareTimers;
extern VirtualTimer gVirtualTimerObject;
VirtualTimerMapper gVirtualTimerMapperObject;



// All active timers are placed in a timerQueue. Every time one is inserted, the list is adjusted in order with the timer that will go off soonest on top and the latest on bottom
// Every time the list is changed the m_ticksTillExpire must be adjusted to reflect the current number of ticks before the topmost timer needs to have its callback called.
// After this adjustment the new timer can be inserted where it will be automatically placed in the correct spot in the queue.
// The SetCompare timer then needs to be set appropriately

////BOOL VirtualTimerMapper::Initialize(VirtualTimerConfigInitialize::VirtualTimerConfig VTconfig)
BOOL VirtualTimerMapper::Initialize(UINT16 temp_HWID, UINT16 temp_countVTimers)
{
	VTM_hardwareTimerId = VTM_countOfVirtualTimers = 0;

	VTM_hardwareTimerId = temp_HWID;
	VTM_countOfVirtualTimers = temp_countVTimers;

	m_lastQueueAdjustmentTime = 0;

	// Start Up Timer
	// Created a new PAL- HAL to connection to ensure that all code is platform independent
	// HALTIMER is a macro defined under platform_selector.h, defining this anywhere else breaks platform abstraction

	if(!CPU_TIMER_Initialize(VTM_hardwareTimerId, FALSE, 0, 0, VirtualTimerCallback, NULL))
		return FALSE;
	else
		return TRUE;

}


// This function takes a timer id as input and changes its state to not running
// This means that the timer does not go back on the queue once its extracted
BOOL VirtualTimerMapper::StopTimer(UINT8 timer_id)
{
	if (timer_id < 0 || timer_id > m_current_timer_id_)
		return FALSE;

	g_VirtualTimerInfo[timer_id].set_m_is_running(FALSE);
	return TRUE;
}

// This function takes a timer id and dtime as input and changes the corresponding values of the timer
// Note if the timer is currently in the system when this happens, the values are modified only when the timer
// finishes its current instance in the queue and when its added back in it assumes the new values
BOOL VirtualTimerMapper::ChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot)
{
	if (timer_id < 0 || timer_id > m_current_timer_id_)
		return FALSE;

	UINT32 ticksPeriod = CPU_MicrosecondsToTicks(period);
	UINT32 ticksStartDelay = CPU_MicrosecondsToTicks(start_delay);

	g_VirtualTimerInfo[timer_id].set_m_start_delay(ticksStartDelay);
	g_VirtualTimerInfo[timer_id].set_m_period(ticksPeriod);
	g_VirtualTimerInfo[timer_id].set_m_is_one_shot(is_one_shot);

	return TRUE;
}

// This function takes a timer id as input and changes the state to running or returns false
// if the timer you specified does not exist
BOOL VirtualTimerMapper::StartTimer(UINT8 timer_id)
{
	if (timer_id < 0 || timer_id > m_current_timer_id_)
		return FALSE;

	// check to see if we are already running
	if (g_VirtualTimerInfo[timer_id].get_m_is_running() == TRUE){
		return TRUE;
	}

	g_VirtualTimerInfo[timer_id].set_m_is_running(TRUE);

	// Adjusting all timers in the timerQueue (even ones that are not active and not in the queue are adjusted but that won't affect anything). We could iterate and only adjust active ones, but it is not worth the time to check.
	////UINT64 currentTime = CPU_Timer_CurrentTicks(timer_id);
	UINT64 currentTime = CPU_Timer_CurrentTicks(VTM_hardwareTimerId);
	UINT64 timeElapsed = currentTime - get_m_lastQueueAdjustmentTime();
	set_m_lastQueueAdjustmentTime(currentTime);

	//TimerHelperFunctions::AdjustVirtTimers();
	for(UINT16 i = 0; i < m_current_timer_id_; i++)
	{
		//Adjust only active timers
		if (g_VirtualTimerInfo[i].get_m_is_running() == TRUE)
			g_VirtualTimerInfo[i].set_m_ticksTillExpire(g_VirtualTimerInfo[i].get_m_ticksTillExpire() - (INT64) timeElapsed);
	}

	// Initializing timer
	//TODO: AnanthAtSamraksh -- below should not be commented out, as ticksTillExpire needs to be set to right value due to above adjustment
	UINT32 ticks;
	ticks = g_VirtualTimerInfo[timer_id].get_m_period();
	//g_VirtualTimerInfo[timer_id].set_m_ticksTillExpire(ticks + CPU_MicrosecondsToTicks(g_VirtualTimerInfo[timer_id].get_m_start_delay()));
	g_VirtualTimerInfo[timer_id].set_m_ticksTillExpire(ticks + g_VirtualTimerInfo[timer_id].get_m_start_delay());


	// Insert this into the queue
	// If this happens to be the smallest, do the necessary bookkeeping and
	// set the new value on the comparator
	if( !timerQueue.Insert( &g_VirtualTimerInfo[timer_id] ) )
		return FALSE;

	VirtualTimerInfo *nextTimer = timerQueue.PeekTop();

	// if the nextTimer to fire is the one we just started then we will set the SetCompare function
	if ( nextTimer->get_m_timer_id() == timer_id )
	{
		////g_STM32F10x_AdvancedTimer.SetCompare(HAL_Time_CurrentTicks(), (UINT32)nextTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);
		////CPU_TIMER_SetCompare(VTM_hardwareTimerId, timer_id, CPU_Timer_CurrentTicks(timer_id), (UINT32)nextTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);

		CPU_TIMER_SetCompare(VTM_hardwareTimerId, (UINT32)nextTimer->get_m_ticksTillExpire() );
		////HAL_Time_SetCompare((UINT32)nextTimer->get_m_ticksTillExpire());
	}

	return TRUE;
}

BOOL VirtualTimerMapper::SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	// Can not accept anymore timers
	if(m_current_timer_id_ > VTM_countOfVirtualTimers)
		return FALSE;

	if (timer_id < 0 || timer_id > m_current_timer_id_)
		return FALSE;


	//UINT32 ticks = CPU_MicrosecondsToTicks(period);
	UINT32 ticksPeriod = CPU_MicrosecondsToTicks(period);
	UINT32 ticksStartDelay = CPU_MicrosecondsToTicks(start_delay);
	//bool timerFound = false;

	if(g_VirtualTimerInfo[timer_id].get_m_timer_id() == timer_id)
	{
		// This timer already exists....changing it.
		g_VirtualTimerInfo[timer_id].set_m_callBack(callback);
		//g_VirtualTimerInfo[j].set_m_period(ticks);
		g_VirtualTimerInfo[timer_id].set_m_period(ticksPeriod);
		g_VirtualTimerInfo[timer_id].set_m_is_one_shot(is_one_shot);
		g_VirtualTimerInfo[timer_id].set_m_is_running(FALSE);
		g_VirtualTimerInfo[timer_id].set_m_reserved(_isreserved);
		//g_VirtualTimerInfotimer_idj].set_m_start_delay(start_delay);
		g_VirtualTimerInfo[timer_id].set_m_start_delay(ticksStartDelay);
		g_VirtualTimerInfo[timer_id].set_m_timer_id(timer_id);
		//g_VirtualTimerInfo[timer_id].set_m_ticksTillExpire(ticks);
		g_VirtualTimerInfo[timer_id].set_m_ticksTillExpire(ticksPeriod + ticksStartDelay);
		//timerFound == true;
	}
	else
	{
		g_VirtualTimerInfo[m_current_timer_id_].set_m_callBack(callback);
		//g_VirtualTimerInfo[m_current_timer_id_].set_m_period(ticks);
		g_VirtualTimerInfo[m_current_timer_id_].set_m_period(ticksPeriod);
		g_VirtualTimerInfo[m_current_timer_id_].set_m_is_one_shot(is_one_shot);
		g_VirtualTimerInfo[m_current_timer_id_].set_m_is_running(FALSE);
		g_VirtualTimerInfo[m_current_timer_id_].set_m_reserved(_isreserved);
		//g_VirtualTimerInfo[m_current_timer_id_].set_m_start_delay(start_delay);
		g_VirtualTimerInfo[m_current_timer_id_].set_m_start_delay(ticksStartDelay);
		g_VirtualTimerInfo[m_current_timer_id_].set_m_timer_id(timer_id);
		//g_VirtualTimerInfo[m_current_timer_id_].set_m_ticksTillExpire(ticks);
		g_VirtualTimerInfo[m_current_timer_id_].set_m_ticksTillExpire(ticksPeriod + ticksStartDelay);
	}

	m_current_timer_id_++;


#if 0
	// A new timer enters the system that is now the smallest timer in the system
	else if(nextTimer->get_m_timer_id() != m_active_timer->get_m_timer_id())
	{
		g_STM32F10x_AdvancedTimer.SetCompare(0, nextTimer->get_m_ticksTillExpire());
		m_active_timer = nextTimer;
		// The new timer does not need to be fixed
		for(UINT16 i = 1; i < m_current_timer_id_; i++)
		{
			m_timer[i].set_m_ticksTillExpire(m_timer[i].get_m_ticksTillExpire() - g_STM32F10x_AdvancedTimer.GetCounter());
		}
	}
#endif

	return TRUE;
}

/* Algorithm for the callback:
 * 1. Extract the top timer and execute its callback function.
 * 2. Then reset its "ticksTillExpire" value.
 * 3. Insert it back into the heap.
 * 4. For all the timers in the heap, go through each of them and adjust their ticksTillExpire value based on the amount of time spent in the callback function.
 * 5. For the top timer, set the interrupt to go off after the "ticksTillExpire" value. */
void VirtualTimerCallback(void *arg)
{
	UINT32 ticks;
	UINT16 i = 0;
	//VirtualTimerInfo newTopTimer;

	UINT16 currentHardwareTimerId = gVirtualTimerObject.VT_hardwareTimerId;
	UINT16 currentVirtualTimerId = gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].m_current_timer_id_;

	//VirtualTimerInfo* topTimer = gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].timerQueue.PeekTop();
	VirtualTimerInfo* topTimer = gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].timerQueue.ExtractTop();

	if(!topTimer)
		return;

	UINT64 currentTime = CPU_Timer_CurrentTicks(currentHardwareTimerId);
	gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].set_m_lastQueueAdjustmentTime(currentTime);

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

	// Callback all timers that need to be served at this point
	// At 48MHz it takes about 1500 ticks for 30us to pass, which is about the time it takes to setup and service a timer, so instead of doing all that we just service the callback now
	//TODO: AnanthAtSamraksh - commented out the while loop
	//while( (topTimer->get_m_ticksTillExpire()) <= 1500)
	//{
		topTimer = gVirtualTimerObject.virtualTimerMapper[k].timerQueue.ExtractTop();
	*/

		/*// Check if the timer has been disabled
		if(topTimer != NULL)
		{
			if(topTimer->get_m_is_running())
				(topTimer->get_m_callback())(NULL);
		}
		else {
			// queue is empty, exit.
			break;
		}*/

	// Check if the timer has been disabled
	if(topTimer != NULL)
	{
		if(topTimer->get_m_is_running())
			(topTimer->get_m_callback())(NULL);
	}


		// if the timer is a one shot we don't place it back on the timer Queue
		/*if(topTimer != NULL) {
			if (topTimer->get_m_is_one_shot()){
				topTimer->set_m_is_running(FALSE);
			} else {
				ticks = topTimer->get_m_dtime();

				topTimer->set_m_ticksTillExpire(ticks + CPU_MicrosecondsToTicks(topTimer->get_m_start_time()));*/

		/*if (topTimer->get_m_is_one_shot()){
			topTimer->set_m_is_running(FALSE);
		} else {
			ticks = topTimer->get_m_period();*/

	// if the timer is a one shot we don't place it back on the timer Queue
	if (topTimer->get_m_is_one_shot()){
		topTimer->set_m_is_running(FALSE);
	} else {
		ticks = topTimer->get_m_period();
		/*topTimer->set_m_ticksTillExpire(ticks + CPU_MicrosecondsToTicks(topTimer->get_m_start_delay()));*/

		//topTimer->set_m_ticksTillExpire(ticks + CPU_MicrosecondsToTicks(topTimer->get_m_start_delay()));
		topTimer->set_m_ticksTillExpire(ticks + topTimer->get_m_start_delay());


				/*// Check if timer deserves to go back or if someone has made it inactive
				if(topTimer->get_m_is_running())
					gHalTimerManagerObject.timerQueue.Insert(topTimer);
			}*/
			/*// Check if timer deserves to go back or if someone has made it inactive
			if(topTimer->get_m_is_running())
				gVirtualTimerObject.virtualTimerMapper[k].timerQueue.Insert(topTimer);*/

		// Check if timer deserves to go back or if someone has made it inactive
		if(topTimer->get_m_is_running()){
			//gVirtualTimerObject.virtualTimerMapper[k].timerQueue.Insert(topTimer);
			gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].timerQueue.Insert(topTimer);
		}
	}

	while(i < currentVirtualTimerId)
	{
		//VirtualTimerInfo newTopTimer;
		//BOOL timerExpired = FALSE;
		currentTime = CPU_Timer_CurrentTicks(currentHardwareTimerId);
		UINT64 timeElapsed = currentTime - gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].get_m_lastQueueAdjustmentTime();
		gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].set_m_lastQueueAdjustmentTime(currentTime);

		// adjusting m_ticksTillExpire again to take into account time spent in callback
		//for(; i < currentVirtualTimerId; i++)
		//{
			//gVirtualTimerObject.virtualTimerMapper[k].g_VirtualTimerInfo[i].set_m_ticksTillExpire(gVirtualTimerObject.virtualTimerMapper[k].g_VirtualTimerInfo[i].get_m_ticksTillExpire() - (INT64) timeElapsed);
			//TODO: AnanthAtSamraksh - a minor optimization. Adjust the ticks only for those timers which are active (get_m_is_running is true)
			if(gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].g_VirtualTimerInfo[i].get_m_is_running())
				gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].g_VirtualTimerInfo[i].set_m_ticksTillExpire(gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].g_VirtualTimerInfo[i].get_m_ticksTillExpire() - (INT64) timeElapsed);

			//if(gVirtualTimerObject.virtualTimerMapper[k].g_VirtualTimerInfo[i].get_m_ticksTillExpire() <= 1000)
			if(gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].g_VirtualTimerInfo[i].get_m_ticksTillExpire() <= 1500)
			{
				//timerExpired = TRUE;
				//newTopTimer = gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].g_VirtualTimerInfo[i];
				if(gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].g_VirtualTimerInfo[i].get_m_is_running())
					(gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].g_VirtualTimerInfo[i].get_m_callback())(NULL);
				//break;
			}
		//}

		//if(timerExpired == TRUE)
		//{
			//topTimer = gVirtualTimerObject.virtualTimerMapper[k].timerQueue.PeekTop();
			//topTimer = gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].timerQueue.PeekTop();
			//if(newTopTimer)
			//{
				//if(newTopTimer.get_m_is_running())
					//(newTopTimer.get_m_callback())(NULL);
			//}
			//timerExpired = FALSE;
		//}
		//else
			//break;
		i++;
	}

	topTimer = gVirtualTimerObject.virtualTimerMapper[currentHardwareTimerId - 1].timerQueue.PeekTop();
	//}

	// if there is a timer in the timerqueue still we will set the advanced timer to interrupt at the correct time
	if(topTimer)
	{
		//g_STM32F10x_AdvancedTimer.SetCompare(HAL_Time_CurrentTicks(), (UINT32)topTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);
		//CPU_Timer_SetCompare(CPU_Timer_CurrentTicks(timer_id), (UINT32)topTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);
		////HAL_Time_SetCompare((UINT32)topTimer->get_m_ticksTillExpire());
		//CPU_TIMER_SetCompare(gVirtualTimerObject.virtualTimerMapper[k].VTM_hardwareTimerId, (UINT32)topTimer->get_m_ticksTillExpire() );
		CPU_TIMER_SetCompare(currentHardwareTimerId, (UINT32)topTimer->get_m_ticksTillExpire() );
	}
	//}
}


#if 0 // HALTimer older implementation deprecated
BOOL HALTimerManager::Initialize()
{

	// Initialize all existing timer id maps numbers to ~0, dont want to use 0 incase user want to use it
	for(int i = 0; i < NUM_VIRTUALTIMER_TIMERS; i++)
	{
		m_timer_id_map[i] = ~0;
	}

	// Start Up Timer
	// Created a new PAL- HAL to connection to ensure that all code is platform independent
	// HALTIMER is a macro defined under platform_selector.h, defining this anywhere else breaks platform abstraction
	if(!CPU_TIMER_Initialize(HALTIMER, TRUE, 0, 0, HALTimerCallback, NULL))
		return FALSE;

	// Number of active timers
	m_number_active_timers_ = 0;

	// Dont  set high resolution mode initially
	m_highResolutionModeEnabled = false;

	// Get ticks for the time specified in Us
	// HALTIMER_RESOLUTION_USEC defined in platform_selector.h
	timer_resolution_in_ticks = CPU_MicrosecondsToTicks((UINT32) HALTIMER_RESOLUTION_USEC);

	// Set compare value for the timer
	if(!CPU_TIMER_SetCompare(HALTIMER, (UINT16)timer_resolution_in_ticks))
		return FALSE;

	return TRUE;
}

BOOL HALTimerManager::StopTimer(UINT8 timer_id)
{
	GLOBAL_LOCK(irq);

	UINT8* active_timer = gVirtualTimerObject.GetActiveTimerList();

	for(int i = 0; i < gVirtualTimerObject.get_m_number_of_active_timers(); i++)
	{
		if(m_timer[active_timer[i]].get_m_timer_id() == timer_id)
			m_timer[active_timer[i]].set_m_is_running(false);
	}

	return TRUE;
}

BOOL HALTimerManager::CreateTimer(UINT8 timer_id, UINT32 start_time, UINT32 dtime, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	GLOBAL_LOCK(irq);
	UINT32 ticks=0;

	// Enable high resolution mode
	if(dtime < HALTIMER_RESOLUTION_USEC)
	{
		timer_resolution_in_ticks = CPU_MicrosecondsToTicks((UINT32) dtime);
		m_highResolutionModeEnabled = TRUE;
	}

	//__ASM volatile("cpsid i");
	// Unable to create requested timer because a timer already exists with this id
	UINT8 timer_index = this->DoesTimerExist(timer_id);
	if(timer_index < NUM_VIRTUALTIMER_TIMERS){
		//it exist, check if its  not running, and reuse timer
		if(!m_timer[timer_index].get_m_is_running()){
			ticks = CPU_MicrosecondsToTicks(dtime);
			m_timer[timer_index].set_m_callBack(callback);
			m_timer[timer_index].set_m_period(ticks);
			m_timer[timer_index].set_m_is_one_shot(is_one_shot);
			m_timer[timer_index].set_m_is_running(TRUE);
			m_timer[timer_index].set_m_reserved(_isreserved);
			m_timer[timer_index].set_m_start_delay(start_time);
			m_timer[timer_index].set_m_timer_id(timer_id);
			m_timer[timer_index].set_m_ticksTillExpire(ticks);
		}else { //it exist and its running, return FALSE
			return FALSE;
		}
	}else { //timer does not exist, create a new one.
		// More than 32 timers have been created, either try and reclaim or return false
		if(m_current_timer_id_ > NUM_VIRTUALTIMER_TIMERS)
			return FALSE;

		ticks = CPU_MicrosecondsToTicks(dtime);

		m_timer[m_current_timer_id_].set_m_callBack(callback);
		m_timer[m_current_timer_id_].set_m_period(ticks);
		m_timer[m_current_timer_id_].set_m_is_one_shot(is_one_shot);
		m_timer[m_current_timer_id_].set_m_is_running(TRUE);
		m_timer[m_current_timer_id_].set_m_reserved(_isreserved);
		m_timer[m_current_timer_id_].set_m_start_delay(start_time);
		m_timer[m_current_timer_id_].set_m_timer_id(timer_id);
		m_timer[m_current_timer_id_].set_m_ticksTillExpire(ticks);

		this->m_active_timer[m_number_active_timers_++] = m_current_timer_id_;

		m_current_timer_id_++;
	}
	//__ASM volatile("cpsie i");
	return TRUE;

}

void HALTimerCallback(void *arg)
{
	// Modifying a private member outside the object is ugly

	HALTimer* timers = gVirtualTimerObject.get_m_timer();
	UINT8* active_timer = gVirtualTimerObject.GetActiveTimerList();
	//UINT16* correction = (UINT16 *) arg;
	//UINT16 currentValue = CPU_TIMER_GetCounter(HALTIMER);
	//UINT16 skew = (currentValue > *correction) ? (currentValue - *correction) : (~0 - *correction + currentValue);

	// O(Number of Active Timers) update, not very impressive but no way around !!!
	// This means that the HALTimer performance deteriorates as the number of simulated timers increases.
	// Alternate implementation would be to use a priority queue with the smallest compare value at the head of
	// the queue. This will reduce the number of times the handler gets handled and will be called only when
	// the dtime expires. But this makes createTimer interface more complicated as timers that enter the
	// system late with smaller values need to be accounted for. But, even in this implementation  the handler
	// will have O(n) updates and then O(1) extract min timer

	for(int i = 0; i < gVirtualTimerObject.get_m_number_of_active_timers(); i++)
	{
		if(timers[active_timer[i]].get_m_is_running())
		{
			if(timers[active_timer[i]].get_m_ticksTillExpire() > (gVirtualTimerObject.get_timer_resolution()+ (250 + 20 * i)))
			{
				// Update the ticks to expire
				// What are 250 and 20 ?
				// They are a rough estimation of the number of ticks expired since the hardware handler was fired
				// to this point. The actual value of skew calculation using the current counter values seem to give
				// less accurate results than these assumed hardcoded values.
				// Will revisit this issue if the performance is bad !!
				timers[active_timer[i]].set_m_ticksTillExpire(timers[active_timer[i]].get_m_ticksTillExpire() - (gVirtualTimerObject.get_timer_resolution() + (250 + 20 * i)));
				//timers[active_timer[i]].set_m_ticksTillExpire(timers[active_timer[i]].get_m_ticksTillExpire() - (gVirtualTimerObject.get_timer_resolution() + skew));
			}
			else
			{
				// Make the callback
				(timers[active_timer[i]].get_m_callback())(NULL);
				// Reset if timer is complete
		        if(timers[active_timer[i]].get_m_is_one_shot()){
		             timers[active_timer[i]].set_m_is_running(false);
		        }

				timers[active_timer[i]].set_m_ticksTillExpire(timers[active_timer[i]].get_m_period());
			}
		}
	}

	// Adding the ability to be able to run at high resolution if needed
	if(gVirtualTimerObject.get_highResolutionMode())
	{
		CPU_TIMER_SetCompare(HALTIMER, (UINT16)gVirtualTimerObject.get_timer_resolution());
	}
}

#if 0
void HALTimerManager::HALTimerCallback(void *arg)
{
	// O(n) update, not very impressive may be use a different data structure ??
	for(int i = 0; i < m_current_timer_id_; i++)
	{
		if(m_timer[i].get_m_is_running())
		{
			if(m_timer[i].get_m_ticksTillExpire() > timer_resolution_in_ticks)
			{
				m_timer[i].set_m_ticksTillExpire(m_timer[i].get_m_ticksTillExpire() - timer_resolution_in_ticks);
			}
			else
			{

				(m_timer[i].get_m_callback())(NULL);
				m_timer[i].set_m_ticksTillExpire(m_timer[i].get_m_period());
			}
		}
	}


}
#endif
#endif

