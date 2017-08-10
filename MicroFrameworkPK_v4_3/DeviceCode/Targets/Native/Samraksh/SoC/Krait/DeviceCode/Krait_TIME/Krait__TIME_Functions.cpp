/*
 * Name : Krait__TIME_Functions.cpp
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

#include "Krait__TIME.h"
//#include "../Krait_TIMER/Krait__TIMER.h"
//#include "../Include/Samraksh/VirtualTimer.h"

extern Krait_Time g_Krait_Time;

//AnanthAtSamraksh
//static void ISR( void* Param );

//AnanthAtSamraksh -- adding timer interfaces as part of porting virtual timers
/*#include "../Krait_INTC/Krait__INTC.h"
static struct ihandler handler[NR_IRQS];

void IRQ_Handler(void *arg);*/

#if 0
BOOL CPU_Timer_Initialize(UINT16 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_PARAM)
{
	/*handler[Timer].func = (int_handler)ISR;
	handler[Timer].arg = ISR_PARAM;

	writel(0, DGT_ENABLE);
	//DGT uses LPXO source which is 27MHz.
	// Set clock divider to 4.
	writel(3, DGT_CLK_CTL);
	writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL);
	writel(0, DGT_CLEAR);
	writel(DGT_ENABLE_EN, DGT_ENABLE);

	CPU_INTC_ActivateInterrupt(INT_DEBUG_TIMER_EXP, IRQ_Handler, NULL);
	CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	return TRUE;*/

	return InitializeTimer(0, ISR, ISR_PARAM);

}

BOOL CPU_Timer_SetCompare(UINT16 Timer, UINT32 CompareValue)
{
	static UINT8 setCompareCount = 0;
	//SetCompare(Timer, CompareValue);
	SetCompareValue(CompareValue);
	//AnanthAtSamraksh
	if(setCompareCount == 0){
		setCompareCount++;
		CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	}
}

UINT16 CPU_Timer_GetCounter(UINT16 Timer)
{
	return GetCounter(Timer);
}

UINT64 CPU_Timer_CurrentTicks(UINT16 Timer)
{
	return GetCounter(Timer);
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
#endif


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
	//AnanthAtSamraksh - defaulting to an int during division results in lose of precision. Hence typecasting to a double.
    //return uSec * ((double)SLOW_CLOCKS_PER_SECOND / (double)ONE_MHZ);
	return (uSec * (SLOW_CLOCKS_PER_SECOND / SLOW_CLOCKS_TEN_MHZ_GCD)) / 4;
#else
    return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
#endif
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec, UINT16 Timer )
{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	//AnanthAtSamraksh - there is a lose of precision during the division below. Hence typecasting to a double.
	//Floating point operations slow down the CPU. So, removing double and getting the same factor using int arithmetic.
	//return uSec * ((double)SLOW_CLOCKS_PER_SECOND / (double)ONE_MHZ);

	//There is a loss of precision for a uSec value that is not exactly divisible by 4
	//4 * SLOW_CLOCKS_TEN_MHZ_GCD = ONE_MHZ
	return (uSec * (SLOW_CLOCKS_PER_SECOND / SLOW_CLOCKS_TEN_MHZ_GCD)) / 4;
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
#endif

// -- //

#if 0
BOOL HAL_Time_Initialize()
{
    return Initialize();
}

BOOL HAL_Time_Uninitialize()
{
    return Uninitialize();
}

UINT64 Time_CurrentTicks()
{
    return HAL_Time_CurrentTicks(0);
}

UINT64 HAL_Time_CurrentTicks(UINT16 Timer)
{
    return TimeNow();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    return TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime(UINT16 Timer)
{
    return CurrentTime();
}

void HAL_Time_SetCompare( UINT64 CompareValue, UINT16 Timer )
{
    SetCompareValue( CompareValue );
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}


void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    Sleep_uSec( uSec );
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
    Sleep_uSec_Loop( uSec );
}
#endif



BOOL HAL_Time_Initialize()
{
	//AnanthAtSamraksh - changed below
    //return g_Krait_Timer.InitializeTimer(0);
	//return CPU_Timer_Initialize(0, TRUE, 0, 0, ISR, NULL);
    //return VirtTimer_Initialize();
    return g_Krait_Time.Initialize();
}

BOOL HAL_Time_Uninitialize()
{
	//AnanthAtSamraksh - changed below
    //return TRUE;
	//return VirtTimer_UnInitialize();
	g_Krait_Time.Uninitialize();
}

INT64 HAL_Time_CurrentTime()
{
	//AnanthAtSamraksh - changed below
    //return CPU_TicksToTime( g_Krait_Timer.bigCounter + g_Krait_Timer.GetCounter(0) );
    //return CPU_TicksToTime( CPU_Timer_CurrentTicks(0) );
	//return CPU_TicksToTime( VirtTimer_GetTicks(0) );

	//return VirtTimer_TicksToTime(0, g_Krait_Time.bigCounter + VirtTimer_GetTicks(0));
	return g_Krait_Time.CurrentTime();
}

UINT64 Time_CurrentTicks()
{
    //return HAL_Time_CurrentTicks(0);
    //return VirtTimer_GetTicks(0);
	//return VirtTimer_GetCounter(0);
	//return g_Krait_Time.CurrentTicks();
	return HAL_Time_CurrentTicks();
}

UINT64 HAL_Time_CurrentTicks()
{
    //return g_Krait_Timer.bigCounter + g_Krait_Timer.GetCounter(0);

	//AnanthAtSamraksh - changed below
	//return CPU_Timer_CurrentTicks(0);
	//return VirtTimer_GetTicks(0);

	//return VirtTimer_GetCounter(0);
	//return VirtTimer_GetTicks(0);
	return g_Krait_Time.CurrentTicks();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    //return CPU_TicksToTime( Ticks );
	//return VirtTimer_TicksToTime(0, Ticks);
	return g_Krait_Time.TicksToTime(Ticks);
}


void HAL_Time_SetCompare( UINT64 CompareValue )
{
	//AnanthAtSamraksh -- always use timer as 0
	//VirtTimer_SetCompare(0, CompareValue);
	g_Krait_Time.SetCompareValue(CompareValue);
	//CPU_Timer_SetCompare( 1, CompareValue );
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}


void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
	//AnanthAtSamraksh - changed below. Note: this is not actual "sleep", but "busy waiting".
    //Sleep_uSec( uSec );
	//CPU_Timer_Sleep_MicroSeconds(uSec, 1);
	//VirtTimer_SleepMicroseconds(0, uSec);
	g_Krait_Time.Sleep_uSec(uSec);
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	//AnanthAtSamraksh - changed below
    //Sleep_uSec_Loop( uSec );
	//CPU_Timer_Sleep_MicroSeconds(uSec, 1);
	//VirtTimer_SleepMicroseconds(0, uSec);
	g_Krait_Time.Sleep_uSec(uSec);
}


#pragma GCC reset_options
