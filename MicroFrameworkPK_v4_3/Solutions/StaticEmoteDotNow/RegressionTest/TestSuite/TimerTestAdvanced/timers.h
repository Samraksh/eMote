////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#define _EMBEDDED_

#include "fixedPoint.h"
#include "testMath.h"

#include "..\..\Log\Log.h"

#ifndef _Timers_TEST_
#define _Timers_TEST_ 1

#define SUCCESS 1
#define FAIL 0

#define LEVEL_0_A 0
#define LEVEL_0_B 1
#define LEVEL_1_A 2
#define LEVEL_1_B 3
#define LEVEL_2	4
#define TESTRNDGEN 5

#define CALL_BACK_FUNCTION(Timer) Timers::ISR_TIMER_Timer

class Timers
{
	int seedValue;
	int numberOfEvents;
	testMath testMathInstance;

public:

//--//
             Timers ( int seedValue, int numberOfEvents );

    //void ISR_TIMER( void* Param );

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A(int Timer);

	BOOL 	 Level_0B();

	BOOL     Level_1A();

	BOOL     Level_1B(int Timer);

	BOOL     Level_2();

	BOOL  	 testMathLibrary();



    //static void ISR_TIMER( void* Param );

	//void ISR_TIMER_3( void* Param );

	//void ISR_TIMER_4( void* Param );

};

#endif

