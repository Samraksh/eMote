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
#include "Krait__Timer.h"


// static prototypes
static void TIME_HANDLER(void *arg);
static inline void flush_timer();


// Code

//AnanthAtSamraksh
#include "../Krait_INTC/Krait__INTC.h"
static struct ihandler handler[NR_IRQS];

UINT64 Krait_Timer::bigCounter = 0;
UINT64 Krait_Timer::nextCompare = 0;
Krait_Timer g_Krait_Timer;

//AnanthAtSamraksh
extern "C"
{

void Timer_Handler(void* arg)
{
	TIME_HANDLER(arg);
}

}

//AnanthAtSamraksh

BOOL Krait_Timer::InitializeTimer ( UINT16 Timer, HAL_CALLBACK_FPN ISR, void* ISR_PARAM )
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
void Krait_Timer::SetCompare(UINT16 Timer, UINT32 Compare)
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

UINT32 Krait_Timer::GetCounter(UINT16 Timer)
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


// Add counter contents and reset to 0. IRQs should be off.
static inline void flush_timer() {
	g_Krait_Timer.bigCounter += readl(DGT_COUNT_VAL);
	writel(0, DGT_CLEAR);
}

// Main handler called on overflow.
static void TIME_HANDLER(void *arg) {
	flush_timer();

	// Don't need to call TimeNow() since we just flushed
	if(g_Krait_Timer.bigCounter >= g_Krait_Timer.nextCompare)
	{
		// this also schedules the next one, if there is one
		writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL); // reset the match value to default
		g_Krait_Timer.nextCompare = 0;

		//AnanthAtSamraksh
		uint32_t num = 0;
		handler[num].func(handler[num].arg);
		//AnanthAtSamraksh
	}
	else
	{
		g_Krait_Timer.SetCompare( (UINT16)0, g_Krait_Timer.nextCompare );
	}
}


