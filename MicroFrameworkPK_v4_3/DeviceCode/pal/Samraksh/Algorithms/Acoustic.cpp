#include <tinyhal.h>
#include "Samraksh\Acoustic.h"

#include "ButterFly.h"
#include "Fft.h"
#include "IntAttrib.h"
#include "TwiddleWalk.h"


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

FftCompFixLenT<Int16T,ButterFlyAccT<Int16T>,BitWalkT<Int16T> >* Fft = NULL;

INT32 FftCompFixLenT_Int16T(/*CompT<Int16T>*/INT16* complex_buffer, INT32 ln2OfN) {

	bool resize = false;
	if(Fft != NULL) {
		resize = (Fft->Ln2OfN() != ln2OfN);
		if(resize) {
			delete Fft;
		}
	}
	if(Fft == NULL || resize) {
		Fft = new FftCompFixLenT<Int16T,ButterFlyAccT<Int16T>,BitWalkT<Int16T> >(ln2OfN);
	}

	INT32 shift = Fft->operator ()((CompT<Int16T>*)(complex_buffer));
	return shift;
}
