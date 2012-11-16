#ifndef _KRAIT__CACHE_H_
#define _KRAIT__CACHE_H_



extern "C" {


void arch_enable_ints(void);
void arch_disable_ints(void);

int atomic_swap(volatile int *ptr, int val);
int atomic_add(volatile int *ptr, int val);
int atomic_and(volatile int *ptr, int val);
int atomic_or(volatile int *ptr, int val);

#endif // !ASSEMBLY
#define ICACHE 1
#define DCACHE 2
#define UCACHE (ICACHE|DCACHE)
#ifndef ASSEMBLY

void arch_disable_cache(UINT32 flags);
void arch_enable_cache(UINT32 flags);

void arch_clean_cache_range(addr_t start, size_t len);
void arch_clean_invalidate_cache_range(addr_t start, size_t len);
void arch_invalidate_cache_range(addr_t start, size_t len);
void arch_sync_cache_range(addr_t start, size_t len);

void arch_idle(void);

void arch_disable_mmu(void);

void arch_switch_stacks_and_call(addr_t call, addr_t stack);

UINT32 arch_cycle_count(void);

#endif


