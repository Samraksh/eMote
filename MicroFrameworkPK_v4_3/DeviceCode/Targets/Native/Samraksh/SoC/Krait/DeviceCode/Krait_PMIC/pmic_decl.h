#ifndef _PMIC_DECL_H_
#define _PMIC_DECL_H_ 1

/*
 * @file: pmic_decl.h
 * @author: Michael Andrew McGrath
 * @copyright: The Samraksh Company, Copyright (c) 2014
 * @date: April 2, 2014
 * @brief: Combines pmic-related functions from multiple incarnations of the PMIC driver.
 * @details: Although the filename pmic_decl.h makes it appear that this file
 * should be placed in DeviceCode\Include\ for global HAL use, really the
 * PMIC driver is shared among just the SOC_ADAPT drivers.  This is an attempt
 * to just have one driver that combines functionality from the LK pm8921
 * and Android pm8xxx drivers and aliases functions where appropriate to
 * beget ONE UNIFIED PMIC DRIVER.
 */


#include "pm8921.h"

//extern "C" {
    /**
     * @brief: initializes the driver with pm8921 owning the device object..
     */
    void PMIC_Initialize(void);
//}

#endif
