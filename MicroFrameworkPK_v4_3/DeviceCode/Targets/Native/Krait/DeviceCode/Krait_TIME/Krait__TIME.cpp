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

#include "Krait__Time.h"

extern Krait_TIMER_Driver g_Krait_Timer_Driver;

Krait_TIME_Driver g_Krait_TIME_Driver;

extern "C"
{
void TIMER_HANDLER(void *param)
{
	CPU_GPIO_SetPinState((GPIO_PIN) 53, TRUE);
	CPU_GPIO_SetPinState((GPIO_PIN) 53, FALSE);
}
}

BOOL Krait_TIME_Driver::Initialize  (){

	CPU_GPIO_EnableOutputPin((GPIO_PIN) 53, FALSE);

	g_Krait_Timer_Driver.Initialize(0, TRUE, 0, 0, TIMER_HANDLER, NULL);

	return TRUE;
}
BOOL Krait_TIME_Driver::Uninitialize(){
	return TRUE;
}

UINT64 Krait_TIME_Driver::CounterValue()
{

	 UINT16 value = Krait_TIMER_Driver :: GetCounter (Krait_TIMER_Driver :: c_SystemTimer);

	 g_Krait_TIME_Driver.m_lastRead &= (0xFFFFFFFFFFFF0000ull);

	 // Need to figure out if there is an overflow register
#if 0
	    /*TODO: See comparison to see if the counter has over-flowed, if so update the
	     * m_lastRead value by the size of the counter
	     */
	    if(Krait_TIMER_Driver :: DidTimeOverFlow( Krait_TIMER_Driver::c_SystemTimer ))
	    {
	    	Krait_TIMER_Driver :: ClearTimeOverFlow( Krait_TIMER_Driver::c_SystemTimer );

	    	g_Krait_TIME_Driver.m_lastRead += (0x1ull << 32);
	    }
#endif
	    //Or else the value gets added simply
	    g_Krait_TIME_Driver.m_lastRead |= value;

	    return g_Krait_TIME_Driver.m_lastRead;

}


void Krait_TIME_Driver::SetCompareValue( UINT64 CompareValue )
{

}

INT64 Krait_TIME_Driver::TicksToTime( UINT64 Ticks )
{

	return 1;
}

INT64 Krait_TIME_Driver::CurrentTime()
{

	return 1;
}

void Krait_TIME_Driver::Sleep_uSec( UINT32 uSec )
{

}

void Krait_TIME_Driver::Sleep_uSec_Loop( UINT32 uSec )
{

}


