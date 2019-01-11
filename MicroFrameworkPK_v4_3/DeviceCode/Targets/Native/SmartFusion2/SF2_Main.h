/*
 * SF2_Main.h
 *
 *  Created on: Oct 16, 2018
 *      Author: Mukundan Sridharan
 */

#ifndef _SF2_MAIN_H_
#define _SF2_MAIN_H_

#include <tinyhal.h>
//#include <stdint.h>
#include <cmsis_gcc.h>
#include <Device/ARM/ARMCM3/Include/ARMCM3.h>
#include <core_cm3.h>

//implemented in simple power
inline uint32_t JTAG_Attached() {
    return (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk);
}

#endif //_SF2_MAIN_H_
