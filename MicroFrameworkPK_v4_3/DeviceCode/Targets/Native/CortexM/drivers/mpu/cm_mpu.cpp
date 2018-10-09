#include <tinyhal_types.h>
#include <SmartPtr_irq.h>
#include <Samraksh/cm_mpu.h>
#include <cmsis/m2sxxx.h>
#include <cmsis/mss_assert.h>
#include <core_cm3.h>


//redefinition here. This is usually done on platform_selector.h, but since this is sort of a chip file,
//need to do this again here
#ifndef GLOBAL_LOCK
#define GLOBAL_LOCK(x)	SmartPtr_IRQ x
#define DISABLE_INTERRUPTS()       SmartPtr_IRQ::ForceDisabled()
#define ENABLE_INTERRUPTS()        SmartPtr_IRQ::ForceEnabled()
#define INTERRUPTS_ENABLED_STATE() SmartPtr_IRQ::GetState()
#define GLOBAL_LOCK_SOCKETS(x)     SmartPtr_IRQ x
#endif

/* various MPU flags */
#define MPU_RASR_AP_PNO_UNO (0x00UL<<MPU_RASR_AP_Pos)
#define MPU_RASR_AP_PRW_UNO (0x01UL<<MPU_RASR_AP_Pos)
#define MPU_RASR_AP_PRW_URO (0x02UL<<MPU_RASR_AP_Pos)
#define MPU_RASR_AP_PRW_URW (0x03UL<<MPU_RASR_AP_Pos)
#define MPU_RASR_AP_PRO_UNO (0x05UL<<MPU_RASR_AP_Pos)
#define MPU_RASR_AP_PRO_URO (0x06UL<<MPU_RASR_AP_Pos)

#define MPU_RASR_XN         (0x01UL<<MPU_RASR_XN_Pos)
#define MPU_RASR_CB_NOCACHE (0x00UL<<MPU_RASR_B_Pos)
#define MPU_RASR_CB_WB_WRA  (0x01UL<<MPU_RASR_B_Pos)
#define MPU_RASR_CB_WT      (0x02UL<<MPU_RASR_B_Pos)
#define MPU_RASR_CB_WB      (0x03UL<<MPU_RASR_B_Pos)
#define MPU_RASR_SRD(x)     (((uint32_t)(x))<<MPU_RASR_SRD_Pos)


MpuRegion_t g_mpuRegions[8];

//Takes mpu permissions and returns the mpu flags to be set
static uint32_t mpu_map_acl(MpuMemPermission_t acl, bool exec)
{
    uint32_t flags;
    uint32_t acl_res;

    /* Map generic ACLs to internal ACLs. */

    switch(acl){
		case AP_NO_NO:
			flags = MPU_RASR_AP_PNO_UNO;
			acl_res =  0;
			break;
		case AP_RO_NO:
			flags = MPU_RASR_AP_PRO_UNO;
			acl_res = MPU_TACL_SREAD;
			break;
		case AP_RO_RO:
			flags = MPU_RASR_AP_PRO_URO;
			acl_res = MPU_TACL_SREAD | MPU_TACL_UREAD;
			break;
		case AP_RW_NO:
			flags = MPU_RASR_AP_PRW_UNO;
			acl_res = MPU_TACL_SREAD | MPU_TACL_SWRITE;
			break;
		case AP_RW_RO:
			flags = MPU_RASR_AP_PRW_URO;
			acl_res = MPU_TACL_SREAD | MPU_TACL_SWRITE | MPU_TACL_UREAD;
			break;
		case AP_RW_RW:
			flags = MPU_RASR_AP_PRW_URW;
			acl_res =  MPU_TACL_UREAD | MPU_TACL_UWRITE |
			           MPU_TACL_SREAD | MPU_TACL_SWRITE;
			break;
		case AP_RESERVED:
		default:
			flags = 0;
			break;
    }


    // Handle code-execute flag.
    if( exec ) {
        /* Can't distinguish between user & supervisor execution. */
        acl_res |= MPU_TACL_UEXECUTE | MPU_TACL_SEXECUTE;
    }
    else {
        flags |= MPU_RASR_XN;
    }

    // Mukundan:We dont have a way to check this
    /*if( acl_res != (acl & MPU_TACL_ACCESS) ) {
        HALT_ERROR(SANITY_CHECK_FAILED, "inferred ACL's (0x%04X) don't match exptected ACL's (0x%04X)\n", acl_res, (acl & MPU_TACL_ACCESS));
    }*/

    return flags;
}


static inline int vmpu_bits(UINT32 size)
{
    /* If size is 0, the result of __builtin_clz is undefined */
    return (0 == size) ? 0: 32 - __builtin_clz(size);
}


uint8_t vmpu_region_bits(UINT32 size)
{
    assert(0 != size);

    uint8_t bits = vmpu_bits(size) - 1;

    /* round up if needed */
    if((1UL << bits) != size) {
        bits++;
    }

    /* minimum region size is 32 bytes */
    if(bits < ARMv7M_MPU_ALIGNMENT_BITS) {
        bits = ARMv7M_MPU_ALIGNMENT_BITS;
    }

    assert(bits == MPU_REGION_BITS(size));
    return bits;
}


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
    addr = (void *)((UINT32)addr & MPU_RBAR_ADDR_Msk);

    // If the region is not executable add the eXecute Never flag.
    if (!executable) {
        rasr += MPU_RASR_XN_Msk;
    }

    // Construct the Region Attributes and Size Register value.
    UINT32 size = ((len - 1) << MPU_RASR_SIZE_Pos);
    rasr += (ap << MPU_RASR_AP_Pos);
    rasr += size;
    rasr += MPU_RASR_ENA_Msk;

    GLOBAL_LOCK(irq);

    // Update the MPU settings
    MPU->RBAR = (uintptr_t)addr + region + MPU_RBAR_VALID_Msk;
    MPU->RASR = rasr;

    //chSysUnlock();

    // Make sure the memory barriers are correct.
    __ISB();
    __DSB();


    //store region information
    g_mpuRegions[region].start=addr;
    g_mpuRegions[region].end=(void*)((UINT32)addr+size);
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
MpuRegion_t* CPU_mpu_findRegion(void* addr) {
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


UINT32 CPU_mpu_region_translate_acl(MpuRegion_t * const region, void* start, UINT32 size,
		MpuMemPermission_t acl, bool acl_exec, UINT32 acl_hw_spec)
{
    UINT32 config, bits, mask, size_rounded, subregions;

    /* verify region alignment */
    bits = vmpu_region_bits(size);
    size_rounded = 1UL << bits;

    if(size_rounded != size) {
        if ((acl & (MPU_TACL_SIZE_ROUND_UP | MPU_TACL_SIZE_ROUND_DOWN)) == 0) {
            debug_printf("SANITY_CHECK_FAILED: box size (%i) not rounded, rounding disabled (rounded=%i)\n", size, size_rounded);
        }

        if (acl & MPU_TACL_SIZE_ROUND_DOWN) {
            bits--;
            if(bits < ARMv7M_MPU_ALIGNMENT_BITS) {
                debug_printf("SANITY_CHECK_FAILED: region size (%i) can't be rounded down\n", size);
            }
            size_rounded = 1UL << bits;
        }
    }

    // check for correctly aligned base address
    mask = size_rounded - 1;

    if((UINT32)start & mask) {
        debug_printf("SANITY_CHECK_FAILED: start address 0x%08X and size (%i) are inconsistent\n", start, size);
    }

    // map generic permissions to mpu internal ACLs
    config = mpu_map_acl(acl, acl_exec);

    // calculate subregions from hw-specific ACL
    subregions = (acl_hw_spec << MPU_RASR_SRD_Pos) & MPU_RASR_SRD_Msk;

    /* enable region & add size */
    region->config = config | MPU_RASR_ENA_Msk | ((UINT32) (bits - 1) << MPU_RASR_SIZE_Pos) | subregions;
    region->start = start;
    region->end = (void*)((uint32_t)start + size_rounded);
    region->acl = acl;

    return size_rounded;
}

uint8_t CPU_mpu_region_bits(UINT32 size)
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
