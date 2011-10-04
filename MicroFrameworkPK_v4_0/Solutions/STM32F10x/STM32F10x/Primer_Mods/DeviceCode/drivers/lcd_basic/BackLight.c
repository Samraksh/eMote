/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        BackLight_On(void);                         // turn on  the LCD backlight
void        BackLight_Off(void);                        // turn off the LCD backlight

/* -------- Private define ------------------------------------------------------------*/

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void BackLight_On(void)
{
   GPIOB->BSRR = 0x00000100;  // PB 8 = BACKLIGHT_/EN  = to Display   = 3 Output Push-pull  50Mhz
}
void BackLight_Off(void)
{
   GPIOB->BRR  = 0x00000100;  // PB 8 = BACKLIGHT_/EN  = to Display   = 3 Output Push-pull  50Mhz
}
