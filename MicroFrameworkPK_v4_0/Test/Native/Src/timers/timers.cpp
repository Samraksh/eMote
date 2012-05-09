////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Timers.h"

#if defined PLATFORM_ARM_STM32F10X
#include "C:\MicroFrameworkPK_v4_0\Solutions\STM32F10x\DeviceCode\drivers\tim\netmf_timers.h"
#endif
//---//

int testCount = 0;

void ISR_TIMER_4( void* Param );
void ISR_TIMER_3( void* Param );
void ISR_TIMER_2( void* Param );

Timers::Timers( int seedValue, int numberOfEvents )
{
	this->seedValue = seedValue;
	this->numberOfEvents = numberOfEvents;
	this->testMathInstance.prng_init(seedValue);
	// Initializes the gpio pins
	CPU_GPIO_Initialize();
	CPU_INTC_Initialize();
};

BOOL Timers::testMathLibrary()
{
	Uint16 loopIterator = 0;

	Uint32 rndmValue = 0;

	for(loopIterator = 0; loopIterator < this->numberOfEvents; loopIterator++)
	{
		rndmValue = Timers::getRndmNumber();

		if(-1 == rndmValue)
		{
			return FAIL;
		}
		else
		{
			hal_printf("\n%d",rndmValue);
		}

	}

	return SUCCESS;

}

BOOL Timers::Level_0A(int Timer)
{
	// Initialize the timer in free running mode with ISR_TIMER_4 as interrupt handler
	if (!Timer_Driver :: Initialize (Timer, TRUE, 0, 0, CALL_BACK_FUNCTION(Timer), NULL))
	{
				return FALSE;
	}

	Timer_Driver::SetCompare( Timer, Timers::getRndmValue());

	while(testCount < 100)
	{

	}

	return TRUE;

}

BOOL Timers::Level_0B()
{
}

BOOL Timers::Level_1A()
{
}

BOOL Timers::Level_1B()
{
}

BOOL Timers::Level_2()
{
}



BOOL Timers::Execute( int testLevel )
{

	// Indicates the start of the test
	CPU_GPIO_EnableOutputPin(1,TRUE);

	// Configure Pin 2 as the pin of interest
	CPU_GPIO_EnableOutputPin(2,FALSE);

	if(testLevel == TESTRNDGEN)
	{
		if(Timers::testMathLibrary(seedValue,numberOfEvent))
		{
			CPU_GPIO_SetPinState(1,FALSE);
			return SUCCESS;
		}
		else
		{
			CPU_GPIO_SetPinState(1,FALSE);
			return FAIL;
		}

	}

	/*
	// Initialize Hal Level Time Driver
	if (!Timer_Driver :: Initialize (4, TRUE, 0, 0, ISR_TIMER_4, NULL))
		{
			return FALSE;
		}

	// Sets compare value to 1000 count values
	Timer_Driver::SetCompare( 4, 1000 );

	while(testCount < 1000)
	{

	}

	// Indicates the end of the test
	CPU_GPIO_SetPinState(1, FALSE);
	*/

}; //Execute

void ISR_TIMER_4( void* Param )
{
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	  {
	    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
	    Timer_Driver::SetCompare( 4, Timers::getRndmValue());
	  }

	if(testCount % 2 == 0)
		CPU_GPIO_SetPinState(2, TRUE);
	else
		CPU_GPIO_SetPinState(2,FALSE);
	testCount++;

}

//--//

