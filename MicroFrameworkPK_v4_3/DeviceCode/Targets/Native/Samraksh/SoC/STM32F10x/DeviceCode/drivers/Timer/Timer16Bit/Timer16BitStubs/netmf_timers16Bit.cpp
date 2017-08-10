/*
 * Samraksh Timer Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Notes -
 * 1) Driver uses NVIC directly, rewrite parts with INTC
 * 2) The delay between initializing timer and enabling interrupts
 */

#include "netmf_timers16Bit.h"

Timer16Bit_Driver g_Timer16Bit_Driver;

HAL_CALLBACK_FPN HALTimerHandlerFPN = NULL;




BOOL Timer16Bit_Driver::Timer_Test()
{
	return TRUE;
}

UINT8 Timer16Bit_Driver::System_Clock_Frequency()
{
	
	return 0;
}

BOOL Timer16Bit_Driver::Initialize( UINT32 Timer, BOOL IsOneShot, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
	

	return TRUE;
}

BOOL Timer16Bit_Driver::Uninitialize ( UINT32 Timer )
{
   

    return TRUE;
}

UINT32 Timer16Bit_Driver::GetMaxTicks()
{
	return (UINT32)0xFFFF;
}

void Timer16Bit_Driver::RCC_DeInit(UINT32 Timer)
{

}

void Timer16Bit_Driver::RCC_Init( UINT32 Timer, UINT32 Clock_Prescaler )
{

}


