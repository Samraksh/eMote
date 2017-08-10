/*
 * Krait_Gyroscope.h
 *
 *  Created on: March 6, 2014
 *      Author: Christopher Morse
 */

#ifndef KRAIT_GYROSCOPE_H
#define KRAIT_GYROSCOPE_H

INT8 gyro_init();
INT8 gyro_reset();
INT8 gyro_self_test();
INT16 gyro_get_X();
INT16 gyro_get_Y();
INT16 gyro_get_Z();
INT16 gyro_get_temp();
UINT8 gyro_raw_read(UINT8 reg);
INT8 gyro_raw_write(UINT8 reg, UINT8 data);
INT8 gyro_config(UINT32 config);
INT8 gyro_offsets(INT16 xOffset, INT16 yOffset, INT16 zOffset);
INT8 gyro_sample_config(UINT32 config);
INT8 gyro_power( UINT32 mode );
INT8 gyro_get_XYZ(INT16 data[]);

#endif // KRAIT_GYROSCOPE_H

