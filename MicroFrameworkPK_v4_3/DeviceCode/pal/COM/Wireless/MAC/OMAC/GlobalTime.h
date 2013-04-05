/*
 * GlobalTime.h
 *
 *  Created on: April 05, 2013
 *      Author: Mukundan Sridharan
 */


#ifndef GLOBALTIME_H_
#define GLOBALTIME_H_

#include <tinyhal.h>

class GlobalTime {
private:
	static INT64 offset;
	float skew;
public:
	UINT64 Read(){
		return Time_GetLocalTime() + offset;
	}
	static void Init(){
		offset=0;
	}
	float GetSkew(){
		return skew;
	}
	BOOL Adjust(UINT64 value, BOOL add){
		if(add){
			offset+=value;
		}else{
			offset = offset - value;
		}
	}
};

#endif //GLOBALTIME_H_
