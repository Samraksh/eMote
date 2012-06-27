#include <VirtualTimer.h>

BOOL VirtualTimerManager::Initialize()
{
	if (!Timer_Driver :: Initialize (Timer_Driver :: c_VirtualTimer, TRUE, 0, 0, Time_Driver :: ISR, NULL))
	{
		return FALSE;
	}
}

BOOL VirtualTimerManager::createTimer(UINT32 timerID, UINT32 startTime, UINT32 dTime, BOOL isOneShot, BOOL _reserved,TIMER_CALLBACK_FPN callBack)
{

}

BOOL VirtualTimerManager::stopTimer(UINT32 timerID)
{

}
