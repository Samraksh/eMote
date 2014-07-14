/*
 * Name : Krait__TIMER_Functions.cpp
 *
 * Author :Mukundan.Sridharan@Samraksh.com
 * Description : Contains the hal level api for time
 * Update:
 *       Date: July 9, 2014
 *       Author: Ananth.Muralidharan@Samraksh.info
 *       Description: Porting virtual timers
 *
 */

#ifndef SAM_FORCE_GCC_CMDLINE_OPTS
#pragma GCC optimize ("O3")
#endif

#include <tinyhal.h>

#include "Krait__TIMER.h"

extern Krait_Timer g_Krait_Timer;


UINT64 TimeNow()
{
	return g_Krait_Timer.bigCounter + g_Krait_Timer.GetCounter(0);
}

#define TICKS_PROXIMITY_FORCE 10
void SetCompareValue( UINT64 CompareValue )
{
	UINT32 diff;
	UINT64 now;
	int forceInterrupt=0;

	if (g_Krait_Timer.nextCompare != 0 && CompareValue > g_Krait_Timer.nextCompare) {
		//hal_printf("debug: new compare after current compare\r\n");
		return;
	}

    GLOBAL_LOCK(irq);

    g_Krait_Timer.nextCompare = CompareValue;

	now = TimeNow();
	if( CompareValue - now <= TICKS_PROXIMITY_FORCE ) {
		forceInterrupt=1;
		//hal_printf("force interrupt\r\n");
	}
	else if(CompareValue - now > MAX_TIMER_ROLLOVER) { // 0xFFFFFF00
		//hal_printf("debug: new compare too far in future, deferring\r\n");
		return; // We'll get it later
	}

	if (forceInterrupt)
		diff = TICKS_PROXIMITY_FORCE; // a small time in the future
	else
	{
		//if(CompareValue < now)
			//diff = now - CompareValue;
		//else
			diff = CompareValue - now;
	}

	//hal_printf("debug: new compare set to: %d\r\n",diff);

	g_Krait_Timer.SetCompare(0,  diff);
	//SetCompare(0,  (UINT32)CompareValue);
	//AnanthAtSamraksh
	//flush_timer(); // Flush DGT to bigCounter
	//writel(diff, DGT_MATCH_VAL);
	//AnanthAtSamraksh
}



BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	return g_Krait_Timer.InitializeTimer(0, ISR, ISR_PARAM);

}

BOOL CPU_Timer_SetCompare(UINT16 Timer, UINT32 CompareValue)
{
	static UINT8 setCompareCount = 0;
	g_Krait_Timer.SetCompare(Timer, CompareValue);
	//SetCompareValue(CompareValue);
	//AnanthAtSamraksh
	if(setCompareCount == 0){
		setCompareCount++;
		CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	}
}

UINT16 CPU_Timer_GetCounter(UINT16 Timer)
{
	return (UINT16)g_Krait_Timer.GetCounter(Timer);
}

UINT64 CPU_Timer_CurrentTicks(UINT16 Timer)
{
	//return g_Krait_Timer.GetCounter(Timer);
	//AnanthAtSamraksh - added below
	return g_Krait_Timer.bigCounter + g_Krait_Timer.GetCounter(0);
}


// Correction factor. Assumes -O0
// Only corrects for native, not managed.
#define TINYCLR_TIMER_MUNGE 4

void CPU_Timer_Sleep_MicroSeconds( UINT32 uSec, UINT16 Timer)
{
	GLOBAL_LOCK(irq);

	if(uSec <= TINYCLR_TIMER_MUNGE) {
		return;
	}

	UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks( uSec - TINYCLR_TIMER_MUNGE );
	UINT32 value   = g_Krait_Timer.GetCounter( 0 );

	while((g_Krait_Timer.GetCounter(0) - value) <= maxDiff);
}


/*void IRQ_Handler(void* arg)
{
	uint32_t num;
	enum handler_return ret;

	num = readl(GIC_CPU_INTACK);
	if (num > NR_IRQS)
		return;

	ret = handler[num].func(handler[num].arg);
	writel(num, GIC_CPU_EOI);
}*/

//AnanthAtSamraksh -- end of timer interfaces


#if 0
UINT32 CPU_SystemClock(UINT16 Timer)
{
    return SYSTEM_CLOCK_HZ;
}

UINT32 CPU_TicksPerSecond(UINT16 Timer)
{
    return SLOW_CLOCKS_PER_SECOND;
}

//--//

UINT64 CPU_TicksToTime( UINT64 Ticks, UINT16 Timer )
{
    Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);

    return Ticks;
}

UINT64 CPU_TicksToTime( UINT32 Ticks32, UINT16 Timer )
{
    UINT64 Ticks;

    Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);

    return Ticks;
}

//--//

UINT64 CPU_MillisecondsToTicks( UINT64 Ticks, UINT16 Timer )
{
    Ticks *= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);

    return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32, UINT16 Timer )
{
    UINT64 Ticks;

    Ticks  = (UINT64)Ticks32 * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks /=                   (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);

    return Ticks;
}

//--//

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec, UINT16 Timer )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec, UINT16 Timer )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	UINT32 val = (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
    return uSec * val;
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

//--//

UINT32 __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}

int __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( int uSec )
{
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);

    return uSec;
}


//--//

int __section(SectionForFlashOperations) CPU_SystemClocksToMicroseconds( int Ticks )
{
    Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
    Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);

    return Ticks;
}

// -- //
#endif




#pragma GCC reset_options
