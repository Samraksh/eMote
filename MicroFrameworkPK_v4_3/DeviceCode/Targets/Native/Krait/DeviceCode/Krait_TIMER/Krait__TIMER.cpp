/*
 *  Name : Krait__TIMER.cpp
 *
 *  Author : Nived.Sivadas@Samraksh.com
 *
 *  Description :
 *
 *  History :
 *
 *
 */


#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait__Timer.h"


Krait_TIMER_Driver g_Krait_TIMER_Driver;

static platform_timer_callback timer_callback;
static void *timer_arg;
static time_t timer_interval;

static volatile uint32_t ticks;


extern "C"
{
void timer_irq(void *arg)
{
	ticks += timer_interval;
	timer_callback(timer_arg, ticks);
}

UINT32 platform_tick_rate(void)
{
	return 2000000;
}

}

// Stub
void HAL_Time_SetCompare_Completion(UINT64 val) {

}


BOOL Krait_TIMER_Driver::Initialize  ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{

	// Seems like there is not a lot to go on for the hardware timers
	writel(0, DGT_ENABLE);

	/* DGT uses LPXO source which is 27MHz.
	 * Set clock divider to 4.
	 */
	writel(3, DGT_CLK_CTL);

	//ticks_per_sec = 6750000;	/* (27 MHz / 4) */

	//uint32_t tick_count = 1000 * platform_tick_rate() / 1000;
	UINT32 tick_count = 6750000;

	timer_callback = (platform_timer_callback) ISR;
	timer_arg = ISR_Param;
	timer_interval = 1000;

	writel(tick_count, DGT_MATCH_VAL);
	writel(0, DGT_CLEAR);
	writel(DGT_ENABLE_EN | DGT_ENABLE_CLR_ON_MATCH_EN, DGT_ENABLE);
	//writel(DGT_ENABLE_EN, DGT_ENABLE);

	//register_int_handler(INT_DEBUG_TIMER_EXP, timer_irq, 0);
	CPU_INTC_ActivateInterrupt(INT_DEBUG_TIMER_EXP, timer_irq, 0);
	//unmask_interrupt(INT_DEBUG_TIMER_EXP);
	CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	//exit_critical_section();

	return TRUE;
}


BOOL Krait_TIMER_Driver::Uninitialize(UINT32 Timer)
{
	writel(0, DGT_ENABLE);

	return TRUE;
}


void Krait_TIMER_Driver::EnableCompareInterrupt(UINT32 Timer)
{
	writel(DGT_ENABLE_CLR_ON_MATCH_EN , DGT_ENABLE);
}

void Krait_TIMER_Driver::DisableCompareInterrupt(UINT32 Timer)
{
	UINT32 dgt_enable = readl(DGT_ENABLE);
	writel((dgt_enable & (~DGT_ENABLE_CLR_ON_MATCH_EN)) , DGT_ENABLE);
}

void Krait_TIMER_Driver::ForceInterrupt(UINT32 Timer)
{
}

void Krait_TIMER_Driver::SetCompare(UINT32 Timer, UINT32 Compare)
{
	// There are only timers in the node
	if(Timer > 2)
		return;

	writel(Compare, DGT_MATCH_VAL);
}

UINT32 Krait_TIMER_Driver::GetCompare(UINT32 Timer)
{
	return readl(DGT_MATCH_VAL);
}

void Krait_TIMER_Driver::SetCounter(UINT32 Timer, UINT32 Count)
{
	writel(Count, DGT_COUNT_VAL);
}

UINT32 Krait_TIMER_Driver::GetCounter(UINT32 Timer)
{
	if(Timer > 2)
		return 0;

	return readl(DGT_COUNT_VAL);
}

//The Krait timer automatically resets the compare register when the match value is hit
BOOL Krait_TIMER_Driver::DidCompareHit(UINT32 Timer)
{
	return FALSE;
}

void Krait_TIMER_Driver::ResetCompareHit(UINT32 Timer)
{
}


void udelay(unsigned usecs)
{
	usecs = (usecs * 33 + 1000 - 33) / 1000;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < usecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}

void mdelay(unsigned msecs)
{
	msecs *= 33;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < msecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}






/*
#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

#define SPSS_TIMER_STATUS_DGT_EN    (1 << 0)

static platform_timer_callback timer_callback;
static void *timer_arg;
static time_t timer_interval;

static volatile uint32_t ticks;

static enum handler_return timer_irq(void *arg)
{
	ticks += timer_interval;
	return timer_callback(timer_arg, ticks);
}

status_t
platform_set_periodic_timer(platform_timer_callback callback,
			    void *arg, time_t interval)
{
	uint32_t tick_count = interval * platform_tick_rate() / 1000;

	enter_critical_section();

	timer_callback = callback;
	timer_arg = arg;
	timer_interval = interval;

	writel(tick_count, DGT_MATCH_VAL);
	writel(0, DGT_CLEAR);
	writel(DGT_ENABLE_EN | DGT_ENABLE_CLR_ON_MATCH_EN, DGT_ENABLE);

	register_int_handler(INT_DEBUG_TIMER_EXP, timer_irq, 0);
	unmask_interrupt(INT_DEBUG_TIMER_EXP);

	exit_critical_section();
	return 0;
}

time_t current_time(void)
{
	return ticks;
}

static void wait_for_timer_op(void)
{
	while (readl(SPSS_TIMER_STATUS) & SPSS_TIMER_STATUS_DGT_EN) ;
}

void platform_uninit_timer(void)
{
	writel(0, DGT_ENABLE);
	wait_for_timer_op();
	writel(0, DGT_CLEAR);
	wait_for_timer_op();
}

void mdelay(unsigned msecs)
{
	msecs *= 33;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < msecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}

void udelay(unsigned usecs)
{
	usecs = (usecs * 33 + 1000 - 33) / 1000;

	writel(0, GPT_CLEAR);
	writel(0, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) != 0) ;

	writel(GPT_ENABLE_EN, GPT_ENABLE);
	while (readl(GPT_COUNT_VAL) < usecs) ;

	writel(0, GPT_ENABLE);
	writel(0, GPT_CLEAR);
}

//Return current time in micro seconds
bigtime_t current_time_hires(void)
{
	return ticks * 1000ULL;
}
*/
