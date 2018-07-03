#include <Samraksh/cm_mpu.h>

#include <cmsis/m2sxxx.h>
#include <core_cm3.h>

MpuRegion_t g_memRegions[8];

void CPU_mpu_enable(void)
{
    MPU->CTRL |= MPU_CTRL_ENABLE_Msk;
}

void CPU_mpu_disable(void)
{
    MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
}

void CPU_mpu_configure_region(UINT8 region, void *addr, UINT8 len,
                          mem_access_permission_t ap, bool executable)
{
    int32_t rasr = 0;

    chDbgAssert(len >= 5, "region too small");

    /* Extract region and address information. */
    region = region & MPU_RBAR_REGION_Msk;
    addr = (void *)((uint32_t)addr & MPU_RBAR_ADDR_Msk);

    /* If the region is not executable add the eXecute Never flag. */
    if (!executable) {
        rasr += MPU_RASR_XN_Msk;
    }

    /* Construct the Region Attributes and Size Register value. */
    rasr += (ap << MPU_RASR_AP_Pos);
    rasr += ((len - 1) << MPU_RASR_SIZE_Pos);
    rasr += MPU_RASR_ENABLE_Msk;

    chSysLock();

    /* Update the MPU settings. */
    MPU->RBAR = (uintptr_t)addr + region + MPU_RBAR_VALID_Msk;
    MPU->RASR = rasr;

    chSysUnlock();

    /* Make sure the memory barriers are correct. */
    __ISB();
    __DSB();
}

void CPU_mpu_init(void)
{
    /* Enable default memory permissions for priviledged code. */
    MPU->CTRL |= MPU_CTRL_PRIVDEFENA_Msk;

    /* Enable MemManage faults. */
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    mpu_enable();
}

MpuRegion_t* CPU_mpu_FindRegion(UINT32 fault_addr) {
	int count=7;
	MpuRegion * region;

	region =memRegions;
	for (; count > 0; count--) {
		if (vmpu_value_in_range(region->start, region->end, address)) {
			return region;
		}
	}

	return NULL;
}
