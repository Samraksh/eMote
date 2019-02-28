/*******************************************************************************
* Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
* Author: Ismail H. Kose <ismail.kose@maximintegrated.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/

#include "..\c_code_calling_cpp.h"
#include "..\btcore\btstack_debug.h"

/***** Definitions *****/
#define USE_RTC_SYSTEM_CLK 0
#define SYSTICK_PERIOD_EXT_CLK 32767

/* Trigger interrupt every second */
static const uint32_t sysTicks = 100000000;
static volatile uint32_t sys_tick_sec = 0;



void systick_handler(void)
{
	sys_tick_sec++;
}

int32_t hal_tick_init(void)
{
	uint32_t ret;

	int ticks = 1000000;
	// one sec interrupt
	SetBTTimerInterrupt(ticks, systick_handler);

	return 0;
}

// returns in usec ticks regardless of clock
uint64_t hal_get_tick(void)
{
	uint64_t tick_sec;

	// system clock is 100 MHz
	uint64_t systick_val = BTGetTicks();
	
	// time in usec
	tick_sec = systick_val * 100;

	return tick_sec;
}

void hal_delay_ms(unsigned int ms)
{
	mf_delay_us(ms*1000);
}

void hal_delay_us(unsigned int us)
{
	mf_delay_us(us);
}

uint32_t hal_get_time_ms(void)
{
	uint64_t tick_sec;

	// system clock is 100 MHz
	uint64_t systick_val = BTGetTicks();
	
	// time in msec
	tick_sec = systick_val * 100000;
	return tick_sec;
}

uint32_t hal_time_ms(void)
{
	return hal_get_time_ms();
}
