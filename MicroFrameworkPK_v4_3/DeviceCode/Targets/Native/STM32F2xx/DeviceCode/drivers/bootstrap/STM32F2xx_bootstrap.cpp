////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**======================================================
 *		SAMRAKSH
 *=======================================================
 *	Author: Mukundan
 *	Date: June 21, 2013
 *	Notes: Threw away the stuff from stm32f10x. Simply reusing the stm32f2xx functions.
 *
 ======================================================**/

#include <tinyhal.h>
#include <system_stm32f2xx.h>
#include <rcc/STM32F2xx_rcc.h>
#include <misc/misc.h>
#include <flash/stm32f2xx_flash.h>


/**
 * Calls various bootstrap routines
 **/
void BootstrapCode() {

	SystemInit();
	SystemCoreClockUpdate();
	PrepareImageRegions();
	//Boot_DisableWDT();
}

