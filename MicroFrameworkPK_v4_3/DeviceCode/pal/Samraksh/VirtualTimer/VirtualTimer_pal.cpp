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
	void HardwareVirtTimerMapper(UINT8 timer_id, UINT8 &mapperTimerId, UINT8 &mapperId)
	{
		mapperTimerId = timer_id;
		mapperId = 0;

		for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
		{
			if(i == 0)
			{
				if(mapperTimerId < gVirtualTimerObject.virtualTimerMapper_0.VTM_countOfVirtualTimers)
				{
					mapperId = i;
					break;
				}
				else
				{
					mapperTimerId -= gVirtualTimerObject.virtualTimerMapper_0.VTM_countOfVirtualTimers;
				}
			}
			else if(i == 1)
			{
				if(mapperTimerId < gVirtualTimerObject.virtualTimerMapper_1.VTM_countOfVirtualTimers)
				{
					mapperId = i;
					break;
				}
				else
				{
					mapperTimerId -= gVirtualTimerObject.virtualTimerMapper_1.VTM_countOfVirtualTimers;
				}
			}

		}
	}
}


//BOOL VirtTimer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
BOOL VirtTimer_Initialize(UINT16 Timer, BOOL IsOneShot, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(Timer, mapperTimerId, mapperId);

	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
		gVirtualTimerObject.VT_hardwareTimerId = g_HardwareTimerIDs[i];
		gVirtualTimerObject.VT_countOfVirtualTimers = g_VirtualTimerPerHardwareTimer[i];

		if(i == 0)
		{
			//if(!gVirtualTimerObject.virtualTimerMapper_0.Initialize(gVirtualTimerObject.VT_hardwareTimerId, gVirtualTimerObject.VT_countOfVirtualTimers, g_HardwareTimerIDs[mapperId], FreeRunning, ClkSource, Prescaler, ISR, ISR_PARAM))
			if(!gVirtualTimerObject.virtualTimerMapper_0.Initialize(gVirtualTimerObject.VT_hardwareTimerId, gVirtualTimerObject.VT_countOfVirtualTimers, g_HardwareTimerIDs[mapperId], IsOneShot, Prescaler, ISR, ISR_PARAM))
				return FALSE;
		}
		else if(i == 1)
		{

		}

	}

	return TRUE;
}

VirtualTimerReturnMessage VirtTimer_SetOrChangeTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	VirtualTimerReturnMessage retVal = TimerSupported;;

	// try
	retVal = VirtTimer_SetTimer(timer_id, start_delay, period, is_one_shot, _isreserved, callback);

	// catch
	if(retVal != TimerSupported) {
		retVal = VirtTimer_Change(timer_id, start_delay, period, is_one_shot);
	}

	ASSERT(retVal == TimerSupported);

	return retVal;
}


VirtualTimerReturnMessage VirtTimer_SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	VirtualTimerReturnMessage retVal = TimerSupported;

	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(!gVirtualTimerObject.virtualTimerMapper_0.SetTimer(mapperTimerId, start_delay, period, is_one_shot, _isreserved, callback)) {
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
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(!gVirtualTimerObject.virtualTimerMapper_0.StartTimer(mapperTimerId)) {
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
	UINT8 mapperTimerId = timer_id;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(gVirtualTimerObject.virtualTimerMapper_0.StopTimer(mapperTimerId) == TimerNotSupported) {
		retVal = TimerNotSupported;
	}
	else {
		retVal = TimerSupported;
	}
	return retVal;
}


VirtualTimerReturnMessage VirtTimer_Change(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot)
{
	VirtualTimerReturnMessage retVal = TimerSupported;
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(mapperId == 0)
	{
		if(!gVirtualTimerObject.virtualTimerMapper_0.ChangeTimer(mapperTimerId, start_delay, period, is_one_shot))
			retVal = TimerNotSupported;
		else
			retVal = TimerSupported;
	}
	return retVal;
}


UINT64 VirtTimer_TicksToTime(UINT8 timer_id, UINT64 Ticks)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_TicksToTime(Ticks, (UINT16)g_HardwareTimerIDs[mapperId]);
}


UINT64 VirtTimer_GetTicks(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_CurrentTicks((UINT16)g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_GetCounter(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_GetCounter((UINT16)g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_SetCounter(UINT8 timer_id, UINT32 Count)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_SetCounter((UINT16)g_HardwareTimerIDs[mapperId], Count);
}

BOOL VirtTimer_SetCompare(UINT8 timer_id, UINT64 CompareValue)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_SetCompare((UINT16)g_HardwareTimerIDs[mapperId], CompareValue );
}


void VirtTimer_SleepMicroseconds(UINT8 timer_id, UINT32 uSec)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	CPU_Timer_Sleep_MicroSeconds(uSec, (UINT16)g_HardwareTimerIDs[mapperId]);
}

BOOL VirtTimer_DidTimerOverflow(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_DidTimerOverflow(g_HardwareTimerIDs[mapperId]);
}

void VirtTimer_ClearTimerOverflow(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	CPU_Timer_ClearTimerOverflow(g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_GetMaxTicks(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_GetMaxTicks(g_HardwareTimerIDs[mapperId]);
}


BOOL VirtTimer_UnInitialize()
{
	BOOL ret = TRUE;
	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
			ret &= gVirtualTimerObject.virtualTimerMapper_0.UnInitialize(g_HardwareTimerIDs[i]);
	}
	return ret;
}

