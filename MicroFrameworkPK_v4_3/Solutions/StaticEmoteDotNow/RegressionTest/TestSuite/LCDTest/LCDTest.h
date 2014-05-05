#include <tinyhal.h>
//#include "../../Log/Log.h"
//#include <i2c/netmf_i2c.h>
#include "LCD.h"

#ifndef _LCD_TEST_
#define _LCD_TEST_  1

#define MAX_EVENTS 100
class LCDTest
{
public:
	EmoteDotNow_LCD_Driver lcd;
        LCDTest ( );
        BOOL    Execute               ( );
};

#endif
