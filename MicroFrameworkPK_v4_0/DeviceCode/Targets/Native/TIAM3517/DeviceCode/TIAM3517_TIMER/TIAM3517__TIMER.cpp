/*
 *  Name : TIAM3517__TIMER.cpp
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
#include "..\TIAM3517.h"

TIAM3517_TIMER_Driver g_TIAM3517_TIMER_Driver;

BOOL TIAM3517_TIMER_Driver::Initialize  ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 externalSync, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    ASSERT(Timer < c_Max_Timers);

    GLOBAL_LOCK(irq);


    // call to initialize c_base;
    //TIAM3517_TIMER::Initme();

    if(g_TIAM3517_TIMER_Driver.m_descriptors[Timer].configured == TRUE) return FALSE;

    // Start up clk to the timer
    //
    //
    //TIAM3517_CLOCK_MANAGER_CM &CLOCKC = TIAM3517::CMGRC();
	TIAM3517_CLOCK_MANAGER_PER_CM &PERCM = TIAM3517::CMGRPCM();
    TIAM3517_CLOCK_MANAGER_WKUP_CM &WKUPCM = TIAM3517::CMGRWCM();



    //DisableCompareInterrupt(Timer);


    g_TIAM3517_TIMER_Driver.m_descriptors[Timer].isr = ISR;
    g_TIAM3517_TIMER_Driver.m_descriptors[Timer].arg = ISR_Param;


    UINT32 interrupt = 0;

    switch(Timer)
    {
	    case 0:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT1;
			WKUPCM.CM_CLKSEL_WKUP |= 1;
			WKUPCM.CM_FCLKEN_WKUP |= 1;
		    break;
	    case 1:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT2;
			PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_ICLKEN_PER |= 1 << 3;
			PERCM.CM_FCLKEN_PER |= 1 << 3;
		    break;
	    case 2:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT3;
		    PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_FCLKEN_PER |= 1 << 4;
			break;
	    case 3:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT4;
		    PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_FCLKEN_PER |= 1 << 5;
			break;
	    case 4:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT5;
		    PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_FCLKEN_PER |= 1 << 6;
			break;
	    case 5:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT6;
		    PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_FCLKEN_PER |= 1 << 7;
			break;
	    case 6:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT7;
		    PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_FCLKEN_PER |= 1 << 8;
			break;
	    case 7:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT8;
		    PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_FCLKEN_PER |= 1 << 9;
			break;
	    case 8:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT9;
			PERCM.CM_CLKSEL_PER |= 1 << (Timer - 1);
			PERCM.CM_FCLKEN_PER |= 1 << 10;
		    break;
	    case 9:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT10;
		    break;
	    case 10:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT11;
		    break;


    }
    if(ISR)
    {

           if( !CPU_INTC_ActivateInterrupt(interrupt, ISR, ISR_Param) ) return FALSE;
    }
    else
    {
	   if( !CPU_INTC_ActivateInterrupt(interrupt, ISR_Default, NULL) ) return FALSE;
    }

	TIAM3517_TIMER &TIMER = getTimer(Timer);

	if(FreeRunning)
	{
		// Auto reload mode overflow
		TIMER.TCLR |= 2;
	}
	else
	{
		// One shot timer
		TIMER.TCLR &= ~2;
	}

	// Enable Compare Mode
	TIMER.TCLR |= (1 << 6);

	// Start Timer
	TIMER.TCLR |= 1;

	g_TIAM3517_TIMER_Driver.m_descriptors[Timer].configured = TRUE;

	return TRUE;
}


BOOL TIAM3517_TIMER_Driver::Uninitialize(UINT32 Timer)
{
	ASSERT(Timer < c_Max_Timers);

	GLOBAL_LOCK(irq);

	UINT32 interrupt = 0;

	DisableCompareInterrupt(Timer);

	if(g_TIAM3517_TIMER_Driver.m_descriptors[Timer].configured == FALSE) return FALSE;

	g_TIAM3517_TIMER_Driver.m_descriptors[Timer].configured = FALSE;

    	switch(Timer)
	{
		case 0:
			 interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT1;
		   	 break;
	   	 case 1:
			 interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT2;
			 break;
	   	 case 2:
			 interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT3;
		   	 break;
	    case 3:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT4;
		    break;
	    case 4:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT5;
		    break;
	    case 5:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT6;
		    break;
	    case 6:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT7;
		    break;
	    case 7:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT8;
		    break;
	    case 8:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT9;
		    break;
	    case 9:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT10;
		    break;
	    case 10:
		    interrupt = TIAM3517_AITC::c_IRQ_INDEX_GPT11;
		    break;

	}

	if(!CPU_INTC_DeactivateInterrupt( interrupt )) return FALSE;


	// Figure out how to shut down clock
	//
	//
	return TRUE;



}



void TIAM3517_TIMER_Driver::ISR_Default(void* PARAM)
{

	while(1);


}


UINT32 TIAM3517_TIMER_Driver::GetCounter(UINT32 Timer)
{
	ASSERT(Timer < c_Max_Timers);

	TIAM3517_TIMER &TIMER = TIAM3517_TIMER_Driver::getTimer(Timer);

	return TIMER.TCRR;

}
