/*
 * Samraksh Time Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Notes - 1) Check ticks to time conversion math
 * 		   2) Implement the Sleep functions, CPU_CPWAIT()
 * 		   3) CounterValue function, maybe need to re-implement in a different way!
 *
 */

#include "netmf_time.h"
//#include "Timer/Timer16Bit/netmf_timers16Bit.h"
#include "../Include/Samraksh/VirtualTimer.h"

//#define DEBUG_TIMER

//static int counter = 0;

Time_Driver g_Time_Driver;
UINT64 Time_Driver::bigCounter = 0;

void HAL_Time_SetCompare_Completion(unsigned long long) {
	return;
}


BOOL Time_Driver::Initialize()
{
	return VirtTimer_Initialize();

	//AnanthAtSamraksh
	/*g_Time_Driver.m_lastRead = 0;
	g_Time_Driver.m_nextCompare = (UINT64) Timer16Bit_Driver :: c_MaxTimerValue;

	if (!Timer16Bit_Driver :: Initialize (Timer16Bit_Driver :: c_SystemTimer, TRUE, 0, 0, Time_Driver :: ISR, NULL))
	{
		return FALSE;
	}

	//redundant; happens in initialize
	//Timer16Bit_Driver :: SetCompare (Timer16Bit_Driver :: c_SystemTimer, Timer16Bit_Driver :: c_MaxTimerValue );

	return TRUE;*/
}

BOOL Time_Driver::Uninitialize()
{
	return VirtTimer_UnInitialize();

	//AnanthAtSamraksh
	/*if(!Timer16Bit_Driver::Uninitialize( Timer16Bit_Driver::c_SystemTimer ))
	{
		return FALSE;
	}
	return TRUE;*/
}

//BOOL state = FALSE;

INT64 Time_Driver::CurrentTicks()
{
	////return VirtTimer_GetCounter(0);
	return VirtTimer_GetTicks(0);
}

UINT64 Time_Driver::CounterValue()
{
	return VirtTimer_GetCounter(0);

	//AnanthAtSamraksh
	/*UINT16 value;

    GLOBAL_LOCK(irq);
	//__asm volatile("CPSID    i");

   	//UINT32 lastLowValue = (UINT32)(g_Time_Driver.m_lastRead & 0xFFFFFFFFFFFF0000ull); //check 16/32 bit
   	value = Timer16Bit_Driver :: GetCounter (Timer16Bit_Driver :: c_SystemTimer);

   	// Nived.Sivadas
   	// Workaround for the unusual didtimeoverflow bug, added another check
   	UINT16 lastSixteenBits = g_Time_Driver.m_lastRead & 0x0000FFFFull;

   	g_Time_Driver.m_lastRead &= (0xFFFFFFFFFFFF0000ull);


    if(Timer16Bit_Driver :: DidTimeOverFlow( Timer16Bit_Driver::c_SystemTimer ) || (value < lastSixteenBits))
    //if(Timer16Bit_Driver :: DidTimeOverFlow( Timer16Bit_Driver::c_SystemTimer ))
    {

    	Timer16Bit_Driver :: ClearTimeOverFlow( Timer16Bit_Driver::c_SystemTimer );
    	//STM_EVAL_LEDToggle((Led_TypeDef)0); //Green
    	g_Time_Driver.m_lastRead += (0x1ull << 16);

    }

    //Or else the value gets added simply
   	g_Time_Driver.m_lastRead += value;

   	ENABLE_INTERRUPTS();

    return g_Time_Driver.m_lastRead;*/

#if 0
	UINT16 lastLowValue = (UINT16)(g_Time_Driver.m_lastRead & 0xFFFFFFFFFFFFFF00ull); //check 16/32 bit

    UINT16 value = Timer16Bit_Driver :: GetCounter (Timer16Bit_Driver :: c_SystemTimer);

    g_Time_Driver.m_lastRead &= (0xFFFFFFFF00000000ull);

    if(lastLowValue > value )
    {
    	//DEBUG_TRACE3(TRACE_COUNTER_OVERFLOWS,"CounterValue: Overflow %08x %08x=>%08x", (g_PXA271_TIME_Driver.m_lastRead >> 32), lastLowValue, value);
    	Timer16Bit_Driver :: ClearTimeOverFlow( Timer16Bit_Driver::c_SystemTimer );
    	STM_EVAL_LEDToggle((Led_TypeDef)0); //Green
    	g_Time_Driver.m_lastRead += (0x1ull << 16);
    }

    g_Time_Driver.m_lastRead |= value;

    return g_Time_Driver.m_lastRead;
#endif
*/

}

void Time_Driver::SetCompareValue( UINT64 CompareValue )
{
	VirtTimer_SetCompare(0, CompareValue);

	//AnanthAtSamraksh
	/*GLOBAL_LOCK(irq);

	g_Time_Driver.m_nextCompare = CompareValue;

    bool fForceInterrupt = false;

    UINT64 CntrValue = CounterValue();

    if(CompareValue <= CntrValue)
    {
        fForceInterrupt = true;
    }

    else
    {
        UINT32 diff;
        //If compare is greater than total counter value, this is the best we can do
        if((CompareValue - CntrValue) > Timer16Bit_Driver::c_MaxTimerValue)
        {
        	diff = Timer16Bit_Driver::c_MaxTimerValue;
        }
        //Else store the difference
        else
        {
        	diff = (UINT32)(CompareValue - CntrValue);
        }

        //Store the compare to now + diff
        Timer16Bit_Driver::SetCompare ( Timer16Bit_Driver::c_SystemTimer, (UINT16)(Timer16Bit_Driver::GetCounter( Timer16Bit_Driver :: c_SystemTimer ) + diff));

        //If meanwhile happens
        if(CounterValue() > CompareValue)
		{
			fForceInterrupt = true;
		}
    }

    if(fForceInterrupt)
    {
    	// Force interrupt to process this.
        Timer16Bit_Driver::ForceInterrupt( Timer16Bit_Driver::c_SystemTimer );
    }*/
}

//AnanthAtSamraksh -- commenting out
/*void Time_Driver::ISR( void* Param )
{

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 );

		if(CounterValue() >= g_Time_Driver.m_nextCompare)
		{
			HAL_COMPLETION::DequeueAndExec();
		}
		else
		{
			// Because we are limited in the resolution of timer,
			// resetting the compare will properly configure the next interrupt.
			SetCompareValue( g_Time_Driver.m_nextCompare );
			//HAL_COMPLETION::DequeueAndExec();
		}
	}

#if 0

	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{

		CPU_GPIO_SetPinState((GPIO_PIN) 3, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 3, FALSE);

		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
		CounterValue();
		SetCompareValue( g_Time_Driver.m_nextCompare );
	}
#endif
}*/

INT64 Time_Driver::TicksToTime( UINT64 Ticks )
{
	//return CPU_TicksToTime( Ticks );
	return VirtTimer_TicksToTime(0, Ticks);
}

INT64 Time_Driver::CurrentTime()
{
	//return CPU_TicksToTime( CounterValue() );

	UINT32 currentValue = VirtTimer_GetTicks(0);

	bigCounter &= (0xFFFFFFFF00000000ull);

	if(VirtTimer_DidTimerOverflow(0))
	{
		VirtTimer_ClearTimerOverflow(0);
		bigCounter += (0x1ull <<32);
	}

	bigCounter |= currentValue;

	//return VirtTimer_TicksToTime(0, VirtTimer_GetTicks(0));
	return VirtTimer_TicksToTime(0, bigCounter);
}

INT64 Time_Driver::TimeNow()
{
	//return CPU_TicksToTime( CounterValue() );
	return CurrentTime();
}

void Time_Driver::Sleep_uSec( UINT32 uSec )
{	
	VirtTimer_SleepMicroseconds(0, uSec);

	//AnanthAtSamraksh
	/*GLOBAL_LOCK(irq);
	//CPU_GPIO_SetPinState((GPIO_PIN) 25, TRUE);
	UINT32 value   = Timer16Bit_Driver::GetCounter( Timer16Bit_Driver::c_SystemTimer );
	UINT32 maxDiff  = CPU_MicrosecondsToTicks( uSec );

	if(maxDiff < 400)
		return;

	//UINT32 value   = Timer16Bit_Driver::GetCounter( Timer16Bit_Driver::c_SystemTimer );

	while((Timer16Bit_Driver::GetCounter( Timer16Bit_Driver::c_SystemTimer ) - value) <= maxDiff);*/

	//CPU_GPIO_SetPinState((GPIO_PIN) 25, FALSE);
	/*
	 *
	if(maxDiff <= AT91_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS)
	{
		maxDiff  = AT91_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;
	}
	else
	{
		maxDiff -= AT91_SLEEP_USEC_FIXED_OVERHEAD_CLOCKS;
	}

    while((Timer16Bit_Driver::ReadCounter( Timer16Bit_Driver::c_SystemTimer ) - value) <= maxDiff);
	 */
}

void Time_Driver::Sleep_uSec_Loop( UINT32 uSec )
{
	VirtTimer_SleepMicroseconds(0, uSec);

	//TODO: Revisit
	//uSec *= (SYSTEM_CYCLE_CLOCK_HZ / CLOCK_COMMON_FACTOR);
	//uSec /= (ONE_MHZ               / CLOCK_COMMON_FACTOR);

	//int iterations = (int)uSec - 14;
	//CYCLE_DELAY_LOOP2(iterations);
}
