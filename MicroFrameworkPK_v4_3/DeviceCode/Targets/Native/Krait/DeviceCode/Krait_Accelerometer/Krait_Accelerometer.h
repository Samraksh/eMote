/*
 * Krait_Accelerometer.h
 *
 *  Created on: Feb 18, 2014
 *      Author: Christopher Morse
 */

#ifndef KRAIT_ACCELEROMETER_H
#define KRAIT_ACCELEROMETER_H

#define BMA250_ADDR (0x18)
INT8 ADAPT_Accel_Init();
INT8 ADAPT_Accel_Reset();
INT8 ADAPT_Accel_SelfTest();
INT16 ADAPT_Accel_GetX();
INT16 ADAPT_Accel_GetY();
INT16 ADAPT_Accel_GetZ();
INT16 ADAPT_Accel_GetTemperature();
UINT8 ADAPT_Accel_Raw_Read(UINT8 reg);
INT8 ADAPT_Accel_Raw_Write(UINT8 reg, UINT8 data);
INT8 ADAPT_Accel_advconfig(UINT32 config);
INT8 ADAPT_Accel_SetPowerMode(UINT32 mode);
INT8 ADAPT_Accel_SetBandwidth(UINT32 bandwidth);
INT8 ADAPT_Accel_SetRange(UINT8 range);
INT8 ADAPT_Accel_GetAll(INT16 data[]);

#endif // KRAIT_ACCELEROMETER_H

