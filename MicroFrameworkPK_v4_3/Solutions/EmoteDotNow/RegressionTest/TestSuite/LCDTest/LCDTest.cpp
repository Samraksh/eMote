#include "LCDTest.h"
#include <DeviceCode\LCD_PCF85162_HAL\LCD_PCF85162.h>
#include <i2c/stm32f10x_i2c.h>
#include <rcc/stm32f10x_rcc.h>
#include <gpio/stm32f10x_gpio.h>


BOOL LCDTest::Execute()
{
	int i;
	lcd.Write(DISPLAY_CHAR_A,DISPLAY_CHAR_B,DISPLAY_CHAR_C,DISPLAY_CHAR_E);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Blink(BLINK_SLOW);
	lcd.Write(DISPLAY_CHAR_F,DISPLAY_CHAR_G,DISPLAY_CHAR_H,DISPLAY_CHAR_I);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_J,DISPLAY_CHAR_L,DISPLAY_CHAR_N,DISPLAY_CHAR_O);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Blink(BLINK_MEDIUM);
	lcd.Write(DISPLAY_CHAR_P,DISPLAY_CHAR_S,DISPLAY_CHAR_U,DISPLAY_CHAR_Y);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_a,DISPLAY_CHAR_b,DISPLAY_CHAR_c,DISPLAY_CHAR_d);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Blink(BLINK_FAST);
	lcd.Write(DISPLAY_CHAR_e,DISPLAY_CHAR_g,DISPLAY_CHAR_h,DISPLAY_CHAR_i);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_j,DISPLAY_CHAR_l,DISPLAY_CHAR_n,DISPLAY_CHAR_o);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Blink(BLINK_OFF);
	lcd.Write(DISPLAY_CHAR_q,DISPLAY_CHAR_r,DISPLAY_CHAR_t,DISPLAY_CHAR_u);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_0,DISPLAY_CHAR_1,DISPLAY_CHAR_2,DISPLAY_CHAR_3);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_4,DISPLAY_CHAR_5,DISPLAY_CHAR_6,DISPLAY_CHAR_7);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_8,DISPLAY_CHAR_9,DISPLAY_CHAR_0,DISPLAY_CHAR_NULL);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteRawBytes(0x11,0x22,0x33,0xff);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(1,DISPLAY_CHAR_A);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(2,DISPLAY_CHAR_A);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(3,DISPLAY_CHAR_A);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(4,DISPLAY_CHAR_A);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_0,DISPLAY_CHAR_1,DISPLAY_CHAR_2,DISPLAY_CHAR_3);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_0 | DISPLAY_DECIMAL_POINT,DISPLAY_CHAR_1 | DISPLAY_DECIMAL_POINT,DISPLAY_CHAR_2 | DISPLAY_DECIMAL_POINT,DISPLAY_CHAR_3 | DISPLAY_DECIMAL_POINT);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(1,DISPLAY_CHAR_o);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(2,DISPLAY_CHAR_o);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(3,DISPLAY_CHAR_o);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.WriteN(4,DISPLAY_CHAR_o);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Uninitialize();
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Initialize();
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_8 | DISPLAY_DECIMAL_POINT, DISPLAY_CHAR_8 | DISPLAY_DECIMAL_POINT, DISPLAY_CHAR_8 | DISPLAY_DECIMAL_POINT, DISPLAY_CHAR_8 | DISPLAY_DECIMAL_POINT);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_0,DISPLAY_CHAR_1,DISPLAY_CHAR_2,DISPLAY_CHAR_3);
	for (i=0; i<2000; i++)
		HAL_Time_Sleep_MicroSeconds(1000000);
	lcd.Write(DISPLAY_CHAR_4,DISPLAY_CHAR_5,DISPLAY_CHAR_6,DISPLAY_CHAR_7);
}

LCDTest::LCDTest( ){
	lcd.Initialize();		
}
