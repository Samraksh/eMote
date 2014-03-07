/*  Name : Krait_GPIO_functions.cpp
 *
 *  Author : Mukundan.Sridharan@Samraksh.com
 *
 *  Description : Contains functions that are needed by the .NET Microframework for GPIO functionality
 */

#include <cores\arm\include\cpu.h>
#include "..\Krait.h"
#include "Krait__GPIO.h"


//--//

BOOL CPU_GPIO_Initialize()
{
    return Krait_GPIO_Driver::Initialize();
}

BOOL CPU_GPIO_Uninitialize()
{
    //return Krait_GPIO_Driver::Uninitialize();
    return TRUE;
}

UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
    return Krait_GPIO_Driver::Attributes( Pin );
    //return 1;
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
    Krait_GPIO_Driver::DisablePin( Pin, ResistorState, Direction, AltFunction );
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
    Krait_GPIO_Driver::EnableOutputPin( Pin, InitialState );
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return Krait_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, 0, IntEdge, ResistorState );
    //return TRUE;
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return Krait_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, ISR_Param, IntEdge, ResistorState );
    //return TRUE;
}

BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
    return Krait_GPIO_Driver::GetPinState( Pin );
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
    Krait_GPIO_Driver::SetPinState( Pin, PinState );
}

INT32 CPU_GPIO_GetPinCount()
{
    //return Krait_GPIO_Driver::c_MaxPins;
    return 0;
}

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
    //Krait_GPIO_Driver::GetPinsMap( pins, size );
}

UINT8 CPU_GPIO_GetSupportedResistorModes( GPIO_PIN pin )
{
    //return Krait_GPIO_Driver::GetSupportedResistorModes( pin );
    return 0;
}

UINT8 CPU_GPIO_GetSupportedInterruptModes( GPIO_PIN pin )
{
    //return Krait_GPIO_Driver::GetSupportedInterruptModes( pin );
    return 0;
}


//--//

BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
    //return Krait_GPIO_Driver::PinIsBusy( Pin );
    return FALSE;
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
    return Krait_GPIO_Driver::ReservePin( Pin, fReserve );
    //return FALSE;
}

UINT32 CPU_GPIO_GetDebounce()
{
    //return Krait_GPIO_Driver::GetDebounce();
    return 0;
}

BOOL CPU_GPIO_SetDebounce( INT64 debounceTimeMilliseconds )
{
    //return Krait_GPIO_Driver::SetDebounce( debounceTimeMilliseconds );
    return 0;
}

