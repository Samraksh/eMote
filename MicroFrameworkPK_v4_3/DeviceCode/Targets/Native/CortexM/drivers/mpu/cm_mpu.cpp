#include <tinyhal_types.h>
#include <Samraksh/cm_mpu.h>
#include <cmsis/m2sxxx.h>
#include <cmsis/mss_assert.h>
#include <core_cm3.h>

MpuRegion_t g_mpuRegions[8];

void CPU_mpu_enable(void)
{
    MPU->CTRL |= MPU_CTRL_ENABLE_Msk;
}

void CPU_mpu_disable(void)
{
    MPU->CTRL &= ~MPU_CTRL_ENABLE_Msk;
}

void CPU_mpu_configure_region(UINT8 region, void *addr, UINT8 len,
                          MpuMemPermission_t ap, bool executable)
{
    int32_t rasr = 0;

    ASSERT(len >= 5); // "region too small"

    // Extract region and address information.
    region = region & MPU_RBAR_REGION_Msk;
    addr = (void *)((uint32_t)addr & MPU_RBAR_ADDR_Msk);

    // If the region is not executable add the eXecute Never flag.
    if (!executable) {
        rasr += MPU_RASR_XN_Msk;
    }

    // Construct the Region Attributes and Size Register value.
    rasr += (ap << MPU_RASR_AP_Pos);
    rasr += ((len - 1) << MPU_RASR_SIZE_Pos);
    rasr += MPU_RASR_ENA_Msk;

    GLOBAL_LOCK();

    // Update the MPU settings
    MPU->RBAR = (uintptr_t)addr + region + MPU_RBAR_VALID_Msk;
    MPU->RASR = rasr;

    //chSysUnlock();

    // Make sure the memory barriers are correct.
    __ISB();
    __DSB();

    //store region information
    g_mpuRegions[region].start=(UINT32)addr;
    g_mpuRegions[region].end=(UINT32)(addr+size);
    g_mpuRegions[region].regionNo= region;
    g_mpuRegions[region].acl = ap;
}

void CPU_mpu_init(void)
{
    /* Enable default memory permissions for priviledged code. */
    MPU->CTRL |= MPU_CTRL_PRIVDEFENA_Msk;

    /* Enable MemManage faults. */
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

    CPU_mpu_enable();
}

//Note: Finds the first region to which address belongs; Overlapping regions could be a problem.
//Because faults are likely to happen to lesser previleged region,
//search needs to happen in the order where we start from least previledged region.
MpuRegion_t* CPU_mpu_findRegion(UINT32 addr) {
	MpuRegion_t *region;
	region =g_mpuRegions;
	for (int count=7; count >= 0; count--) {
		region =&g_mpuRegions[count];
		if (region->start <= addr && addr < region->end) {
			return region;
		}
	}
	return NULL;
}


uint32_t CPU_mpu_region_translate_acl(MpuRegion * const region, uint32_t start, uint32_t size,
		MpuMemPermission_t acl, uint32_t acl_hw_spec)
{
    uint32_t config, bits, mask, size_rounded, subregions;

    /* verify region alignment */
    bits = vmpu_region_bits(size);
    size_rounded = 1UL << bits;

    if(size_rounded != size) {
        if ((acl & (UVISOR_TACL_SIZE_ROUND_UP | UVISOR_TACL_SIZE_ROUND_DOWN)) == 0) {
            HALT_ERROR(SANITY_CHECK_FAILED, "box size (%i) not rounded, rounding disabled (rounded=%i)\n", size, size_rounded);
        }

        if (acl & UVISOR_TACL_SIZE_ROUND_DOWN) {
            bits--;
            if(bits < ARMv7M_MPU_ALIGNMENT_BITS) {
                HALT_ERROR(SANITY_CHECK_FAILED, "region size (%i) can't be rounded down\n", size);
            }
            size_rounded = 1UL << bits;
        }
    }

    /* check for correctly aligned base address */
    mask = size_rounded - 1;

    if(start & mask) {
        HALT_ERROR(SANITY_CHECK_FAILED, "start address 0x%08X and size (%i) are inconsistent\n", start, size);
    }

    /* map generic ACL's to internal ACL's */
    config = vmpu_map_acl(acl);

    /* calculate subregions from hw-specific ACL */
    subregions = (acl_hw_spec << MPU_RASR_SRD_Pos) & MPU_RASR_SRD_Msk;

    /* enable region & add size */
    region->config = config | MPU_RASR_ENABLE_Msk | ((uint32_t) (bits - 1) << MPU_RASR_SIZE_Pos) | subregions;
    region->start = start;
    region->end = start + size_rounded;
    region->acl = acl;

    return size_rounded;
}

uint8_t CPU_mpu_region_bits(uint32_t size)
{
    assert(0 != size);

    uint8_t bits = vmpu_bits(size) - 1;

    // round up if needed
    if((1UL << bits) != size) {
        bits++;
    }

    // minimum region size is 32 bytes
    if(bits < ARMv7M_MPU_ALIGNMENT_BITS) {
        bits = ARMv7M_MPU_ALIGNMENT_BITS;
    }

    assert(bits == MPU_REGION_BITS(size));
    return bits;
}
