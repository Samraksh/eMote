/*
 * Name : TIAM3517_bootstrap.cpp
 *
 * Author : Nived.Sivadas@Samraksh.com
 *
 * Description : Low level initializations of the PRCM etc are done here, contents of this file follow uboot\cpu\arm_co  * rtexa8\omap3\clock.c
 */

#include "..\Krait.h"


void BootstrapCode()
{
	PrepareImageRegions();
}


