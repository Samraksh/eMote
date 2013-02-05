// LCD_PCF85162.h

#ifndef LCD_PCF85162_H
#define LCD_PCF85162_H

#define DISPLAY_DECIMAL_POINT 0x80

enum BlinkTypeDriver {
	BLINK_OFF,
	BLINK_SLOW,
	BLINK_MEDIUM,
	BLINK_FAST
};

enum SymbolTypeDriver
    {
        DISPLAY_CHAR_NULL,
        DISPLAY_CHAR_A,
        DISPLAY_CHAR_B,
        DISPLAY_CHAR_C,
        DISPLAY_CHAR_D,
        DISPLAY_CHAR_E,
        DISPLAY_CHAR_F,
        DISPLAY_CHAR_G,
        DISPLAY_CHAR_H,
        DISPLAY_CHAR_I,
        DISPLAY_CHAR_J,
        DISPLAY_CHAR_K,
        DISPLAY_CHAR_L,
        DISPLAY_CHAR_M,
        DISPLAY_CHAR_N,
        DISPLAY_CHAR_O,
        DISPLAY_CHAR_P,
        DISPLAY_CHAR_Q,
        DISPLAY_CHAR_R,
        DISPLAY_CHAR_S,
        DISPLAY_CHAR_T,
        DISPLAY_CHAR_U,
        DISPLAY_CHAR_V,
        DISPLAY_CHAR_W,
        DISPLAY_CHAR_X,
        DISPLAY_CHAR_Y,
        DISPLAY_CHAR_Z,
        DISPLAY_CHAR_a,
        DISPLAY_CHAR_b,
        DISPLAY_CHAR_c,
        DISPLAY_CHAR_d,
        DISPLAY_CHAR_e,
        DISPLAY_CHAR_f,
        DISPLAY_CHAR_g,
        DISPLAY_CHAR_h,
        DISPLAY_CHAR_i,
        DISPLAY_CHAR_j,
        DISPLAY_CHAR_k,
        DISPLAY_CHAR_l,
        DISPLAY_CHAR_m,
        DISPLAY_CHAR_n,
        DISPLAY_CHAR_o,
        DISPLAY_CHAR_p,
        DISPLAY_CHAR_q,
        DISPLAY_CHAR_r,
        DISPLAY_CHAR_s,
        DISPLAY_CHAR_t,
        DISPLAY_CHAR_u,
        DISPLAY_CHAR_v,
        DISPLAY_CHAR_w,
        DISPLAY_CHAR_x,
        DISPLAY_CHAR_y,
        DISPLAY_CHAR_z,
        DISPLAY_CHAR_0,
        DISPLAY_CHAR_1,
        DISPLAY_CHAR_2,
        DISPLAY_CHAR_3,
        DISPLAY_CHAR_4,
        DISPLAY_CHAR_5,
        DISPLAY_CHAR_6,
        DISPLAY_CHAR_7,
        DISPLAY_CHAR_8,
        DISPLAY_CHAR_9
    };

class LCD_PCF85162_Driver
{
private:
	int currentColumn1;
	int currentColumn2;
	int currentColumn3;
	int currentColumn4;

	bool DP1;
	bool DP2;
	bool DP3;
	bool DP4;
public:
	LCD_PCF85162_Driver();
	bool Initialize();
	bool Uninitialize();
	bool Write(int data4, int data3, int data2, int data1);
	bool SetDP(bool dp4, bool dp3, bool dp2, bool dp1);
	bool WriteN(int column, int data);
	bool WriteRawBytes(int data4, int data3, int data2, int data1);
	bool Blink(int blinkType);
};

extern LCD_PCF85162_Driver g_LCD_PCF85162_Driver;

// Nived.Sivadas - Replacing all infinite whiles with timeouts
// We will take failure to write one byte over complete system crash
#define INIT_I2C_STATE_CHECK()				UINT16 poll_counter;

#define DID_I2C_STATE_CHANGE(x, y)       poll_counter = 0;				\
										 do{ 							\
											if(poll_counter == 0xff)    \
											{  								\
												hal_printf("I2C State change failed");  \
												return false;				\
											} 								\
											poll_counter++; 				\
										  }while(!I2C_CheckEvent(x, y));							\

#endif
