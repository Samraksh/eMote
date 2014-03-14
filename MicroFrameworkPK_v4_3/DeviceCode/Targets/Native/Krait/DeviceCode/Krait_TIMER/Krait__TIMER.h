/*
 * Krait__TIMER.h
 *
 *  Created on: Sep 14, 2012
 *      Author: Mukundan Sridharan
 */

#ifndef KRAIT__TIMER_H_
#define KRAIT__TIMER_H_
#include <tinyhal.h>
#include "..\Krait.h"

#define GPT_ENABLE_CLR_ON_MATCH_EN        2
#define GPT_ENABLE_EN                     1
#define DGT_ENABLE_CLR_ON_MATCH_EN        2
#define DGT_ENABLE_EN                     1

#define GIC_PPI_START 16
#define GIC_SPI_START 32

#define INT_DEBUG_TIMER_EXP     (GIC_PPI_START + 1)

#define SPSS_TIMER_STATUS_DGT_EN    (1 << 0)

#define MSM_TMR_BASE        0x0200A000
#define MSM_DGT_BASE        (MSM_TMR_BASE + 0x24)
#define DGT_REG(off)        (MSM_DGT_BASE + (off))
#define DGT_ENABLE           DGT_REG(0x0008)
#define DGT_CLK_CTL          DGT_REG(0x0010)
#define DGT_MATCH_VAL        DGT_REG(0x0000)
#define DGT_CLEAR            DGT_REG(0x000C)

#define DGT 0

typedef enum handler_return (*platform_timer_callback)(void *arg, time_t now);

//status_t platform_set_periodic_timer(platform_timer_callback callback, void *arg, time_t interval);

void mdelay(unsigned msecs);
void udelay(unsigned usecs);

extern "C"
{
UINT32 platform_tick_rate(void);
}



struct Krait_TIMER_Driver
{
	static const UINT32 c_SystemTimer   = DGT;
	static const UINT32 c_MaxTimers = 1;


	static BOOL Initialize( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param );
	static BOOL Uninitialize(UINT32 Timer);
	static void EnableCompareInterrupt(UINT32 Timer);
	static void DisableCompareInterrupt(UINT32 Timer);
	static void ForceInterrupt(UINT32 Timer);
	static void SetCompare(UINT32 Timer, UINT32 Compare);
	static UINT32 GetCompare(UINT32 Timer);
	static void SetCounter(UINT32 Timer, UINT32 Count);
	static UINT32 GetCounter(UINT32 Timer);
	static BOOL DidCompareHit(UINT32 Timer);
	static void ResetCompareHit(UINT32 Timer);

};

#endif /* KRAIT__TIMER_H_ */
