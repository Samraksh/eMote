/****************** (C) Matthias *******************************************************/
/* optimized for PRIMER2, extended and amended by Matthias email webmaster@tg-info.de  */
/* automatically created with "Primer_Fonts_V1.1.xls"                                  */
/* -------- Includes ------------------------------------------------------------------*/
/* -------- Create Global variables ---------------------------------------------------*/
#define		RGB_RED         0x00F8
#define 	RGB_BLACK       0x0000
#define 	RGB_WHITE       0xffff
#define	    RGB_BLUE        0x1F00
#define 	RGB_GREEN       0xE007
#define 	RGB_YELLOW      (RGB_GREEN|RGB_RED)
#define 	RGB_MAGENTA     (RGB_BLUE|RGB_RED)
#define 	RGB_LIGHTBLUE   (RGB_BLUE|RGB_GREEN)
#define 	RGB_ORANGE      (RGB_RED | 0xE001)       // green/2 + red
#define 	RGB_PINK        (RGB_MAGENTA | 0xE001)   // green/2 + magenta
//          RGB is 16-bit coded as    G2G1G0B4 B3B2B1B0 R4R3R2R1 R0G5G4G3
#define     RGB_MAKE(xR,xG,xB)   (((((xG)>>2)&0x07)<<13)+((((xG)>>2))>>3)+(((xB)>>3)<<8)+(((xR)>>3)<<3))
