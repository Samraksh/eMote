
// A simple timer implementation using the Cortex M3 24-bit counter @ 1 MHz
// Because TinyBooter doesn't need the full VT system
// --NPS

//#include "SimpleTimer.h"
//#include <Time_decl.h>
#include <max3263x.h>
#include <tinyhal.h>
#include <CPU_INTC_Decl.h>


#define MAX_RELOAD 0x00FFFFFF
#define TIME_BASE_FACTOR 10 // 10 MHz / 1 MHz
#define SYSTICK_RESET_COMPENSATE_INT 0
#define SYSTICK_RESET_COMPENSATE_OTHER 0

static volatile UINT64 bigCounter;

extern "C" {
void SimpleTimer_Handler(void *param);
}

BOOL HAL_Time_Initialize()
{
	/*NVIC_InitTypeDef NVIC_InitStructure;
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/

	//NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

	CPU_INTC_Initialize (  );

	if( !CPU_INTC_ActivateInterrupt(SysTick_IRQn, SimpleTimer_Handler, NULL) )
			return DS_Fail;

	bigCounter = 0;

	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->LOAD = MAX_RELOAD;
	SysTick->VAL = 0;
	//SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL  = SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	return TRUE;
}

// Must catch every interrupt before next roll-over, so should be 1st or 2nd highest priority
extern "C" {
void SimpleTimer_Handler(void *param) {
	bigCounter += SysTick->LOAD + (SysTick->LOAD - SysTick->VAL) + SYSTICK_RESET_COMPENSATE_INT;
	SysTick->LOAD = MAX_RELOAD;
	SysTick->VAL = 0;
	HAL_COMPLETION::DequeueAndExec();
}
}

BOOL HAL_Time_Uninitialize()
{
	SysTick->CTRL = 0;
	return TRUE;
}

// Convert to 100ns ticks
INT64 HAL_Time_CurrentTime()
{
	// HAL_Time_CurrentTicks() is 1 MHz timebase, need 10 MHz (100ns)
	return HAL_Time_CurrentTicks()*TIME_BASE_FACTOR;
}

UINT64 Time_CurrentTicks()
{
	return HAL_Time_CurrentTicks();
}


UINT64 HAL_Time_CurrentTicks()
{
	GLOBAL_LOCK(irq);
	return bigCounter + (SysTick->LOAD - SysTick->VAL);
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	return Ticks*TIME_BASE_FACTOR;
}

static void SetCompare(UINT64 t) {

	GLOBAL_LOCK(irq);

	// Catch it later if too far out
	if ( (t - bigCounter - 1) > MAX_RELOAD) {
		return;
	}

	// Flush the timer
	bigCounter += (SysTick->LOAD - SysTick->VAL) + SYSTICK_RESET_COMPENSATE_OTHER;
	SysTick->LOAD = (t - bigCounter - 1);
	SysTick->VAL = 0;
}

void HAL_Time_SetCompare(UINT64 CompareValue)
{
	SetCompare(CompareValue);
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
	*a = 1;
	*b = 1;
	*c = 0;
}

// Happily, 1 MHz == 1 microsecond, so we can count ticks
void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
	GLOBAL_LOCK(irq);
	HAL_Time_Sleep_MicroSeconds_InterruptEnabled(uSec);
}

// Happily, 1 MHz == 1 microsecond, so we can count ticks
void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	UINT64 target = bigCounter + (SysTick->LOAD - SysTick->VAL) + uSec;
	while(bigCounter + (SysTick->LOAD - SysTick->VAL) < target); // spin
}

UINT32 CPU_TicksPerSecond(UINT16 t) {
	return 1000000;
}

UINT32 CPU_MicrosecondsToTicks(unsigned int uSec, unsigned short timer) {
	return uSec;
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec, UINT16 Timer ) {
    return uSec;
}

UINT64 CPU_MillisecondsToTicks(unsigned int ms, unsigned short timer) {
	return ms*1000;
}
