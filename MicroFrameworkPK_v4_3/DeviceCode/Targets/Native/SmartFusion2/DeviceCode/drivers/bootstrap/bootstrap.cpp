#include <tinyhal.h>
#include <cmsis/m2sxxx.h>
#include <Timer/mss_timer.h>
#include <CMSIS/system_m2sxxx.h>

static void RCC_Configuration(void);


void LowLevelInit (void)
{

}

static void reset_all_timers(void) {
  SysTick->CTRL = 0;
  MSS_TIM1_stop();
  MSS_TIM2_stop();

}


//RCC_Configuration : Controls the speed at which the System Clock, ADCClock, AHB, APB1 and APB2 run
static void RCC_Configuration(void)
{
  // Most if not all of this should be moved to CPU_Initialize() power driver. --NPS
  //RCC_DeInit();
  
  // Reset Timers
  reset_all_timers();

  //CPU_ChangePowerLevel(POWER_LEVEL__HIGH_POWER);

  // Enabling FSMC peripheral clock here
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
}


/**
 * Calls various bootstrap routines
 **/
void BootstrapCode() {

	//LowLevelInit();
	PrepareImageRegions();
	RCC_Configuration();
}

extern  "C"
{
void SystemInit_ExtMemCtl(void)
{

}

}
