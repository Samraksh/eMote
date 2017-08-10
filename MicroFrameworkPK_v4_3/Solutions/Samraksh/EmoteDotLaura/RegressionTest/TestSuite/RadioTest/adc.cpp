////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Timers.h"
#include "c:\MicroFrameworkPK_v4_0\Solutions\Samraksh\Samraksh\STM32F10x\DeviceCode\drivers\tim\netmf_timers.h"

//---//

int testCount = 0;

testMath g_testMathInstance;

UINT16 randomBuffer[600];
UINT16 bufferIterator = 0;

void ISR_TIMER( void* Param );

ADC::ADC( int seedValue, int numberOfEvents )
{
	this->seedValue = seedValue;
	this->numberOfEvents = numberOfEvents;
	g_testMathInstance.prng_init(seedValue);
	CPU_GPIO_Initialize();
	CPU_INTC_Initialize();
	
	for(int i = 0; i < 600; i++)
		randomBuffer[i] = g_testMathInstance.pareto_prng();


}

BOOL ADC::Level_0()
{



}

BOOL ADC::Level_1()
{


}



