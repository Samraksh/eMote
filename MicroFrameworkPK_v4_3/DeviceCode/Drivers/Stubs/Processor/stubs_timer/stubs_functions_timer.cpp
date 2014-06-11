////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

UINT64 CPU_Timer_CurrentTicks()
{
	return 0;
}


INT64 CPU_Timer_TicksToTime( UINT64 Ticks )
{
	return 0;
}

INT64 CPU_Timer_CurrentTime()
{
	return 0;
}

void CPU_Timer_SetCompare( UINT64 CompareValue )
{
}


void CPU_Timer_SetCompare_Completion( UINT64 CompareValue )
{
}

void CPU_Timer_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    //*a = 1;
    //*b = 1;
    //*c = 0;

	// "correct"*A = raw*B + C
	// Correct is defined as 10 MHz
	// Raw is actually 8 MHz, so ratio is 1.25 = 5/4
	// 4*correct = 5*raw + 0
	*a = 4;
    *b = 5;
    *c = 0;
}


void CPU_Timer_Sleep_MicroSeconds( UINT32 uSec )
{

}

void CPU_Timer_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{

}

