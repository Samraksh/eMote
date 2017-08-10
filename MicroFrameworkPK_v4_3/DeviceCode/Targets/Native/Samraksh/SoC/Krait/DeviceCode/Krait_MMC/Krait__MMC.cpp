#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait__MMC.h"

BOOL CPU_MMC_Init()
{
	UINT32 base_addr;
	UINT32 slot = 1;

	base_addr = mmc_sdc_base[slot- 1];

	if(mmc_boot_main(slot, base_addr))
	{
		slot = 3;
		base_addr = mmc_sdc_base[slot - 1];
		if (mmc_boot_main(slot, base_addr)) {
					hal_printf("mmc init failed!");
					ASSERT(0);
		}
	}

}
