/*  Name : Krait_Gyroscope.cpp
 *
 *  Author : Christopher Morse
 *
 *  Description :
 *
 */
#include <tinyhal.h>
#include "..\Krait.h"
#include "Krait_Gyroscope.h"

INT8 Initialize()
{
    INT8 retVal = 0;
	return retVal;
}

INT8 Reset()
{
	INT8 retVal = 0;
	return retVal;
}

INT8 SelfTest()
{
    INT8 retVal = 0;
	return retVal;
}

INT16 GetX()
{
    INT16 xVal = 0; 
    return xVal;
}

INT16 GetY()
{
    INT16 yVal = 0; 
    return yVal;
}

INT16 GetZ()
{
    INT16 zVal = 0; 
    return zVal;
}

INT16 GetTemperature()
{
    INT16 temperature = 0;
	return temperature;
}

UINT8 ReadRegister(UINT8 reg)
{
    UINT8 data = 0;
    return data;
}

INT8 WriteRegister( UINT8 reg, UINT8 data )
{
	INT8 retVal = 0;
	return retVal;
}

INT8 AdvancedConfiguration( UINT32 config )
{
    INT8 retVal = 0;
	return retVal;
}

INT8 SetOffsets(INT16 xOffset, INT16 yOffset, INT16 zOffset)
{
	INT8 retVal = 0;
	return retVal;
}

INT8 SampleConfiguration(UIN32 config)
{
	INT8 retVal = 0;
	return retVal;
}

INT8 ADAPT_Accel_GetAll( INT16 data[] )
{
    INT8 retVal = 0;
	
	data[0] = 1;
	data[1] = 2;
	data[2] = 3;
	
    return retVal;
}
