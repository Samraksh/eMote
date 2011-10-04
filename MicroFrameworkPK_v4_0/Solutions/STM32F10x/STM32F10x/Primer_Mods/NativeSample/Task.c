/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x.h"
#include    "stm32f10x_can.h"
#include    "_Font/RGB.h"

/* -------- Used Global variables -----------------------------------------------------*/
/* -------- Main.c --------------------------------------------------------------------*/
extern bool Main_System_Init_Finished_Bol;              // Once all configuration is done it is set to true
/* -------- Led.c ---------------------------------------------------------------------*/
extern void Led_Set_Value_u8(u8 Value);              // Bit 0 is green, Bit 1 is red
/* -------- _Font/Font.c --------------------------------------------------------------*/
extern bool Font_Transparent_Bol;                       // if true, the background image is not overwritten. Background image is "Pictures/bmp_background_image.h generate" with "Create_background_image.xls"
extern u16  Font_Text_Color_u16;                        // Text color of the selected font
extern u16  Font_BGnd_Color_u16;                        // Background color of the selected font

/* -------- Create Global variables ---------------------------------------------------*/
void        Task_delay_ms(volatile uint32_t nTime);     // ms delay function

/* -------- Private define ------------------------------------------------------------*/

/* -------- Private variables ---------------------------------------------------------*/
uint8_t     LED_count_u08 = 0;
vu32        TimingDelay_u32;
u16         Tasks_Counter_0002ms_u16 =   0;
u16         Tasks_Counter_0005ms_u16 =   2;
u16         Tasks_Counter_0010ms_u16 =   3;
u16         Tasks_Counter_0020ms_u16 =   4;
u16         Tasks_Counter_0100ms_u16 =  86;
u16         Tasks_Counter_0200ms_u16 = 187;
u16         Tasks_Counter_0500ms_u16 = 488;
u16         Tasks_Counter_1000ms_u16 = 989;

/* -------- Prototyp local ------------------------------------------------------------*/
void        Tasks_Handler();
void        Tasks_0001ms(void);
void        Tasks_0002ms(void);
void        Tasks_0005ms(void);
void        Tasks_0010ms(void);
void        Tasks_0020ms(void);
void        Tasks_0100ms(void);
void        Tasks_0200ms(void);
void        Tasks_0500ms(void);
void        Tasks_1000ms(void);

/* -------- Code ----------------------------------------------------------------------*/
void Tasks_0001ms(){}

void Tasks_0002ms(){}

void Tasks_0005ms()
{
   CAN_Debug_Send_All_Primer_Data();
}

void Tasks_0010ms(){}

void Tasks_0020ms()
{
   AccelSensor_Read_Sensor();
   BatteryStatus_Read_Status();
   Joystick_Read_Joy();
   TempSensor_Read_Sensor();
   TouchScreen_Read_Touch();
   Vbat_Read_Vbat();
}
void Tasks_0100ms(){}

void Tasks_0200ms()
{
   Led_Set_Value_u8(LED_count_u08++);
}

void Tasks_0500ms(){}

void Tasks_1000ms(){}

void Tasks_Handler()
{
   if ( Main_System_Init_Finished_Bol == TRUE )
   {
      /**/                               Tasks_0001ms();
      if(Tasks_Counter_0002ms_u16 == 0 ){Tasks_0002ms();}
      if(Tasks_Counter_0005ms_u16 == 0 ){Tasks_0005ms();}
      if(Tasks_Counter_0010ms_u16 == 0 ){Tasks_0010ms();}
      if(Tasks_Counter_0020ms_u16 == 0 ){Tasks_0020ms();}
      if(Tasks_Counter_0100ms_u16 == 0 ){Tasks_0100ms();}
      if(Tasks_Counter_0200ms_u16 == 0 ){Tasks_0200ms();}
      if(Tasks_Counter_0500ms_u16 == 0 ){Tasks_0500ms();}
      if(Tasks_Counter_1000ms_u16 == 0 ){Tasks_1000ms();}
      
      Tasks_Counter_0002ms_u16 ++;
      Tasks_Counter_0005ms_u16 ++;
      Tasks_Counter_0010ms_u16 ++;
      Tasks_Counter_0020ms_u16 ++;
      Tasks_Counter_0100ms_u16 ++;
      Tasks_Counter_0200ms_u16 ++;
      Tasks_Counter_0500ms_u16 ++;
      Tasks_Counter_1000ms_u16 ++;
   }
   if( Tasks_Counter_0002ms_u16 >   1 ) { Tasks_Counter_0002ms_u16 = 0;}
   if( Tasks_Counter_0005ms_u16 >   4 ) { Tasks_Counter_0005ms_u16 = 0;}
   if( Tasks_Counter_0010ms_u16 >   9 ) { Tasks_Counter_0010ms_u16 = 0;}
   if( Tasks_Counter_0020ms_u16 >  19 ) { Tasks_Counter_0020ms_u16 = 0;}
   if( Tasks_Counter_0100ms_u16 >  99 ) { Tasks_Counter_0100ms_u16 = 0;}
   if( Tasks_Counter_0200ms_u16 > 199 ) { Tasks_Counter_0200ms_u16 = 0;}
   if( Tasks_Counter_0500ms_u16 > 499 ) { Tasks_Counter_0500ms_u16 = 0;}
   if( Tasks_Counter_1000ms_u16 > 999 ) { Tasks_Counter_1000ms_u16 = 0;}
   // for DELAY funktion
   if (TimingDelay_u32 != 0) { TimingDelay_u32--; }
}
void Task_delay_ms(volatile uint32_t nTime)
{
   TimingDelay_u32 = nTime;
   while(TimingDelay_u32 != 0);
}
