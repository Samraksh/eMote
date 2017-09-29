#include "timer_ext.h"

#include <max3263x.h>
#include <mxc_sys.h>
#include <Samraksh/VirtualTimer.h>
#include <pwr/netmf_pwr.h>
#include <tmr.h>

Max3263x_Timer_Ext g_Timer_Ext_Driver;
//#define Max3263x_SYSTEM_TIME MXC_TMR1
#define Max3263x_EXT_TIMER MXC_TMR0

//void ISR_TIM32_SYSTEM_TIME(void* Param);
void ISR_TIM16_ONE_SHOT(void* Param);

const UINT16 TIME_CUSHION = 2000;  // 15 us
const UINT16 MAX_ALLOWABLE_WAIT = 0xFF;

int debugCnt = 0;



bool Max3263x_Timer_Ext::Initialize(HAL_CALLBACK_FPN ISR, UINT32 ISR_Param)
{
	int error = 0;
	tmr32_cfg_t counter_cfg;
    //uint32_t timeOut = 1;
	tmrCounter = Max3263x_EXT_TIMER;
	m_clockRate = 32768;//in hz

	// configuring the one shot compare timer

	// Return if already initialized
	if(Max3263x_Timer_Ext::initialized)
		return TRUE;

	// Maintains the last recorded 32 bit counter value
	setCompareRunning = false;

	callBackISR = ISR;
	callBackISR_Param = ISR_Param;

	//Configure hardware

	//Connected the external clock to timer.
	//Setup GPIO to timer intput function

	sys_cfg_tmr_t gpio;
	gpio.port = (UINT32) (EXT_TIMER0_INPUT_PIN / 8);
	gpio.mask = (UINT32) (EXT_TIMER0_INPUT_PIN % 8);
	gpio.func = GPIO_FUNC_TMR;
	gpio.pad = GPIO_PAD_INPUT;

	//initialize timer and GPIO
	error = TMR_Init(Max3263x_EXT_TIMER, 0, &gpio);

	if(error != E_NO_ERROR)
		return error;

	counter_cfg.mode = TMR32_MODE_COUNTER;
	counter_cfg.polarity = TMR_POLARITY_FALLING_EDGE;
	counter_cfg.compareCount = countLimit;

	//configure and start the timer
	TMR32_Config(Max3263x_EXT_TIMER, &counter_cfg);
	TMR32_Start(Max3263x_EXT_TIMER);

	//Enable interrupt.
	CPU_INTC_ActivateInterrupt(TMR0_0_IRQn, ISR_TIM16_ONE_SHOT, NULL);
    TMR32_EnableINT(Max3263x_EXT_TIMER);

    Max3263x_Timer_Ext::initialized = TRUE;

    return TRUE;
}


// Returns the current 32 bit value of the hardware counter
UINT16 Max3263x_Timer_Ext::GetCounter()
{
	return tmrCounter->count16_0;
}


UINT16 Max3263x_Timer_Ext::SetCounter(UINT16 counterValue)
{
	tmrCounter->count32 = counterValue   ;
	return tmrCounter->count16_0;
}

bool Max3263x_Timer_Ext::UnInitialize()
{
    /*CPU_INTC_DeactivateInterrupt( TIM1_CC_IRQn );
    CPU_INTC_DeactivateInterrupt( TIM2_IRQn );
    callBackISR = NULL;
    TIM_DeInit( TIM1 );
    TIM_DeInit( TIM2 );*/

    return TRUE;
}


// Set compare happens in two stages, the first stage involves setting the msb on tim2
// the second stage involves lsb on tim1
bool Max3263x_Timer_Ext::SetCompare(UINT16 compareValue)
{
	debugCnt++;
	int error = 0;
	tmr32_cfg_t counter_cfg;

	GLOBAL_LOCK(irq);
	volatile UINT16 now = GetCounter();

	// making sure we have enough time before the timer fires to exit SetCompare, the VT callback and the timer interrupt
	if (compareValue < (now + TIME_CUSHION)){
		compareValue = (now + TIME_CUSHION);
	}

	if ( (compareValue - now) > MAX_ALLOWABLE_WAIT ){ // checking to see if our wait time maxes out our upper 16-bit timer
		// we are to trigger at a value that maxes out our upper 16-bit timer so in order to wait as long as possible and not
		// incorrectly trigger the miss condition below we set the timer for the longest possible value that won't trigger the miss condition
		compareValue = MAX_ALLOWABLE_WAIT;
	}

	UINT16 totalWait = compareValue - GetCounter();

	if (setCompareRunning == true){
		//stop timer
	    TMR32_Stop(Max3263x_EXT_TIMER);
		Max3263x_EXT_TIMER->ctrl = 0;
		Max3263x_EXT_TIMER->count32 = 0;
		Max3263x_EXT_TIMER->count16_0 = 0;
		Max3263x_EXT_TIMER->count16_1 = 0;
		Max3263x_EXT_TIMER->intfl = MXC_F_TMR_INTFL_TIMER0 | MXC_F_TMR_INTFL_TIMER1;
	}
	setCompareRunning = true;

	counter_cfg.mode = TMR32_MODE_ONE_SHOT;
	counter_cfg.compareCount = totalWait & 0xFFFFFFFF;

	TMR32_Config(Max3263x_EXT_TIMER, &counter_cfg);
    TMR32_Start(Max3263x_EXT_TIMER);

	return TRUE;
}

void ISR_TIM16_ONE_SHOT( void* Param )
{
	TMR32_ClearFlag(Max3263x_EXT_TIMER);

	g_Timer_Ext_Driver.setCompareRunning = false;
	g_Timer_Ext_Driver.callBackISR(&g_Timer_Ext_Driver.callBackISR_Param);
}

/*void ISR_TIM32_SYSTEM_TIME( void* Param )
{
	g_Timer_Ext_Driver.m_systemTime += (0x1ull <<32);
	TMR32_ClearFlag(Max3263x_SYSTEM_TIME);
}

BOOL Max3263x_Timer_Ext::AddTicks(UINT64 ticksToAdd)
{
	UINT64 originalTime =Get64Counter();

	UINT16 ticksToAdd_TIM1 = ticksToAdd & 0xffff;
	UINT16 ticksToAdd_TIM2 = (ticksToAdd & 0xffff0000) >> 16;
	UINT64 ticksToAdd_UPPER32 = (UINT64)((ticksToAdd & 0xffffffff00000000) >> 32);

	GLOBAL_LOCK(irq);
	UINT32 TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
	UINT32 TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
	UINT64 systemTime_new = (UINT64)(g_Timer_Ext_Driver.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
	TIM1->CNT = TIM1_new;
	TIM2->CNT = TIM2_new;
	g_Timer_Ext_Driver.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;

	// it should only take a few thousand ticks to calculate everything...a rollover could happen though so we just do it again
	while ((g_Timer_Ext_Driver.m_systemTime - (originalTime + ticksToAdd)) > 3000){
		originalTime =Get64Counter();
		TIM1_new = TIM1->CNT + ticksToAdd_TIM1;
		TIM2_new = (UINT32)TIM2->CNT + (UINT32)ticksToAdd_TIM2 + (UINT32)(TIM1_new >> 16);
		systemTime_new = (UINT64)(g_Timer_Ext_Driver.m_systemTime >> 32) + ticksToAdd_UPPER32 + (UINT64)(TIM2_new >> 16);
		TIM1->CNT = TIM1_new;
		TIM2->CNT = TIM2_new;
		g_Timer_Ext_Driver.m_systemTime = (UINT64)(systemTime_new<<32) + (UINT64)(TIM2->CNT<<16) + (UINT64)TIM1->CNT;
	}

	//hal_printf("%llx %llx %llx %llx\r\n",g_Timer_Ext_Driver.m_systemTime, Get64Counter(), (originalTime ), (g_Timer_Ext_Driver.m_systemTime - (originalTime + ticksToAdd)));
	return 0;
}

UINT64 Max3263x_Timer_Ext::Get64Counter()
{
	// keeping an interrupt from happening right now causing timer problems where the value can be wrong at times.
	GLOBAL_LOCK(irq);

	UINT32 currentValue = tmrSystemTime->count32;
	if (TMR32_GetFlag(Max3263x_SYSTEM_TIME) != 0){
		g_Timer_Ext_Driver.m_systemTime += (0x1ull <<32);
		TMR32_ClearFlag(Max3263x_SYSTEM_TIME);	
		currentValue = tmrSystemTime->count32;
	}

	m_systemTime &= (0xFFFFFFFF00000000ull);
	m_systemTime |= currentValue;

	return m_systemTime;
}


DeviceStatus Max3263x_Timer_Ext::InitializeSystemTime(){
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

	return TRUE;
}
*/
