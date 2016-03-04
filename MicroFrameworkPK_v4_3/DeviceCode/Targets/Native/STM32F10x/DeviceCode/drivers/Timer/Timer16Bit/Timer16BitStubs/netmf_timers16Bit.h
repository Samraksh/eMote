/*
 * Samraksh Timer Driver
 * Initial Create - Kartik Natarajan
 * 04/20/2011
 * Notes -
 * 1) Configuring the TIM2, 3, 4 as of now.
 * 2) Add clock source as a prescaled value from the APB1 clock
 */
#ifndef _NETMF_TIMERS_H_
#define _NETMF_TIMERS_H_


#include <tinyhal.h>
#include <stm32f10x.h>



#define MAX_TIMER 10
#define DEBUG_ON

struct Timer16Bit_Driver
{
	
#ifdef DEBUG_ON
	static BOOL Timer_Test();
#endif

	
	static BOOL Initialize   ( UINT32 Timer, BOOL FreeRunning, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL Uninitialize ( UINT32 Timer );
    static UINT8 System_Clock_Frequency();

    UINT32 GetMaxTicks();
    //static void Timer_IRQ_Handler( UINT32 Timer );

	static void EnableCompareInterrupt( UINT32 Timer )
    {

	}

	static void DisableCompareInterrupt( UINT32 Timer )
    {

	}

	static void ForceInterrupt( UINT32 Timer )
    {

	}

	static void SetCompare( UINT32 Timer, UINT32 Compare )
    {

	}

	static UINT16 GetCompare ( UINT32 Timer )
	{
		return 0;
	}

	static UINT16 GetCounter( UINT32 Timer )
    {
		return 0;
	}

	static void SetCounter ( UINT32 Timer, UINT32 Count )
	{

	}

	//TODO: Verify the working of this function from main.cpp

	static BOOL DidTimerOverFlow ( UINT32 Timer )
	{
		return FALSE;
	}

	//TODO: Verify the working of this function!

	static void ClearTimerOverFlow ( UINT32 Timer )
	{


	}

	static BOOL DidCompareHit( UINT32 Timer )
    {
		return FALSE;

	}

	static void ResetCompareHit (UINT32 Timer )
	{

	}

    struct Descriptors
    {
 
    };
	static void RCC_Init( UINT32 Timer, UINT32 Clock_Prescaler );
	static void RCC_DeInit(UINT32 Timer);
};


#endif
