#include <tinyhal.h>
#include "mmu.h"
#include "..\Krait.h"

extern "C"
{
extern void arm_mmu_init(void);
extern void arch_disable_mmu(void);
extern void arm_invalidate_tlb(void);
}

bool MMUIsEnabled = FALSE;

// The tlb is a static array used by the mmu and does not conform to the MF standard
void CPU_EnableMMU(void *TLB)
{
	MMUIsEnabled = TRUE;
	arm_mmu_init();
}

void CPU_DisableMMU()
{
	MMUIsEnabled = FALSE;
	arch_disable_mmu();
}

void CPU_InvalidateTLBs(           )
{
	arm_invalidate_tlb();
}


BOOL CPU_IsMMUEnabled  (           )
{
	return MMUIsEnabled;
}





