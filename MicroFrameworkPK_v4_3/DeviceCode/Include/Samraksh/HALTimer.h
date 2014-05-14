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
#include "heap.h"
#include "Tasklet.h"
#endif

// Defines the number of virtual timers in the system. Increasing this number is fine if you have a large amount of RAM

// Defines the signature of the timer callback function
typedef void (*TIMER_CALLBACK_FPN)( void* arg );

class HALTimer
{

	TaskletType m_timerTasklet;

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
	INT64   m_ticksTillExpire_;

	// A pointer to the function that will be called on an interrupt
	TIMER_CALLBACK_FPN m_callBack_;

public:
	// Mutators for all the attributes of HALTimer Object

	void Initialize()
	{
		m_timerTasklet.action = NULL;
		m_timerTasklet.data = NULL;
	}

	BOOL operator>(const HALTimer &other) const
	{
		if(m_ticksTillExpire_ > other.m_ticksTillExpire_)
			return TRUE;
		else
			return FALSE;

	}

	BOOL operator<(const HALTimer &other) const
	{
		if(m_ticksTillExpire_ < other.m_ticksTillExpire_)
			return TRUE;
		else
			return FALSE;
	}

	void set_m_ticksTillExpire(UINT64 d)
	{
		m_ticksTillExpire_ = d;

	}

	void set_m_timerTasklet(TIMER_CALLBACK_FPN callback)
	{
		m_timerTasklet.action = (HAL_CALLBACK_FPN) callback;
		m_timerTasklet.data = NULL;
	}

	TaskletType* GetTasklet()
	{
		return &m_timerTasklet;
	}

	INT64 get_m_ticksTillExpire()
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
		set_m_timerTasklet(callback);
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

template<class T>
struct TimerCompare
{
public:
	BOOL operator()(T timer1, T timer2)
	{
		return (*timer1 > *timer2);
	}
};


class HALTimerManager
{

	UINT64 m_lastInterruptFireTime;

public:


	HALTimer m_timer[NUM_HALTIMER_TIMERS];

	Hal_Heap_KnownSize<HALTimer*,NUM_HALTIMER_TIMERS , TimerCompare<HALTimer*> > timerQueue;

	UINT16 m_current_timer_id_;

	//HALTimer *m_active_timer;

	BOOL Initialize();

	BOOL CreateTimer(UINT8 timer_id, UINT32 start_time, UINT32 dtime, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback);

	BOOL StopTimer(UINT8 timer_id);

	BOOL StartTimer(UINT8 timer_id);

	BOOL ChangeTimer(UINT8 timer_id, UINT32 dtime);


	BOOL DeInitialize();

	Hal_Heap_KnownSize<HALTimer*, NUM_HALTIMER_TIMERS, TimerCompare<HALTimer*> >* GetTimerQueue()
	{
		return &timerQueue;
	}

	UINT64 get_m_lastInterruptFireTime()
	{
		return m_lastInterruptFireTime;
	}

	void set_m_lastInterruptFireTime(UINT64 lastInterruptFireTime)
	{
		m_lastInterruptFireTime = lastInterruptFireTime;
	}

};


#endif
