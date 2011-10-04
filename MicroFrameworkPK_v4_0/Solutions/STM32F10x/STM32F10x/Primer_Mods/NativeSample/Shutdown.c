/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        Shutdown_Me(void);                          // After the call, the primer off

/* -------- Private define ------------------------------------------------------------*/

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void Shutdown_Me(void)
{
   // PC13 = SHUTDOWN     = to "SHUTDOWN"        = 3 Output Push-pull  50Mhz
   GPIOC->BSRR = 0x00002000;
}
