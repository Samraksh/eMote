/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x_can.h"

/* -------- Used Global variables -----------------------------------------------------*/
/* -------- AccelSensor.c -------------------------------------------------------------*/
extern s16  AccelSensor_X_Roh_s16;                      // Accel Sensor X unfiltered
extern s16  AccelSensor_Y_Roh_s16;                      // Accel Sensor Y unfiltered
extern s16  AccelSensor_Z_Roh_s16;                      // Accel Sensor Z unfiltered
extern s16  AccelSensor_X_Fil_s16;                      // Accel Sensor X filtered
extern s16  AccelSensor_Y_Fil_s16;                      // Accel Sensor Y filtered
extern s16  AccelSensor_Z_Fil_s16;                      // Accel Sensor Z filtered
/* -------- BatteryStatus.c -----------------------------------------------------------*/
extern u8   BatteryStatus_Status_u08;                   // Battery Status  0=Battery_absent , 1=Battery_Charge_done 2=Battery_Charge_in_progress 3=Battery_Stand_By_mode
/* -------- _Font/Font.c --------------------------------------------------------------*/
extern u8   Font_Width_u08;                             // Font width of the selected font
extern u8   Font_Height_u08;                            // Font height of the selected font
extern u8   Font_Starts_u08;                            // first sign of the selected font
extern u8   Font_Ends_u08;                              // last character of the selected font
extern u8   Font_Scale_u08;                             // Scale of the selected font
extern bool Font_Transparent_Bol;                       // if true, the background image is not overwritten. Background image is "Pictures/bmp_background_image.h generate" with "Create_background_image.xls"
extern u16  Font_Text_Color_u16;                        // Text color of the selected font
extern u16  Font_BGnd_Color_u16;                        // Background color of the selected font
/* -------- Joystick.c ---------------------------------------------------*/
extern u8   Joystick_Up____Counter_u08;                 // press in the variable increased by one, so that the press time can be evaluated (max 250)
extern u8   Joystick_Down__Counter_u08;                 // press in the variable increased by one, so that the press time can be evaluated (max 250)
extern u8   Joystick_Right_Counter_u08;                 // press in the variable increased by one, so that the press time can be evaluated (max 250)
extern u8   Joystick_Left__Counter_u08;                 // press in the variable increased by one, so that the press time can be evaluated (max 250)
extern u8   Joystick_Push__Counter_u08;                 // press in the variable increased by one, so that the press time can be evaluated (max 250)
/* -------- Main.c --------------------------------------------------------------------*/
extern bool Main_System_Init_Finished_Bol;              // Once all configuration is done it is set to true
/* -------- TempSensor.c --------------------------------------------------------------*/
extern s16  TempSensor_Value_Fil_s16;                   // Temperature filter 100 corresponds to 10 degrees
extern s16  TempSensor_Value_Roh_s16;                   // Temperature unfiltered
extern u16  TempSensor_Value_Spg_u16;                   // Voltage sensor unfiltered
/* -------- TouchScreen.c -------------------------------------------------------------*/
extern u8   TouchScreen_X_Pos_u08;                      // The name says it all
extern u8   TouchScreen_Y_Pos_u08;                      // The name says it all
extern bool TouchScreen_Press_Bol;                      // True if press
/* -------- Vbat.c --------------------------------------------------------------------*/
extern u16  Vbat_mV_Fil_u16;                            // Vbatt filtered
extern u16  Vbat_mV_Roh_u16;                            // Vbatt unfiltered

/* -------- Create Global variables ---------------------------------------------------*/
void        CAN_Debug_Send_All_Primer_Data(void);       // Sends several can messages with the sensors values

/* -------- Private define ------------------------------------------------------------*/
u8          SendZyklus = 0;

/* -------- Private variables ---------------------------------------------------------*/

/* -------- Prototyp local ------------------------------------------------------------*/


/* -------- Code ----------------------------------------------------------------------*/
void CAN_Debug_Send_All_Primer_Data(void)
{
   SendZyklus++;
   CanTxMsg CanMsg;
   
   CanMsg.StdId   = 0x600 + SendZyklus;
   CanMsg.ExtId   = 0x01;
   CanMsg.RTR     = CAN_RTR_DATA;
   CanMsg.IDE     = CAN_ID_STD;
   CanMsg.DLC     = 8;
   
   switch (SendZyklus)
   {
      case (0):
      {
         CanMsg.Data[0] = Joystick_Up____Counter_u08;
         CanMsg.Data[1] = Joystick_Down__Counter_u08;
         CanMsg.Data[2] = Joystick_Right_Counter_u08;
         CanMsg.Data[3] = Joystick_Left__Counter_u08;
         CanMsg.Data[4] = Joystick_Push__Counter_u08;
         CanMsg.Data[5] = TouchScreen_X_Pos_u08;
         CanMsg.Data[6] = TouchScreen_Y_Pos_u08;
         CanMsg.Data[7] = (BatteryStatus_Status_u08 << 1) | (TouchScreen_Press_Bol & 0x1);
         break;
      }
      case (1):
      {
         CanMsg.Data[0] = (Vbat_mV_Fil_u16                ) & 0xFF;
         CanMsg.Data[1] = (Vbat_mV_Fil_u16          >>  8 ) & 0xFF;
         CanMsg.Data[2] = (TempSensor_Value_Fil_s16       ) & 0xFF;
         CanMsg.Data[3] = (TempSensor_Value_Fil_s16 >>  8 ) & 0xFF;
         CanMsg.Data[4] = (TempSensor_Value_Spg_u16       ) & 0xFF;
         CanMsg.Data[5] = (TempSensor_Value_Spg_u16 >>  8 ) & 0xFF;
         CanMsg.Data[6] = 0;
         CanMsg.Data[7] = 0;
         break;
      }
      case (2):
      {
         CanMsg.Data[0] = (AccelSensor_X_Fil_s16       ) & 0xFF;
         CanMsg.Data[1] = (AccelSensor_X_Fil_s16 >>  8 ) & 0xFF;
         CanMsg.Data[2] = (AccelSensor_Y_Fil_s16       ) & 0xFF;
         CanMsg.Data[3] = (AccelSensor_Y_Fil_s16 >>  8 ) & 0xFF;
         CanMsg.Data[4] = (AccelSensor_Z_Fil_s16       ) & 0xFF;
         CanMsg.Data[5] = (AccelSensor_Z_Fil_s16 >>  8 ) & 0xFF;
         CanMsg.Data[6] = 0;
         CanMsg.Data[7] = 0;
         SendZyklus = -1;
         break;
      }
      default:
      {
         SendZyklus = -1;
         break;
      }
   }
   
   CAN_Transmit(CAN1, &CanMsg);
   
}
