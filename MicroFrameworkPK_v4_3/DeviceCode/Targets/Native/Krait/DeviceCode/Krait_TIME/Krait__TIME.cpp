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


#include <tinyhal.h>
#include "Krait__Time.h"
#include "Krait__Timer.h"

/******* TIMER CODE (AS OPPOSED TO TIME) **********/

// static prototypes
static void TIME_HANDLER(void *arg);
static inline void flush_timer();


// Code

static BOOL InitializeTimer ( UINT32 Timer )
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

	CPU_INTC_ActivateInterrupt(INT_DEBUG_TIMER_EXP, TIME_HANDLER, 0);
	CPU_INTC_InterruptEnable( INT_DEBUG_TIMER_EXP );
	
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
static void SetCompare(UINT32 Timer, UINT32 Compare)
{
	UINT32 now;
	flush_timer(); // Flush DGT to bigCounter
	writel(Compare, DGT_MATCH_VAL); // let the overflow do its magic
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

static inline UINT32 GetCounter(UINT32 Timer)
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

/******* TIME CODE (AS OPPOSED TO TIMER) **********/

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
		HAL_COMPLETION::DequeueAndExec();
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

UINT64 TimeNow()
{
	return bigCounter + GetCounter(0);
}

#define TICKS_PROXIMITY_FORCE 50
void SetCompareValue( UINT64 CompareValue )
{
	UINT32 diff;
	UINT64 now;
	int forceInterrupt=0;
	
	if (nextCompare != 0 && CompareValue > nextCompare) {
		//hal_printf("debug: new compare after current compare\r\n");
		return;
	}

    GLOBAL_LOCK(irq);

	nextCompare = CompareValue;

	now = TimeNow();
	if( CompareValue - now <= TICKS_PROXIMITY_FORCE ) {
		forceInterrupt=1;
		//hal_printf("force interrupt\r\n");
	}
	else if(CompareValue - now > MAX_TIMER_ROLLOVER) { // 0xFFFFFF00
		//hal_printf("debug: new compare too far in future, deferring\r\n");
		return; // We'll get it later
	}

	if (forceInterrupt)
		diff = TICKS_PROXIMITY_FORCE; // a small time in the future
	else
		diff = CompareValue - now;

	//hal_printf("debug: new compare set to: %d\r\n",diff);

	SetCompare(0,  diff);
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
#define TINYCLR_TIMER_MUNGE 13

void Sleep_uSec( UINT32 uSec )
{
	GLOBAL_LOCK(irq);

	if(uSec <= TINYCLR_TIMER_MUNGE) {
		return;
	}

	UINT32 value   = GetCounter( 0 );
	UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks( uSec-TINYCLR_TIMER_MUNGE ); 

	while((GetCounter(0) - value) <= maxDiff);
}

// Supposed to be implemented by calculating the approx number of instruction executed during this time and doing a for loop like implementation corresponding to the number of instructions
// Also this function runs with interrupts enabled, may need to look at the actual implementation 
void Sleep_uSec_Loop( UINT32 uSec )
{
	if(uSec <= TINYCLR_TIMER_MUNGE) {
		return;
	}

	UINT32 value   = GetCounter( 0 );
	UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks(uSec - TINYCLR_TIMER_MUNGE); 

	while((GetCounter(0) - value) <= maxDiff);
}
