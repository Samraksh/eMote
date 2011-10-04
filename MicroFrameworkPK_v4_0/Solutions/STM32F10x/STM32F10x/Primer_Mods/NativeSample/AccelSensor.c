/********************* (C) COPYRIGHT 2007 RAISONANCE S.A.S. ****************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"
#include    "stm32f10x_spi.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        AccelSensor_Init_Sensor(void);              // needs to be started once
void        AccelSensor_Read_Sensor(void);              // After the call the following variables are set
s16         AccelSensor_X_Roh_s16 = 0;                  // Accel Sensor X unfiltered
s16         AccelSensor_Y_Roh_s16 = 0;                  // Accel Sensor Y unfiltered
s16         AccelSensor_Z_Roh_s16 = 0;                  // Accel Sensor Z unfiltered
s16         AccelSensor_X_Fil_s16 = 0;                  // Accel Sensor X filtered
s16         AccelSensor_Y_Fil_s16 = 0;                  // Accel Sensor Y filtered
s16         AccelSensor_Z_Fil_s16 = 0;                  // Accel Sensor Z filtered

/* -------- Private define ------------------------------------------------------------*/
#define     filtereigenschaft      0.95                     /* All the more closer to 1, the greater will be smoothed */
#define     ChipSelectLOW()     GPIOE->BRR  = 0x00000004    /* PE 2 = CS_MEMS    = to 3 axis Sensor = 3 Output Push-pull  50Mhz */
#define     ChipSelectHigh()    GPIOE->BSRR = 0x00000004    /* PE 2 = CS_MEMS    = to 3 axis Sensor = 3 Output Push-pull  50Mhz */

/* -------- Private variables ---------------------------------------------------------*/
float       AccelSensor_X_tmp_f32 = 0;
float       AccelSensor_Y_tmp_f32 = 0;
float       AccelSensor_Z_tmp_f32 = 0;

/* -------- Prototyp local ------------------------------------------------------------*/
static u8   SPI1SendData_u08( u8 byte );
u8          AccelSensor_ReadID(void);
void        AccelSensor_WakeUp(void);

/* -------- Code ----------------------------------------------------------------------*/
void AccelSensor_Read_Sensor(void)
{
   ChipSelectLOW();
   SPI1SendData_u08( 0xE8 );
   AccelSensor_X_Roh_s16  =   SPI1SendData_u08( 0xA5 )       ;
   AccelSensor_X_Roh_s16 |= ( SPI1SendData_u08( 0xA5 ) << 8 );
   AccelSensor_Y_Roh_s16  =   SPI1SendData_u08( 0xA5 )       ;
   AccelSensor_Y_Roh_s16 |= ( SPI1SendData_u08( 0xA5 ) << 8 );
   AccelSensor_Z_Roh_s16  =   SPI1SendData_u08( 0xA5 )       ;
   AccelSensor_Z_Roh_s16 |= ( SPI1SendData_u08( 0xA5 ) << 8 );
   ChipSelectHigh();
   
   // Filter
   AccelSensor_X_Fil_s16 = AccelSensor_X_tmp_f32;
   AccelSensor_X_tmp_f32 = AccelSensor_X_tmp_f32 * filtereigenschaft + (float)AccelSensor_X_Roh_s16 *(1-filtereigenschaft);
   AccelSensor_Y_Fil_s16 = AccelSensor_Y_tmp_f32;
   AccelSensor_Y_tmp_f32 = AccelSensor_Y_tmp_f32 * filtereigenschaft + (float)AccelSensor_Y_Roh_s16 *(1-filtereigenschaft);
   AccelSensor_Z_Fil_s16 = AccelSensor_Z_tmp_f32;
   AccelSensor_Z_tmp_f32 = AccelSensor_Z_tmp_f32 * filtereigenschaft + (float)AccelSensor_Z_Roh_s16 *(1-filtereigenschaft);
}

static u8 SPI1SendData_u08( u8 byte )
{
   while( ( SPI1->SR & SPI_I2S_FLAG_TXE  ) == RESET ); // wait of emty puffer
   SPI1->DR = byte ;                                   // send
   while( ( SPI1->SR & SPI_I2S_FLAG_RXNE ) == RESET ); // wait of send ending
   return SPI1->DR;                                    // return Rx
}
void AccelSensor_Init_Sensor(void)
{
   if( AccelSensor_ReadID() != 0x3A )
   {
      s32 i;
      for( i = 0 ; i < 20 ; i++ )
      {
         // PA 5 = SPI_SCK       = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
         // PA 6 = SPI_MISO      = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
         // PA 7 = SPI_MOSI      = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
         GPIOA->CRL = (GPIOA->CRL & 0x000FFFFF) | 0x33300000;
         
         GPIOA->BSRR = 0x00000080;
         ChipSelectLOW();
         GPIOA->BRR  = 0x00000020;
         GPIOA->BSRR = 0x00000020;
         ChipSelectHigh();
         
         // PA 5 = SPI_SCK       = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
         // PA 6 = SPI_MISO      = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
         // PA 7 = SPI_MOSI      = to 3 axis Sensor   = B Alternate Function Output Push-pull  50Mhz
         GPIOA->CRL = (GPIOA->CRL & 0x000FFFFF) | 0xBBB00000;
         
         if ( AccelSensor_ReadID() == 0x3A )
         {
            break;
         }
      }
   }
   AccelSensor_WakeUp();
}
u8 AccelSensor_ReadID( void )
{
   u8 Temp ;
   ChipSelectLOW();
   SPI1SendData_u08( 0x8F );
   Temp = SPI1SendData_u08( 0xA5 );
   ChipSelectHigh();
   return Temp;
}
void AccelSensor_WakeUp( void )
{
   u8 Temp;
   ChipSelectLOW();
   SPI1SendData_u08( 0xA0 );
   Temp = SPI1SendData_u08( 0xA5 );
   ChipSelectHigh();
   Temp = Temp | 0xC0 | 0x30;
   ChipSelectLOW();
   SPI1SendData_u08( 0x20 );
   SPI1SendData_u08( Temp );
   ChipSelectHigh();
}
