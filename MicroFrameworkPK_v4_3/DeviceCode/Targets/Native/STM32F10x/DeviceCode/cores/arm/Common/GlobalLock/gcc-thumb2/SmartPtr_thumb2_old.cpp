#include <tinyhal.h>

//--//
#if defined(PLATFORM_ARM_STM32F10x)
#include <..\Targets\Native\STM32F10x\DeviceCode\processor\stm32f10x.h>
#endif

#define DISABLED_MASK 0x1

UINT32 GetPRIMASK(void)
{
  register uint32_t r2         __ASM("primask");
  return(r2);
}

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
        m_state = GetPRIMASK();
    	// m_state  __ASM("primask");

        __enable_irq();
    }
}

void SmartPtr_IRQ::Probe()
{
    UINT32 Cp = m_state;

    if ((Cp & DISABLED_MASK) == 0)
    {
        UINT32 s = GetPRIMASK();

        __enable_irq();

        // just to allow an interupt to an occur
        __NOP();

        // restore irq state
        __set_PRIMASK(s);
    }
}

BOOL SmartPtr_IRQ::GetState(void* context)
{
	//return TRUE;
    return (0 == (GetPRIMASK() & DISABLED_MASK));
}

BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
    __disable_irq();
    return TRUE;
}

BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
    __enable_irq();
    return true;
}

void SmartPtr_IRQ::Disable()
{
    m_state = GetPRIMASK();

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

