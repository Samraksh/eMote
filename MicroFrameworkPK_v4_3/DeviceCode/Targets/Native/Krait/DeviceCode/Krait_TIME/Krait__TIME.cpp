/*
 *  Name : Krait__TIMER.cpp
 *
 *  Author : Nived.Sivadas@Samraksh.com
 *
 *  Description : The main system time is maintained here. The hardware clock used is the debug timer 
 *
 *  History : v1.0 - Initial Version - Nived.Sivadas
 *
 *
 */


#include <tinyhal.h>

#include "Krait__Time.h"

extern Krait_TIMER_Driver g_Krait_Timer_Driver;

Krait_TIME_Driver g_Krait_TIME_Driver;

extern "C"
{
void TIMER_HANDLER(void *param)
{
#if defined(DEBUG_TIMER)
	CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);
#endif
	
	if(g_Krait_TIME_Driver.CounterValue() >= g_Krait_TIME_Driver.m_nextCompare)
	{
	     // this also schedules the next one, if there is one
	     HAL_COMPLETION::DequeueAndExec();
	}
	else
	{
	        g_Krait_TIME_Driver.SetCompareValue( g_Krait_TIME_Driver.m_nextCompare );
	}
}
}

BOOL Krait_TIME_Driver::Initialize()
{
#if defined(DEBUG_TIMER)
	CPU_GPIO_EnableOutputPin((GPIO_PIN) 53, FALSE);
#endif
	
	// Initial numbers pulled from the Imote, Why ? Don't know, "the dots don't connect looking ahead" - Steve Jobs
	g_Krait_TIME_Driver.m_lastRead    = 0;
	// What is this magical 6750000 number ? pulled from lk : Ans : Looks like 6750000 means the timer fires every 1s 
	g_Krait_TIME_Driver.m_nextCompare = 6750000;
	
	g_Krait_TIME_Driver.m_lastCompare = g_Krait_TIME_Driver.m_nextCompare;

	// This function intially sets the compare value to 6750000 intially, rendering the next line moot
	g_Krait_Timer_Driver.Initialize(0, TRUE, 0, 0, TIMER_HANDLER, NULL);
	
	//Krait_TIMER_Driver::SetCompare( Krait_TIMER_Driver::c_SystemTimer, c_OverflowCheck );

	//Krait_TIMER_Driver::EnableCompareInterrupt( Krait_TIMER_Driver::c_SystemTimer );


	return TRUE;
}
BOOL Krait_TIME_Driver::Uninitialize(){
	return TRUE;
}

UINT64 Krait_TIME_Driver::CounterValue()
{
	GLOBAL_LOCK(irq);

	UINT32 lastLowValue = (UINT32)(g_Krait_TIME_Driver.m_lastRead & 0x00000000FFFFFFFFull);

	UINT32 value = Krait_TIMER_Driver::GetCounter (Krait_TIMER_Driver::c_SystemTimer);

	g_Krait_TIME_Driver.m_lastRead &= (0xFFFFFFFF00000000ull);

	if(lastLowValue > value )
	{
		//DEBUG_TRACE3(TRACE_COUNTER_OVERFLOWS,"CounterValue: Overflow %08x %08x=>%08x", (g_PXA271_TIME_Driver.m_lastRead >> 32), lastLowValue, value);
		//hal_printf("System Timer Overflowed, CounterValue : Overflow %08x %08x=>%08x", (g_Krait_TIME_Driver.m_lastRead >> 32), lastLowValue, value);

		// The following line allows Sleep to work properly but breaks the running of C# code
		// g_Krait_TIME_Driver.m_lastRead += g_Krait_TIME_Driver.m_lastCompare;
		g_Krait_TIME_Driver.m_lastRead += (0x1ull << 32);
	}
	    //Or else the value gets added simply
	g_Krait_TIME_Driver.m_lastRead |= value;

	return g_Krait_TIME_Driver.m_lastRead;

}


void Krait_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{
    GLOBAL_LOCK(irq);
    
    g_Krait_TIME_Driver.m_lastCompare = g_Krait_TIME_Driver.m_nextCompare;
    
    g_Krait_TIME_Driver.m_nextCompare = CompareValue;

    bool fForceInterrupt = false;

    // At this point we don't know if there is a register that we can check so the timer just returns false here and reset does not do anything
    // We can afford to do this because the hardware automatically clears the interrupt
    if(Krait_TIMER_Driver::DidCompareHit( Krait_TIMER_Driver :: c_SystemTimer ))
    {
        Krait_TIMER_Driver::ResetCompareHit( Krait_TIMER_Driver :: c_SystemTimer );
    }

    UINT64 CntrValue = CounterValue();

    if(CompareValue <= CntrValue)
    {
	// Hmm, by the time we reach here the time has already passed
        fForceInterrupt = true; 
    }
    else
    {
        UINT32 diff;

        if((CompareValue - CntrValue) > 0xFFFFFFFFull)
        {
            diff = 0xFFFFFFFFul;
        }
        else
        {
            diff = (UINT32)(CompareValue - CntrValue);
        }        

        Krait_TIMER_Driver::SetCompare( Krait_TIMER_Driver :: c_SystemTimer,  Krait_TIMER_Driver::GetCounter( Krait_TIMER_Driver :: c_SystemTimer ) + diff );

        if(CounterValue() > CompareValue)
        {
            fForceInterrupt = true;
        }
    }

    if(fForceInterrupt)
    {
	
        // Force interrupt to process this.
        //Krait_TIMER_Driver::ForceInterrupt( Krait_TIMER_Driver :: c_SystemTimer );
	// Set compare pretty soon, since we dont seem to have/know a force interrupt register 
	Krait_TIMER_Driver::SetCompare( Krait_TIMER_Driver :: c_SystemTimer,  Krait_TIMER_Driver::GetCounter( Krait_TIMER_Driver :: c_SystemTimer ) + 100 );
    }
	
}

INT64 Krait_TIME_Driver::TicksToTime( UINT64 Ticks )
{
	return CPU_TicksToTime( Ticks );
}

INT64 Krait_TIME_Driver::CurrentTime()
{
	INT64 Time = CPU_TicksToTime( CounterValue() );
	
	return Time;
}

void Krait_TIME_Driver::Sleep_uSec( UINT32 uSec )
{
     GLOBAL_LOCK(irq);

     UINT32 value   = Krait_TIMER_Driver::GetCounter( Krait_TIMER_Driver :: c_SystemTimer );
     UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks( uSec ); 
     
     while((Krait_TIMER_Driver::GetCounter( Krait_TIMER_Driver :: c_SystemTimer) - value) <= maxDiff);
     
}

// Supposed to be implemented by calculating the approx number of instruction executed during this time and doing a for loop like implementation corresponding to the number of instructions
// Also this function runs with interrupts enabled, may need to look at the actual implementation 
void Krait_TIME_Driver::Sleep_uSec_Loop( UINT32 uSec )
{

     UINT32 value   = Krait_TIMER_Driver::GetCounter( Krait_TIMER_Driver :: c_SystemTimer );
     UINT32 maxDiff  = CPU_MicrosecondsToSystemClocks( uSec ); 
     
     while((Krait_TIMER_Driver::GetCounter( Krait_TIMER_Driver :: c_SystemTimer) - value) <= maxDiff);
}


