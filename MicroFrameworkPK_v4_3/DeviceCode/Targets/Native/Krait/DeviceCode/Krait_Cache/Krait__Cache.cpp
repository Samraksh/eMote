#include <tinyhal.h>
#include "Krait__Cache.h"


void CPU_EnableCaches()
{
	arch_enable_cache(UCACHE);
}


void CPU_DisableCaches()
{
	arch_disable_cache(UCACHE);
}
