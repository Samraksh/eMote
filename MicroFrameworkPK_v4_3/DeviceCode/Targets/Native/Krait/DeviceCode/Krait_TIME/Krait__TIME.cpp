/*
 *  Name : Krait__TIMER.cpp
 *
 *  Author : nathan.stohs@samraksh.com
 *
 *  Description : The main system time is maintained here. The hardware clock used is the debug timer.
 *
 *  History : v1.0 - Initial Version - Nived.Sivadas
 *	History : v2.0 - Rewrite and Combine TIME and TIMER - Nathan Stohs 2014-05-23
 *
 */

#ifndef SAM_FORCE_GCC_CMDLINE_OPTS
#pragma GCC optimize ("O3")
#endif


#include <tinyhal.h>
#include "Krait__Time.h"
#include "../Krait_TIMER/Krait__Timer.h"


#if 0
/******* TIMER CODE (AS OPPOSED TO TIME) **********/

// static prototypes
static void TIME_HANDLER(void *arg);
static inline void flush_timer();


// Code

//AnanthAtSamraksh
#include "../Krait_INTC/Krait__INTC.h"
static struct ihandler handler[NR_IRQS];

//AnanthAtSamraksh
extern "C"
{

void Timer_Handler(void* arg)
{
	TIME_HANDLER(arg);
}

}

//AnanthAtSamraksh

BOOL InitializeTimer ( UINT16 Timer, HAL_CALLBACK_FPN ISR, void* ISR_PARAM )
{
	GLOBAL_LOCK(irq);
	
	if (Timer != 0) {
		ASSERT();
		return FALSE; // So far we only support DGT aka Timer0
	}

	writel(0, DGT_ENABLE);

	//DGT uses LPXO source which is 27MHz.
	// Set clock divider to 4.
	
	writel(3, DGT_CLK_CTL);
	
	writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL);
	
	writel(0, DGT_CLEAR);
	writel(DGT_ENABLE_EN, DGT_ENABLE);

	//AnanthAtSamraksh
	handler[Timer].func = (int_handler)ISR;
	handler[Timer].arg = ISR_PARAM;
	//AnanthAtSamraksh

	CPU_INTC_ActivateInterrupt(INT_DEBUG_TIMER_EXP, TIME_HANDLER, 0);
	//CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	
	hal_printf("\rKrait DGT initialized\r\n");

	return TRUE;
}


// Not used, but no reason to assert.
static BOOL Uninitialize(UINT32 Timer)
{
	writel(0, DGT_ENABLE);
	return TRUE;
}

// Always on
static void EnableCompareInterrupt(UINT32 Timer)
{
	ASSERT();
	//writel(DGT_ENABLE_CLR_ON_MATCH_EN , DGT_ENABLE);
}

// Always on
static void DisableCompareInterrupt(UINT32 Timer)
{
	ASSERT();
	//UINT32 dgt_enable = readl(DGT_ENABLE);
	//writel((dgt_enable & (~DGT_ENABLE_CLR_ON_MATCH_EN)) , DGT_ENABLE);
}

// Not used.
static void ForceInterrupt(UINT32 Timer)
{
	ASSERT();
}

// Set an interrupt for a number of ticks in the future
void SetCompare(UINT16 Timer, UINT32 Compare)
{
	flush_timer(); // Flush DGT to bigCounter
	writel(Compare, DGT_MATCH_VAL);
}


static UINT32 GetCompare(UINT32 Timer)
{
	ASSERT(); // Shouldn't be used anywhere?
	return readl(DGT_MATCH_VAL);
}

// Nobody touches my counter! (except init).
static void SetCounter(UINT32 Timer, UINT32 Count)
{
	ASSERT();
}

UINT32 GetCounter(UINT16 Timer)
{
	return readl(DGT_COUNT_VAL);
}


static BOOL DidCompareHit(UINT32 Timer)
{
	ASSERT();
	return FALSE;
}

// I wish I knew what this was for....
void HAL_Time_SetCompare_Completion(UINT64 val) {
	return;
}

static void ResetCompareHit(UINT32 Timer)
{
	ASSERT();
}

/******* END TIMER CODE ***************************/
#endif

/******* TIME CODE (AS OPPOSED TO TIMER) **********/
#if 0

static UINT64 bigCounter;
static UINT64 nextCompare;

// Add counter contents and reset to 0. IRQs should be off.
static inline void flush_timer() {
	bigCounter += readl(DGT_COUNT_VAL);
	writel(0, DGT_CLEAR);
}

// Main handler called on overflow.
static void TIME_HANDLER(void *arg) {
	flush_timer();
	
	// Don't need to call TimeNow() since we just flushed
	if(bigCounter >= nextCompare)
	{
		// this also schedules the next one, if there is one
		writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL); // reset the match value to default
		nextCompare = 0;

		//AnanthAtSamraksh
		//HAL_COMPLETION::DequeueAndExec();

		//AnanthAtSamraksh - taken from IRQ_Handler
		uint32_t num = 0;
		//enum handler_return ret;

		//num = readl(GIC_CPU_INTACK);
		//if (num > NR_IRQS)
			//return;
		handler[num].func(handler[num].arg);
		//AnanthAtSamraksh
	}
	else
	{
		SetCompareValue( nextCompare );
	}
}


BOOL Initialize()
{
	bigCounter  = 0;
	nextCompare = 0;
	
	InitializeTimer(0);

	return TRUE;
}


BOOL Uninitialize(){
	return TRUE;
}
#endif

UINT64 TimeNow()
{
	return g_Krait_Timer.bigCounter + g_Krait_Timer.GetCounter(0);
}

#define SETCOMPARE_TIME_OVERHEAD 65
#define SETCOMPARE_MIN_TIME 80

#define TICKS_PROXIMITY_FORCE 10
void SetCompareValue( UINT64 CompareValue )
{
	UINT32 diff;
	UINT64 now;

	GLOBAL_LOCK(irq);

	// New compare is after current compare.
	// Undefined. Should never happen. Just drop it.
	if (g_Krait_Timer.nextCompare != 0 && CompareValue > g_Krait_Timer.nextCompare) {
		return;
	}

    g_Krait_Timer.nextCompare = CompareValue;

	now = TimeNow();

	if ( now >= CompareValue ) {
		SetCompare(0,  SETCOMPARE_MIN_TIME);
		return;
	}

	{
		//if(CompareValue < now)
			//diff = now - CompareValue;
		//else
			diff = CompareValue - now;
	}

	if (diff > MAX_TIMER_ROLLOVER) {
		return;
	}

	if ( (diff-SETCOMPARE_TIME_OVERHEAD) < SETCOMPARE_MIN_TIME || diff < SETCOMPARE_TIME_OVERHEAD) {
		g_Krait_Timer.SetCompare(0, SETCOMPARE_MIN_TIME);
		return;
	}

	g_Krait_Timer.SetCompare(0,  diff - SETCOMPARE_TIME_OVERHEAD);
}

INT64 TicksToTime( UINT64 Ticks )
{
	return CPU_TicksToTime( Ticks );
}

INT64 CurrentTime()
{
	return CPU_TicksToTime( TimeNow() );
}

// Correction factor. Assumes -O0
// Only corrects for native, not managed.
#define TINYCLR_TIMER_MUNGE 4

void Sleep_uSec( UINT32 uSec )
{
	GLOBAL_LOCK(irq);

	if(uSec <= TINYCLR_TIMER_MUNGE) {
		return;
	}

	UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks( uSec-TINYCLR_TIMER_MUNGE ); 
	UINT32 value   = g_Krait_Timer.GetCounter( 0 );

	while((g_Krait_Timer.GetCounter(0) - value) <= maxDiff);
}

// Supposed to be implemented by calculating the approx number of instruction executed during this time and doing a for loop like implementation corresponding to the number of instructions
// Also this function runs with interrupts enabled, may need to look at the actual implementation 
void Sleep_uSec_Loop( UINT32 uSec )
{
	if(uSec <= TINYCLR_TIMER_MUNGE) {
		return;
	}

	UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks(uSec - TINYCLR_TIMER_MUNGE);
	UINT32 value   = g_Krait_Timer.GetCounter( 0 );

	while((g_Krait_Timer.GetCounter(0) - value) <= maxDiff);
}
#pragma GCC reset_options
