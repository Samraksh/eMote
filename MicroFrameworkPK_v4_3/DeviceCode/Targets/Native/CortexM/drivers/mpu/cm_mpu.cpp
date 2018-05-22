#include <Samraksh/cm_mpu.h>

#include <cmsis/m2sxxx.h>
#include <core_cm3.h>

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

    /* NULL pointer protection, highest priority. */
    mpu_configure_region(7, NULL, 5, AP_NO_NO, false);

    /* Mark RAM as non executable. Using lowest priority means explicitely
     * allowing a RAM region to be executable is possible.
     *
     * We can use a fixed address range for RAM because it is a standard define
     * by ARM. */
    void *ram_base = (void *)0x20000000;
    int ram_size_pow2 = 29; /* 2^29 = 0.5GB. */
    mpu_configure_region(0, ram_base, ram_size_pow2, AP_RW_RW, false);

    mpu_enable();
}

void MemManage_Handler(void)
{
    static char msg[128];
    struct port_extctx ctx;

    uint32_t MMFSR;

    /* Setup default error message. */
    strcpy(msg, __FUNCTION__);

    /* Get context info */
    memcpy(&ctx, (void*)__get_PSP(), sizeof(struct port_extctx));

    /* Get Memory Managment fault adress register */
    MMFSR = SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk;

    /* Data access violation */
    if (MMFSR & (1 << 1)) {
        snprintf(msg, sizeof(msg),
                 "Invalid access to %p (pc=%p)", (void *)SCB->MMFAR, ctx.pc);
    }

    /* Instruction address violation. */
    if (MMFSR & (1 << 0)) {
        snprintf(msg, sizeof(msg),
                 "Jumped to XN region %p (lr_thd=%p)",
                    (void *)SCB->MMFAR, ctx.lr_thd);
    }

    chSysHalt(msg);
}
