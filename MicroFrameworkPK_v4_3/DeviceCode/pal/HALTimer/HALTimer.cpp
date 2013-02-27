
#include <Samraksh/HALTimer.h>

// Assumptions:
// ============
// Assumes that the HAL core hardware timer is running at the same configuration  as the system timer becauses it uses the CPU_MicrosecondsToTicks
// interface
void HALTimerCallback(void *arg);

/*void GenerateSoftPendSV_HALTimerInterrupt(){
	//E000ED04
	// *((uint32_t volatile *)0x00000038) = 0x10000000;
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
	// *((uint32_t volatile *)SCB->ICSR = SCB_ICSR_PENDSVSET;
}
*/

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

	// Get ticks for the time specified in Us
	// HALTIMER_RESOLUTION_USEC defined in platform_selector.h
	timer_resolution_in_ticks = CPU_MicrosecondsToTicks((UINT32) HALTIMER_RESOLUTION_USEC);

	// Set compare value for the timer
	if(!CPU_TIMER_SetCompare(HALTIMER, (UINT16)timer_resolution_in_ticks))
		return FALSE;

	return TRUE;
}

BOOL HALTimerManager::CreateTimer(UINT8 timer_id, UINT32 start_time, UINT32 dtime, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	GLOBAL_LOCK(irq);
	UINT32 ticks=0;
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


