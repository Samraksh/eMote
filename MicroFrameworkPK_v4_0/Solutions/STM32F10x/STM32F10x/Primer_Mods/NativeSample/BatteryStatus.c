/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        BatteryStatus_Read_Status(void);            // After the call the following variables are set
u8          BatteryStatus_Status_u08 = 0;               // Battery Status  0=Battery_absent , 1=Battery_Charge_done 2=Battery_Charge_in_progress 3=Battery_Stand_By_mode

/* -------- Private define ------------------------------------------------------------*/

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void BatteryStatus_Read_Status(void)
{
   // PC 6 = STATUS1      = to BATTERY UNIT      = 4 Input floating
   // PC 7 = STATUS2      = to BATTERY UNIT      = 4 Input floating
   BatteryStatus_Status_u08 = (GPIOC->IDR >> 6 ) & 0x03;
}
