////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**======================================================
 *		SAMRAKSH
 *=======================================================
 *	NAME	DATE	NOTES
 *	----	----	--------------------
 *	@NK1	07/01	Initial create
 *
 ======================================================**/

#include "stm32f10x_led.h"

/*----------------------------------------------------------------------------
 *        Local definitions
 *----------------------------------------------------------------------------*/
 
//--//
/**
 * LED_GREEN - 6
 **/
void LED_GREEN() {
  uint32_t tmp;
  RCC->APB2ENR |= 0x80;
  tmp = (GPIOF->CRL & 0xF0FFFFFF);
  GPIOF->CRL = (tmp | 0x03000000);
  GPIOF->BSRR = (0x1 << 6);
}

void LED_GREEN_OFF() {
  GPIOF->BRR = (0x1 << 6);
}

/**
 * LED_ORANGE - 7
 **/
void LED_ORANGE() {
  uint32_t tmp;
  RCC->APB2ENR |= 0x80;
  tmp = (GPIOF->CRL & 0x0FFFFFFF);
  GPIOF->CRL = (tmp | 0x30000000);
  GPIOF->BSRR = (0x1 << 7);
}

void LED_ORANGE_OFF() {
  GPIOF->BRR = (0x1 << 7);
}

void LED_WARNING() {
  uint32_t ik;
  while(1) {
	LED_ORANGE();
	for(ik = 0; ik < 100000; ik++);
	LED_ORANGE_OFF();
	for(ik = 0; ik < 100000; ik++);
  }
}

/**
 * LED_RED - 8
 **/
void LED_RED() {
  uint32_t tmp;
  RCC->APB2ENR |= 0x80;
  tmp = (GPIOF->CRH & 0xFFFFFFF0);
  GPIOF->CRH = (tmp | 0x00000003);
  GPIOF->BSRR = (0x1 << 8);
}

void LED_RED_OFF() {
  GPIOF->BRR = (0x1 << 8);
}

void LED_ERROR() {
  uint32_t ik;
  while(1) {
	LED_RED();
	for(ik = 0; ik < 100000; ik++);
	LED_RED_OFF();
	for(ik = 0; ik < 100000; ik++);
  }
}

void LED_ERROR1() {
  uint32_t ik;
  while(1) {
	LED_RED();
	for(ik = 0; ik < 100000; ik++);
	LED_RED_OFF();
	for(ik = 0; ik < 100000; ik++);
  }
}

/**
 * LED_BLUE - 9
 **/
void LED_BLUE() {
  uint32_t tmp;
  RCC->APB2ENR |= 0x80;
  tmp = (GPIOF->CRH & 0xFFFFFF0F);
  GPIOF->CRH = (tmp | 0x00000030);
  GPIOF->BSRR = (0x1 << 9);
}

void LED_BLUE_OFF() {
  GPIOF->BRR = (0x1 << 9);
}

