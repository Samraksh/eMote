#include <tinyhal_types.h>
//#include <SmartPtr_irq.h>
#include <Samraksh/cm_mpu.h>
#include <cmsis/mss_assert.h>
#include <cmsis/m2sxxx.h>
#include <core_cm3.h>
#include <cmsis_gcc.h>


//redefinition here. This is usually done on platform_selector.h, but since this is sort of a chip file,
//need to do this again here
/*#ifndef GLOBAL_LOCK
#define GLOBAL_LOCK(x)	SmartPtr_IRQ x
#define DISABLE_INTERRUPTS()       SmartPtr_IRQ::ForceDisabled()
#define ENABLE_INTERRUPTS()        SmartPtr_IRQ::ForceEnabled()
#define INTERRUPTS_ENABLED_STATE() SmartPtr_IRQ::GetState()
#define GLOBAL_LOCK_SOCKETS(x)     SmartPtr_IRQ x
#endif
*/

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
#define MPU_RASR_SRD(x)     (((UINT32)(x))<<MPU_RASR_SRD_Pos)

#define MPU_MTYPE_NORMAL          (8 << 16)

#define MPU_RBAR(region,addr)   (((UINT32)(region))|MPU_RBAR_VALID_Msk|addr)
#define MPU_RBAR_RNR(addr)     (addr)


MpuRegion_t g_mpuRegions[8];

//Takes mpu permissions and returns the mpu flags to be set
static UINT32 mpu_map_acl(MpuMemPermission_t acl, BOOL exec)
{
    UINT32 flags;
    UINT32 acl_res;

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


uint8_t mpu_region_bits(UINT32 size)
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

/// Compute the MPU region size for the given BSS sections and stack sizes.
/// The function also updates the region_start parameter to meet the alignment
/// requirements of the MPU.
//static UINT32 mpu_sram_region_size(UINT32 * region_start, UINT32 const bss_size, UINT32 const stack_size)
static UINT32 mpu_align_region_size(UINT32 * region_start, UINT32 const requestedsize)
{
    int bits = mpu_region_bits(requestedsize);

    /// Calculate the whole MPU region size.
    /// Note: In order to support subregions the region size is at least 2**8.
    if (bits < 8) {
        bits = 8;
    }
    UINT32 region_size = 1UL << bits;

    /// Ensure that the MPU region is aligned to a multiple of its size.
    if ((*region_start & (region_size - 1)) != 0) {
        *region_start = (*region_start & ~(region_size - 1)) + region_size;
    }

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

void CPU_mpu_configure_region(UINT8 regionNo, UINT32 startAddr, UINT32 regionSize,
                          MpuMemPermission_t ap, BOOL executable)
{
	BOOL priv=FALSE;
	//check for priviledged mode
	if (__get_IPSR() || !(__get_CONTROL() & 0x1))
	{
		priv=TRUE;
	}
	// You need to be in Privilged mode to make changes to MPU.
	ASSERT(priv);

	volatile UINT32 sizePow =  mpu_align_region_size(&startAddr, regionSize);

    ASSERT(sizePow >= 5); // "region too small"

    // Extract region and address information.
    regionNo = regionNo & MPU_RBAR_REGION_Msk;
    UINT32 addr = (startAddr & MPU_RBAR_ADDR_Msk);

    // If the region is not executable add the eXecute Never flag.
    //if (!executable) {
    //    rasr += MPU_RASR_XN_Msk;
    //}

    UINT32 rasr = mpu_map_acl(ap,executable);
    // Construct the Region Attributes and Size Register value.
    UINT32 size = ((UINT32)(sizePow - 1) << MPU_RASR_SIZE_Pos);
    rasr= rasr | MPU_RASR_ENABLE_Msk | size;
    rasr=rasr | MPU_MTYPE_NORMAL;

    /* more complicated uvisor type calculation
    UINT32 regionSize=mpu_sram_region_size(startAddr, size);
    MpuRegion_t *region = &g_mpuRegions[regionIndex];
    UINT32 rounded_size = CPU_mpu_region_translate_acl(&region, startAddr, regionSize,
        acl, 0);

    // apply RASR & RBAR
    MPU->RBAR = MPU_RBAR(regionIndex, region->start);
    MPU->RASR = region->config;
    */

    //update global region state
    g_mpuRegions[regionNo].start=addr;
    g_mpuRegions[regionNo].end=addr+regionSize;
    g_mpuRegions[regionNo].regionNo= regionNo;
    g_mpuRegions[regionNo].acl = ap;
    g_mpuRegions[regionNo].config = rasr;

    CPU_mpu_set_acl(regionNo,&g_mpuRegions[regionNo]);

}

void CPU_mpu_init(void)
{
	uint32_t aligment_mask;

	// Disable the MPU.
	MPU->CTRL = 0;
	// Check MPU region alignment using region number zero.
	MPU->RNR = 0;
	MPU->RBAR = MPU_RBAR_ADDR_Msk;
	aligment_mask = ~MPU->RBAR;
	MPU->RBAR = 0;

	// Enable mem, bus and usage faults.
	SCB->SHCSR |= (SCB_SHCSR_USGFAULTENA_Msk) |
				  (SCB_SHCSR_BUSFAULTENA_Msk) |
				  (SCB_SHCSR_MEMFAULTENA_Msk);

}

//Note: Finds the first region to which address belongs; Overlapping regions could be a problem.
//Because faults are likely to happen to lesser previleged region,
//search needs to happen in the order where we start from least previledged region.
MpuRegion_t* CPU_mpu_findRegion(void* addr) {
	MpuRegion_t *region;
	region =g_mpuRegions;
	for (int count=7; count >= 0; count--) {
		region =&g_mpuRegions[count];
		if (region->start <= (UINT32)addr && (UINT32)addr < region->end) {
			return region;
		}
	}
	return NULL;
}


UINT32 CPU_mpu_region_translate_acl(MpuRegion_t * const region, UINT32 startAddr, UINT32 size,
		MpuMemPermission_t acl, BOOL acl_exec, UINT32 acl_hw_spec)
{
    UINT32 config, bits, mask, size_rounded, subregions;

    /* verify region alignment */
    bits = mpu_region_bits(size);
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

    if(startAddr & mask) {
        debug_printf("SANITY_CHECK_FAILED: start address 0x%08X and size (%i) are inconsistent\n", startAddr, size);
    }

    // map generic permissions to mpu internal ACLs
    config = mpu_map_acl(acl, acl_exec);

    // calculate subregions from hw-specific ACL
    subregions = (acl_hw_spec << MPU_RASR_SRD_Pos) & MPU_RASR_SRD_Msk;

    // enable region & add size
    //region->config = config | MPU_RASR_ENA_Msk | ((UINT32) (bits - 1) << MPU_RASR_SIZE_Pos) | subregions;
    region->config = config | MPU_RASR_ENABLE_Msk | ((UINT32) (bits - 1) << MPU_RASR_SIZE_Pos) ;
    region->start = startAddr;
    region->end = startAddr + size_rounded;
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

void CPU_mpu_lock(void){
	// DMB to ensure MPU update after all transfer to memory completed
	__DMB();

	// Finally enable the MPU.
	MPU->CTRL = MPU_CTRL_ENABLE_Msk | MPU_CTRL_PRIVDEFENA_Msk;
	MPU->CTRL = MPU_CTRL_ENABLE_Msk;

	// DSB & ISB to ensure subsequent data & instruction transfers are using updated MPU settings
	__DSB();
	__ISB();
}

void CPU_mpu_invalidate_region(UINT8 regionNo){
	MPU->RNR = regionNo;
	MPU->RASR = 0;
	MPU->RBAR = 0;
}

void CPU_mpu_set_acl(UINT8 regionNo, MpuRegion_t* region){
	//disable interrupts
	 //__disable_irq();

	// Make sure the memory barriers are correct.
	//__ISB(); //sync instruction access
	//__DSB(); // sync data access

	// Update MPU region attributes

	//MPU->RNR=regionNo;
	//No need to set the region number register (rnr). It will be set through rbar
	MPU->RBAR=MPU_RBAR(regionNo,region->start);
	MPU->RASR = region->config;

	 //__enable_irq();
}
