/*
 * Krait_Gyroscope.h
 *
 *  Created on: March 6, 2014
 *      Author: Christopher Morse
 */

#ifndef KRAIT_GYROSCOPE_H
#define KRAIT_GYROSCOPE_H

INT8 Initialize();
INT8 Reset();
INT8 SelfTest();
INT16 GetX();
INT16 GetY();
INT16 GetZ();
INT16 GetTemperature();
UINT8 ReadRegister(UINT8 reg);
INT8 WriteRegister(UINT8 reg, UINT8 data);
INT8 AdvancedConfiguration(UINT32 config);
INT8 SetOffsets(INT16 xOffset, INT16 yOffset, INT16 zOffset);
INT8 SampleConfiguration(UINT32 config);
INT8 GetAllData(INT16 data[]);

#endif // KRAIT_GYROSCOPE_H

