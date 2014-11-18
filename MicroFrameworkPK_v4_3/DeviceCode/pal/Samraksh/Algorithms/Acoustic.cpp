#include <tinyhal.h>
#include "Samraksh\Acoustic.h"

INT8 testFunction(UINT16* buffer, INT32 length, INT32 Pa, INT32 Pm){
	int i;

	for (i=0; i<length; i++){
		buffer[i] = buffer[i] * 2;
	}

	return TRUE;
}
