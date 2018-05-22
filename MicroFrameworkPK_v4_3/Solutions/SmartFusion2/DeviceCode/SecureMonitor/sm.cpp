#include "sm.h"

#include <Samraksh/cm_mpu.h>

void SecureMonitor_Initialize(){
	  /* Disable code execution from RAM. */
	    CPU_mpu_configure_region(6, // Region number.
	                         (void*)0x20000000, // Base address
	                         20, // 1MB (2^20)
	                         AP_RW_RW,
	                         false); // Non executable.
}
