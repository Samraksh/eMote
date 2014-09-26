/*
 * Samraksh virtual timer driver
 * Initial Create - Mukundan Sridharan; Ananth Muralidharan
 * 07/24/2014
 *
 */

#include <Samraksh/VirtualTimer.h>


extern const UINT8 g_CountOfHardwareTimers;

VirtualTimer gVirtualTimerObject;

//Unnamed namespace
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


VirtualTimerReturnMessage VirtTimer_SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(mapperId == 0)
	{
		if(!gVirtualTimerObject.virtualTimerMapper_0.SetTimer(mapperTimerId, start_delay, period, is_one_shot, _isreserved, callback))
			return TimerReserved;
		else
			return TimerSupported;
	}
	else if(mapperId == 1)
	{

	}
}


VirtualTimerReturnMessage VirtTimer_Start(UINT8 timer_id)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 29, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 29, FALSE);
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(mapperId == 0)
	{
		if(!gVirtualTimerObject.virtualTimerMapper_0.StartTimer(mapperTimerId))
			return TimerNotSupported;
		else
			return TimerSupported;
	}
	else if(mapperId == 1)
	{

	}
}


//AnanthAtSamraksh - when the virtual timer is stopped, it is not released by default. User has to explicitly release it by setting _isreserved to FALSE
VirtualTimerReturnMessage VirtTimer_Stop(UINT8 timer_id)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 30, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 30, FALSE);
	UINT8 mapperTimerId = timer_id;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(mapperId == 0)
	{
		if(gVirtualTimerObject.virtualTimerMapper_0.StopTimer(mapperTimerId) == TimerNotSupported)
			return TimerNotSupported;
		else
			return TimerSupported;
	}
	else if(mapperId == 1)
	{

	}
}


VirtualTimerReturnMessage VirtTimer_Change(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(mapperId == 0)
	{
		if(!gVirtualTimerObject.virtualTimerMapper_0.ChangeTimer(mapperTimerId, start_delay, period, is_one_shot))
			return TimerNotSupported;
		else
			return TimerSupported;
	}
	else if(mapperId == 1)
	{

	}
}


UINT64 VirtTimer_TicksToTime(UINT8 timer_id, UINT64 Ticks)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	//return CPU_TicksToTime(Ticks, g_HardwareTimerIDs[mapperId]);
	return CPU_TicksToMicroseconds(Ticks, g_HardwareTimerIDs[mapperId]);
}


UINT32 VirtTimer_GetTicks(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_CurrentTicks(g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_GetCounter(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_GetCounter(g_HardwareTimerIDs[mapperId]);
}

UINT32 VirtTimer_SetCounter(UINT8 timer_id, UINT32 Count)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_SetCounter(g_HardwareTimerIDs[mapperId], Count);
}

BOOL VirtTimer_SetCompare(UINT8 timer_id, UINT64 CompareValue)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Timer_SetCompare(g_HardwareTimerIDs[mapperId], CompareValue );
}


void VirtTimer_SleepMicroseconds(UINT8 timer_id, UINT32 uSec)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	CPU_Timer_Sleep_MicroSeconds(uSec, g_HardwareTimerIDs[mapperId]);
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
	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
		if(i == 0)
		{
			gVirtualTimerObject.virtualTimerMapper_0.UnInitialize(g_HardwareTimerIDs[i]);
		}
		else if(i == 1)
		{

		}
	}
}

