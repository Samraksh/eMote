#include "netmf_time.h"

//#define DEBUG_PRINT

//AnanthAtSamraksh
#include "../Include/Samraksh/VirtualTimer.h"

extern Timer16Bit_Driver g_Timer16Bit_Driver;
extern STM32F10x_AdvancedTimer g_STM32F10x_AdvancedTimer;
static void ISR( void* Param );

extern const UINT8 ADVTIMER_32BIT;
extern const UINT8 TIMER1_16BIT;
extern const UINT8 TIMER2_16BIT;
//AnanthAtSamraksh

#if 0
void CPU_CPWAIT()
{
	//TODO Implement it in boot area
#ifdef DEBUG_PRINT
	debug_printf("In CPU_CPWAI\n");
#endif
}

UINT32 CPU_SystemClock()
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_SystemClock");
#endif
	return SYSTEM_CLOCK_HZ;
}

UINT32 CPU_TicksPerSecond()
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksPerSecond");
#endif
	return SLOW_CLOCKS_PER_SECOND;
}

UINT64 CPU_TicksToTime( UINT64 Ticks )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT64), Ticks %lld\n", Ticks);
#endif
    Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
    Ticks /= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT64), Ticks %lld\n", Ticks);
#endif
	return Ticks;
}

UINT64 CPU_TicksToTime( UINT32 Ticks32 )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT32), Ticks32 %u\n", Ticks32);
#endif
	UINT64 Ticks;
	Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
	Ticks /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_TicksToTime(UINT32), Ticks %lld\n", Ticks);
#endif
	return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT64 Ticks )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT64), Ticks %lld\n", Ticks);
#endif
	Ticks *= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
	// Nived.Sivadas - rewriting to avoid possibility of floating point arithmetic in intemediate stages
	//Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
	Ticks = (Ticks * SLOW_CLOCKS_MILLISECOND_GCD)/ 1000;
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT64), Ticks %lld\n", Ticks);
#endif
	return Ticks;
}

UINT64 CPU_MillisecondsToTicks( UINT32 Ticks32 )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT32), Ticks %u\n", Ticks32);
#endif
    UINT64 Ticks;
    Ticks  = (UINT64)Ticks32 * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
    // Nived.Sivadas - rewriting to avoid possibility of floating point arithmetic in intemediate stages
    //Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
    Ticks = (Ticks * SLOW_CLOCKS_MILLISECOND_GCD)/ 1000;
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MillisecondsToTicks(UINT32), Ticks %lld\n", Ticks);
#endif
    return Ticks;
}

//TODO: AnanthAtSamraksh - what to do with below 2 functions
UINT64 CPU_MicrosecondsToTicks( UINT64 uSec )
{
	//return uSec;
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#ifdef DEBUG_PRINT
	UINT64 value;
	debug_printf("CPU_MicrosecondsToTicks, Ticks %lld\n", uSec);
	value = uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
	debug_printf("Return Value is, %lld\n", value);
#endif
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicrosecondsToTicks(UINT64/#else), Ticks %u\n", uSec);
#endif
   // return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
	return uSec * 48;
#endif
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec )
{
	//return uSec;
	//return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#ifdef DEBUG_PRINT
	UINT32 value32;
	debug_printf("In CPU_MicrosecondsToTicks(UINT32), Ticks %u\n", uSec);
	value32 = uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
	debug_printf("Return Value is, %u\n", value32);
#endif
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicrosecondsToTicks(UINT32/#else), Ticks %u\n", uSec);
#endif
    //return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
	return uSec * 48;
#endif
}


UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks(UINT32), Ticks before %u\n", uSec);
#endif
	uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
	uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks(UINT32), Ticks after %u\n", uSec);
#endif
	return uSec;
}

int CPU_MicosecondsToSystemClocks( int uSec )
{
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks, Ticks before %d\n", uSec);
#endif
    uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
    uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicosecondsToSystemClocks, Ticks after %d\n", uSec);
#endif
	return uSec;
}

int CPU_SystemClocksToMicroseconds( int Ticks )
{
	//return Ticks;
#ifdef DEBUG_PRINT
	debug_printf("In CPU_SystemClocksToMicroseconds, Ticks before %d\n", Ticks);
#endif
    Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
    Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
#ifdef DEBUG_PRINT
	debug_printf("In CPU_SystemClocksToMicroseconds, Ticks after %d\n", Ticks);
#endif
   return Ticks;
}
#endif

//#pragma arm section code = "SectionForFlashOperations"

//UINT32 __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( UINT32 uSec )
//{
    //uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
//    //uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
//
//    return uSec;
//}
//
//int __section(SectionForFlashOperations) CPU_MicrosecondsToSystemClocks( int uSec )
//{
//    //uSec *= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
//    //uSec /= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
//
//    return uSec;
//}
//
//
////--//
//
//int __section(SectionForFlashOperations) CPU_SystemClocksToMicroseconds( int Ticks )
//{
//    //Ticks *= (ONE_MHZ        /CLOCK_COMMON_FACTOR);
//    //Ticks /= (SYSTEM_CLOCK_HZ/CLOCK_COMMON_FACTOR);
//
//    return Ticks;
//}
//
//#pragma arm section code

#if 0
BOOL HAL_Time_Initialize()
{
	//return TRUE;
	return Time_Driver::Initialize();
}

BOOL HAL_Time_Uninitialize()
{
	return Time_Driver::Uninitialize();
}

UINT64 HAL_Time_CurrentTicks()
{
	return Time_Driver::CounterValue();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	return Time_Driver::TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime()
{
	return Time_Driver::CurrentTime();
}

void HAL_Time_SetCompare( UINT64 CompareValue )
{
	Time_Driver::SetCompareValue( CompareValue );
}

void HAL_Time_SetCompare_Completion(UINT64 val)
{
	return;
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

UINT64 Time_CurrentTicks(UINT16 Timer)
{
    return HAL_Time_CurrentTicks(Timer);
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    Time_Driver::Sleep_uSec( uSec );
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	Time_Driver::Sleep_uSec_Loop( uSec );
}
#endif




UINT32 CPU_SystemClock(UINT16 Timer)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		return g_HardwareTimerFrequency[1];
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return g_HardwareTimerFrequency[0];
	}
	//return SystemTimerClock;
}

UINT32 CPU_TicksPerSecond(UINT16 Timer)
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		return SLOW_CLOCKS_PER_SECOND;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return g_HardwareTimerFrequency[0];
	}

	//return SystemTimerClock;
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT64 Ticks, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		Ticks *= (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
		Ticks /= (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return Ticks;
	}
}

// OK, so the DriftParameters *should* correct this in the PAL layer
// But that begs the question of what this function is really for...
UINT64 CPU_TicksToTime( UINT32 Ticks32, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks  = (UINT64)Ticks32 * (TEN_MHZ               /SLOW_CLOCKS_TEN_MHZ_GCD);
		Ticks /=                   (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_TEN_MHZ_GCD);
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return Ticks32;
	}
}

UINT64 CPU_MillisecondsToTicks( UINT64 mSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks = mSec * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
		// Nived.Sivadas - rewriting to avoid possibility of floating point arithmetic in intemediate stages
		//Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
		Ticks = (Ticks * SLOW_CLOCKS_MILLISECOND_GCD)/ 1000;
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( mSec * (g_HardwareTimerFrequency[0] / 1000) );
	}
	//return mSec * (SystemTimerClock/1000);
}


UINT64 CPU_MillisecondsToTicks( UINT32 mSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT64 Ticks;
		Ticks  = (UINT64)mSec * (SLOW_CLOCKS_PER_SECOND/SLOW_CLOCKS_MILLISECOND_GCD);
		// Nived.Sivadas - rewriting to avoid possibility of floating point arithmetic in intemediate stages
		//Ticks /= (1000                  /SLOW_CLOCKS_MILLISECOND_GCD);
		Ticks = (Ticks * SLOW_CLOCKS_MILLISECOND_GCD)/ 1000;
		return Ticks;
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( mSec * (g_HardwareTimerFrequency[0] / 1000) );
	}
	//return mSec * (SystemTimerClock/1000);
}


UINT64 CPU_MicrosecondsToTicks( UINT64 uSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#ifdef DEBUG_PRINT
	UINT64 value;
	debug_printf("CPU_MicrosecondsToTicks, Ticks %lld\n", uSec);
	value = uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
	debug_printf("Return Value is, %lld\n", value);
#endif
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicrosecondsToTicks(UINT64/#else), Ticks %u\n", uSec);
#endif
   // return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
	return uSec * 48;
#endif
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( uSec * (g_HardwareTimerFrequency[0] / 1000000) );
	}
	//return uSec * (SystemTimerClock/1000000);
}


UINT32 CPU_MicrosecondsToTicks( UINT32 uSec, UINT16 Timer )
{
	if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
#if ONE_MHZ < SLOW_CLOCKS_PER_SECOND
	return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#ifdef DEBUG_PRINT
	UINT32 value32;
	debug_printf("In CPU_MicrosecondsToTicks(UINT32), Ticks %u\n", uSec);
	value32 = uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
	debug_printf("Return Value is, %u\n", value32);
#endif
    return uSec * (SLOW_CLOCKS_PER_SECOND / ONE_MHZ);
#else
#ifdef DEBUG_PRINT
	debug_printf("In CPU_MicrosecondsToTicks(UINT32/#else), Ticks %u\n", uSec);
#endif
    //return uSec / (ONE_MHZ / SLOW_CLOCKS_PER_SECOND);
	return uSec * 48;
#endif
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		return ( uSec * (g_HardwareTimerFrequency[0] / 1000000) );
	}
	//return uSec * (SystemTimerClock/1000000);
}





BOOL HAL_Time_Initialize()
{
	//AnanthAtSamraksh - changed below

	//return Time_Driver::Initialize();
	/*if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		g_Time_Driver.m_lastRead = 0;
		g_Time_Driver.m_nextCompare = (UINT64) g_Timer16Bit_Driver.c_MaxTimerValue;

		if(!CPU_Timer_Initialize(Timer, TRUE, 0, 0, ISR, NULL))
			return FALSE;

		//if (!g_Timer16Bit_Driver.Initialize (g_Timer16Bit_Driver.c_SystemTimer, TRUE, 0, 0, ISR, NULL))
		//{
			//return FALSE;
		//}
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		if(!CPU_Timer_Initialize(Timer, TRUE, 0, 0, ISR, NULL))
			return FALSE;

		//if (g_STM32F10x_AdvancedTimer.Initialize (0, ISR, NULL) != DS_Success)
		//{
			//return FALSE;
		//}
	}
	return TRUE;*/

	return VirtTimer_Initialize();
}

BOOL HAL_Time_Uninitialize()
{
	//AnanthAtSamraksh - changed below

	//return Time_Driver::Uninitialize();
	/*if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		if(!g_Timer16Bit_Driver.Uninitialize( g_Timer16Bit_Driver.c_SystemTimer ))
		{
			return FALSE;
		}
	}
	else if(Timer == ADVTIMER_32BIT)
	{

	}*/

	return VirtTimer_UnInitialize();
}


UINT64 Time_CurrentTicks()
{
	//AnanthAtSamraksh - changed below
    //return HAL_Time_CurrentTicks(Timer);
	//return VirtTimer_GetTicks(0);
	return VirtTimer_GetCounter(0);
}


UINT64 HAL_Time_CurrentTicks()
{
	//AnanthAtSamraksh - changed below
	//return VirtTimer_GetTicks(0);
	return VirtTimer_GetCounter(0);

	/*if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		UINT16 value;
		UINT64 m_lastRead = 0;
		GLOBAL_LOCK(irq);
		value = g_Timer16Bit_Driver.GetCounter(g_Timer16Bit_Driver.c_SystemTimer);

		// Nived.Sivadas
		// Workaround for the unusual didtimeoverflow bug, added another check
		UINT16 lastSixteenBits = m_lastRead & 0x0000FFFFull;
		m_lastRead &= (0xFFFFFFFFFFFF0000ull);

		if(g_Timer16Bit_Driver.DidTimeOverFlow( g_Timer16Bit_Driver.c_SystemTimer ) || (value < lastSixteenBits))
		{
			g_Timer16Bit_Driver.ClearTimeOverFlow( g_Timer16Bit_Driver.c_SystemTimer );
			m_lastRead += (0x1ull << 16);
		}

		//Or else the value gets added simply
		m_lastRead += value;

		ENABLE_INTERRUPTS();

		return m_lastRead;
	}
	//TODO: AnanthAtSamraksh -- verify below, especially "value"
	else if(Timer == ADVTIMER_32BIT)
	{
		UINT32 value;
		UINT64 m_lastRead = 0;
		GLOBAL_LOCK(irq);
		value = g_STM32F10x_AdvancedTimer.Get64Counter();

		UINT32 last32Bits = m_lastRead & 0x00000000FFFFFFFFull;
		m_lastRead &= (0xFFFFFFFF00000000ull);

		if(g_STM32F10x_AdvancedTimer.DidTimerOverflow( ) || (value < last32Bits))
		{
			g_STM32F10x_AdvancedTimer.ClearTimerOverflow( );
			m_lastRead += (0x1ull << 32);
		}

		//Or else the value gets added simply
		m_lastRead += value;

		ENABLE_INTERRUPTS();

		return m_lastRead;
	}*/
	//return Time_Driver::CounterValue();
}

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
	//return Time_Driver::TicksToTime( Ticks );
	return CPU_TicksToTime( Ticks );
}

INT64 HAL_Time_CurrentTime()
{
	//return Time_Driver::CurrentTime();
	//return CPU_TicksToTime( HAL_Time_CurrentTicks(Timer) );
	//return CPU_TicksToTime( CPU_Timer_CurrentTicks(0) );
	return CPU_TicksToTime( VirtTimer_GetTicks(0) );
}

void HAL_Time_SetCompare( UINT64 CompareValue )
{
	//AnanthAtSamraksh -- changed below
	//CPU_Timer_SetCompare( 1, CompareValue );
	VirtTimer_SetCompare(0, CompareValue);

	////Time_Driver::SetCompareValue( CompareValue );
	/*if(Timer == TIMER1_16BIT || Timer == TIMER2_16BIT)
	{
		////g_Time_Driver.SetCompareValue( CompareValue );
		GLOBAL_LOCK(irq);
		g_Time_Driver.m_nextCompare = CompareValue;
		bool fForceInterrupt = false;
		UINT64 CntrValue = HAL_Time_CurrentTicks(Timer);

		if(CompareValue <= CntrValue)
		{
			fForceInterrupt = true;
		}
		else
		{
			UINT32 diff;
			//If compare is greater than total counter value, this is the best we can do
			if((CompareValue - CntrValue) > g_Timer16Bit_Driver.c_MaxTimerValue)
			{
				diff = g_Timer16Bit_Driver.c_MaxTimerValue;
			}
			//Else store the difference
			else
			{
				diff = (UINT32)(CompareValue - CntrValue);
			}

			//Store the compare to now + diff
			g_Timer16Bit_Driver.SetCompare ( g_Timer16Bit_Driver.c_SystemTimer, (UINT16)(g_Timer16Bit_Driver.GetCounter( g_Timer16Bit_Driver.c_SystemTimer ) + diff));

			//If meanwhile happens
			if(HAL_Time_CurrentTicks(Timer) > CompareValue)
			{
				fForceInterrupt = true;
			}
		}

		if(fForceInterrupt)
		{
			// Force interrupt to process this.
			g_Timer16Bit_Driver.ForceInterrupt( g_Timer16Bit_Driver.c_SystemTimer );
		}
	}
	else if(Timer == ADVTIMER_32BIT)
	{
		GLOBAL_LOCK(irq);
		g_Time_Driver.m_nextCompare = CompareValue;
		bool fForceInterrupt = false;
		UINT64 CntrValue = HAL_Time_CurrentTicks(Timer);

		if(CompareValue <= CntrValue)
		{
			fForceInterrupt = true;
		}
		else
		{
			//AnanthAtSamraksh -- commented out diff, as time keeping is done in the time driver
			//UINT32 diff;
			//If compare is greater than total counter value, this is the best we can do
			//if((CompareValue - CntrValue) > 0xFFFF)
			//{
				//diff = 0xFFFF;
			//}
			//Else store the difference
			//else
			//{
				//diff = (UINT32)(CompareValue - CntrValue);
			//}

			//Store the compare to now + diff
			//g_STM32F10x_AdvancedTimer.SetCompare(CPU_Timer_CurrentTicks(Timer), diff, SET_COMPARE_TIMER);

			g_STM32F10x_AdvancedTimer.SetCompare(CPU_Timer_CurrentTicks(Timer), CompareValue, SET_COMPARE_TIMER);
			////g_STM32F10x_AdvancedTimer.SetCompare ( g_STM32F10x_AdvancedTimer.c_SystemTimer, (UINT16)(Timer16Bit_Driver::GetCounter( Timer16Bit_Driver :: c_SystemTimer ) + diff));
			//If meanwhile happens
			if(HAL_Time_CurrentTicks(Timer) > CompareValue)
			{
				fForceInterrupt = true;
			}
		}

		if(fForceInterrupt)
		{
			// Force interrupt to process this.
			////Timer16Bit_Driver::ForceInterrupt( Timer16Bit_Driver::c_SystemTimer );
			g_STM32F10x_AdvancedTimer.TriggerSoftwareInterrupt();
		}
	}*/
}

void HAL_Time_SetCompare_Completion(UINT64 val)
{
	return;
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
	//AnanthAtSamraksh -- added below
    //Time_Driver::Sleep_uSec( uSec );
	//CPU_Timer_Sleep_MicroSeconds(uSec, 1);
	VirtTimer_SleepMicroseconds(0, uSec);
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
	//AnanthAtSamraksh -- added below
	//Time_Driver::Sleep_uSec_Loop( uSec );
	//CPU_Timer_Sleep_MicroSeconds(uSec, 1);
	VirtTimer_SleepMicroseconds(0, uSec);
}

