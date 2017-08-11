#include <tinyhal.h>
#include <stm32f10x.h>

#if defined(SAMRAKSH_RTOS_EXT)
#warning "GLOBAL_LOCK may interfere with real time. Need a new version of SmartPtr that utilizes the BASEPRI register to disable all interrupts except realtime."
#endif

//--//

const uint32_t DISABLED_MASK = 0x1;

SmartPtr_IRQ::SmartPtr_IRQ(void* context)
{
    m_context = context;
    Disable();
}

SmartPtr_IRQ::~SmartPtr_IRQ()
{
    Restore();
}

BOOL SmartPtr_IRQ::WasDisabled()
{
    return (m_state & DISABLED_MASK) == DISABLED_MASK;
}

void SmartPtr_IRQ::Acquire()
{
    UINT32 Cp = m_state;

    if ((Cp & DISABLED_MASK) == DISABLED_MASK)
    {
        Disable();
    }
}

void SmartPtr_IRQ::Release()
{
    UINT32 Cp = m_state;

    if ((Cp & DISABLED_MASK) == 0)
    {
        m_state = __get_PRIMASK();
        __enable_irq();
    }
}

void SmartPtr_IRQ::Probe()
{
    UINT32 Cp = m_state;

    if ((Cp & DISABLED_MASK) == 0)
    {
        register UINT32 state = __get_PRIMASK();

        __enable_irq();

        // just to allow an interupt to an occur
        __NOP();

        // restore irq state
        __set_PRIMASK( state );
    }
}

BOOL SmartPtr_IRQ::GetState(void* context)
{
	register UINT32 Cp = __get_PRIMASK();
    return (0 == (Cp & DISABLED_MASK));
}

BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
    __disable_irq();
    return true;
}

BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
    __enable_irq();
    return true;
}

void SmartPtr_IRQ::Disable()
{
    m_state = __get_PRIMASK();

    __disable_irq();
}

void SmartPtr_IRQ::Restore()
{
    UINT32 Cp = m_state;

    if ((Cp & DISABLED_MASK) == 0)
    {
        __enable_irq();
    }
}

