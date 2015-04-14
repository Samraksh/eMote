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
#include "Time_decl.h"
#endif


// Defines the number of virtual timers in the system. Increasing this number is fine if you have a large amount of RAM

// Defines the signature of the timer callback function
typedef void (*TIMER_CALLBACK_FPN)( void* arg );


typedef enum _VirtualTimerReturnMessage
{
	TimerReserved = -2,
	TimerNotSupported = -1,
	TimerSupported = 1
}VirtualTimerReturnMessage;



class VirtualTimerInfo
{

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

	// maintains the ticks needed to load into the comparator
	UINT64   m_ticks_when_match_;

	// A pointer to the function that will be called on an interrupt
	TIMER_CALLBACK_FPN m_callBack_;

public:
	// Mutators for all the attributes of HALTimer Object

	void Initialize()
	{

	}

	BOOL operator>(const VirtualTimerInfo &other) const
	{
		if(m_ticks_when_match_ > other.m_ticks_when_match_)
			return TRUE;
		else
			return FALSE;

	}

	BOOL operator<(const VirtualTimerInfo &other) const
	{
		if(m_ticks_when_match_ < other.m_ticks_when_match_)
			return TRUE;
		else
			return FALSE;
	}

	void set_m_ticks_when_match_(UINT64 d)
	{
		if (d < 0){
			d = 0;
		}
		m_ticks_when_match_ = d;
	}

	UINT64 get_m_ticks_when_match_()
	{
		return m_ticks_when_match_;
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
		if(timer1 && timer2)
			return (*timer1 > *timer2);
	}
};



struct VirtualTimerConfig
{
	UINT8 hardwareTimerId;
	UINT8 countOfVirtualTimers;
};


template<UINT8 TimerInfoSize>
class VirtualTimerMapper
{
private:
	UINT64 m_lastQueueAdjustmentTime;

public:

	UINT16 m_current_timer_cnt_;
	UINT16 m_current_timer_running_;

	UINT16 VTM_hardwareTimerId;
	UINT16 VTM_countOfVirtualTimers;
	VirtualTimerInfo g_VirtualTimerInfo[TimerInfoSize];

	//BOOL Initialize(UINT16, UINT16, UINT16 Timer = 0, BOOL FreeRunning = FALSE, UINT32 ClkSource = 0, UINT32 Prescaler = 0, HAL_CALLBACK_FPN ISR = NULL, void* ISR_PARAM = NULL);
	BOOL Initialize(UINT16, UINT16, UINT16 Timer = 0, BOOL IsOneShot = FALSE, UINT32 Prescaler = 0, HAL_CALLBACK_FPN ISR = NULL, void* ISR_PARAM = NULL);

	BOOL SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback);

	BOOL StartTimer(UINT8 timer_id);

	BOOL StopTimer(UINT8 timer_id);

	BOOL ChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot);

	BOOL UnInitialize(UINT16);

	inline BOOL VirtTimerIndexMapper(UINT8 timer_id, UINT8 &VTimerIndex);

};


extern const UINT8 g_CountOfHardwareTimers;
extern const UINT8 g_HardwareTimerIDs[g_CountOfHardwareTimers];
extern const UINT8 g_VirtualTimerPerHardwareTimer[g_CountOfHardwareTimers];


class VirtualTimer
{
private:


public:
	UINT16 VT_hardwareTimerId;
	UINT16 VT_countOfVirtualTimers;

	//For additional virtual timer support, adjust values here as well as in VirtualTimer.cpp, platform_selector.h
#ifdef PLATFORM_ARM_EmoteDotNow
	VirtualTimerMapper<8> virtualTimerMapper_0;
	VirtualTimerMapper<10> virtualTimerMapper_1;
#else
	VirtualTimerMapper<8> virtualTimerMapper_0;
	VirtualTimerMapper<8> virtualTimerMapper_1;
#endif

};

	//PAL interface for VirtualTimers
	//BOOL VirtTimer_Initialize(UINT16 Timer = 0, BOOL FreeRunning = FALSE, UINT32 ClkSource = 0, UINT32 Prescaler = 0, HAL_CALLBACK_FPN ISR = NULL, void* ISR_PARAM = NULL);
	BOOL VirtTimer_Initialize(UINT16 Timer = 0, BOOL IsOneShot = FALSE, UINT32 Prescaler = 0, HAL_CALLBACK_FPN ISR = NULL, void* ISR_PARAM = NULL);
	BOOL VirtTimer_UnInitialize();
	//VirtualTimerReturnMessage VirtTimer_IsValid(UINT8 timer_id);
	VirtualTimerReturnMessage VirtTimer_SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback);
	VirtualTimerReturnMessage VirtTimer_Start(UINT8 timer_id);
	VirtualTimerReturnMessage VirtTimer_Stop(UINT8 timer_id);
	VirtualTimerReturnMessage VirtTimer_Change(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot);

	UINT32 VirtTimer_SetCounter(UINT8 timer_id, UINT32 Count);
	UINT32 VirtTimer_GetCounter(UINT8 timer_id);
	UINT64 VirtTimer_GetTicks(UINT8 timer_id);
	UINT64 VirtTimer_TicksToTime(UINT8 timer_id, UINT64 Ticks);
	BOOL VirtTimer_SetCompare(UINT8 timer_id, UINT64 CompareValue);
	void VirtTimer_SleepMicroseconds(UINT8 timer_id, UINT32 uSec);

	BOOL VirtTimer_DidTimerOverflow(UINT8 timer_id);
	void VirtTimer_ClearTimerOverflow(UINT8 timer_id);

	UINT32 VirtTimer_GetMaxTicks(UINT8 timer_id);




#endif