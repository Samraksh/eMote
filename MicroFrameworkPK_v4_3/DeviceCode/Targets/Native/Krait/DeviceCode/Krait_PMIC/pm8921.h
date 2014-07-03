#ifndef _PM8921_H_
#define _PM8921_H_ 1
/**
 * @file: pm8921.h
 * @author: Michael Andrew McGrath
 * @copyright: The Samraksh Company, Copyright (c) 2014
 * @date: April 2, 2014
 * @brief: functions in Little Kernel's PMIC
 * @details: PM8921 funcion prototypes. most PMIC control registers are already
 * defined in Krait.h
 */

typedef int (*pm8921_read_func)(UINT8 *data, UINT32 length, UINT32 addr);
typedef int (*pm8921_write_func)(UINT8 *data, UINT32 length, UINT32 addr);

typedef struct
{
	UINT32 initialized;

	pm8921_read_func	read;
	pm8921_write_func	write;

} pm8921_dev_t;



    int pm8921_config_reset_pwr_off(unsigned reset);


#endif

