/*
 * Author: Mukundan Sridhran
 *
 * API definitions for Memory Protection Unit
 */

#ifndef _CM_MPU_H_
#define _CM_MPU_H_


// based on examples at
// https://github.com/cvra/arm-cortex-mpu/blob/master/mpu.h


// set default minimum region address alignment
#ifndef ARMv7M_MPU_ALIGNMENT_BITS
#define ARMv7M_MPU_ALIGNMENT_BITS 5
#endif //ARMv7M_MPU_ALIGNMENT_BITS

// set default MPU region count
#ifndef ARMv7M_MPU_REGIONS
#define ARMv7M_MPU_REGIONS 8
#endif //ARMv7M_MPU_REGIONS


#define MPU_REGION_BITS(x)       (((x)<=32UL)?5:(((x)<=64UL)?\
    6:(((x)<=128UL)?7:(((x)<=256UL)?8:(((x)<=512UL)?9:(((x)<=1024UL)?\
    10:(((x)<=2048UL)?11:(((x)<=4096UL)?12:(((x)<=8192UL)?\
    13:(((x)<=16384UL)?14:(((x)<=32768UL)?15:(((x)<=65536UL)?\
    16:(((x)<=131072UL)?17:(((x)<=262144UL)?18:(((x)<=524288UL)?\
    19:(((x)<=1048576UL)?20:(((x)<=2097152UL)?21:(((x)<=4194304UL)?\
    22:(((x)<=8388608UL)?23:(((x)<=16777216UL)?24:(((x)<=33554432UL)?\
    25:(((x)<=67108864UL)?26:(((x)<=134217728UL)?27:(((x)<=268435456UL)?\
    28:(((x)<=536870912UL)?29:(((x)<=1073741824UL)?30:(((x)<=2147483648UL)?\
    31:32)))))))))))))))))))))))))))

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
    void *start;     // Start address of the MPU region.
    void *end;       // End address of the MPU region.
    UINT32 regionNo;    // MPU specific config data (permissions, subregions, etc)
    UINT32 config;
    MpuMemPermission_t acl;   //region permissions
} MpuRegion_t;

void CPU_mpu_init(void);

void CPU_mpu_enable(void);

void CPU_mpu_disable(void);

void CPU_mpu_configure_region(UINT8 region, void *addr, UINT8 log2_len,
                          MpuMemPermission_t ap, bool executable);

MpuRegion_t* CPU_mpu_findRegion(void* addr);

UINT32 CPU_mpu_region_translate_acl(MpuRegion_t * const region, void* start, UINT32 size,
		MpuMemPermission_t acl, UINT32 acl_hw_spec);


#ifdef __cplusplus
}
#endif

#endif //_CM_MPU_H_
