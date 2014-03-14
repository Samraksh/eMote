#include <Samraksh/VirtualTimer.h>
#include <Samraksh/Hal_util.h>

VirtualTimerManager gVirtualTimerManagerObject;

BOOL VirtualTimerManager::Initialize()
{
	return TRUE;
}

BOOL VirtualTimerManager::StopTimer(UINT8 timer_id)
{
	return TRUE;
}


BOOL VirtualTimerManager::ChangeTimer(UINT8 timer_id, UINT32 dtime)
{
	return TRUE;
}

BOOL VirtualTimerManager::StartTimer(UINT8 timer_id)
{
	return TRUE;
}

BOOL VirtualTimerManager::CreateTimer(UINT8 timer_id, UINT32 start_time, UINT32 dtime, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	return TRUE;
}
