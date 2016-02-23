#ifndef OMACCLOK_H_
#define OMACCLOK_H_

#include <Samraksh/Hal_util.h>

#define DEBUG_TSYNC_PIN

class OMACClock{
	UINT64 m_inter_clock_offset;
public:
	OMACClock();
	UINT64 GetCurrentTimeinTicks();
	UINT64 ConvertTickstoMicroSecs(const UINT64& ticks);
	UINT64 CreateSyncPointBetweenClocks();

};

OMACClock::OMACClock(){
	m_inter_clock_offset = 0;
}

UINT64 OMACClock::CreateSyncPointBetweenClocks(){
	if(0 && OMACClockSpecifier == LFCLOCKID){

		m_inter_clock_offset = HAL_Time_CurrentTicks() - VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio ;
	}
}

UINT64 OMACClock::GetCurrentTimeinTicks(){ //This function gets the time ticks required for OMAC
	if(OMACClockSpecifier == LFCLOCKID){
		return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio + m_inter_clock_offset;
	}
	else{
		return HAL_Time_CurrentTicks();
	}
}

UINT64 OMACClock::ConvertTickstoMicroSecs(const UINT64& ticks){ //This function gets the time ticks required for OMAC
	//return VirtTimer_GetTicks(VIRT_TIMER_OMAC_SCHEDULER) * OMACClocktoSystemClockFreqRatio;
	return HAL_Time_TicksToTime(ticks);
}


#endif /* OMACCLOK_H_ */
