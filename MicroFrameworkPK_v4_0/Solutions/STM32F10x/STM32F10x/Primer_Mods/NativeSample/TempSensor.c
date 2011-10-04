/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"
#include    "stm32f10x_adc.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        TempSensor_Read_Sensor(void);               // After the call the following variables are set
s16         TempSensor_Value_Fil_s16 = 0;               // Temperature filter 100 corresponds to 10 degrees
s16         TempSensor_Value_Roh_s16 = 0;               // Temperature unfiltered
u16         TempSensor_Value_Spg_u16 = 0;               // Voltage sensor unfiltered

/* -------- Private define ------------------------------------------------------------*/
#define     Vref_MV      2880
#define     filtereigenschaft 0.98 // All the more closer to 1, the greater will be smoothed
#define     V25_MV   1365
#define     AVG_uV     13298

/* -------- Private variables ---------------------------------------------------------*/
float       TempSensor_Value_tem_f32 =   0;

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void TempSensor_Read_Sensor(void)
{
   TempSensor_Value_Spg_u16 = ( ( ADC1->JDR2 * Vref_MV ) / 0x1000 );
   
   TempSensor_Value_Roh_s16 = ( ( ( V25_MV - TempSensor_Value_Spg_u16 ) *1000 ) / ( AVG_uV / 10 ) ) + 250 ;
   // Filter
   TempSensor_Value_Fil_s16 = TempSensor_Value_tem_f32;
   TempSensor_Value_tem_f32 = TempSensor_Value_tem_f32 * filtereigenschaft + (float)TempSensor_Value_Roh_s16 *(1-filtereigenschaft);
}
