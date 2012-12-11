// LCD.h

#ifndef EMOTEDOTNOW_LCD_H
#define EMOTEDOTNOW_LCD_H

/*enum BlinkType {
	BLINK_OFF,
	BLINK_SLOW,
	BLINK_MEDIUM,
	BLINK_FAST
};

enum SymbolType
    {
        DISPLAY_CHAR_A,
        DISPLAY_CHAR_B,
        DISPLAY_CHAR_C,
        DISPLAY_CHAR_E,
        DISPLAY_CHAR_F,
        DISPLAY_CHAR_G,
        DISPLAY_CHAR_H,
        DISPLAY_CHAR_I,
        DISPLAY_CHAR_J,
        DISPLAY_CHAR_L,
        DISPLAY_CHAR_N,
        DISPLAY_CHAR_O,
        DISPLAY_CHAR_P,
        DISPLAY_CHAR_S,
        DISPLAY_CHAR_U,
        DISPLAY_CHAR_Y,
        DISPLAY_CHAR_a,
        DISPLAY_CHAR_b,
        DISPLAY_CHAR_c,
        DISPLAY_CHAR_d,
        DISPLAY_CHAR_e,
        DISPLAY_CHAR_g,
        DISPLAY_CHAR_h,
        DISPLAY_CHAR_i,
        DISPLAY_CHAR_j,
        DISPLAY_CHAR_l,
        DISPLAY_CHAR_n,
        DISPLAY_CHAR_o,
        DISPLAY_CHAR_q,
        DISPLAY_CHAR_r,
        DISPLAY_CHAR_t,
        DISPLAY_CHAR_u,
        DISPLAY_CHAR_0,
        DISPLAY_CHAR_1,
        DISPLAY_CHAR_2,
        DISPLAY_CHAR_3,
        DISPLAY_CHAR_4,
        DISPLAY_CHAR_5,
        DISPLAY_CHAR_6,
        DISPLAY_CHAR_7,
        DISPLAY_CHAR_8,
        DISPLAY_CHAR_9,
        DISPLAY_CHAR_NULL
    };
*/
const int DISPLAY_DECIMAL_POINT = 0x80;

class EmoteDotNow_LCD_Driver
{
private:
	int currentColumn1;
	int currentColumn2;
	int currentColumn3;
	int currentColumn4;
public:
	EmoteDotNow_LCD_Driver();
	bool Initialize();
	bool Uninitialize();
	bool Write(int data4, int data3, int data2, int data1);
	bool WriteN(int column, int data);
	bool WriteRawBytes(int data4, int data3, int data2, int data1);
	bool Blink(int blinkType);
};

#endif
