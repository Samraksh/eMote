/********************* (C) COPYRIGHT 2007 RAISONANCE S.A.S. ****************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* -------- Includes ------------------------------------------------------------------*/
#include    "stm32f10x_lcd_basic.h"
#include    "_Background_image/bmp_background_image.h"

/* -------- Used Global variables -----------------------------------------------------*/
/* -------- _Font/Font.c --------------------------------------------------------------*/
extern void Font_Set_Font_u8( u8 Font );                // to change the font
#define     Font_7x8                      1             // available fonts
#define     Font_7x11                     2             // available fonts
#define     Font_10x16                    3             // available fonts
#define     Font_16x16                    4             // available fonts
#define     Font_Symbols                  5             // available fonts
#define     Font_BigNumber                6             // available fonts
#define     Font_Primer                   0             // available fonts
extern u8   Font_Width_u08;                             // Font width of the selected font
extern u8   Font_Height_u08;                            // Font height of the selected font
extern u8   Font_Starts_u08;                            // first sign of the selected font
extern u8   Font_Ends_u08;                              // last character of the selected font
extern u8   Font_Scale_u08;                             // Scale of the selected font
extern bool Font_Transparent_Bol;                       // if true, the background image is not overwritten. Background image is "Pictures/bmp_background_image.h generate" with "Create_background_image.xls"
extern u16  Font_Text_Color_u16;                        // Text color of the selected font
extern u16  Font_BGnd_Color_u16;                        // Background color of the selected font
extern u8*  Font_Character_Table_P08;                   // pointer on the current font

/* -------- Private define ------------------------------------------------------------*/

/* -------- Private variables ---------------------------------------------------------*/
vu32        TimingDelay_u32;

/* -------- Prototyp local ------------------------------------------------------------*/
//void        Task_delay_ms(volatile uint32_t nTime);     // ms delay function
void        LCD_WriteDataCMD( u8 Data );
void        LCD_WriteDataLCD( u8 Data );
static void LCD_CheckLCDStatus( void );
static void LCD_DrawChar( u8 x, u8 y, const u8* Char );
u32         LCD_ReadLCDData( void );
static void LCD_ST7732S_Controller_init(void);
void        LCD_SetRect_For_Cmd( s16 x, s16 y, s16 width, s16 height );
void        LCD_background_image( void );

/* -------- Code ----------------------------------------------------------------------*/
/*void Task_delay_ms(volatile uint32_t nTime)
{
   TimingDelay_u32 = nTime;
   while(TimingDelay_u32 != 0);
   //uint32_t i; 
   //for(i = 0; i < nTime; ++i);
}*/
void LCD_WriteDataCMD( u8 Data )
{
   LCD_CtrlLines_RS_Low;
   LCD_CtrlLines_RD_High;
   LCD_CtrlLines_CS_Low;
   LCD_CtrlLines_WR_Low;
   LCD_Write_Data(Data);
   LCD_CtrlLines_WR_High;
}
void LCD_WriteDataLCD( u8 Data )
{
   LCD_CtrlLines_RS_High;
   LCD_CtrlLines_RD_High;
   LCD_CtrlLines_CS_Low;
   LCD_CtrlLines_WR_Low;
   LCD_Write_Data(Data);
   LCD_CtrlLines_WR_High;
}
static void LCD_CheckLCDStatus( void )
{
   LCD_WriteDataCMD( ST7732_RDDID );
   LCD_DataLinesInput();/* Configure Data lines as Input */
   LCD_CtrlLines_RS_Low;/* Start the LCD send data sequence */
   LCD_CtrlLines_RD_Low;
   LCD_CtrlLines_CS_Low;
   LCD_CtrlLines_WR_High;
   LCD_CtrlLines_RD_High;/* Read data to the LCD */
   LCD_CtrlLines_RD_Low;
   LCD_Read__Data(); // ID1
   LCD_CtrlLines_RD_High;
   LCD_CtrlLines_RD_Low;
   LCD_Read__Data(); // ID2
   LCD_CtrlLines_RD_High;
   LCD_CtrlLines_RD_Low;
   LCD_Read__Data(); // ID3
   LCD_DataLinesOutput();
   LCD_ReadLCDData();
   LCD_ReadLCDData();
   LCD_ReadLCDData();
}
static void LCD_DrawChar( u8 x, u8 y, const u8* Char )
{
   int width0, height0, lin0, lin1, n, i, i0, i1, j, k1, k2, xx, yy;
   int x0     = MAX(x, 0);
   int y0     = MAX(y, 0);
   width0     = MIN(Font_Width_u08  * Font_Scale_u08, SCREEN_WIDTH  - x0);
   height0    = MIN(Font_Height_u08 * Font_Scale_u08, SCREEN_HEIGHT - y0);
   
   if ( y0 > y ) height0 -= (y0-y);
   if ( x0 > x ) width0  -= (x0-x);
   
   if ( (width0 <= 0) || (height0 <= 0) ) return;
   
   LCD_SetRect_For_Cmd( x0, y0, width0, height0);  // Select the area for LCD output.
   xx = x0;
   yy = y0;
   
   LCD_WriteDataCMD( ST7732_RAMWR );
   
   i0   = (x0 - x) / Font_Scale_u08;
   i1   = width0 / Font_Scale_u08;
   lin0 = (y0 - y) ;
   lin1 = height0 + lin0;
   x0   = i1 - i0;
   for( i = i0; i < i1; i++ )
   {
      for( k1 = 0; k1 < Font_Scale_u08; k1++ )
      {
         if ( Font_Height_u08 <= 8 )
         {
            n = 0;
            for( j = 0x80; j; j >>= 1 ) // 8
            {
               for( k2 = 0; k2 < Font_Scale_u08; k2++ , n++)
               {
                  if ( ( n < lin0 ) || (n>=lin1) ) continue;
                  LCD_WriteDataLCD( ( Char[i] & j ) ? ( Font_Text_Color_u16 & 255 ) : ( ( Font_Transparent_Bol ) ? ( background_image[yy+n][xx] & 255 ) : ( Font_BGnd_Color_u16 & 255 ) ) );
                  LCD_WriteDataLCD( ( Char[i] & j ) ? ( Font_Text_Color_u16 >> 8  ) : ( ( Font_Transparent_Bol ) ? ( background_image[yy+n][xx] >> 8  ) : ( Font_BGnd_Color_u16 >> 8  ) ) );
               }
            }
         }
         else
         {
            n = 0;
            for( j = 0x80; j; j >>= 1 ) // 8
            {
               for( k2 = 0; k2 < Font_Scale_u08; k2++ , n++)
               {
                  if ( ( n < lin0 ) || (n>=lin1) ) continue;
                  LCD_WriteDataLCD( ( Char[ 2*i   ] & j ) ? ( Font_Text_Color_u16 & 255 ) : ( ( Font_Transparent_Bol ) ? ( background_image[yy+n][xx] & 255 ) : ( Font_BGnd_Color_u16 >> 8  ) ) );
                  LCD_WriteDataLCD( ( Char[ 2*i   ] & j ) ? ( Font_Text_Color_u16 >> 8  ) : ( ( Font_Transparent_Bol ) ? ( background_image[yy+n][xx] >> 8  ) : ( Font_BGnd_Color_u16 >> 8  ) ) );
               }
            }
            for( j = 0x80; j; j >>= 1 ) // 8
            {
               for( k2 = 0; k2 < Font_Scale_u08; k2++ , n++)
               {
                  if ( ( n < lin0 ) || (n>=lin1) ) continue;
                  LCD_WriteDataLCD( ( Char[(2*i)+1] & j ) ? ( Font_Text_Color_u16 & 255 ) : ( ( Font_Transparent_Bol ) ? ( background_image[yy+n][xx] & 255 ) : ( Font_BGnd_Color_u16 >> 8  ) ) );
                  LCD_WriteDataLCD( ( Char[(2*i)+1] & j ) ? ( Font_Text_Color_u16 >> 8  ) : ( ( Font_Transparent_Bol ) ? ( background_image[yy+n][xx] >> 8  ) : ( Font_BGnd_Color_u16 >> 8  ) ) );
               }
            }
         }
         xx++;
      }
   }
}
static void LCD_DisplayRotate( u8 Pos )
{
   LCD_WriteDataCMD( ST7732_MADCTR );
   switch( Pos )
   {
      case  3 : LCD_WriteDataLCD( 0x50 ); break;
      case  6 : LCD_WriteDataLCD( 0xF0 ); break;
      case  9 : LCD_WriteDataLCD( 0x90 ); break;
      default : LCD_WriteDataLCD( 0x30 ); break;
   }
}
void LCD_ST7732S_Controller_init( void )
{
   LCD_CtrlLines_RST_High; /** Apply hardware reset **/
   LCD_CtrlLines_RST_Low;
   Task_delay_ms( 50 );
   
   LCD_CtrlLines_RST_High;
   Task_delay_ms( 10 );
   
   LCD_DataLinesOutput(); //default mode is output
   
   LCD_CheckLCDStatus();
   
   Task_delay_ms(100);
   LCD_CtrlLines_RST_Low;
   Task_delay_ms(100);
   LCD_CtrlLines_RST_High;
   Task_delay_ms(100);
   
   LCD_WriteDataCMD(ST7732_SWRESET); //Software Reset
   Task_delay_ms(180); //DELAY 150MS
   
   LCD_WriteDataCMD(ST7732_SLPIN); //sleep IN
   Task_delay_ms(100);
   LCD_WriteDataCMD(ST7732_SLPOUT); //sleep out
   Task_delay_ms(150);
   
   LCD_WriteDataCMD(ST7732_FRMCTR1); // frame
   LCD_WriteDataLCD(0x06);
   LCD_WriteDataLCD(0x03);
   LCD_WriteDataLCD(0x02);
   
   LCD_WriteDataCMD(ST7732_INVCTR); // 0xb4
   LCD_WriteDataLCD(0x03);
   
   LCD_WriteDataCMD(ST7732_DISSET5); // frame 0xB6
   LCD_WriteDataLCD(0x02);
   LCD_WriteDataLCD(0x0e);
   
   LCD_WriteDataCMD(ST7732_DISPCTRL); // 0xF5
   LCD_WriteDataLCD(0x1a);
   
   LCD_WriteDataCMD(0xc0); //Power Control 1 ( gvdd vci1)
   LCD_WriteDataLCD(0x02);
   LCD_WriteDataLCD(0x00);
   
   LCD_WriteDataCMD(0xc1); //Power Control 2( avdd vcl vgh vgl)
   LCD_WriteDataLCD(0x05);
   
   LCD_WriteDataCMD(0xc2); //Power Control 3 ( in normal)
   LCD_WriteDataLCD(0x02);
   LCD_WriteDataLCD(0x02);
   
   LCD_WriteDataCMD(0xc3); //Power Control 4 ( in Idle mode)
   LCD_WriteDataLCD(0x01);
   LCD_WriteDataLCD(0x02);
   
   LCD_WriteDataCMD(0xc4); //Power Control 5 ( in partial mode)
   LCD_WriteDataLCD(0x01);
   LCD_WriteDataLCD(0x02);
   
   LCD_WriteDataCMD(0xc5); //Vcom Control ( vcomh,voml)
   LCD_WriteDataLCD(0x47);
   LCD_WriteDataLCD(0x3a);
   
   LCD_WriteDataCMD(ST7732_OSCADJ); // 0xF2 intern OSC 80Hz
   LCD_WriteDataLCD(0x02);
   
   LCD_WriteDataCMD(ST7732_DEFADJ); //0xF6
   LCD_WriteDataLCD(0x4c);
   
   LCD_WriteDataCMD(0xf8); //cmd 0xf8,dat 0x06; LOAD
   LCD_WriteDataLCD(0x06); //SLEEP OUT LOAD DEFAULT
   
   /****************gamma adjust **********************/
   LCD_WriteDataCMD(0xe0); //gamma
   LCD_WriteDataLCD(0x06);
   LCD_WriteDataLCD(0x1c);
   LCD_WriteDataLCD(0x1f);
   LCD_WriteDataLCD(0x1f);
   LCD_WriteDataLCD(0x18);
   LCD_WriteDataLCD(0x13);
   LCD_WriteDataLCD(0x06);
   LCD_WriteDataLCD(0x03);
   LCD_WriteDataLCD(0x03);
   LCD_WriteDataLCD(0x04);
   LCD_WriteDataLCD(0x07);
   LCD_WriteDataLCD(0x07);
   LCD_WriteDataLCD(0x00);
   
   LCD_WriteDataCMD(0xe1); //gamma
   LCD_WriteDataLCD(0x0a);
   LCD_WriteDataLCD(0x14);
   LCD_WriteDataLCD(0x1b);
   LCD_WriteDataLCD(0x18);
   LCD_WriteDataLCD(0x12);
   LCD_WriteDataLCD(0x0e);
   LCD_WriteDataLCD(0x02);
   LCD_WriteDataLCD(0x01);
   LCD_WriteDataLCD(0x00);
   LCD_WriteDataLCD(0x01);
   LCD_WriteDataLCD(0x08);
   LCD_WriteDataLCD(0x07);
   LCD_WriteDataLCD(0x00);
   
   LCD_WriteDataCMD(ST7732_MADCTR); // Memory Access Control 0x36
   LCD_WriteDataLCD(V12_MADCTRVAL);
   
   LCD_WriteDataCMD(0x2a); //Column Range
   LCD_WriteDataLCD(0x00);
   LCD_WriteDataLCD(0x00);
   LCD_WriteDataLCD(0x00);
   LCD_WriteDataLCD(0x7F);
   
   LCD_WriteDataCMD(0x2b); //page Range
   LCD_WriteDataLCD(0x00);
   LCD_WriteDataLCD(0x00);
   LCD_WriteDataLCD(0x00);
   LCD_WriteDataLCD(0x7F); // configured as 128x128
   
   LCD_WriteDataCMD(ST7732_COLMOD); // Color mode = 65k 0x3A
   LCD_WriteDataLCD(0x55);
   
   LCD_WriteDataCMD(ST7732_TEON); // 0x35
   LCD_WriteDataLCD(0x00);
   
   LCD_WriteDataCMD(0x29); //display on
   Task_delay_ms(20);
   
   //LCD_background_image();
}

void LCD_Init( void )
{
   LCD_CtrlLines_RS_High;         /* Configure control lines signals as output mode */
   LCD_CtrlLines_RD_High;
   LCD_CtrlLines_CS_High;
   LCD_CtrlLines_WR_High;
   LCD_CtrlLines_RST_Low;
   LCD_ST7732S_Controller_init(); /* Configuration sequence */
}
void LCD_Draw_Surface( u8 x, u8 y, u8 width, u8 height, u16 color )
{
   u8 Line, Column;
   LCD_SetRect_For_Cmd( x, y, width, height );  /* Select LCD screen area. */
   LCD_WriteDataCMD( ST7732_RAMWR );            /* Send LCD RAM write command. */
   for( Line = 0; Line < width; Line++ )        /* Fill selected LCD screen area with provided color. */
   {
      for( Column = 0; Column < height; Column++ )
      {
         LCD_WriteDataLCD(   color        & 0xff );
         LCD_WriteDataLCD( ( color >> 8 ) & 0xff );
      }
   }
}
void LCD_background_image()
{
   u8 Line, Column;
   LCD_SetRect_For_Cmd( 0, 0, 128,  160 ); /* Select LCD screen area. */
   LCD_WriteDataCMD( ST7732_RAMWR );       /* Send LCD RAM write command. */
   for( Line = 0; Line < 128; Line++ )     /* Fill selected LCD screen area with provided color. */
   {
      for( Column = 0; Column < 160; Column++ )
      {
         LCD_WriteDataLCD(   background_image[Column][Line]        & 0xff );
         LCD_WriteDataLCD( ( background_image[Column][Line] >> 8 ) & 0xff );
      }
   }
}
u32  LCD_ReadLCDData( void )
{
   u32 LCDData = 0;
   LCD_DataLinesInput();   /* Configure Data lines as Input */
   LCD_CtrlLines_RS_High;  /* Start the LCD send data sequence */
   LCD_CtrlLines_WR_High;
   LCD_CtrlLines_CS_Low;
   LCD_CtrlLines_RD_Low;
   LCDData = LCD_Read__Data();
   LCD_CtrlLines_RD_High;
   LCD_DataLinesOutput();
   return LCDData;
}
void LCD_Draw_Rectangle( u8 x, u8 y, u8 width, u8 height, u16 color )
{
   LCD_Draw_Surface( x            , y             , width, 1     , color );// Draw horizontal sides.
   LCD_Draw_Surface( x            , y + height - 1, width, 1     , color );
   LCD_Draw_Surface( x            , y             , 1    , height, color );// Draw vertical sides.
   LCD_Draw_Surface( x + width - 1, y             , 1    , height, color );
}
void LCD_DrawPixel( u8 XPos, u8 YPos, u16 Color )
{
   LCD_SetRect_For_Cmd( XPos, YPos, 1, 1 ); // Select LCD screen area.
   LCD_WriteDataCMD( ST7732_RAMWR );        // Send LCD RAM write command.
   LCD_WriteDataLCD( Color      );          // Draw pixel.
   LCD_WriteDataLCD( Color >> 8 );
}
void LCD_SetRect_For_Cmd( s16 x, s16 y, s16 width, s16 height )
{
   LCD_WriteDataCMD( ST7732_CASET   );
   LCD_WriteDataLCD( 0              );
   LCD_WriteDataLCD( y              );
   LCD_WriteDataLCD( 0              );
   LCD_WriteDataLCD( y + height - 1 );
   LCD_WriteDataCMD( ST7732_RASET   );
   LCD_WriteDataLCD( 0              );
   LCD_WriteDataLCD( x              );
   LCD_WriteDataLCD( 0              );
   LCD_WriteDataLCD( x + width - 1  );
}
void LCD_Write_Text( u8 x, u8 y, const char* Text, u8 Text_Len )
{
   u8 ref_x = x, i , c;
   Text_Len = MIN(Text_Len,18);                     /* Up to 18 characters */
   for ( i = 0 ; i < Text_Len ; i++ )               /* Display each character on LCD */
   {
      c = *Text;
      if ( c ){ Text++; }                           /* Point to the next character */
      else    { c = ' '; }                          /* fill with space when len exceeds strlen(ptr) */
      c = MIN(c,Font_Ends_u08);
      c = c - Font_Starts_u08;
      LCD_DrawChar( ref_x, y, (u8*)&Font_Character_Table_P08[ (c) * (Font_Width_u08*(((Font_Height_u08-1)/8)+1)) ]  );/* Display one character on LCD */
      ref_x+= (Font_Width_u08 * Font_Scale_u08);    /* Increment the column position by Font_Width_u08 */
   }
}

void LCD_Draw_Color_Image( u8 x , u8 y , u8 width, u8 height, const u16* imageptr )
{
   int xx,yy,i;
   
   LCD_SetRect_For_Cmd( x, y, width, height );  // Select screen area to access.
   LCD_WriteDataCMD(ST7732_RAMWR);
   i = 0;
   if (Font_Transparent_Bol)
   {
      for( xx = x; xx < ( x + width  );xx++ )
      {
         for(yy = y; yy < ( y + height ); yy++ )
         {
            LCD_WriteDataLCD( ( imageptr[ i ] == Font_BGnd_Color_u16 ) ? ( background_image[yy][xx]       ) : ( imageptr[ i ]       ) & 0xff );
            LCD_WriteDataLCD( ( imageptr[ i ] == Font_BGnd_Color_u16 ) ? ( background_image[yy][xx]  >> 8 ) : ( imageptr[ i ]  >> 8 ) & 0xff );
            i++;
         }
      }
   }
   else
   {
      for( i = 0; i < ( width * height ); i++ )
      {
         LCD_WriteDataLCD(   imageptr[ i ]        & 0xff );
         LCD_WriteDataLCD( ( imageptr[ i ] >> 8 ) & 0xff );
      }
   }
}
void LCD_Draw_BW_Image( u8 x , u8 y , u8 width, u8 height, const u8* imageptr )
{
   int xx,yy,i;
   u8 BitSel = 0x80;
   
   LCD_SetRect_For_Cmd( x, y, width, height );  // Select screen area to access.
   LCD_WriteDataCMD(ST7732_RAMWR);
   i = 0;
   if (Font_Transparent_Bol)
   {
      for( xx = x; xx < ( x + width  ); xx++ )
      {
         for(yy = y; yy < ( y + height ); yy++ )
         {
            if ( imageptr[ i ] & BitSel )
            {
               LCD_WriteDataLCD(   Font_Text_Color_u16        & 0xff );
               LCD_WriteDataLCD( ( Font_Text_Color_u16 >> 8 ) & 0xff );
            }
            else
            {
               LCD_WriteDataLCD(   background_image[yy][xx]         & 0xff );
               LCD_WriteDataLCD( ( background_image[yy][xx]  >> 8 ) & 0xff );
            }
            BitSel = BitSel >> 1;
            if ( BitSel == 0 ){ i++; BitSel = 0x80; }
         }
      }
   }
   else
   {
      for( xx = x; xx < ( x + width  ); xx++ )
      {
         for(yy = y; yy < ( y + height ); yy++ )
         {
            if ( imageptr[ i ] & BitSel )
            {
               LCD_WriteDataLCD(   Font_Text_Color_u16        & 0xff );
               LCD_WriteDataLCD( ( Font_Text_Color_u16 >> 8 ) & 0xff );
            }
            else
            {
               LCD_WriteDataLCD(   Font_BGnd_Color_u16        & 0xff );
               LCD_WriteDataLCD( ( Font_BGnd_Color_u16 >> 8 ) & 0xff );
            }
            BitSel = BitSel >> 1;
            if ( BitSel == 0 ){ i++; BitSel = 0x80; }
         }
      }
   }
}
void LCD_Draw_Line(u8 x1, u8 y1, u8 x2, u8 y2, u16 color )
{
   int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
   x1 = MIN( x1, SCREEN_WIDTH);
   y1 = MIN( y1, SCREEN_HEIGHT);
   x2 = MIN( x2, SCREEN_WIDTH);
   y2 = MIN( y2, SCREEN_HEIGHT);
   
   if ( x1 == x2 )        // Vertical Line
   {
      if ( y1 > y2 ){ LCD_Draw_Surface( x1, y2, 1, y1-y2+1, color ); }
      else          { LCD_Draw_Surface( x1, y1, 1, y2-y1+1, color ); }
   }
   else if ( y1 == y2 )  // Horizontal Line
   {
      if ( x1 > x2 ){ LCD_Draw_Surface( x2, y1, x1-x2+1, 1, color ); }
      else          { LCD_Draw_Surface( x1, y1, x2-x1+1, 1, color ); }
   }
   else
   {
      dx=x2-x1;      /* the horizontal distance of the line */
      dy=y2-y1;      /* the vertical distance of the line */
      dxabs=abs(dx);
      dyabs=abs(dy);
      sdx=sgn(dx);
      sdy=sgn(dy);
      x=dyabs>>1;
      y=dxabs>>1;
      px=x1;
      py=y1;
      
      if (dxabs>=dyabs) /* the line is more horizontal than vertical */
      {
         for(i=0;i<dxabs;i++)
         {
            y+=dyabs;
            if (y>=dxabs)
            {
               y-=dxabs;
               py+=sdy;
            }
            px+=sdx;
            LCD_DrawPixel(px,py,color);
         }
      }
      else /* the line is more vertical than horizontal */
      {
         for(i=0;i<dyabs;i++)
         {
            x+=dxabs;
            if (x>=dyabs)
            {
               x-=dyabs;
               px+=sdx;
            }
            py+=sdy;
            LCD_DrawPixel(px,py,color);
         }
      }
   }
}
void LCD_Delite_Line (u8 x1, u8 y1, u8 x2, u8 y2 )
{
   int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;
   x1 = MIN( x1, SCREEN_WIDTH);
   y1 = MIN( y1, SCREEN_HEIGHT);
   x2 = MIN( x2, SCREEN_WIDTH);
   y2 = MIN( y2, SCREEN_HEIGHT);
   
   dx=x2-x1;      /* the horizontal distance of the line */
   dy=y2-y1;      /* the vertical distance of the line */
   dxabs=abs(dx);
   dyabs=abs(dy);
   sdx=sgn(dx);
   sdy=sgn(dy);
   x=dyabs>>1;
   y=dxabs>>1;
   px=x1;
   py=y1;
   
   if (dxabs>=dyabs) /* the line is more horizontal than vertical */
   {
      for(i=0;i<dxabs;i++)
      {
         y+=dyabs;
         if (y>=dxabs)
         {
            y-=dxabs;
            py+=sdy;
         }
         px+=sdx;
         LCD_DrawPixel(px,py,background_image[py][px]);
      }
   }
   else /* the line is more vertical than horizontal */
   {
      for(i=0;i<dyabs;i++)
      {
         x+=dxabs;
         if (x>=dyabs)
         {
            x-=dyabs;
            px+=sdx;
         }
         py+=sdy;
         LCD_DrawPixel(px,py,background_image[py][px]);
      }
   }
}
