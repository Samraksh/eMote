#include <Samraksh/VirtualTimer.h>


extern const UINT8 g_CountOfHardwareTimers;

VirtualTimer gVirtualTimerObject;

////TODO: AnanthAtSamraksh -- check this with Mukundan
/*BOOL CPU_Timer_Initialize()
{
	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
		if(!gVirtualTimerObject.virtualTimerMapper[i].Initialize(g_HardwareTimerIDs[i], g_VirtualTimerPerHardwareTimer[i]))
			return FALSE;
	}
	return TRUE;
}*/

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


			/*if(mapperTimerId < gVirtualTimerObject.virtualTimerMapper[i].VTM_countOfVirtualTimers)
			{
				mapperId = i;
				break;
			}
			else
			{
				mapperTimerId -= gVirtualTimerObject.virtualTimerMapper[i].VTM_countOfVirtualTimers;
			}*/
		}
	}
}


BOOL VirtTimer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(Timer, mapperTimerId, mapperId);

	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
		gVirtualTimerObject.VT_hardwareTimerId = g_HardwareTimerIDs[i];
		gVirtualTimerObject.VT_countOfVirtualTimers = g_VirtualTimerPerHardwareTimer[i];

		/*if(!gVirtualTimerObject.virtualTimerMapper[i].Initialize(gVirtualTimerObject.VT_hardwareTimerId, gVirtualTimerObject.VT_countOfVirtualTimers))
			return FALSE;*/

		if(i == 0)
		{
			if(!gVirtualTimerObject.virtualTimerMapper_0.Initialize(gVirtualTimerObject.VT_hardwareTimerId, gVirtualTimerObject.VT_countOfVirtualTimers, g_HardwareTimerIDs[mapperId], FreeRunning, ClkSource, Prescaler, ISR, ISR_PARAM))
				return FALSE;
		}
		else if(i == 1)
		{

		}

	}

	return TRUE;
}



/*VirtualTimerReturnMessage VirtTimer_IsValid(UINT8 timer_id)
{
	if (timer_id < 0)
		return TimerNotSupported;

	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(mapperTimerId > 0)
		return TimerSupported;
	else
		return TimerNotSupported;
}*/


VirtualTimerReturnMessage VirtTimer_SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	/*if(!gVirtualTimerObject.virtualTimerMapper[mapperId].SetTimer(mapperTimerId, start_delay, period, is_one_shot, _isreserved, callback))
		return TimerNotSupported;
	else
		return TimerSupported;*/


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
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	/*if(!gVirtualTimerObject.virtualTimerMapper[mapperId].StartTimer(mapperTimerId))
		return TimerNotSupported;
	else
		return TimerSupported;*/

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
	UINT8 mapperTimerId = timer_id;
	UINT8 mapperId = 0;

	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	/*if(gVirtualTimerObject.virtualTimerMapper[mapperId].StopTimer(mapperTimerId) == TimerNotSupported)
		return TimerNotSupported;
	else
		return TimerSupported;*/

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

	/*if(!gVirtualTimerObject.virtualTimerMapper[mapperId].ChangeTimer(mapperTimerId, start_delay, period, is_one_shot))
		return TimerNotSupported;
	else
		return TimerSupported;*/

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

	return CPU_TicksToTime(Ticks, g_HardwareTimerIDs[mapperId]);
}


UINT32 VirtTimer_GetTicks(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	//AnanthAtSamraksh - changed below
	//return HAL_Time_CurrentTicks(g_HardwareTimerIDs[mapperId]);
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

/*UINT32 VirtTimer_MicrosecondsToSystemClocks(UINT32 uSec)
{
	return CPU_MicrosecondsToSystemClocks(uSec);
}*/


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

		//gVirtualTimerObject.virtualTimerMapper[i].UnInitialize();
	}
}

