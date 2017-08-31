#include "timer_32bit.h"
#include <max3263x.h>
#include <mxc_sys.h>
#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>
#include <tmr.h>

Max3263x_timer_32bit g_Timer32Bit_Driver;
#define Max3263x_SYSTEM_TIME MXC_TMR1
#define Max3263x_ONE_SHOT MXC_TMR2

void ISR_TIM32_SYSTEM_TIME(void* Param);
void ISR_TIM32_ONE_SHOT(void* Param);

const UINT64 TIME_CUSHION = 2000;  // 15 us
const UINT64 MAX_ALLOWABLE_WAIT = 0xFFFFFFFF;

int debugCnt = 0;

// Returns the current 32 bit value of the hardware counter
UINT32 Max3263x_timer_32bit::GetCounter()
{
	return tmrSystemTime->count32;
}

BOOL Max3263x_timer_32bit::AddTicks(UINT64 ticksToAdd)
{
/*	UINT64 originalTime =Get64Counter();

	UINT16 ticksToAdd_TIM1 = ticksToAdd & 0xffff;
	UINT16 ticksToAdd_TIM2 = (ticksToAdd & 0xffff0000) >> 16;
	UINT64 ticksToAdd_UPPER32 = (UINT64)((ticksToAdd & 0xffffffff00000000) >> 32);

	GLOBAL_LOCK(irq);
	UINT32 TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
	UINT32 TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
	UINT64 systemTime_new = (UINT64)(g_Timer32Bit_Driver.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
	TIM1->CNT = TIM1_new;
	TIM2->CNT = TIM2_new;
	g_Timer32Bit_Driver.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;

	// it should only take a few thousand ticks to calculate everything...a rollover could happen though so we just do it again
	while ((g_Timer32Bit_Driver.m_systemTime - (originalTime + ticksToAdd)) > 3000){
		originalTime =Get64Counter();
		TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
		TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
		systemTime_new = (UINT64)(g_Timer32Bit_Driver.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
		TIM1->CNT = TIM1_new;
		TIM2->CNT = TIM2_new;
		g_Timer32Bit_Driver.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;
	}*/

	//hal_printf("%llx %llx %llx %llx\r\n",g_Timer32Bit_Driver.m_systemTime, Get64Counter(), (originalTime ), (g_Timer32Bit_Driver.m_systemTime - (originalTime + ticksToAdd)));
	return 0;
}

UINT32 Max3263x_timer_32bit::SetCounter(UINT32 counterValue)
{
	tmrSystemTime->count32 = counterValue;
	return tmrSystemTime->count32;
}


UINT64 Max3263x_timer_32bit::Get64Counter()
{
	// keeping an interrupt from happening right now causing timer problems where the value can be wrong at times.
	GLOBAL_LOCK(irq);

	UINT32 currentValue = tmrSystemTime->count32;
	if (TMR32_GetFlag(Max3263x_SYSTEM_TIME) != 0){
		g_Timer32Bit_Driver.m_systemTime += (0x1ull <<32);
		TMR32_ClearFlag(Max3263x_SYSTEM_TIME);	
		currentValue = tmrSystemTime->count32;
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	return m_systemTime;
}

DeviceStatus Max3263x_timer_32bit::InitializeSystemTime(){
	int error = 0;
	tmr32_cfg_t cont_cfg;
	tmrSystemTime = Max3263x_SYSTEM_TIME;

	m_systemTime = 0;
	tmrSystemTime->count32 = 0;

	// configuring the system timer
	
    //enable timer interrupt
	CPU_INTC_ActivateInterrupt(TMR1_0_IRQn, ISR_TIM32_SYSTEM_TIME, NULL);
	tmrSystemTime->inten |= MXC_F_TMR_INTEN_TIMER0;

    //initialize timer and GPIO
	tmr_prescale_t prescale = TMR_PRESCALE_DIV_2_0;
    error = TMR_Init(Max3263x_SYSTEM_TIME, prescale, NULL);

    cont_cfg.mode = TMR32_MODE_CONTINUOUS;
    cont_cfg.polarity = TMR_POLARITY_INIT_LOW;	//start GPIO low

	// all we do is load the max value, interrupt on that, increment m_systemTime and have it auto-reload the max value
	cont_cfg.compareCount = MAX_ALLOWABLE_WAIT;
    //configure and start the timer
    TMR32_Config(Max3263x_SYSTEM_TIME, &cont_cfg);
    
	uint32_t ctrl = tmrSystemTime->ctrl;
    ctrl &= ~(MXC_F_TMR_CTRL_PRESCALE); //clear prescaler bits
    ctrl |= TMR_PRESCALE_DIV_2_0 << MXC_F_TMR_CTRL_PRESCALE_POS;        //set prescaler
    ctrl |= MXC_F_TMR_CTRL_ENABLE0;     //set enable to start the timer

    tmrSystemTime->ctrl = ctrl;

	return DS_Success;
}

DeviceStatus Max3263x_timer_32bit::InitializeOneShot(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
{
	int error = 0;
	tmr32_cfg_t oneshot_cfg;
    uint32_t timeOut = 1; 
	tmrOneShot = Max3263x_ONE_SHOT;
	
	// configuring the one shot compare timer

	// Return if already initialized
	if(Max3263x_timer_32bit::initialized)
		return DS_Success;

	Max3263x_timer_32bit::initialized = TRUE;

	// Maintains the last recorded 32 bit counter value
	setCompareRunning = false;

	callBackISR = ISR;
	callBackISR_Param = ISR_Param;

	CPU_INTC_ActivateInterrupt(TMR2_0_IRQn, ISR_TIM32_ONE_SHOT, NULL);
    TMR32_EnableINT(Max3263x_ONE_SHOT);
    
    //initialize timer
    tmr_prescale_t prescale = TMR_PRESCALE_DIV_2_0;
    error = TMR_Init(Max3263x_ONE_SHOT, prescale, NULL);

    oneshot_cfg.mode = TMR32_MODE_ONE_SHOT;
    error = TMR32_TimeToTicks(Max3263x_ONE_SHOT, 500, TMR_UNIT_MILLISEC, &(oneshot_cfg.compareCount));
    oneshot_cfg.polarity = TMR_POLARITY_INIT_LOW;

    //configure and start the timer
    TMR32_Config(Max3263x_ONE_SHOT, &oneshot_cfg);
    TMR32_Start(Max3263x_ONE_SHOT);
	
    return DS_Success;
}

DeviceStatus Max3263x_timer_32bit::UnInitialize()
{
    /*CPU_INTC_DeactivateInterrupt( TIM1_CC_IRQn );
    CPU_INTC_DeactivateInterrupt( TIM2_IRQn );
    callBackISR = NULL;
    TIM_DeInit( TIM1 );
    TIM_DeInit( TIM2 );*/

    return DS_Success;
}

#if defined(DEBUG_EMOTE_ADVTIME)
volatile UINT64 badSetComparesCount = 0;       //!< number of requests set in the past.
volatile UINT64 badSetComparesAvg = 0;         //!< average delay of requests set in the past.
volatile UINT64 badSetComparesMax = 0;         //!< observed worst-case.
#endif
// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
DeviceStatus Max3263x_timer_32bit::SetCompare(UINT64 compareValue)
{
	debugCnt++;
	int error = 0;
	tmr32_cfg_t oneshot_cfg;

	GLOBAL_LOCK(irq);
	volatile UINT64 now = Get64Counter();

	// making sure we have enough time before the timer fires to exit SetCompare, the VT callback and the timer interrupt
	if (compareValue < (now + TIME_CUSHION)){
		compareValue = (now + TIME_CUSHION);
	}  

	if ( (compareValue - now) > MAX_ALLOWABLE_WAIT ){ // checking to see if our wait time maxes out our upper 16-bit timer		
		// we are to trigger at a value that maxes out our upper 16-bit timer so in order to wait as long as possible and not
		// incorrectly trigger the miss condition below we set the timer for the longest possible value that won't trigger the miss condition
		compareValue = MAX_ALLOWABLE_WAIT; 
	} 

	uint32_t totalWait = compareValue - Get64Counter();

	if (setCompareRunning == true){
		//stop timer
	    TMR32_Stop(Max3263x_ONE_SHOT);
		Max3263x_ONE_SHOT->ctrl = 0;
		Max3263x_ONE_SHOT->count32 = 0;
		Max3263x_ONE_SHOT->count16_0 = 0;
		Max3263x_ONE_SHOT->count16_1 = 0;
		Max3263x_ONE_SHOT->intfl = MXC_F_TMR_INTFL_TIMER0 | MXC_F_TMR_INTFL_TIMER1;
	}
	setCompareRunning = true;

	oneshot_cfg.mode = TMR32_MODE_ONE_SHOT;
	oneshot_cfg.compareCount = totalWait & 0xFFFFFFFF;

	TMR32_Config(Max3263x_ONE_SHOT, &oneshot_cfg);
    TMR32_Start(Max3263x_ONE_SHOT);

	return DS_Success;
}

UINT32 Max3263x_timer_32bit::GetMaxTicks()
{
	return (UINT32)0xFFFFFFFF;
}


void ISR_TIM32_SYSTEM_TIME( void* Param )
{
	g_Timer32Bit_Driver.m_systemTime += (0x1ull <<32);
	TMR32_ClearFlag(Max3263x_SYSTEM_TIME);	
}

void ISR_TIM32_ONE_SHOT( void* Param )
{
	TMR32_ClearFlag(Max3263x_ONE_SHOT);

	g_Timer32Bit_Driver.setCompareRunning = false; 
	g_Timer32Bit_Driver.callBackISR(&g_Timer32Bit_Driver.callBackISR_Param);
}
