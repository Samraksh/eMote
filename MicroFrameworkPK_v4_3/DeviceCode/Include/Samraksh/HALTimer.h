/*
 *	Name		:		HALTimer.h
 *
 *  Author		:		nived.sivadas@samraksh.com
 *
 *	Description :
 */

#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_

#ifdef _RUN_ON_WINDOWS_
#include "WindowsUtil.h"
#else
#include <tinyhal.h>
#endif

// Defines the number of virtual timers in the system. Increasing this number is fine if you have a large amount of RAM

// Defines the signature of the timer callback function
typedef void (*TIMER_CALLBACK_FPN)( void* arg );

class HALTimer
{
	// The Id referenced by the use of the timer
	UINT8 m_timer_id_;

	// The expected start time of the timer
	UINT32 m_start_time_;

	// What is the delta after which the timer should fire
	UINT32 m_dtime_;

	// Is the timer oneshot ?? or repetitive ??
	BOOL   m_is_one_shot_;

	// Is the timer running, set this to false if you want to invalidate the timer
	BOOL   m_is_running_;

	// Reserve the timer
	BOOL   m_reserved_;

	// maintians the number of ticks left to expire
	UINT64   m_ticksTillExpire_;

	// A pointer to the function that will be called on an interrupt
	TIMER_CALLBACK_FPN m_callBack_;

public:
	// Mutators for all the attributes of HALTimer Object

	void set_m_ticksTillExpire(UINT64 d)
	{
		m_ticksTillExpire_ = d;

	}

	UINT64 get_m_ticksTillExpire()
	{
		return m_ticksTillExpire_;
	}

	void set_m_timer_id(UINT8 timer_id)
	{
		m_timer_id_ = timer_id;
	}

	UINT8 get_m_timer_id()
	{
		return m_timer_id_;
	}

	void set_m_start_timer(UINT32 start_time)
	{
		m_start_time_ = start_time;
	}

	UINT32 get_m_start_time()
	{
		return m_start_time_;
	}

	void set_m_dtime(UINT32 dtime)
	{
		m_dtime_ = dtime;
	}

	UINT32 get_m_dtime()
	{
		return m_dtime_;
	}

	void set_m_is_one_shot(BOOL is_one_shot)
	{
		m_is_one_shot_ = is_one_shot;
	}

	BOOL get_m_is_one_shot()
	{
		return m_is_one_shot_;
	}

	void set_m_is_running(BOOL is_running)
	{
		m_is_running_ = is_running;
	}

	BOOL get_m_is_running()
	{
		return m_is_running_;
	}

	void set_m_callBack(TIMER_CALLBACK_FPN callback)
	{
		m_callBack_ = callback;
	}

	TIMER_CALLBACK_FPN get_m_callback()
	{
		return m_callBack_;
	}

	void set_m_reserved(bool reserved)
	{
		m_reserved_ = reserved;
	}

	BOOL get_m_reserved()
	{
		return m_reserved_;
	}
};

class HALTimerManager
{
	HALTimer m_timer[NUM_HALTIMER_TIMERS];

	UINT8 m_timer_id_map[NUM_HALTIMER_TIMERS];

	UINT8 m_active_timer[NUM_HALTIMER_TIMERS];

	UINT8 m_number_active_timers_;

	static UINT8 m_current_timer_id_;

	UINT64 timer_resolution_in_ticks;


	UINT8 DoesTimerExist(UINT8 id)
	{
		for(int i = 0; i < m_current_timer_id_; i++)
		{
			// Uncomment once reclaim strategy is clear
			//if(m_timer[i].get_m_timer_id() == id && m_timer[id].get_m_is_running() == TRUE)
			if(m_timer[i].get_m_timer_id() == id)
				return i;

		}
		return NUM_HALTIMER_TIMERS+1;
	}

public:

	UINT8* GetActiveTimerList()
	{
		return m_active_timer;
	}

	UINT8 get_m_number_of_active_timers()
	{
		return m_number_active_timers_;
	}

	HALTimer* get_m_timer()
	{
		return m_timer;
	}

	UINT8 get_m_current_timer_id()
	{
		return m_current_timer_id_;
	}

	UINT64 get_timer_resolution()
	{
		return timer_resolution_in_ticks;
	}

	BOOL Initialize();

	BOOL CreateTimer(UINT8 timer_id, UINT32 start_time, UINT32 dtime, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback);

	BOOL StopTimer(UINT8 timer_id);

	BOOL DeInitialize();

	//void HALTimerCallback(void *arg);

};

UINT8 HALTimerManager::m_current_timer_id_ = 0;

HALTimerManager gHalTimerManagerObject;


#endif
