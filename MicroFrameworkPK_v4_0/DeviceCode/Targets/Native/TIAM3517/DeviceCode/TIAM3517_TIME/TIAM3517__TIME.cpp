/*
 * Name : TIAM3517__TIME.cpp
 *
 * Author : nived.sivadas@samrkash.com
 *
 * Description :
 */


#include <tinyhal.h>
#include "..\TIAM3517.h"

TIAM3517_TIME_Driver g_TIAM3517_TIME_Driver;

BOOL TIAM3517_TIME_Driver::Initialize()
{
	g_TIAM3517_TIME_Driver.m_lastRead    = 0;
	g_TIAM3517_TIME_Driver.m_nextCompare = 0x0000FFFFFFFFFFFFull;

	// Initializes the timer1 as the system time timer
	if(!(TIAM3517_TIMER_Driver::Initialize(TIAM3517_Driver::c_SystemTime_Timer,TRUE,0,0, ISR, NULL)))
	{
		return FALSE;
	}

	TIAM3517_TIMER_Driver::SetCompare( TIAM3517_Driver::c_SystemTime_Timer, c_OverflowCheck );

    TIAM3517_TIMER_Driver::EnableCompareInterrupt( TIAM3517_Driver::c_SystemTime_Timer );

	return TRUE;

}


BOOL TIAM3517_TIME_Driver::Uninitialize()
{
	if(!TIAM3517_TIMER_Driver::Uninitialize( TIAM3517_Driver::c_SystemTime_Timer )) return FALSE;

	    return TRUE;

}

UINT64 TIAM3517_TIME_Driver::CounterValue()
{
	 GLOBAL_LOCK(irq);

	 UINT32 lastLowValue = (UINT32)(g_TIAM3517_TIME_Driver.m_lastRead & 0x00000000FFFFFFFFull);

	 UINT32 value = TIAM3517_TIMER_Driver::GetCounter( TIAM3517_Driver::c_SystemTime_Timer );

	 g_TIAM3517_TIME_Driver.m_lastRead &= (0xFFFFFFFF00000000ull);

	 if(lastLowValue > value )
	 {
	      //DEBUG_TRACE3(TRACE_COUNTER_OVERFLOWS,"CounterValue: Overflow %08x %08x=>%08x", (g_TIAM3517_TIME_Driver.m_lastRead >> 32), lastLowValue, value);

	      g_TIAM3517_TIME_Driver.m_lastRead += (0x1ull << 32);
	 }

    g_TIAM3517_TIME_Driver.m_lastRead |= value;

    return g_TIAM3517_TIME_Driver.m_lastRead;

}

void TIAM3517_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{
	GLOBAL_LOCK(irq);

	g_TIAM3517_TIME_Driver.m_nextCompare = CompareValue;

	bool fForceInterrupt = false;

	if(TIAM3517_TIMER_Driver::DidCompareHit( TIAM3517_Driver::c_SystemTime_Timer ))
	{
			TIAM3517_TIMER_Driver::ResetCompareHit( TIAM3517_Driver::c_SystemTime_Timer );
	}

	UINT64 CntrValue = CounterValue();

	if(CompareValue <= CntrValue)
	{
	        fForceInterrupt = true; //DEBUG_TRACE1(TRACE_SETCOMPARE,"ForceInterrupt: %d\r\n", __LINE__);
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

			TIAM3517_TIMER_Driver::SetCompare( TIAM3517_Driver::c_SystemTime_Timer, TIAM3517_TIMER_Driver::GetCounter( TIAM3517_Driver::c_SystemTime_Timer ) + diff );

           if(CounterValue() > CompareValue)
           {
                fForceInterrupt = true;
           }
     }

     if(fForceInterrupt)
     {
	            // Force interrupt to process this.
    	 TIAM3517_TIMER_Driver::ForceInterrupt( TIAM3517_Driver::c_SystemTime_Timer );
     }

}

void TIAM3517_TIME_Driver::ISR(void * Param)
{

		if(TIAM3517_TIMER_Driver::DidCompareHit( TIAM3517_Driver::c_SystemTime_Timer ))
	    {
	        TIAM3517_TIMER_Driver::ResetCompareHit( TIAM3517_Driver::c_SystemTime_Timer );
	    }

	    if(CounterValue() >= g_TIAM3517_TIME_Driver.m_nextCompare)
	    {
	        // this also schedules the next one, if there is one
	        HAL_COMPLETION::DequeueAndExec();
	    }
	    else
	    {
	        //
	        // Because we are limited in the resolution of timer,
	        // resetting the compare will properly configure the next interrupt.
	        //
	        SetCompareValue( g_TIAM3517_TIME_Driver.m_nextCompare );
	    }

}


INT64 TIAM3517_TIME_Driver::TicksToTime(UINT64 Ticks)
{
	return CPU_TicksToTime(Ticks);
}

INT64 TIAM3517_TIME_Driver::CurrentTime()
{


	INT64 Time = CPU_TicksToTime( CounterValue() );

	return Time;

}


void TIAM3517_TIME_Driver::Sleep_uSec(UINT32 uSec)
{




}


void TIAM3517_TIME_Driver::Sleep_uSec_Loop(UINT32 uSec)
{
	long tmo = uSec * (SYSTEM_CLOCK_HZ / 1000) / 1000;
	UINT32 COUNTERMAX=0xFFFFFFFF;
	UINT32 now,last = TIAM3517_TIMER_Driver::GetCounter( TIAM3517_Driver::c_SystemTime_Timer );
	//unsigned long now, last = readl(&timer_base->tcrr);

	while (tmo > 0) {
		now = TIAM3517_TIMER_Driver::GetCounter( TIAM3517_Driver::c_SystemTime_Timer );
		if (last > now) /* count up timer overflow */
			tmo -= (COUNTERMAX - last) + now;
		else{
			tmo -= now - last;
		}
		last=now;
	}
}


