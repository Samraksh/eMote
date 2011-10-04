/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"

/* -------- Used Global variables -----------------------------------------------------*/

/* -------- Create Global variables ---------------------------------------------------*/
void        Joystick_Read_Joy(void);                    // After the call the following variables are set
u8          Joystick_Up____Counter_u08 = 0;             // press in the variable increased by one, so that the press time can be evaluated (max 250)
u8          Joystick_Down__Counter_u08 = 0;             // press in the variable increased by one, so that the press time can be evaluated (max 250)
u8          Joystick_Right_Counter_u08 = 0;             // press in the variable increased by one, so that the press time can be evaluated (max 250)
u8          Joystick_Left__Counter_u08 = 0;             // press in the variable increased by one, so that the press time can be evaluated (max 250)
u8          Joystick_Push__Counter_u08 = 0;             // press in the variable increased by one, so that the press time can be evaluated (max 250)

/* -------- Private define ------------------------------------------------------------*/
#define     JOYSTICK_U 0x00000008
#define     JOYSTICK_D 0x00000010
#define     JOYSTICK_R 0x00000020
#define     JOYSTICK_L 0x00000040

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/

/* -------- Code ----------------------------------------------------------------------*/
void Joystick_Read_Joy()
{
   u32 temp;
   temp = GPIOE->IDR;
   /*-------------------------------------------------------------------------*/
   // PE 3 = JOYSTICK_L = to JOYSTICK      = 8 Input pull-down Set ODR 0
   if ( temp & JOYSTICK_L ) // PE 3 = JOYSTICK_L = to JOYSTICK
   {
      Joystick_Left__Counter_u08 ++;
      if ( Joystick_Left__Counter_u08 > 250 ) Joystick_Left__Counter_u08 = 250;
   }
   else
   {
      Joystick_Left__Counter_u08 = 0;
   }
   /*-------------------------------------------------------------------------*/
   // PE 4 = JOYSTICK_R = to JOYSTICK      = 8 Input pull-down Set ODR 0
   if ( temp & JOYSTICK_R ) // PE 4 = JOYSTICK_R = to JOYSTICK
   {
      Joystick_Right_Counter_u08 ++;
      if ( Joystick_Right_Counter_u08 > 250 ) Joystick_Right_Counter_u08 = 250;
   }
   else
   {
      Joystick_Right_Counter_u08 = 0;
   }
   /*-------------------------------------------------------------------------*/
   // PE 5 = JOYSTICK_U = to JOYSTICK      = 8 Input pull-down Set ODR 0
   if ( temp & JOYSTICK_U )
   {
      Joystick_Up____Counter_u08 ++;
      if ( Joystick_Up____Counter_u08 > 250 ) Joystick_Up____Counter_u08 = 250;
   }
   else
   {
      Joystick_Up____Counter_u08 = 0;
   }
   /*-------------------------------------------------------------------------*/
   // PE 6 = JOYSTICK_D = to JOYSTICK      = 8 Input pull-down Set ODR 0
   if ( temp & JOYSTICK_D )
   {
      Joystick_Down__Counter_u08 ++;
      if ( Joystick_Down__Counter_u08 > 250 ) Joystick_Down__Counter_u08 = 250;
   }
   else
   {
      Joystick_Down__Counter_u08 = 0;
   }
   /*-------------------------------------------------------------------------*/
   // PA 8 = PBUTTON    = to JOYSTICK      = 8 Input pull-down Set ODR 0
   temp = GPIOA->IDR;
   if ( temp & 0x00000100 )
   {
      Joystick_Push__Counter_u08 ++;
      if ( Joystick_Push__Counter_u08 > 250 ) Joystick_Push__Counter_u08 = 250;
   }
   else
   {
      Joystick_Push__Counter_u08 = 0;
   }
}
