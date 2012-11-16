/*
 * Krait__TIMER.h
 *
 *  Created on: Sep 14, 2012
 *      Author: Mukundan Sridharan
 */

#ifndef KRAIT__TIMER_H_
#define KRAIT__TIMER_H_
#include <tinyhal.h>
#include "..\Krait_Clock\Krait_Clock.h"
#include "Krait__TIMER_RegDef.h"

#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

#define GIC_PPI_START 16
#define GIC_SPI_START 32

#define INT_DEBUG_TIMER_EXP     (GIC_PPI_START + 1)

#define TIMER_MATCH_VAL         0x0000
#define TIMER_COUNT_VAL         0x0004
#define TIMER_ENABLE            0x0008
#define TIMER_CLEAR             0x000C
#define DGT_CLK_CTL             0x0034

#define SPSS_TIMER_STATUS_DGT_EN    (1 << 0)
#if 0
#define MSM_TMR_BASE        0x0200A000
#define MSM_DGT_BASE        (MSM_TMR_BASE + 0x24)
#define DGT_REG(off)        (MSM_DGT_BASE + (off))
#define DGT_ENABLE           DGT_REG(0x0008)
#define DGT_CLK_CTL          DGT_REG(0x0010)
#define DGT_MATCH_VAL        DGT_REG(0x0000)
#define DGT_CLEAR            DGT_REG(0x000C)
#endif

#define LOCAL_TIMER 0
#define GLOBAL_TIMER 1


//typedef enum handler_return (*platform_timer_callback)(void *arg, time_t now);

//status_t platform_set_periodic_timer(platform_timer_callback callback, void *arg, time_t interval);

//void mdelay(unsigned msecs);
//void udelay(unsigned usecs);

//extern "C"
//{
//uint32_t platform_tick_rate(void);
//}

enum {
	DGT_CLK_CTL_DIV_1 = 0,
	DGT_CLK_CTL_DIV_2 = 1,
	DGT_CLK_CTL_DIV_3 = 2,
	DGT_CLK_CTL_DIV_4 = 3,
};

enum {
	MSM_CLOCK_FLAGS_UNSTABLE_COUNT = 1U << 0,
	MSM_CLOCK_FLAGS_ODD_MATCH_WRITE = 1U << 1,
	MSM_CLOCK_FLAGS_DELAYED_WRITE_POST = 1U << 2,
};

enum {
	MSM_CLOCK_GPT,
	MSM_CLOCK_DGT,
};

struct msm_clock_percpu_data {
	UINT32                  last_set;
	UINT32                  sleep_offset;
	UINT32                  alarm_vtime;
	UINT32                  alarm;
	UINT32                  non_sleep_offset;
	UINT32                  in_sync;
	cycle_t                   stopped_tick;
	int                       stopped;
	UINT32                  last_sync_gpt;
	UINT64                      last_sync_jiffies;
};

// Structure designed with linux msm timer driver as reference
struct Krait_TIMER
{

	struct KraitClockEventDevice clockevent;
	struct KraitClock clocksource;

	UINT32		irq;
	void 		*regbase;
	UINT32		freq;
	UINT32 		shift;
	UINT32 		flags;
	UINT32 		write_delay;
	UINT32 		rollover_offset;
	UINT32 		index;
	void		*global_counter;
	void 		*local_counter;
	UINT32		status_mask;

	union
	{
		struct clock_event_device  *evt;
		//struct clock_event_device __percpu  **percpu_evt;
		struct clock_event_device **percpu_evt;
	};
};


struct Krait_TIMER_Driver
{

	Krait_TIMER krait_timers[2];

	BOOL Initialize  ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param );
	BOOL Uninitialize(UINT32 Timer);
	void EnableCompareInterrupt(UINT32 Timer);
	void DisableCompareInterrupt(UINT32 Timer);
	void ForceInterrupt(UINT32 Timer);
	void SetCompare(UINT32 Timer, UINT32 Compare);
	UINT32 GetCompare(UINT32 Timer);
	void SetCounter(UINT32 Timer, UINT32 Count);
	BOOL DidCompareHit(UINT32 Timer);
	void ResetCompareHit(UINT32 Timer);

};


UINT64 msm_gpt_read(struct KraitClock *cs);
UINT64 msm_dgt_read(struct KraitClock *cs);
INT32 msm_timer_set_next_event(UINT64 cycles, struct KraitClockEventDevice *evt);
void msm_timer_set_mode(enum clock_event_mode mode, struct KraitClockEventDevice *evt);

#define NR_TIMERS 2

//static DEFINE_PER_CPU(struct msm_clock_percpu_data[NR_TIMERS], msm_clocks_percpu);

#if 0
static struct Krait_TIMER krait_timers[] = {
		{
			    {
						"gp_timer",
						CLOCK_EVT_FEAT_ONESHOT,
						32,
						200,
						msm_timer_set_next_event,
						msm_timer_set_mode,
				},
				{
						"gp_timer",
						200,
						msm_gpt_read,
						CLOCKSOURCE_MASK(32),
						17,
						CLOCK_SOURCE_IS_CONTINUOUS,
				},
				INT_GP_TIMER_EXP,
				MSM_TMR_BASE + 0x4,
				32768,
				MSM_CLOCK_GPT,
				9,
		},
		{
				{
						"dg_timer",
						CLOCK_EVT_FEAT_ONESHOT,
						32,
						DG_TIMER_RATING,
						msm_timer_set_next_event,
						msm_timer_set_mode,
				},
				{
						"dg_timer",
						DG_TIMER_RATING,
						msm_dgt_read,
						CLOCKSOURCE_MASK(32),
						24,
						CLOCK_SOURCE_IS_CONTINUOUS,
				},
				INT_DEBUG_TIMER_EXP,
				MSM_TMR_BASE + 0x24,
				MSM_CLOCK_DGT,
				9,
		},

};
#endif
#define NR_TIMERS ARRAY_SIZE(krait_timers)

// For Some reason this does not seem  to work but the above does
#if 0
static struct Krait_TIMER krait_timers[] = {
	[MSM_CLOCK_GPT] = {
		.clockevent = {
			.name           = "gp_timer",
			.features       = CLOCK_EVT_FEAT_ONESHOT,
			.shift          = 32,
			.rating         = 200,
			.set_next_event = msm_timer_set_next_event,
			.set_mode       = msm_timer_set_mode,
		},
		.clocksource = {
			.name           = "gp_timer",
			.rating         = 200,
			.read           = msm_gpt_read,
			.mask           = CLOCKSOURCE_MASK(32),
			.shift          = 17,
			.flags          = CLOCK_SOURCE_IS_CONTINUOUS,
		},
		.irq = INT_GP_TIMER_EXP,
		.regbase = MSM_TMR_BASE + 0x4,
		.freq = 32768,
		.index = MSM_CLOCK_GPT,
		.write_delay = 9,
	},
	[MSM_CLOCK_DGT] = {
		.clockevent = {
			.name           = "dg_timer",
			.features       = CLOCK_EVT_FEAT_ONESHOT,
			.shift          = 32,
			.rating         = DG_TIMER_RATING,
			.set_next_event = msm_timer_set_next_event,
			.set_mode       = msm_timer_set_mode,
		},
		.clocksource = {
			.name           = "dg_timer",
			.rating         = DG_TIMER_RATING,
			.read           = msm_dgt_read,
			.mask           = CLOCKSOURCE_MASK(32),
			.shift          = 24,
			.flags          = CLOCK_SOURCE_IS_CONTINUOUS,
		},
		.irq = INT_DEBUG_TIMER_EXP,
		.regbase = MSM_TMR_BASE + 0x24,
		.index = MSM_CLOCK_DGT,
		.write_delay = 9,
	}
};
#endif


#endif /* KRAIT__TIMER_H_ */
