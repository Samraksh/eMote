/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"
#include    "stm32f10x_adc.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        Vbat_Read_Vbat(void);                       // After the call the following variables are set
u16         Vbat_mV_Fil_u16 = 0;                        // Vbatt filtered
u16         Vbat_mV_Roh_u16 = 0;                        // Vbatt unfiltered

/* -------- Private define ------------------------------------------------------------*/
#define     Vref_MV           2880
#define     filtereigenschaft 0.98 // All the more closer to 1, the greater will be smoothed

/* -------- Private variables ---------------------------------------------------------*/
float       Vbat_mV_Tem_f32 = 0;

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void Vbat_Read_Vbat(void)
{
   Vbat_mV_Roh_u16 = ( ( ADC1->DR * 2 ) * Vref_MV ) / 0x1000;
   // Filter
   Vbat_mV_Fil_u16 = Vbat_mV_Tem_f32;
   Vbat_mV_Tem_f32 = Vbat_mV_Tem_f32 * filtereigenschaft + (float)Vbat_mV_Roh_u16 *(1-filtereigenschaft);
}
