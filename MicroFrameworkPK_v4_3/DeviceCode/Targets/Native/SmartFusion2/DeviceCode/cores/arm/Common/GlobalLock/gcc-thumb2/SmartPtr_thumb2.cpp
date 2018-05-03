#include <tinyhal_types.h>
#include <SmartPtr_irq.h>

//#include <tinyhal.h>

//--//

#define DISABLED_MASK 0x1
#define __ASM __asm
#define __INLINE inline


#if (defined (__GNUC__)) /*------------------ GNU Compiler ---------------------*/
/* GNU gcc specific functions */

static __INLINE void __enable_irq()               { __ASM volatile ("cpsie i"); }
static __INLINE void __disable_irq()              { __ASM volatile ("cpsid i"); }

static __INLINE void __enable_fault_irq()         { __ASM volatile ("cpsie f"); }
static __INLINE void __disable_fault_irq()        { __ASM volatile ("cpsid f"); }

static __INLINE void __NOP()                      { __ASM volatile ("nop"); }
static __INLINE void __WFI()                      { __ASM volatile ("wfi"); }
static __INLINE void __WFE()                      { __ASM volatile ("wfe"); }
static __INLINE void __SEV()                      { __ASM volatile ("sev"); }
static __INLINE void __ISB()                      { __ASM volatile ("isb"); }
static __INLINE void __DSB()                      { __ASM volatile ("dsb"); }
static __INLINE void __DMB()                      { __ASM volatile ("dmb"); }
static __INLINE void __CLREX()                    { __ASM volatile ("clrex"); }

UINT32 __get_PRIMASK(void)
{
  UINT32 result=0;

  __ASM volatile ("MRS %0, primask" : "=r" (result) );
  return(result);
}

#endif



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
        register UINT32 Cs = __get_PRIMASK();

        m_state = Cs;

        __enable_irq();
    }
}

void SmartPtr_IRQ::Probe()
{
    UINT32 Cp = m_state;

    if ((Cp & DISABLED_MASK) == 0)
    {
        register UINT32 Cs =__get_PRIMASK();

        UINT32 s = Cs;

        __enable_irq();

        // just to allow an interupt to an occur
        __NOP();

        // restore irq state
        Cs = s;
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
    register UINT32 Cp = __get_PRIMASK();

    m_state = Cp;

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

