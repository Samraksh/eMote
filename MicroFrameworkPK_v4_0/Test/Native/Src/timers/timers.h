////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "testMath.h"
#include "..\Log\Log.h"

#ifndef _Timers_TEST_
#define _Timers_TEST_ 1

#define SUCCESS 1
#define FAIL 0

#define LEVEL_0.A 0
#define LEVEL_0.B 1
#define LEVEL_1.A 2
#define LEVEL_1.B 3
#define LEVEL_2	4
#define TESTRNDGEN 5

#define CALLBACKFUNCTION(Timer) ISR_TIMER_Timer

class Timers
{
	int seedValue;
	int numberOfEvent;
	testMath testMathInstance;

public:

//--//
             Timers ( int seedValue, int numberOfEvents );

    static long getRndmNumber()
    {
    	return this->testMathInstance.prng();
    }

    BOOL     Execute( int testLevel );

	BOOL 	 Level_0A();

	BOOL 	 Level_0B();

	BOOL     Level_1A();

	BOOL     Level_1B();

	BOOL     Level_2();

	BOOL  	 testMathLibrary();

};

#endif

