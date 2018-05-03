#include <tinyhal_types.h>
#include <SmartPtr_irq.h>
#include <hal.h>

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
        m_state = (UINT32)HAL_get_interrupts();
        HAL_restore_interrupts((psr_t)m_state);
    }
}

//Not sure what the whole purpose is
void SmartPtr_IRQ::Probe()
{
   /*
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
    */
}

BOOL SmartPtr_IRQ::GetState(void* context)
{
	register UINT32 Cp = (UINT32)HAL_get_interrupts();
    return (0 == (Cp & DISABLED_MASK));
}

BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
	HAL_disable_interrupts();
    return true;
}

BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
	HAL_enable_interrupts();
    return true;
}

void SmartPtr_IRQ::Disable()
{
    m_state = (UINT32) HAL_disable_interrupts();
}

void SmartPtr_IRQ::Restore()
{
    HAL_restore_interrupts((psr_t)m_state);
}

