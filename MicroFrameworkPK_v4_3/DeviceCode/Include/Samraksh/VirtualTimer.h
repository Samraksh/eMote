/*
 *	Name		:		VirtualTimer.h
 *
 *  Author		:		nived.sivadas@samraksh.com; mukundan.sridharan@samraksh.com; ananth.muralidharan@samraksh.com
 *
 *	Description :
 */

#ifndef _VIRTUAL_TIMER_H_
#define _VIRTUAL_TIMER_H_

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


typedef enum _VirtualTimerReturnMessage
{
	TimerNotSupported = -1,
	TimerSupported = 1
}VirtualTimerReturnMessage;



class VirtualTimerInfo
{

	TaskletType m_timerTasklet;

	// The Id referenced by the use of the timer
	UINT8 m_timer_id_;

	// The expected start time of the timer
	UINT32 m_start_delay_;

	// What is the delta after which the timer should fire
	UINT32 m_period_;

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

	BOOL operator>(const VirtualTimerInfo &other) const
	{
		if(m_ticksTillExpire_ > other.m_ticksTillExpire_)
			return TRUE;
		else
			return FALSE;

	}

	BOOL operator<(const VirtualTimerInfo &other) const
	{
		if(m_ticksTillExpire_ < other.m_ticksTillExpire_)
			return TRUE;
		else
			return FALSE;
	}

	void set_m_ticksTillExpire(INT64 d)
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

	void set_m_start_delay(UINT32 start_delay)
	{
		m_start_delay_ = start_delay;
	}

	UINT32 get_m_start_delay()
	{
		return m_start_delay_;
	}

	void set_m_period(UINT32 period)
	{
		m_period_ = period;
	}

	UINT32 get_m_period()
	{
		return m_period_;
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
		////return (timer1->get_m_ticksTillExpire() > timer2->get_m_ticksTillExpire());
		/*if(timer1->get_m_timer_run_count() == timer2->get_m_timer_run_count())
		{
			INT64 runCount = timer1->get_m_timer_run_count();
			runCount += 1;
			timer1->set_m_timer_run_count(runCount);
		}

		return (timer1->get_m_timer_run_count() > timer2->get_m_timer_run_count());*/
	}
};



struct VirtualTimerConfig
{
	UINT8 hardwareTimerId;
	UINT8 countOfVirtualTimers;
};

////Forward-declaration
////class VirtualTimerMapper;


//template<UINT8 TimerInfoSize>
class VirtualTimerMapper
{
private:
	//UINT64 m_lastInterruptFireTime;
	UINT64 m_lastQueueAdjustmentTime;

public:

	UINT16 m_current_timer_id_;

	UINT16 VTM_hardwareTimerId;
	UINT16 VTM_countOfVirtualTimers;
	//TODO: AnanthAtSamraksh -- check if this is correct
	////template<UINT8 T> VirtualTimerInfo<T> g_VirtualTimerInfo[T];
	//Tried using a template, but did not work. Allocate an array of size equal to the count of largest count of virtual timers
	//VirtualTimerInfo g_VirtualTimerInfo[TimerInfoSize];
	VirtualTimerInfo g_VirtualTimerInfo[8];
	//TODO: AnanthAtSamraksh - might need to change VTM_countOfVirtualTimers to 8 (the max virt timers defined in platform_selector.h
	Hal_Heap_KnownSize<VirtualTimerInfo*, 8, TimerCompare<VirtualTimerInfo*> > timerQueue;

	//HALTimer *m_active_timer;

	/*void Set_HardwareTimerId(UINT8);
	UINT8 Get_HardwareTimerId();
	void Set_CountOfVirtualTimers(UINT8);
	UINT8 Get_CountOfVirtualTimers();*/

	////BOOL Initialize(VirtualTimerConfigInitialize::VirtualTimerConfig VTconfig);
	BOOL Initialize(UINT16, UINT16);

	BOOL SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback);

	BOOL StartTimer(UINT8 timer_id);

	BOOL StopTimer(UINT8 timer_id);

	BOOL ChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot);


	BOOL UnInitialize();

	Hal_Heap_KnownSize<VirtualTimerInfo*, 8, TimerCompare<VirtualTimerInfo*> >* GetTimerQueue()
	{
		return &timerQueue;
	}

	UINT64 get_m_lastQueueAdjustmentTime()
	{
		return m_lastQueueAdjustmentTime;
	}

	void set_m_lastQueueAdjustmentTime(UINT64 lastQueueAdjustmentTime)
	{
		m_lastQueueAdjustmentTime = lastQueueAdjustmentTime;
	}

};


extern const UINT8 g_CountOfHardwareTimers;
//extern const UINT8 g_VirtualTimerPerHardwareTimer[g_CountOfHardwareTimers];

//template<UINT8 HardwareTimerId>
class VirtualTimer
{
private:

public:
	UINT16 VT_hardwareTimerId;
	UINT16 VT_countOfVirtualTimers;
	////VirtualTimerInfo g_VirtualTimerInfo[];

	////VirtualTimerMapper virtualTimerMapperObj;
	//VirtualTimerMapper<g_VirtualTimerPerHardwareTimer[HardwareTimerId]> virtualTimerMapper[g_CountOfHardwareTimers];
	VirtualTimerMapper virtualTimerMapper[g_CountOfHardwareTimers];
};

	////BOOL Initialize(VirtualTimerConfig& config);
	//PAL interface for VirtualTimers
	BOOL VirtTimer_Initialize();
	VirtualTimerReturnMessage VirtTimer_IsValid(UINT8 timer_id);
	VirtualTimerReturnMessage VirtTimer_SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback);
	VirtualTimerReturnMessage VirtTimer_Start(UINT8 timer_id);
	VirtualTimerReturnMessage VirtTimer_Stop(UINT8 timer_id);
	VirtualTimerReturnMessage VirtTimer_Change(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot);
	BOOL VirtTimer_UnInitialize();

	UINT64 VirtTimer_GetTicks(UINT8 timer_id);

	////TODO: AnanthAtSamraksh -- check this with Mukundan
	BOOL CPU_Timer_Initialize();
//};




#endif
