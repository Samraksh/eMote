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
#include "Krait__TIMER.h"

Krait_TIMER_Driver g_Krait_Timer_Driver;

#if 0
static platform_timer_callback timer_callback;
static void *timer_arg;
static time_t timer_interval;

static volatile uint32_t ticks;

unsigned int gpt_hz = 32768;
unsigned int sclk_hz = 32768;


static int global_timer_offset;
static int msm_global_timer;

extern "C"
{
void timer_irq(void *arg)
{
	ticks += timer_interval;
	timer_callback(timer_arg, ticks);
}

UINT32 platform_tick_rate(void)
{
	return 6750000;
}

}
#endif

static int global_timer_offset;
static int msm_global_timer;


unsigned int gpt_hz = 32768;
unsigned int sclk_hz = 32768;

extern "C"
{
void timer_irq(void *arg)
{
//	ticks += timer_interval;
	//timer_callback(timer_arg, ticks);
}
}

static struct Krait_TIMER *clockevent_to_clock(struct KraitClockEventDevice *evt)
{
	int i;

	//if (!is_smp())
	//	return container_of(evt, struct msm_clock, clockevent);

	for (i = 0; i < 2; i++)
		if (evt == &(g_Krait_Timer_Driver.krait_timers[i].clockevent))
			return &(g_Krait_Timer_Driver.krait_timers[i]);
	return &(g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT]);
}

static UINT32 msm_read_timer_count(struct Krait_TIMER *timer, int global)
{
	UINT32 t1, t2, t3;
	int loop_count = 0;
	void __iomem *addr = timer->regbase + TIMER_COUNT_VAL +
		global*global_timer_offset;

	if (!(timer->flags & MSM_CLOCK_FLAGS_UNSTABLE_COUNT))
		return __raw_readl(addr);

	t1 = __raw_readl(addr);
	t2 = __raw_readl_no_log(addr);
	if ((t2-t1) <= 1)
		return t2;
	while (1) {
		t1 = __raw_readl_no_log(addr);
		t2 = __raw_readl_no_log(addr);
		t3 = __raw_readl_no_log(addr);
		//cpu_relax();
		if ((t3-t2) <= 1)
			return t3;
		if ((t2-t1) <= 1)
			return t2;
		if ((t2 >= t1) && (t3 >= t2))
			return t2;
		if (++loop_count == 5) {
			while(1);
			return t3;
		}
	}
}


INT32 msm_timer_set_next_event(UINT64 cycles, struct KraitClockEventDevice *evt)
{

	struct Krait_TIMER *timer;
	struct msm_clock_percpu_data *clock_state;
#if 0
	UINT32 now;
	UINT32 alarm;
	INT32 late;
	INT32 i;

	timer = clockevent_to_clock(evt);
	//clock_state = &__get_cpu_var(msm_clocks_percpu)[timer->index];
	//	if (clock_state->stopped)
	//		return 0;
		now = msm_read_timer_count(timer, LOCAL_TIMER);
		alarm = now + (cycles << timer->shift);
		if (timer->flags & MSM_CLOCK_FLAGS_ODD_MATCH_WRITE)
			while (now == clock_state->last_set)
				now = msm_read_timer_count(timer, LOCAL_TIMER);

		clock_state->alarm = alarm;
		__raw_writel(alarm, timer->regbase + TIMER_MATCH_VAL);

		if (timer->flags & MSM_CLOCK_FLAGS_DELAYED_WRITE_POST) {
			/* read the counter four extra times to make sure write posts
			   before reading the time */
			for (i = 0; i < 4; i++)
				__raw_readl_no_log(timer->regbase + TIMER_COUNT_VAL);
		}
		now = msm_read_timer_count(timer, LOCAL_TIMER);
		clock_state->last_set = now;
		clock_state->alarm_vtime = alarm + clock_state->sleep_offset;
		late = now - alarm;
		if (late >= (int)(-timer->write_delay << timer->shift) &&
		    late < timer->freq*5)
			return -ETIME;
#endif
		return 0;

}

void msm_timer_set_mode(enum clock_event_mode mode, struct clock_event_device *evt)
{

}

UINT64 msm_gpt_read(KraitClock *cs)
{
	struct Krait_TIMER *timer = &g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT];
#if 0
		struct msm_clock_percpu_data *clock_state =
			&per_cpu(msm_clocks_percpu, 0)[MSM_CLOCK_GPT];


		if (clock_state->stopped)
			return clock_state->stopped_tick;
#endif
		//return msm_read_timer_count(timer, GLOBAL_TIMER) +
		//	clock_state->sleep_offset;
		return msm_read_timer_count(timer, GLOBAL_TIMER);
}

UINT64 msm_dgt_read(KraitClock *cs)
{
	struct Krait_TIMER *timer = &g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT];
#if 0
		struct msm_clock_percpu_data *clock_state =
			&per_cpu(msm_clocks_percpu, 0)[MSM_CLOCK_DGT];

		if (clock_state->stopped)
			return clock_state->stopped_tick >> timer->shift;

		return (msm_read_timer_count(timer, GLOBAL_TIMER) +
			clock_state->sleep_offset) >> timer->shift;
#endif
		return msm_read_timer_count(timer, GLOBAL_TIMER);
}

// Initializes the gpt  and the dbt structure
BOOL InitializeTimerStructure()
{
	KraitClockEventDevice clock_event;
	KraitClock clock_source;

	clock_event.name = "gp_timer";
	clock_event.features = CLOCK_EVT_FEAT_ONESHOT;
	clock_event.shift = 32;
	clock_event.rating = 200;
	clock_event.set_next_event = msm_timer_set_next_event;
	clock_event.set_mode       = msm_timer_set_mode;

	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT].clockevent = clock_event;

	clock_source.name 			= "gp_timer";
	clock_source.rating 		= 200;
	clock_source.read 			= msm_gpt_read;
	clock_source.mask           = CLOCKSOURCE_MASK(32);
	clock_source.shift          = 17;
	clock_source.flags          = CLOCK_SOURCE_IS_CONTINUOUS;

	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT].clocksource = clock_source;

	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT].irq = INT_GP_TIMER_EXP;
	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT].regbase = (void *) (MSM_TMR_BASE + 0x4);
	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT].freq = 32768;
	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT].index = MSM_CLOCK_GPT;
	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT].write_delay = 9;

	clock_event.name           = "dg_timer";
	clock_event.features       = CLOCK_EVT_FEAT_ONESHOT;
	clock_event.shift          = 32;
	clock_event.rating         = DG_TIMER_RATING;
	clock_event.set_next_event = msm_timer_set_next_event;
	clock_event.set_mode       = msm_timer_set_mode;

	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT].clockevent = clock_event;

	clock_source.name 		   = "dg_timer";
	clock_source.rating         = DG_TIMER_RATING;
	clock_source.read           = msm_dgt_read;
	clock_source.mask           = CLOCKSOURCE_MASK(32);
	clock_source.shift          = 24;
	clock_source.flags          = CLOCK_SOURCE_IS_CONTINUOUS;

	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT].clocksource = clock_source;

	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT].irq = INT_DEBUG_TIMER_EXP;
	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT].regbase = (void *) (MSM_TMR_BASE + 0x24);
	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT].index = MSM_CLOCK_DGT;
	g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT].write_delay = 9;

	return TRUE;

}

// Unlike traditional systems where you can configure any number of timers, the linux code in general seems to deal with
// two timers called gpt and dbt and because of the lack of documentation there is no way of confirming if any
// other timers are configurable. This function therefore intiializes gpt and dbt
// The Callback function and the param values are the only arguments actually used internally by this function
BOOL Krait_TIMER_Driver::Initialize  ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
#if 0
	// Seems like there is not a lot to go on for the hardware timers
	writel(0, DGT_ENABLE);

	/* DGT uses LPXO source which is 27MHz.
	 * Set clock divider to 4.
	 */
	writel(3, DGT_CLK_CTL);

	//ticks_per_sec = 6750000;	/* (27 MHz / 4) */

	uint32_t tick_count = 1000 * platform_tick_rate() / 1000;

	timer_callback = (platform_timer_callback) ISR;
	timer_arg = ISR_Param;
	timer_interval = 1000;

	writel(tick_count, DGT_MATCH_VAL);
	writel(0, DGT_CLEAR);
	writel(DGT_ENABLE_EN | DGT_ENABLE_CLR_ON_MATCH_EN, DGT_ENABLE);

	//register_int_handler(INT_DEBUG_TIMER_EXP, timer_irq, 0);
	CPU_INTC_ActivateInterrupt(INT_DEBUG_TIMER_EXP, timer_irq, 0);
	//unmask_interrupt(INT_DEBUG_TIMER_EXP);
	CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	//exit_critical_section();
#endif

	InitializeTimerStructure();

	struct Krait_TIMER *gpt = &krait_timers[MSM_CLOCK_GPT];
	struct Krait_TIMER *dgt = &krait_timers[MSM_CLOCK_DGT];

	dgt->freq = 6750000;
	__raw_writel(DGT_CLK_CTL_DIV_4, MSM_TMR_BASE + DGT_CLK_CTL);

	//gpt->status_mask = BIT(10);
	//dgt->status_mask = BIT(2);;
	global_timer_offset = MSM_TMR0_BASE - MSM_TMR_BASE;
	gpt->status_mask = 1 << 9;
	dgt->status_mask = 1 << 1;;
	gpt->freq = 32765;
	gpt_hz = 32765;
	sclk_hz = 32765;

   for(UINT32 i = 0; i < ARRAY_SIZE(krait_timers); i++)
   {
	   struct Krait_TIMER *timer = &krait_timers[i];
	   struct KraitClockEventDevice *ce = &timer->clockevent;
	   struct KraitClock *cs = &timer->clocksource;
	   __raw_writel(0, timer->regbase + TIMER_ENABLE);
	   __raw_writel(0, timer->regbase + TIMER_CLEAR);
	   __raw_writel(~0, timer->regbase + TIMER_MATCH_VAL);
   }

   CPU_INTC_ActivateInterrupt(INT_GP_TIMER_EXP, ISR, ISR_Param);
   CPU_INTC_InterruptEnable( INT_GP_TIMER_EXP );

	return TRUE;
}


BOOL Krait_TIMER_Driver::Uninitialize(UINT32 Timer)
{
}


void Krait_TIMER_Driver::EnableCompareInterrupt(UINT32 Timer)
{

}

void Krait_TIMER_Driver::DisableCompareInterrupt(UINT32 Timer)
{
}

void Krait_TIMER_Driver::ForceInterrupt(UINT32 Timer)
{

}

void Krait_TIMER_Driver::SetCompare(UINT32 Timer, UINT32 Compare)
{
	if(Timer > 0)
		return;

	Krait_TIMER *timer;

	if(Timer == MSM_CLOCK_GPT)
		timer = &(g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT]);
	else
		timer = &(g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT]);

	__raw_writel(Compare, timer->regbase + TIMER_MATCH_VAL);
}

UINT32 Krait_TIMER_Driver::GetCompare(UINT32 Timer)
{
	Krait_TIMER *timer;
	if(Timer > 0)
		return 0;

	if(Timer == MSM_CLOCK_GPT)
			timer = &(g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT]);
		else
			timer = &(g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT]);

	return __raw_readl((volatile void *) (timer->regbase + TIMER_MATCH_VAL));
}

void Krait_TIMER_Driver::SetCounter(UINT32 Timer, UINT32 Count)
{
	Krait_TIMER *timer;
		if(Timer > 0)
			return;

		if(Timer == MSM_CLOCK_GPT)
				timer = &(g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_GPT]);
			else
				timer = &(g_Krait_Timer_Driver.krait_timers[MSM_CLOCK_DGT]);

		__raw_writel(Count, timer->regbase + TIMER_COUNT_VAL);

}

BOOL Krait_TIMER_Driver::DidCompareHit(UINT32 Timer)
{

	return FALSE;
}

void Krait_TIMER_Driver::ResetCompareHit(UINT32 Timer)
{

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
