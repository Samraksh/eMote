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
#include "Krait__TIMER.h"


// static prototypes
static void TIMER_HANDLER(void *arg);
//static inline void flush_timer();
//static void Timer_Handler(void* arg);


// Code

//AnanthAtSamraksh
#include "../Krait_INTC/Krait__INTC.h"
//#include "../Krait_TIME/Krait__TIME.h"
static struct ihandler handler[NR_IRQS];

////UINT64 Krait_Timer::bigCounter = 0;
UINT32 Krait_Timer::nextCompare = 0;
Krait_Timer g_Krait_Timer;
//extern Krait_Time g_Krait_Time;

//AnanthAtSamraksh
/*extern "C"
{

void Timer_Handler(void* arg)
{
	g_Krait_Time.TIME_HANDLER(arg);
}

}*/

//AnanthAtSamraksh

BOOL Krait_Timer::InitializeTimer ( UINT16 Timer, BOOL IsOneShot, HAL_CALLBACK_FPN ISR, void* ISR_PARAM )
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

	//AnanthAtSamraksh - there is just one timer (0), so store ISR in position 0.
	/*handler[Timer].func = (int_handler)ISR;
	handler[Timer].arg = ISR_PARAM;*/
	handler[0].func = (int_handler)ISR;
	handler[0].arg = ISR_PARAM;
	timerOverflowFlag = FALSE;
	g_Krait_Timer.IsOneShot = IsOneShot;
	//AnanthAtSamraksh

	CPU_INTC_ActivateInterrupt(INT_DEBUG_TIMER_EXP, TIMER_HANDLER, 0);
	CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );

	hal_printf("\rKrait DGT initialized\r\n");

	return TRUE;
}


// Add counter contents and reset to 0. IRQs should be off.
/*static inline void flush_timer() {
	g_Krait_Timer.bigCounter += readl(DGT_COUNT_VAL);
	writel(0, DGT_CLEAR);
}*/

// Main handler called on overflow.
static void TIMER_HANDLER(void *arg) {
	////g_Krait_Time.flush_time();
	//AnanthAtSamraksh
	////flush_timer();

	UINT32 currentTicks = readl(DGT_COUNT_VAL);

	writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL); // reset the match value to default
	g_Krait_Timer.nextCompare = 0;
	handler[0].func(handler[0].arg);

	if(g_Krait_Timer.IsOneShot)
	{
		handler[0] = {0};
	}
	// Don't need to call TimeNow() since we just flushed
	//if(g_Krait_Timer.bigCounter >= g_Krait_Timer.nextCompare)
	//if(currentTicks >= g_Krait_Timer.nextCompare)
	/*if(g_Krait_Timer.IsOneShot)
	{
		// this also schedules the next one, if there is one
		//AnanthAtSamraksh -- commenting out below as it sets the setCompare to go off after a very long time and stops the callback from being invoked.
		writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL); // reset the match value to default
		g_Krait_Timer.nextCompare = 0;

		////HAL_COMPLETION::DequeueAndExec();
		//AnanthAtSamraksh
		//// writel(Compare, DGT_MATCH_VAL) is setCompare and takes absolute time (current + future) in ticks.
		//// When time expires, an interrupt is raised which calls the function registered while enabling the interrupt, which happens to be TIMER_HANDLER in this case.
		////writel(readl(DGT_COUNT_VAL) + 0x10000, DGT_MATCH_VAL);

		//uint32_t num = 0;
		//handler[num].func(handler[num].arg);
		handler[0].func(handler[0].arg);
		handler[0] = {0};
	}
	else
	{*/
		//overflow case
		//g_Krait_Timer.SetCompare( (UINT16)0, g_Krait_Timer.nextCompare );
		//CPU_Timer_SetCompare(0, g_Krait_Timer.nextCompare);

		/*CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
		CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);*/

		//writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL); // reset the match value to default
		//if((currentTicks + g_Krait_Timer.nextCompare) < 0x3FFFF000)
		/*if((currentTicks + g_Krait_Timer.nextCompare) > 0x3FFFFFFF)
		{
			//debug_printf("TIMER_HANDLER - clearing timer\r\n");
			writel(0, DGT_CLEAR);
			g_Krait_Timer.SetCompare(0, g_Krait_Timer.nextCompare);
		}*/
		//else
			//g_Krait_Timer.SetCompare(0, (currentTicks + g_Krait_Timer.nextCompare));
		////debug_printf("currentTicks: %d \r\n", currentTicks);

		////g_Krait_Timer.SetCompare(0, g_Krait_Timer.nextCompare);
		////g_Krait_Timer.SetCompare(0, (currentTicks + g_Krait_Timer.nextCompare));

		/*writel(MAX_TIMER_ROLLOVER, DGT_MATCH_VAL); // reset the match value to default
		g_Krait_Timer.nextCompare = 0;
		handler[0].func(handler[0].arg);
	}*/

	//AnanthAtSamraksh
	/*currentTicks = readl(DGT_COUNT_VAL);
	if(currentTicks > MAX_TIMER_ROLLOVER || currentTicks < 0xFFFF)
		g_Krait_Timer.timerOverflowFlag = TRUE;*/

	/*uint32_t num = 0;
	handler[num].func(handler[num].arg);

	UINT32 currentTicks = readl(DGT_COUNT_VAL);
	if(currentTicks > MAX_TIMER_ROLLOVER)
		g_Krait_Timer.timerOverflowFlag = TRUE;
	CPU_Timer_SetCompare(0, g_Krait_Timer.nextCompare & 0xffff);*/

	/*//AnanthAtSamraksh
	uint32_t num = 0;
	handler[num].func(handler[num].arg);
	//AnanthAtSamraksh*/
}


BOOL Krait_Timer::DidTimerOverflow()
{
	return timerOverflowFlag;
}

void Krait_Timer::ClearTimerOverflow()
{
	timerOverflowFlag = FALSE;
}

UINT32 Krait_Timer::GetMaxTicks()
{
	return 0xFFFFFFFF;	//2^32
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
	////g_Krait_Time.flush_time(); // Flush DGT to bigCounter
	//AnanthAtSamraksh
	////flush_timer(); // Flush DGT to bigCounter
	/*CPU_GPIO_SetPinState((GPIO_PIN) 52, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 52, FALSE);*/
	//if(Compare > 0x3FFFFF00)
		//debug_printf("Compare: %d \r\n", Compare);
	////writel(0, DGT_CLEAR);
	writel(Compare, DGT_MATCH_VAL);
}


static UINT32 GetCompare(UINT32 Timer)
{
	ASSERT(); // Shouldn't be used anywhere?
	return readl(DGT_MATCH_VAL);
}

// Nobody touches my counter! (except init).
void Krait_Timer::SetCounter(UINT32 Timer, UINT32 Count)
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
/*void HAL_Time_SetCompare_Completion(UINT64 val) {
	return;
}*/

static void ResetCompareHit(UINT32 Timer)
{
	ASSERT();
}





