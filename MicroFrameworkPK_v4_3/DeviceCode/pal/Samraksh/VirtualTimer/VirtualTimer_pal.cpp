/*
 * Samraksh virtual timer driver
 * Initial Create - Mukundan Sridharan; Ananth Muralidharan
 * 07/24/2014
 *
 */

#include <Samraksh/VirtualTimer.h>


extern const UINT8 g_CountOfHardwareTimers;

VirtualTimer gVirtualTimerObject;

namespace VirtTimerHelperFunctions
{
	// used to take the timer (RTC_32BIT, ADVTIMER_32BIT) and get the correct VT mapper
	void AssignMapper(UINT32 hardwareTimerId, UINT8 &currentVTMapper)
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

	void HardwareVirtTimerMapper(UINT8 timer_id, UINT8 &mapperId)
	{
		mapperId = 0;

		for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
		{
			for (UINT16 j = 0; j < gVirtualTimerObject.virtualTimerMapper[i].VTM_countOfVirtualTimers; j++)
			{
				if(gVirtualTimerObject.virtualTimerMapper[i].g_VirtualTimerInfo[j].get_m_timer_id() == timer_id)
				{
					mapperId = i;
					return;
				}
			}
		}
	}
}


//BOOL VirtTimer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
BOOL VirtTimer_Initialize()
{
	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
		if(!gVirtualTimerObject.virtualTimerMapper[i].Initialize(g_HardwareTimerIDs[i], g_VirtualTimerPerHardwareTimer))
			return FALSE;
	}

	return TRUE;
}

VirtualTimerReturnMessage VirtTimer_SetOrChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback, UINT8 hardwareTimerId)
{
	VirtualTimerReturnMessage retVal = TimerSupported;;

	// try
	retVal = VirtTimer_SetTimer(timer_id, start_delay, period, is_one_shot, _isreserved, callback, hardwareTimerId);

	// catch
	if(retVal != TimerSupported) {
		retVal = VirtTimer_Change(timer_id, start_delay, period, is_one_shot, hardwareTimerId);
	}

	ASSERT(retVal == TimerSupported);

	return retVal;
}


VirtualTimerReturnMessage VirtTimer_SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback, UINT8 hardwareTimerId)
{
	VirtualTimerReturnMessage retVal = TimerSupported;
	UINT8 VTMapper = 0;
	VirtTimerHelperFunctions::AssignMapper(hardwareTimerId, VTMapper);

	if(!gVirtualTimerObject.virtualTimerMapper[VTMapper].SetTimer(timer_id, start_delay, period, is_one_shot, _isreserved, callback)) {
		retVal = TimerReserved;
	}
	else {
		retVal = TimerSupported;
	}
	return retVal;
}


VirtualTimerReturnMessage VirtTimer_Start(UINT8 timer_id)
{
	VirtualTimerReturnMessage retVal = TimerSupported;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	if(!gVirtualTimerObject.virtualTimerMapper[mapperId].StartTimer(timer_id)) {
		retVal = TimerNotSupported;
	}
	else {
		retVal = TimerSupported;
	}
	return retVal;
}


//AnanthAtSamraksh - when the virtual timer is stopped, it is not released by default. User has to explicitly release it by setting _isreserved to FALSE
VirtualTimerReturnMessage VirtTimer_Stop(UINT8 timer_id)
{
	VirtualTimerReturnMessage retVal = TimerSupported;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	if(gVirtualTimerObject.virtualTimerMapper[mapperId].StopTimer(timer_id) == TimerNotSupported) {
		retVal = TimerNotSupported;
	}
	else {
		retVal = TimerSupported;
	}
	return retVal;
}


VirtualTimerReturnMessage VirtTimer_Change(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, UINT8 hardwareTimerId)
{
	VirtualTimerReturnMessage retVal = TimerSupported;
	UINT8 VTMapper = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, VTMapper);

	if(!gVirtualTimerObject.virtualTimerMapper[VTMapper].ChangeTimer(timer_id, start_delay, period, is_one_shot))
		retVal = TimerNotSupported;
	else
		retVal = TimerSupported;
	return retVal;
}


UINT64 VirtTimer_TicksToTime(UINT8 timer_id, UINT64 Ticks)
{
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	return CPU_TicksToTime(Ticks, (UINT16)g_HardwareTimerIDs[mapperId]);
}


UINT64 VirtTimer_GetTicks(UINT8 timer_id)
{
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	return CPU_Timer_CurrentTicks((UINT16)g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_GetCounter(UINT8 timer_id)
{
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	return CPU_Timer_GetCounter((UINT16)g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_SetCounter(UINT8 timer_id, UINT32 Count)
{
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	return CPU_Timer_SetCounter((UINT16)g_HardwareTimerIDs[mapperId], Count);
}

BOOL VirtTimer_SetCompare(UINT8 timer_id, UINT64 CompareValue)
{
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	return CPU_Timer_SetCompare((UINT16)g_HardwareTimerIDs[mapperId], CompareValue );
}


void VirtTimer_SleepMicroseconds(UINT8 timer_id, UINT32 uSec)
{
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	CPU_Timer_Sleep_MicroSeconds(uSec, (UINT16)g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_GetMaxTicks(UINT8 timer_id)
{
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperId);

	return CPU_Timer_GetMaxTicks(g_HardwareTimerIDs[mapperId]);
}


BOOL VirtTimer_UnInitialize()
{
	BOOL ret = TRUE;
	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
			ret &= gVirtualTimerObject.virtualTimerMapper[i].UnInitialize(g_HardwareTimerIDs[i]);
	}
	return ret;
}

