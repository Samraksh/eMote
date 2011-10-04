#ifndef LCD_H
#define LCD_H

extern void sendLCDdataCmd(u8 par1, u8 A0_val);
extern void init_LCD();
extern void LCD_Display_array(const unsigned char *data, int size);
extern void LCD_Display_repeat(const unsigned char data, int count);
extern void LCD_Display_string(const char* string);
extern void LCD_Display_char(const char c);
extern void LCD_Fill_Blank();
extern void LCD_Reset();
#endif
