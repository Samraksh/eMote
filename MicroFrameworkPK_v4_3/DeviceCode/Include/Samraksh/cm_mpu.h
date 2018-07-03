/*
 * Author: Mukundan Sridhran
 *
 * API definitions for Memory Protection Unit
 */

#ifndef _CM_MPU_H_
#define _CM_MPU_H_


/*based on examples at
https://github.com/cvra/arm-cortex-mpu/blob/master/mpu.h
*/

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    AP_NO_NO=0x0,
    AP_RW_NO=0x1,
    AP_RW_RO=0x2,
    AP_RW_RW=0x3,
    AP_RESERVED=0x4,
    AP_RO_NO=0x5,
    AP_RO_RO=0x6,
} MpuMemPermission_t;


typedef struct
{
    uint32_t start;     /**< Start address of the MPU region. */
    uint32_t end;       /**< End address of the MPU region. */
    //uint32_t config;    /**< MPU specific config data (permissions, subregions, etc). */
    MpuMemPermission_t acl;   /**< Original box ACL. */
} MpuRegion_t;

void CPU_mpu_init(void);

void CPU_mpu_enable(void);

void CPU_mpu_disable(void);

void CPU_mpu_configure_region(UINT8 region, void *addr, UINT8 log2_len,
                          mem_access_permission_t ap, bool executable);




#ifdef __cplusplus
}
#endif

#endif //_CM_MPU_H_
