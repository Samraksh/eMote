/********************* (C) COPYRIGHT 2007 RAISONANCE S.A.S. *******************/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Type def  -----------------------------------------------------------------*/
#define     SCREEN_HEIGHT           160
#define     SCREEN_WIDTH            128
#define     LCD_CtrlLines_RD_Low    GPIOD->BRR  = 0x00000010
#define     LCD_CtrlLines_RD_High   GPIOD->BSRR = 0x00000010
#define     LCD_CtrlLines_WR_Low    GPIOD->BRR  = 0x00000020
#define     LCD_CtrlLines_WR_High   GPIOD->BSRR = 0x00000020
#define     LCD_CtrlLines_RST_Low   GPIOD->BRR  = 0x00000040
#define     LCD_CtrlLines_RST_High  GPIOD->BSRR = 0x00000040
#define     LCD_CtrlLines_CS_Low    GPIOD->BRR  = 0x00000080
#define     LCD_CtrlLines_CS_High   GPIOD->BSRR = 0x00000080
#define     LCD_CtrlLines_RS_Low    GPIOD->BRR  = 0x00000800
#define     LCD_CtrlLines_RS_High   GPIOD->BSRR = 0x00000800
//#define     LCD_Write_Data(a)       GPIOE->ODR = ( GPIOE->ODR & 0x807F ) | ((a) << 7)
//#define     LCD_Write_Data(a)       GPIOE->BRSR = (0x7F80 << 16) | ((a) << 7)  /* Thanks to "ntrf.zns" */
#define     LCD_Write_Data(a)       GPIOE->BSRR = ((u32)0x7F800000) | ((a) << 7)  /* Thanks to "ntrf.zns" */
#define     LCD_Read__Data()                     ((GPIOE->IDR & 0x7F80 )        >> 7)
#define     LCD_DataLinesInput()    GPIOE->CRH = 0x44444444;GPIOE->CRL = 0x48888333;GPIOE->ODR = 0b0000000000000000
#define     LCD_DataLinesOutput()   GPIOE->CRH = 0x43333333;GPIOE->CRL = 0x38888333;GPIOE->ODR = 0b0000000000000000

#define     MAX(i,j) ( ((i)>(j)) ?  (i) : (j) )
#define     MIN(i,j) ( ((i)<(j)) ?  (i) : (j) )
#define     abs(X)   ( ((X)< 0 ) ? -(X) : (X) )
#define     sgn(X)   ( ((X)< 0 ) ?  -1  :  1  )

/* LCD Commands */
#define DISPLAY_ON               0xAF
#define DISPLAY_OFF              0xAE
#define START_LINE               0xC0
#define START_COLUMN             0x00
#define CLOCKWISE_OUTPUT         0xA0
#define DYNAMIC_DRIVE            0xA4
#define DUTY_CYCLE               0xA9
#define READ_MODIFY_WRITE_OFF    0xEE
#define SOFTWARE_RESET           0xE2

#define ST7732_NOP               0x00
#define ST7732_SWRESET           0x01
#define ST7732_RDDID             0x04
#define ST7732_RDDST             0x09
#define ST7732_RDDPM             0x0A
#define ST7732_RDDMADCTR         0x0B
#define ST7732_RDDCOLMOD         0x0C
#define ST7732_RDDIM             0x0D
#define ST7732_RDDSM             0x0E
#define ST7732_RDDSDR            0x0F

#define ST7732_SLPIN             0x10
#define ST7732_SLPOUT            0x11
#define ST7732_PTLON             0x12
#define ST7732_NORON             0x13

#define ST7732_INVOFF            0x20
#define ST7732_INVON             0x21
#define ST7732_GAMSET            0x26
#define ST7732_DISPOFF           0x28
#define ST7732_DISPON            0x29
#define ST7732_CASET             0x2A
#define ST7732_RASET             0x2B
#define ST7732_RAMWR             0x2C
#define ST7732_RGBSET            0x2D
#define ST7732_RAMRD             0x2E

#define ST7732_PTLAR             0x30
#define ST7732_SCRLAR            0x33
#define ST7732_TEOFF             0x34
#define ST7732_TEON              0x35
#define ST7732_MADCTR            0x36
#define ST7732_VSCSAD            0x37
#define ST7732_IDMOFF            0x38
#define ST7732_IDMON             0x39
#define ST7732_COLMOD            0x3A

#define ST7732_RDID1             0xDA
#define ST7732_RDID2             0xDB
#define ST7732_RDID3             0xDC

#define ST7732_RGBCTR            0xB0
#define ST7732_FRMCTR1           0xB1
#define ST7732_FRMCTR2           0xB2
#define ST7732_FRMCTR3           0xB3
#define ST7732_INVCTR            0xB4
#define ST7732_RGBBPCTR          0xB5
#define ST7732_DISSET5           0xB6

#define ST7732_PWCTR1           0xC0
#define ST7732_PWCTR2            0xC1
#define ST7732_PWCTR3            0xC2
#define ST7732_PWCTR4           0xC3
#define ST7732_PWCTR5           0xC4
#define ST7732_VMCTR1           0xC5
#define ST7732_VMOFCTR           0xC7

#define ST7732_WRID2             0xD1
#define ST7732_WRID3             0xD2
#define ST7732_RDID4            0xD3
#define ST7732_NVCTR1            0xD9
#define ST7732_NVCTR2            0xDE
#define ST7732_NVCTR3            0xDF

#define ST7732_GAMCTRP1          0xE0
#define ST7732_GAMCTRN1          0xE1

#define ST7732_AUTOCTRL          0xF1
#define ST7732_OSCADJ            0xF2
#define ST7732_DISPCTRL          0xF5
#define ST7732_DEFADJ            0xF6

#define V9_MADCTRVAL  0xd0 /*!< Left orientation value. */
#define V12_MADCTRVAL 0x70 /*!< Up orientation value. MX+MV */
#define V3_MADCTRVAL  0x10 /*!< Right orientation value. MY+MX */
#define V6_MADCTRVAL  0xB0 /*!< Bottom orientation value. MY+ MV */

#endif /*__LCD_H */
