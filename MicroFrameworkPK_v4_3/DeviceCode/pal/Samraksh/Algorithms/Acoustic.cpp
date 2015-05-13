#include <tinyhal.h>
#include "Samraksh\Acoustic.h"

INT8 testFunction(UINT16* buffer, INT32 length, double* processingOutput, bool historyUpdateControl, INT32 Pa, INT32 Pm){
	int i;

	for (i=0; i<length; i++){
		buffer[i] = buffer[i] * 2;
	}

	if (historyUpdateControl == true){
		processingOutput[0] = 10;
		processingOutput[1] = 10;
		processingOutput[2] = 10;
		processingOutput[3] = 10;
		processingOutput[4] = 10;
		processingOutput[5] = 10;
	} else {
		processingOutput[0] = 1;
		processingOutput[1] = 1;
		processingOutput[2] = 2;
		processingOutput[3] = 3;
		processingOutput[4] = 5;
		processingOutput[5] = 8;
	}

	return TRUE;
}
