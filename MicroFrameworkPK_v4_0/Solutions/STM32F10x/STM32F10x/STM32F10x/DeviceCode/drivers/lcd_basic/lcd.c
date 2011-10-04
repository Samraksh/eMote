/*===========================================================================
 * DESCRIPTION:
 *
 * This file contains functions to control REva 3's onboard LCD display
 * through SPI.
 *
 *---------------------------------------------------------------------------
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *---------------------------------------------------------------------------
 * Copyright (c) Raisonance S.A.S., 2004-2009
 *==========================================================================*/

#include <string.h>
#include <stm32f10x.h>

#include "lcd.h"

//
// LCD mapping:
// The LCD ST7549T is connected with 3-pin SPI interface as follows:
// -----------------------
//   LCD pin | REva signal
// ----------+------------
//   D0      | LED0
//   D1      | LED1
//   CSB     | LED2
//   RESB    | LED3
// -----------------------
//

#ifdef _STM32x_
  // LED0
  #define LCD_PORT_SCK  GPIOC
  #define LCD_PIN_SCK   GPIO_Pin_6
  // LED1
  #define LCD_PORT_MOSI GPIOC
  #define LCD_PIN_MOSI  GPIO_Pin_7
  // LED2
  #define LCD_PORT_CS   GPIOC
  #define LCD_PIN_CS    GPIO_Pin_8
  // LED3
  #define LCD_PORT_RST  GPIOC
  #define LCD_PIN_RST   GPIO_Pin_9

  #define GPIO_BitWrite(a,b,c) GPIO_WriteBit(a,b,c)
  #define DELAY_LCD_RESET 1
#else
  // LED0
  #define LCD_PORT_SCK  GPIO0
  #define LCD_PIN_SCK   0
  // LED1
  #define LCD_PORT_MOSI GPIO0
  #define LCD_PIN_MOSI  1
  // LED2
  #define LCD_PORT_CS   GPIO1
  #define LCD_PIN_CS    4
  // LED3
  #define LCD_PORT_RST  GPIO1
  #define LCD_PIN_RST   5

  #define DELAY_LCD_RESET  1
#endif

// The following is the character code table. Each character is mapped onto
// a 6x8 matrix.
// Note that some characters are missing. Most noticeably lowercase
// characters (but uppercase can be used instead), 0x20 (space), 0x7B {,
//  0x7C |, 0x7D } and 0x7E ~
// Characters below 0x21 are not available.
//
// Lines are composed of 102 columns, but only 96 of them are visible, which
// limits ourselves to 16 characters per line.


#define ASCII_TABLE_OFFSET 0x21
#define ASCII_LETTERS_MIN_OFFSET 0x61

static const u8 AsciiMainTable[64 * 6] =
    {
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x21 !
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x22 "
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x23 #
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x24 $
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x25 %
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x26 &
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x27 '
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x28 (
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x29 )
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x2A *
    0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, // ASCII 0x2B +
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x2C ,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x00, // ASCII 0x2D -
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x2E .
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x2F /
    0x3E, 0x45, 0x49, 0x51, 0x3E, 0x00, // ASCII 0x30 0
    0x11, 0x21, 0x7F, 0x01, 0x01, 0x00, // ASCII 0x31 1
    0x21, 0x43, 0x45, 0x49, 0x31, 0x00, // ASCII 0x32 2
    0x41, 0x49, 0x49, 0x49, 0x36, 0x00, // ASCII 0x33 3
    0x0C, 0x14, 0x24, 0x7F, 0x04, 0x00, // ASCII 0x34 4
    0x79, 0x49, 0x49, 0x49, 0x06, 0x00, // ASCII 0x35 5
    0x06, 0x19, 0x29, 0x49, 0x06, 0x00, // ASCII 0x36 6
    0x43, 0x44, 0x48, 0x50, 0x60, 0x00, // ASCII 0x37 7
    0x36, 0x49, 0x49, 0x49, 0x36, 0x00, // ASCII 0x38 8
    0x30, 0x49, 0x4A, 0x4C, 0x38, 0x00, // ASCII 0x39 9
    0x00, 0x00, 0x36, 0x36, 0x00, 0x00, // ASCII 0x3A :
    0x00, 0x01, 0x37, 0x36, 0x00, 0x00, // ASCII 0x3B ;
    0x00, 0x04, 0x0A, 0x11, 0x00, 0x00, // ASCII 0x3C <
    0x00, 0x0A, 0x0A, 0x0A, 0x0A, 0x00, // ASCII 0x3D =
    0x00, 0x11, 0x0A, 0x04, 0x00, 0x00, // ASCII 0x3E >
    0x00, 0x30, 0x40, 0x4D, 0x30, 0x00, // ASCII 0x3F ?
    0x3E, 0x41, 0x5D, 0x5D, 0x3C, 0x00, // ASCII 0x40 @
    0x1F, 0x24, 0x44, 0x24, 0x1F, 0x00, // ASCII 0x41 A
    0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, // ASCII 0x42 B
    0x3E, 0x41, 0x41, 0x41, 0x41, 0x00, // ASCII 0x43 C
    0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00, // ASCII 0x44 D
    0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, // ASCII 0x45 E
    0x7F, 0x48, 0x48, 0x48, 0x40, 0x00, // ASCII 0x46 F
    0x3E, 0x41, 0x49, 0x49, 0x2E, 0x00, // ASCII 0x47 G
    0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, // ASCII 0x48 H
    0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, // ASCII 0x49 I
    0x06, 0x01, 0x41, 0x7E, 0x40, 0x00, // ASCII 0x4A J
    0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, // ASCII 0x4B K
    0x7F, 0x01, 0x01, 0x01, 0x01, 0x00, // ASCII 0x4C L
    0x7F, 0x20, 0x10, 0x20, 0x7F, 0x00, // ASCII 0x4D M
    0x7F, 0x10, 0x08, 0x04, 0x7F, 0x00, // ASCII 0x4E N
    0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, // ASCII 0x4F O
    0x7F, 0x48, 0x48, 0x48, 0x30, 0x00, // ASCII 0x50 P
    0x3E, 0x41, 0x43, 0x47, 0x3D, 0x00, // ASCII 0x51 Q
    0x7F, 0x48, 0x48, 0x4C, 0x33, 0x00, // ASCII 0x52 R
    0x31, 0x49, 0x49, 0x49, 0x06, 0x00, // ASCII 0x53 S
    0x40, 0x40, 0x7F, 0x40, 0x40, 0x00, // ASCII 0x54 T
    0x7E, 0x01, 0x01, 0x01, 0x7E, 0x00, // ASCII 0x55 U
    0x7C, 0x02, 0x01, 0x02, 0x7C, 0x00, // ASCII 0x56 V
    0x7F, 0x02, 0x04, 0x02, 0x7F, 0x00, // ASCII 0x57 W
    0x63, 0x14, 0x08, 0x14, 0x63, 0x00, // ASCII 0x58 X
    0x60, 0x10, 0x0F, 0x10, 0x60, 0x00, // ASCII 0x59 Y
    0x43, 0x45, 0x49, 0x51, 0x61, 0x00, // ASCII 0x5A Z
    0x00, 0x00, 0x7F, 0x41, 0x00, 0x00, // ASCII 0x5B [
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, // ASCII 0x5C \
    0x00, 0x41, 0x7F, 0x00, 0x00, 0x00, // ASCII 0x5D ]
    0x10, 0x20, 0x40, 0x20, 0x10, 0x00, // ASCII 0x5E ^
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, // ASCII 0x5F _
    0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA  // ASCII 0x60 `
    };

void delay ( int n )
    {
    volatile long int i;

    while (n--)
        {
        for ( i = 0 ; i < 1000 ; i++ )
            {;}
        }
    }

void sendLCDdataCmd ( u8 par1, u8 A0_val )
    {
    unsigned short val, i;

    GPIO_BitWrite ( LCD_PORT_CS, LCD_PIN_CS, 0 );       //clk at 0

    if(A0_val)
        val = par1 | 0x100;
    else
        val = par1;

    for ( i = 0x100; i != 0; i >>= 1 )                  //send 9 bits
        {
        GPIO_BitWrite ( LCD_PORT_SCK, LCD_PIN_SCK, 0 ); //clk at 0
        GPIO_BitWrite ( LCD_PORT_MOSI, LCD_PIN_MOSI,
                ( val & i ) ? 1 : 0 );                  //data on MOSI
        GPIO_BitWrite ( LCD_PORT_SCK, LCD_PIN_SCK, 1 ); //rising (effective) edge
        }

    GPIO_BitWrite ( LCD_PORT_MOSI, LCD_PIN_MOSI, 1 );   //keep data to 1
    GPIO_BitWrite ( LCD_PORT_CS, LCD_PIN_CS, 1 );       //clk at 0
    }

void init_LCD()
    {
    unsigned char lcm_adj=15;

#ifdef _STM32x_
    // Configure GPIOs on STM32
    GPIO_InitTypeDef struct_init;
    struct_init.GPIO_Pin   = LCD_PIN_SCK | LCD_PIN_MOSI | LCD_PIN_RST | LCD_PIN_CS ;
    struct_init.GPIO_Speed = GPIO_Speed_50MHz ;
    struct_init.GPIO_Mode  = GPIO_Mode_Out_PP ;
    GPIO_Init(GPIOC, &struct_init);
    //GPIO_Write( GPIOC, LCD_PIN_SCK | LCD_PIN_MOSI | LCD_PIN_RST | LCD_PIN_CS);
#else
    // Configure GPIOs on STR71x
    GPIO_Config ( LCD_PORT_MOSI, 0x03, GPIO_OUT_PP);    //take control of the signals MOSI-MISO-SCLK
    GPIO_Config ( LCD_PORT_CS, 0x30, GPIO_OUT_PP);      //take control of the signals CS-RST
#endif

    //Reset the LCD controller
    GPIO_BitWrite ( LCD_PORT_RST, LCD_PIN_RST, 0 );     //rst at 0
    delay (DELAY_LCD_RESET);
    GPIO_BitWrite ( LCD_PORT_RST, LCD_PIN_RST, 1 );     //rst at 1

#define H1H0 0x20 //001 MX MY PD H1 H0

    sendLCDdataCmd(H1H0+3,0);       //H1H0 = 11b
    sendLCDdataCmd(0x0f, 0);        //Set Frequency=65Hz
    sendLCDdataCmd(0x95, 0);        //boost efficiency level=3; booster*3

    sendLCDdataCmd(H1H0+2,0);       //H1H0 = 10b
    sendLCDdataCmd(0x04, 0);        //COM0~66 [PS=0-->MUX=68]
    sendLCDdataCmd(0x12, 0);        //start from common=16
    sendLCDdataCmd(0x09, 0);        //Partial screen size [WS=1-->MUX=1:33]

    sendLCDdataCmd(H1H0+1, 0);      //H1H0 = 01b
    sendLCDdataCmd(0x10+2,0);       //system bias 9 = 1:65/1:68
    sendLCDdataCmd(0x80+lcm_adj,0); //Set VOP value
    sendLCDdataCmd(0x40+51, 0);     //Set start line=51

    sendLCDdataCmd(H1H0+0, 0);      //H1H0 = 00b
    sendLCDdataCmd(0x05, 0);        //Set Vlcd Range a2=6.75V
    sendLCDdataCmd(0x0c, 0);        //Display control [D=1; E=0]

    sendLCDdataCmd(0x80+3, 0);      // Set X address of RAM ([0:101])
    sendLCDdataCmd(0x40+0, 0);      // Set Y address of RAM ([0:9])
    }

void LCD_Reset()
    {
    sendLCDdataCmd(H1H0+0,0);       //H1H0 = 00b
    sendLCDdataCmd(0x80+3, 0);      // Set X address of RAM ([0:101])
    sendLCDdataCmd(0x40+0, 0);      // Set Y address of RAM ([0:9])
    }

void LCD_Display_array(const unsigned char *data, int size)
    {
    int i;

    if(!data)
        return;

    for( i = 0; i < size ; i++)
        sendLCDdataCmd(data[i],1);
    }

void LCD_Display_repeat(const unsigned char data, int count)
    {
    int i;

    for( i = 0; i < count ; i++ )
        sendLCDdataCmd( data, 1 );
    }

void LCD_Display_char(const char c)
    {
#define IS_IN_RANGE(val, start, size) (((val)>=(start)) && ((val)<((start)+(size))))
#define DISPCHAR(val, offset, table) LCD_Display_array( (table)+(((val)-(offset))*6), 6 )

    if( c == ' ') // It's a space
        LCD_Display_repeat( 0x00, 6);

    else if(IS_IN_RANGE(c, ASCII_TABLE_OFFSET, 64)) // it's in the main table
        DISPCHAR(c, ASCII_TABLE_OFFSET, AsciiMainTable);

    else if(IS_IN_RANGE(c, ASCII_LETTERS_MIN_OFFSET, 26)) // a downcase letter
        DISPCHAR(c, ASCII_TABLE_OFFSET+0x20, AsciiMainTable);

    else // if not found, display horizontal lines
        LCD_Display_repeat( 0x55, 6);
   }

void LCD_Display_string( const char *string)
    {
    char* pos;
    char screen_buffer[17*3+1];
    memset(screen_buffer, ' ', 17*3);
    screen_buffer[17*3] = 0 ;

    if(strlen(string)>16*2) // 3 lines or more (we keep only 3 lines)
        {
        memcpy(screen_buffer, string+(16*2), strlen(string)-16*2);
        memcpy(screen_buffer+17, string+16, 16);
        memcpy(screen_buffer+17*2, string, 16);
        screen_buffer[strlen(string)+2] = ' ';
        }
    else
        {
        if(strlen(string)>16) // 2 lines
            {
            memcpy(screen_buffer+17, string+16, strlen(string)-16);
            memcpy(screen_buffer+17*2, string, 16);
            screen_buffer[strlen(string)+1] = ' ';
            }
        else // 1 line
            {
            memcpy(screen_buffer+17*2, string, strlen(string));
            }
        }

    for( pos=screen_buffer ; (*pos) ; pos++)
        LCD_Display_char(*pos);
    }

void LCD_Fill_Blank()
    {
    int i;

    for ( i = 0 ; i < 102*4 ; i++ )
        sendLCDdataCmd(0x00,1);
    }

void LCD_Fill_Lines()
    {
    LCD_Display_repeat( 0x55, 102*4);
    }

void LCD_Clear()
    {
    init_LCD();
    LCD_Fill_Blank();
    init_LCD();
    }
