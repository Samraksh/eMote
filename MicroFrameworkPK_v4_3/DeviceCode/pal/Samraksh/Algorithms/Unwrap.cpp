#include <tinyhal.h>
#include "Samraksh\Unwrap.h"

static int wPhase = 0, uwPhase = 0, wPhase_prev = 0, uwPhase_prev = 0;


enum PI
{
    HALF = 6434,
    FULL = 12868,
    NEG_FULL = -12868,
    TWO = 25736,
    NEG_HALF = -6434
};


int partitions(int low, int high, INT16* buffer){
	int p = low, r = high, x = buffer[r], i = p - 1;
    INT16 tempVal;
    for (int j = p; j <= r-1; j++)
    {
    	if (buffer[j] <= x)
        {
        	i++;
            tempVal = buffer[i];
            buffer[i] = buffer[j];
            buffer[j] = tempVal;
        }
    }
    tempVal = buffer[i+1];
    buffer[i+1] = buffer[r];
    buffer[r] = tempVal;

    return i + 1;
}

INT16 findMedian(INT16* buffer, INT32 length)
{
	int left = 0;
    int right = length-1;
    int kth = (int)(length >> 1); // divide by 2
    while (true)
    {
    	int pivotIndex = partitions(left, right, buffer);

        int len = pivotIndex - left + 1;

        if (kth == len){
			//buffer[0] = buffer[pivotIndex]; // for debugging median
            return buffer[pivotIndex];
		}
        else if (kth < len)
        	right = pivotIndex - 1;
        else
        {
        	kth = kth - len;
            left = pivotIndex + 1;
    	}
	}
}

BOOL calculatePhase(INT16* bufferI, INT16* bufferQ, UINT16* bufferUnwrap, INT32 length, INT16 medianI, INT16 medianQ, INT16* arcTan, INT32 threshold, INT32 noiseRejection)
{
	int i;
	int unwrappedPhase;
	int minPhase, maxPhase;
	BOOL detection = false, threshholdMet = false;

	for (i=0; i<length; i++){
		bufferI[i] = bufferI[i] - medianI;
		bufferQ[i] = bufferQ[i] - medianQ; 
		unwrappedPhase = (unwrapPhase(bufferI[i], bufferQ[i], arcTan, noiseRejection) >> 12);	// divide by 4096
		bufferUnwrap[i] = (UINT16)unwrappedPhase;

		if (i == 0) {minPhase = maxPhase = unwrappedPhase;}

    	if (unwrappedPhase < minPhase) minPhase = unwrappedPhase;
    	else if (unwrappedPhase > maxPhase) maxPhase = unwrappedPhase;
	}

	if (maxPhase - minPhase > threshold)
    {
        threshholdMet = 1;
    }
    else
    {
        threshholdMet = 0;
    }	

	detection = threshholdMet;

	return detection;
}

int findArcTan(int small, int big, INT16* arcTan)
{
	int temp;
    // small * 4096
    temp = (small << 12) / big;
    return arcTan[temp];
}

int unwrapPhase(INT16 valueI, INT16 valueQ, INT16* arcTan, INT32 noiseRejection)
{
	int phase_diff;
    int newPhase = 0;

    if (valueI > 0 && valueQ >= 0)
    {			//1st Quadrant: arg = atan(imag/real)
    	if (valueQ <= valueI) 					// Q/I is in {0,1}
        	newPhase = findArcTan(valueQ, valueI, arcTan);
        else
        	newPhase = (int)HALF - findArcTan(valueI, valueQ, arcTan);	// atan(x) = pi/2 - atan(1/x) for x > 0		
    }
    else if (valueI < 0 && valueQ >= 0)
    {		//2nd quadrant: arg = pi - atan(abs(imag/real)
    	if (valueQ <= abs(valueI))
        	newPhase = (int)FULL - findArcTan(valueQ, abs(valueI), arcTan);
            else
            	newPhase = (int)HALF + findArcTan(abs(valueI), valueQ, arcTan);  // pi - (pi/2 - atan(1/x))
    }
    else if (valueI < 0 && valueQ < 0)
    {			// 3rd quadrant: arg = -pi + atan(b/a)
    	if (abs(valueQ) <= abs(valueI))
        	newPhase = (int)NEG_FULL + findArcTan(abs(valueQ), abs(valueI), arcTan);
        else
        	newPhase = (int)NEG_HALF - findArcTan(abs(valueI), abs(valueQ), arcTan);	// -pi + pi/2 - atan(1/x)
    }
    else if (valueI > 0 && valueQ < 0)
    {							//4th quadrant: arg = - atan(b/a)
    	if (abs(valueQ) <= valueI)
        	newPhase = 0 - findArcTan(abs(valueQ), valueI, arcTan);
        else
        	newPhase = (int)NEG_HALF + findArcTan(valueI, abs(valueQ), arcTan);
    }

	if (noiseRejection != 0){
		// Ignore small changes
		if ( abs(valueI) < noiseRejection && abs(valueQ) < noiseRejection ) {
			newPhase = wPhase_prev;
		}
	}

	wPhase = newPhase;

    phase_diff = wPhase - wPhase_prev + (int)FULL;
    if (phase_diff < 0)
    	phase_diff += (int)TWO;
    else if (phase_diff > (int)TWO)
    	phase_diff -= (int)TWO;
    uwPhase = uwPhase_prev + phase_diff - (int)FULL;

    wPhase_prev = wPhase;
    uwPhase_prev = uwPhase;

    return uwPhase;
}
