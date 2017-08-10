#ifndef __KRAIT__MMC_H_
#define __KRAIT__MMC_H_

#include "mmc.h"

#define MSM_SDC1_BASE       0x12400000
#define MSM_SDC2_BASE       0x12140000
#define MSM_SDC3_BASE       0x12180000
#define MSM_SDC4_BASE       0x121C0000

static unsigned mmc_sdc_base[] =
    { MSM_SDC1_BASE, MSM_SDC2_BASE, MSM_SDC3_BASE, MSM_SDC4_BASE };



#endif
