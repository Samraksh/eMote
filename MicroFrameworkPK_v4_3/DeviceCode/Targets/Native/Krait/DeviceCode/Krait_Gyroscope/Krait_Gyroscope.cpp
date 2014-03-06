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

INT8 gyro_init()
{
    INT8 retVal = 0;
	return retVal;
}

INT8 gyro_reset()
{
	INT8 retVal = 0;
	return retVal;
}

INT8 gyro_self_test()
{
    INT8 retVal = 0;
	return retVal;
}

INT16 gyro_get_X()
{
    INT16 xVal = 1; 
    return xVal;
}

INT16 gyro_get_Y()
{
    INT16 yVal = 22; 
    return yVal;
}

INT16 gyro_get_Z()
{
    INT16 zVal = 444; 
    return zVal;
}

INT16 gyro_get_temp()
{
    INT16 temperature = 0;
	return temperature;
}

UINT8 gyro_raw_read(UINT8 reg)
{
    UINT8 data = 0;
    return data;
}

INT8 gyro_raw_write( UINT8 reg, UINT8 data )
{
	INT8 retVal = 0;
	return retVal;
}

INT8 gyro_config( UINT32 config )
{
    INT8 retVal = 0;
	return retVal;
}

INT8 gyro_offsets(INT16 xOffset, INT16 yOffset, INT16 zOffset)
{
	INT8 retVal = 0;
	return retVal;
}

INT8 gyro_sample_config(UINT32 config)
{
	INT8 retVal = 0;
	return retVal;
}

INT8 gyro_power( UINT32 mode )
{
	INT8 retVal = 0;
	return retVal;
}

INT8 gyro_get_XYZ( INT16 data[] )
{
    INT8 retVal = 0;
	
	data[0] = 1;
	data[1] = 2;
	data[2] = 3;
	
    return retVal;
}
