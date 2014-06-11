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
			if(mapperTimerId < gVirtualTimerObject.virtualTimerMapper[mapperId].VTM_countOfVirtualTimers)
			{
				mapperId = i;
				break;
			}
			else
			{
				mapperTimerId -= gVirtualTimerObject.virtualTimerMapper[mapperId].VTM_countOfVirtualTimers;
			}
		}
	}
}


BOOL VirtTimer_Initialize()
{
	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
		gVirtualTimerObject.VT_hardwareTimerId = g_HardwareTimerIDs[i];
		gVirtualTimerObject.VT_countOfVirtualTimers = g_VirtualTimerPerHardwareTimer[i];

		if(!gVirtualTimerObject.virtualTimerMapper[i].Initialize(gVirtualTimerObject.VT_hardwareTimerId, gVirtualTimerObject.VT_countOfVirtualTimers))
			return FALSE;
	}

	return TRUE;
}

//TODO: AnanthAtSamraksh -- is this right
UINT64 VirtTimer_GetTicks(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	return CPU_Time_CurrentTicks();
}


VirtualTimerReturnMessage VirtTimer_IsValid(UINT8 timer_id)
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
}


VirtualTimerReturnMessage VirtTimer_SetTimer(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(!gVirtualTimerObject.virtualTimerMapper[mapperId].SetTimer(mapperTimerId, start_delay, period, is_one_shot, _isreserved, callback))
		return TimerNotSupported;
	else
		return TimerSupported;

}


VirtualTimerReturnMessage VirtTimer_Start(UINT8 timer_id)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(!gVirtualTimerObject.virtualTimerMapper[mapperId].StartTimer(mapperTimerId))
		return TimerNotSupported;
	else
		return TimerSupported;

}


VirtualTimerReturnMessage VirtTimer_Stop(UINT8 timer_id)
{
	UINT8 mapperTimerId = timer_id;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(gVirtualTimerObject.virtualTimerMapper[mapperId].StopTimer(mapperTimerId) == TimerNotSupported)
		return TimerNotSupported;
	else
		return TimerSupported;

}


VirtualTimerReturnMessage VirtTimer_Change(UINT8 timer_id, UINT32 start_delay, UINT32 period, BOOL is_one_shot)
{
	UINT8 mapperTimerId = 0;
	UINT8 mapperId = 0;
	VirtTimerHelperFunctions::HardwareVirtTimerMapper(timer_id, mapperTimerId, mapperId);

	if(!gVirtualTimerObject.virtualTimerMapper[mapperId].ChangeTimer(mapperTimerId, start_delay, period, is_one_shot))
		return TimerNotSupported;
	else
		return TimerSupported;

}


BOOL VirtTimer_UnInitialize()
{
	for(UINT16 i = 0; i < g_CountOfHardwareTimers; i++)
	{
		gVirtualTimerObject.virtualTimerMapper[i].UnInitialize();
	}
}

