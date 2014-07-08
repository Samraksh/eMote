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

// Maximum match value.
#define MAX_TIMER_ROLLOVER 0xFFFFFF00

// Maximum number of microseconds for a simple sleep_usec
// Based on 32-bit DGT running at 6.75 MHz
#define MAX_USEC_SLEEP 636291451

BOOL InitializeTimer ( UINT16 Timer );
void SetCompare(UINT16 Timer, UINT32 Compare);
UINT32 GetCounter(UINT16 Timer);


#endif /* KRAIT__TIMER_H_ */
