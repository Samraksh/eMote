/********************* (C) COPYRIGHT 2007 RAISONANCE S.A.S. ****************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"
#include    "stm32f10x_adc.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        TouchScreen_Read_Touch(void);             // After the call the following variables are set
u8          TouchScreen_X_Pos_u08 =     0;            // The name says it all
u8          TouchScreen_Y_Pos_u08 =     0;            // The name says it all
bool        TouchScreen_Press_Bol = FALSE;            // True if press

/* -------- Private define ------------------------------------------------------------*/

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void TouchScreen_Read_Touch(void)
{
   u32   Tx,Ty,Tt;
   vs32  X, Y, T,q ;
   
   Tx = ADC1->JDR1;
   Tt = ADC1->JDR3;
   Ty = ADC1->JDR4;
   
   q =    (4095-Ty);
   X =    (1000*    Tx ) / q;
   Y =  - (1000*(Ty-Tt)) / q;
   T =    (1000*(Tt-Tx)) / q;
   
   TouchScreen_Press_Bol = ( Tt < 3000 );
   
   if ( TouchScreen_Press_Bol )
   {
      // Correction
      Y = ( ( 570 * Y) - (  15 * T ) - 19400 ) / 1000;
      X = ( ( 490 * X) + ( 170 * Y ) - 15157 ) / 1000;
      
      if ( X < 0 ) X = 0;
      if ( Y < 0 ) Y = 0;
      
      TouchScreen_X_Pos_u08 = X;
      TouchScreen_Y_Pos_u08 = Y;
   }
   else
   {
      TouchScreen_X_Pos_u08 = 0;
      TouchScreen_Y_Pos_u08 = 0;
   }
}
