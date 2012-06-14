/*
 * 	Name : hal_lcd_basic.cpp
 *
 *  Author : Nived.Sivadas@Samraksh.com
 *
 *  Description : Helps configure and use lcd for debugging purposes on the evaluation board
 *
 *
 */

#include <tinyhal.h>
#include "STM32F10x_lcd_basic.h"

int globalLineNumber = 1;

void hal_lcd_init()
{
	STM3210E_LCD_Init();
	LCD_Clear(White);
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(White);

}


void hal_lcd_write(const char *string)
{
	// resetting the lcd line number when it reaches 9
	if(globalLineNumber == 9)
	{
		LCD_Clear(White);
		globalLineNumber = 0;
	}
	LCD_DisplayStringLine(LINE(globalLineNumber++),(UINT8 *) string);

}
