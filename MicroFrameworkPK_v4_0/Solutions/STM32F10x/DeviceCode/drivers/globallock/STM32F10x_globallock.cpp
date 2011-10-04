////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#define INTC_MASK_LOW	0xFFFFFFF7
#define INTC_MASK_HIGH	0x0FFFFFFF

UINT8  SmartPtr_IRQ_IsLocked = 0;
UINT32 SmartPtr_IRQ_HoldLow  = 0;
UINT32 SmartPtr_IRQ_HoldHigh = 0;
UINT8  SmartPtr_IRQ_WasDisabled = 0;

SmartPtr_IRQ::SmartPtr_IRQ(void* context)
{
	//Acquire();
}

SmartPtr_IRQ::~SmartPtr_IRQ() 
{
	//Release();
}

BOOL SmartPtr_IRQ::WasDisabled()
{
    //return (BOOL)(SmartPtr_IRQ_WasDisabled);
	return TRUE;
}

// in order to achieve greater speed we should find another way to store currently active interrupts, perhaps in the AIC?
void SmartPtr_IRQ::Acquire()
{
	/*if(!SmartPtr_IRQ_IsLocked) {
		SmartPtr_IRQ_IsLocked = 1;
		SmartPtr_IRQ_HoldLow = NVIC->ISER[0] & INTC_MASK_LOW;
		NVIC->ICER[0] = SmartPtr_IRQ_HoldLow;
		SmartPtr_IRQ_HoldHigh = NVIC->ISER[1] & INTC_MASK_HIGH;
		NVIC->ICER[1] = SmartPtr_IRQ_HoldHigh;
	} else {
		SmartPtr_IRQ_WasDisabled = 1;
	}*/
}

void SmartPtr_IRQ::Release()
{
 	//NVIC->ISER[0] = SmartPtr_IRQ_HoldLow;
	//NVIC->ISER[1] = SmartPtr_IRQ_HoldHigh;
	//SmartPtr_IRQ_IsLocked = 0;
	//SmartPtr_IRQ_WasDisabled = 0;
}

void SmartPtr_IRQ::Probe()
{
	// possibly implement smarter control by reading pending bits
 	//NVIC->ISER[0] = SmartPtr_IRQ_HoldLow;
	//NVIC->ICER[0] = SmartPtr_IRQ_HoldLow;
	//NVIC->ISER[1] = SmartPtr_IRQ_HoldHigh;
	//NVIC->ICER[1] = SmartPtr_IRQ_HoldHigh;
}

BOOL SmartPtr_IRQ::GetState(void* context)
{
    //return (BOOL)(SmartPtr_IRQ_IsLocked);
	return TRUE;
}

BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
	//NVIC->ICER[0] = INTC_MASK_LOW;
	//NVIC->ICER[1] = INTC_MASK_HIGH;
	return TRUE;
}

BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
	// not effective restoration method
 	//NVIC->ISER[0] = SmartPtr_IRQ_HoldLow;
	//NVIC->ISER[1] = SmartPtr_IRQ_HoldHigh;
    return TRUE; 
}

void SmartPtr_IRQ::Disable()
{
	//Acquire();
}

void SmartPtr_IRQ::Restore()
{
	//Release();
}
