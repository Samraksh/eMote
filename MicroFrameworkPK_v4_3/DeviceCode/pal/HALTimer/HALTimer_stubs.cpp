#include <Samraksh/HALTimer.h>
#include <Samraksh/Hal_util.h>

HALTimerManager gHalTimerManagerObject;

BOOL HALTimerManager::Initialize()
{
	return TRUE;
}

BOOL HALTimerManager::StopTimer(UINT8 timer_id)
{
	return TRUE;
}


BOOL HALTimerManager::ChangeTimer(UINT8 timer_id, UINT32 dtime)
{
	return TRUE;
}

BOOL HALTimerManager::StartTimer(UINT8 timer_id)
{
	return TRUE;
}

BOOL HALTimerManager::CreateTimer(UINT8 timer_id, UINT32 start_time, UINT32 dtime, BOOL is_one_shot, BOOL _isreserved, TIMER_CALLBACK_FPN callback)
{
	return TRUE;
}