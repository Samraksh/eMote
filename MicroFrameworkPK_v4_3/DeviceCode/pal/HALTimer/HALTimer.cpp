
#include <Samraksh/HALTimer.h>
#include <Samraksh/Hal_util.h>
#include <advancedtim/netmf_advancedtimer.h>

// Assumptions:
// ============
// Assumes that the HAL core hardware timer is running at the same configuration  as the system timer becauses it uses the CPU_MicrosecondsToTicks
// interface
void HALTimerCallback(void *arg);

extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
HALTimerManager gHalTimerManagerObject;

/*void GenerateSoftPendSV_HALTimerInterrupt(){
	//E000ED04
	// *((uint32_t volatile *)0x00000038) = 0x10000000;
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
	// *((uint32_t volatile *)SCB->ICSR = SCB_ICSR_PENDSVSET;
}*/

BOOL HALTimerManager::Initialize()
{
	// Start Up Timer
	// Created a new PAL- HAL to connection to ensure that all code is platform independent
	// HALTIMER is a macro defined under platform_selector.h, defining this anywhere else breaks platform abstraction
	if(g_STM32F10x_AdvancedTimer.Initialize(0, HALTimerCallback, NULL) != DS_Success)
		return DS_Fail;

	// There are not active timers in the system
	//m_active_timer = NULL;

	m_lastInterruptFireTime = 0;
}

// This function takes a timer id as input and changes its state to not running
// This means that the timer does not go back on the queue once its extracted
BOOL HALTimerManager::StopTimer(UINT8 timer_id)
{
	for(UINT16 i = 0; i < m_current_timer_id_; i++)
	{
		if(m_timer[i].get_m_timer_id() == timer_id)
		{
			m_timer[i].set_m_is_running(FALSE);
			return TRUE;
		}
	}

	return FALSE;
}

// This function takes a timer id and dtime as input and changes the corresponding values of the timer
// Note if the timer is currently in the system when this happens, the values are modified only when the timer
// finishes its current instance in the queue and when its added back in it assumes the new values
BOOL HALTimerManager::ChangeTimer(UINT8 timer_id, UINT32 dtime)
{
	UINT32 ticks = CPU_MicrosecondsToTicks(dtime);

	for(UINT16 i = 0; i < m_current_timer_id_; i++)
	{
		if(m_timer[i].get_m_timer_id() == timer_id)
		{
			m_timer[i].set_m_dtime(ticks);

			return TRUE;
		}
	}

	return FALSE;
}

// This function takes a timer id as input and changes the state to running or returns false
// if the timer you specified does not exist
BOOL HALTimerManager::StartTimer(UINT8 timer_id)
{
	for(UINT16 i = 0; i < m_current_timer_id_; i++)
	{
		if(m_timer[i].get_m_timer_id() == timer_id)
		{
			// check to see if we are already running
			if (m_timer[i].get_m_is_running() == TRUE){
				return TRUE;
			}

			m_timer[i].set_m_is_running(TRUE);

			// Insert this back into the queue
			// If this happens to be the smallest, do the necessary bookkeeping and
			// set the new value on the comparator
			if(!timerQueue.Insert(&m_timer[i]))
				return FALSE;

			HALTimer *nextTimer = timerQueue.PeekTop();

			// if the nextTimer to fire is the one we just started then we will set the SetCompare function
		 	if ( nextTimer->get_m_timer_id() == timer_id ){
				UINT64 currentTime = HAL_Time_CurrentTicks();
				UINT64 timeElapsed = currentTime - gHalTimerManagerObject.get_m_lastInterruptFireTime();
				gHalTimerManagerObject.set_m_lastInterruptFireTime(currentTime);

				for(UINT16 j = 0; j < gHalTimerManagerObject.m_current_timer_id_; j++)
				{
					gHalTimerManagerObject.m_timer[j].set_m_ticksTillExpire(gHalTimerManagerObject.m_timer[j].get_m_ticksTillExpire() - (INT64) timeElapsed);
				}

				g_STM32F10x_AdvancedTimer.SetCompare(currentTime, (UINT32)nextTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL HALTimerManager::CreateTimer(UINT8 timer_id, UINT32 start_time, UINT32 dtime, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	// Can not accept anymore timers
	if(m_current_timer_id_ > NUM_HALTIMER_TIMERS)
		return FALSE;

	UINT32 ticks = CPU_MicrosecondsToTicks(dtime);
	bool timerFound = false;

	for(int i = 0; i < m_current_timer_id_; i++)
	{
		if (m_timer[i].get_m_timer_id() == timer_id){
			// This timer already exists....changing it.
			m_timer[i].set_m_callBack(callback);
			m_timer[i].set_m_dtime(ticks);
			m_timer[i].set_m_is_one_shot(is_one_shot);
			m_timer[i].set_m_is_running(FALSE);
			m_timer[i].set_m_reserved(_isreserved);
			m_timer[i].set_m_start_timer(start_time);
			m_timer[i].set_m_timer_id(timer_id);
			m_timer[i].set_m_ticksTillExpire(ticks);
			timerFound == true;
		} 
	}
	if (timerFound == false){
		// Add this timer to the timer array
		m_timer[m_current_timer_id_].set_m_callBack(callback);
		m_timer[m_current_timer_id_].set_m_dtime(ticks);
		m_timer[m_current_timer_id_].set_m_is_one_shot(is_one_shot);
		m_timer[m_current_timer_id_].set_m_is_running(FALSE);
		m_timer[m_current_timer_id_].set_m_reserved(_isreserved);
		m_timer[m_current_timer_id_].set_m_start_timer(start_time);
		m_timer[m_current_timer_id_].set_m_timer_id(timer_id);
		m_timer[m_current_timer_id_].set_m_ticksTillExpire(ticks);

		m_current_timer_id_++;
	}
	/*HALTimer *nextTimer = timerQueue.PeekTop();

	if(m_active_timer == NULL)
	{
		m_lastInterruptFireTime = HAL_Time_CurrentTicks();
		g_STM32F10x_AdvancedTimer.SetCompare(0, nextTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);
		m_active_timer = nextTimer;
	}*/
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

void HALTimerCallback(void *arg)
{
	UINT32 ticks;

	HALTimer* topTimer = gHalTimerManagerObject.timerQueue.PeekTop();

	if(!topTimer)
		return;

	// Measure time elapsed since last interrupt fired
	// This is necessary because the hardware timer can have other users and not just the virtual timer layer
	// So if some spurious user set a compare value, it can not be assumed that the minimum timer in the queue
	// just fired. This creates a possibility that the ticks till expire would be negative
	// as timeElapsed can be greater than time set in the compare value
	// is timeElapsed can never be negative which has been proved through testing of the HAL_Time_CurrentTicks
	UINT64 currentTime = HAL_Time_CurrentTicks();
	UINT64 timeElapsed = currentTime - gHalTimerManagerObject.get_m_lastInterruptFireTime();
	gHalTimerManagerObject.set_m_lastInterruptFireTime(currentTime);

	for(UINT16 i = 0; i < gHalTimerManagerObject.m_current_timer_id_; i++)
	{
		//gHalTimerManagerObject.m_timer[i].set_m_ticksTillExpire(gHalTimerManagerObject.m_timer[i].get_m_ticksTillExpire() - g_STM32F10x_AdvancedTimer.GetCounter());
		gHalTimerManagerObject.m_timer[i].set_m_ticksTillExpire(gHalTimerManagerObject.m_timer[i].get_m_ticksTillExpire() - (INT64) timeElapsed);
	}

	// Callback all timers that need to be served at this point
	while( (topTimer->get_m_ticksTillExpire() - (INT64) timeElapsed) <= 1500)
	{
		topTimer = gHalTimerManagerObject.timerQueue.ExtractTop();

		// Check if the timer has been disabled
		if(topTimer != NULL)
		{
			if(topTimer->get_m_is_running())
				(topTimer->get_m_callback())(NULL);
		}

		if (topTimer->get_m_is_one_shot()){
			topTimer->set_m_is_running(FALSE);
		} else {
			ticks = topTimer->get_m_dtime();

			topTimer->set_m_ticksTillExpire(ticks);

			// Check if timer deserves to go back or if someone has made it inactive
			if(topTimer->get_m_is_running())
				gHalTimerManagerObject.timerQueue.Insert(topTimer);
		}
		topTimer = gHalTimerManagerObject.timerQueue.PeekTop();

		currentTime = HAL_Time_CurrentTicks();
		timeElapsed = currentTime - gHalTimerManagerObject.get_m_lastInterruptFireTime();
		gHalTimerManagerObject.set_m_lastInterruptFireTime(currentTime);	
	}

	// adjusting m_ticksTillExpire again to take into account time spent in callbacks	
	for(UINT16 i = 0; i < gHalTimerManagerObject.m_current_timer_id_; i++)
	{
		gHalTimerManagerObject.m_timer[i].set_m_ticksTillExpire(gHalTimerManagerObject.m_timer[i].get_m_ticksTillExpire() - (INT64) timeElapsed);
	}

	// Factoring time spent in the handler
	if(topTimer)
	{
		//g_STM32F10x_AdvancedTimer.SetCompare(*((UINT32 *)arg), (UINT32)topTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);
		g_STM32F10x_AdvancedTimer.SetCompare(currentTime, (UINT32)topTimer->get_m_ticksTillExpire(), SET_COMPARE_TIMER);
	}
}


#if 0 // HALTimer older implementation deprecated
BOOL HALTimerManager::Initialize()
{

	// Initialize all existing timer id maps numbers to ~0, dont want to use 0 incase user want to use it
	for(int i = 0; i < NUM_HALTIMER_TIMERS; i++)
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

	UINT8* active_timer = gHalTimerManagerObject.GetActiveTimerList();

	for(int i = 0; i < gHalTimerManagerObject.get_m_number_of_active_timers(); i++)
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
	if(timer_index < NUM_HALTIMER_TIMERS){
		//it exist, check if its  not running, and reuse timer
		if(!m_timer[timer_index].get_m_is_running()){
			ticks = CPU_MicrosecondsToTicks(dtime);
			m_timer[timer_index].set_m_callBack(callback);
			m_timer[timer_index].set_m_dtime(ticks);
			m_timer[timer_index].set_m_is_one_shot(is_one_shot);
			m_timer[timer_index].set_m_is_running(TRUE);
			m_timer[timer_index].set_m_reserved(_isreserved);
			m_timer[timer_index].set_m_start_timer(start_time);
			m_timer[timer_index].set_m_timer_id(timer_id);
			m_timer[timer_index].set_m_ticksTillExpire(ticks);
		}else { //it exist and its running, return FALSE
			return FALSE;
		}
	}else { //timer does not exist, create a new one.
		// More than 32 timers have been created, either try and reclaim or return false
		if(m_current_timer_id_ > NUM_HALTIMER_TIMERS)
			return FALSE;

		ticks = CPU_MicrosecondsToTicks(dtime);

		m_timer[m_current_timer_id_].set_m_callBack(callback);
		m_timer[m_current_timer_id_].set_m_dtime(ticks);
		m_timer[m_current_timer_id_].set_m_is_one_shot(is_one_shot);
		m_timer[m_current_timer_id_].set_m_is_running(TRUE);
		m_timer[m_current_timer_id_].set_m_reserved(_isreserved);
		m_timer[m_current_timer_id_].set_m_start_timer(start_time);
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

	HALTimer* timers = gHalTimerManagerObject.get_m_timer();
	UINT8* active_timer = gHalTimerManagerObject.GetActiveTimerList();
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

	for(int i = 0; i < gHalTimerManagerObject.get_m_number_of_active_timers(); i++)
	{
		if(timers[active_timer[i]].get_m_is_running())
		{
			if(timers[active_timer[i]].get_m_ticksTillExpire() > (gHalTimerManagerObject.get_timer_resolution()+ (250 + 20 * i)))
			{
				// Update the ticks to expire
				// What are 250 and 20 ?
				// They are a rough estimation of the number of ticks expired since the hardware handler was fired
				// to this point. The actual value of skew calculation using the current counter values seem to give
				// less accurate results than these assumed hardcoded values.
				// Will revisit this issue if the performance is bad !!
				timers[active_timer[i]].set_m_ticksTillExpire(timers[active_timer[i]].get_m_ticksTillExpire() - (gHalTimerManagerObject.get_timer_resolution() + (250 + 20 * i)));
				//timers[active_timer[i]].set_m_ticksTillExpire(timers[active_timer[i]].get_m_ticksTillExpire() - (gHalTimerManagerObject.get_timer_resolution() + skew));
			}
			else
			{
				// Make the callback
				(timers[active_timer[i]].get_m_callback())(NULL);
				// Reset if timer is complete
		        if(timers[active_timer[i]].get_m_is_one_shot()){
		             timers[active_timer[i]].set_m_is_running(false);
		        }

				timers[active_timer[i]].set_m_ticksTillExpire(timers[active_timer[i]].get_m_dtime());
			}
		}
	}

	// Adding the ability to be able to run at high resolution if needed
	if(gHalTimerManagerObject.get_highResolutionMode())
	{
		CPU_TIMER_SetCompare(HALTIMER, (UINT16)gHalTimerManagerObject.get_timer_resolution());
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
				m_timer[i].set_m_ticksTillExpire(m_timer[i].get_m_dtime());
			}
		}
	}


}
#endif
#endif

