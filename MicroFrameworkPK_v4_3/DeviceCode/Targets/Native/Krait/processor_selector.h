////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _KRAIT_PROCESSOR_SELECTOR_H_
#define _KRAIT_PROCESSOR_SELECTOR_H_ 1

#define LK 1
#define WITH_DEBUG_UART 1
//#define DISPLAY_SPLASH_SCREEN 1
#define DISPLAY_TYPE_MIPI 1
//#define DISPLAY_MIPI_PANEL_TOSHIBA_MDT61 1
//#define CONT_SPLASH_SCREEN 1
#define MEMSIZE 0X00200000
#define MEMBASE 0X90000000
#define BASE_ADDR 0X90000000
#define TAGS_ADDR BASE_ADDR+0X00000100
#define KERNEL_ADDR BASE_ADDR+0X00008000
#define RAMDISK_ADDR BASE_ADDR+0X01000000
#define SCRATCH_ADDR 0X90000000
#define ARM_CPU_CORE_KRAIT
#define WITH_CPU_EARLY_INIT 0
#define WITH_CPU_WARM_BOOT 0
#define MMC_SLOT 1
#define MDP4 1
#define SSD_ENABLE
#define ARM_CPU_CORTEX_A8 1
#define ARM_WITH_CP15 1
#define ARM_WITH_MMU 1
#define ARM_ISA_ARMV7 1
#define ARM_WITH_VFP 1
#define ARM_WITH_NEON 1
#define ARM_WITH_THUMB 1
#define ARM_WITH_THUMB2 1
#define ARM_WITH_CACHE 1
#define ARM_WITH_L2 1
#define BOARD TITAN
#define PROJECT_TITAN 1
#define TARGET_TITAN 1
#define PLATFORM_MSM8960 1
#define ARCH_ARM 1
#define WITH_APP_ABOOT 1
#define WITH_DEV_FBCON 1
#define WITH_DEV_KEYS 1
#define WITH_DEV_PMIC_PM8921 1
#define WITH_DEV_SSBI 1
#define WITH_LIB_DEBUG 1
#define WITH_LIB_HEAP 1
#define WITH_LIB_LIBC 1
#define WITH_LIB_OPENSSL 1
#define WITH_LIB_OPENSSL_CRYPTO 1
#define WITH_LIB_PTABLE 1
#define DEBUG 1
#define SAM_CPPALLOC 1 /* allow operators new and delete on CLR_RT_Heap. */
#endif

