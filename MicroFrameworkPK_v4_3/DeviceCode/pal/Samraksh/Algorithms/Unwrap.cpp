#include <tinyhal.h>
#include "Samraksh\Unwrap.h"

static int unwrappedPhaseCrossProduct = 0;
static int prevQ = 0, prevI = 0;

static int unwrappedPhaseCrossProductMax = 0;
static int prevQMax = 0, prevIMax = 0;

static int unwrappedPhaseCrossProductZero = 0;
static int prevQZero = 0, prevIZero = 0;

static int unwrappedPhaseCrossMid = 0;

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

INT16 findMedian(UINT16* buffer, INT32 length)
{
	int left = 0;
    int right = length-1;
    int kth = (int)(length >> 1); // divide by 2

	INT16 iBuffer[length];
	int i;
	for (i = 0; i< length; i++){
		iBuffer[i] = (INT16)buffer[i];
	}

    while (true)
    {
    	int pivotIndex = partitions(left, right, iBuffer);

        int len = pivotIndex - left + 1;

        if (kth == len){
			//buffer[0] = buffer[pivotIndex]; // for debugging median
            return iBuffer[pivotIndex];
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

int calculatePhase(UINT16* bufferI, UINT16* bufferQ, UINT16* bufferUnwrap, INT32 length, INT16 medianI, INT16 medianQ, INT32 noiseRejection, UINT16 debugVal, UINT16 IDNumber, UINT16 versionNumber)
{
	int i;
	int unwrappedPhase;
	int unwrappedPhaseZero;
	int unwrappedPhaseMax;
	static UINT16 markerPrimary = 0xa5a5;
	static UINT16 markerRepeat = 0xf0f0;
	static UINT16 countPrimary = 1;
	static UINT16 countRepeat = 0;
	static UINT16 checksumPrimary = 0;
	static UINT16 checksumRepeat = 0;
	// currently there is no need to use protected data, so commenting this out to save the space
	//static UINT16 prevBufferI[250];
	//static UINT16 prevBufferQ[250];
	static UINT8 uartPort = 0;
	INT16 iBufferI[length];
	INT16 iBufferQ[length];
	int midPoint = (int)length>>1;

	unwrappedPhaseCrossProduct = 0;
	unwrappedPhaseCrossProductMax = 0;
	unwrappedPhaseCrossProductZero = 0;

	if (debugVal == 6){
		USART_Write( uartPort, (char *)&markerPrimary, 2 );
	}
	if (debugVal == 2){
		USART_Write( uartPort, (char *)&markerPrimary, 2 );
		checksumPrimary = markerPrimary;
		USART_Write( uartPort, (char *)&countPrimary, 2 );
		checksumPrimary += countPrimary;
		countPrimary++;
		USART_Write( uartPort, (char *)&IDNumber, 2 );
		checksumPrimary += IDNumber;
		USART_Write( uartPort, (char *)&versionNumber, 2 );
		checksumPrimary += versionNumber;
	}

	for (i=0; i<length; i++){
		if ((debugVal == 1) || (debugVal == 2) || (debugVal == 6)){
			USART_Write( uartPort, (char *)&bufferI[i], 2 );
			checksumPrimary += bufferI[i];
			USART_Write( uartPort, (char *)&bufferQ[i], 2 );
			checksumPrimary += bufferQ[i];
		}
		iBufferI[i] = (INT16)bufferI[i] - medianI;
		iBufferQ[i] = (INT16)bufferQ[i] - medianQ; 
		unwrapCrossProduct(iBufferI[i], iBufferQ[i], noiseRejection);

		// saving mid-point value
		if (i == midPoint)
			unwrappedPhaseCrossMid = unwrappedPhaseCrossProduct;

		bufferUnwrap[i] = (UINT16)unwrappedPhaseCrossProduct;

		if (debugVal == 1){
			USART_Write( uartPort, (char *)&bufferUnwrap[i], 2 );
		}
	}

	if (debugVal == 2){
		USART_Write( uartPort, (char *)&checksumPrimary, 2 );

		// the previous second's worth of data is resent here
		USART_Write( uartPort, (char *)&markerRepeat, 2 );
		checksumRepeat = markerRepeat;
		USART_Write( uartPort, (char *)&countRepeat, 2 );
		checksumRepeat += countRepeat;
		countRepeat++;
		USART_Write( uartPort, (char *)&IDNumber, 2 );
		checksumRepeat += IDNumber;
		USART_Write( uartPort, (char *)&versionNumber, 2 );
		checksumRepeat += versionNumber;
		/*for (i=0; i<length; i++){
			USART_Write( uartPort, (char *)&prevBufferI[i], 2 );
			checksumRepeat += prevBufferI[i];
			USART_Write( uartPort, (char *)&prevBufferQ[i], 2 );
			checksumRepeat += prevBufferQ[i];
		}*/
		USART_Write( uartPort, (char *)&checksumRepeat, 2 );

		//memcpy(prevBufferI, bufferI, 500);
		//memcpy(prevBufferQ, bufferQ, 500);
	}

	return (abs(unwrappedPhaseCrossProduct));
}

void unwrapCrossProduct(INT16 valueI, INT16 valueQ, INT32 noiseRejection)
{
	int crossProductResultValue = 0;
	int cprod = 0;

	// Ignore small changes
	if ( abs(valueI) > noiseRejection || abs(valueQ) > noiseRejection ) {
    	cprod = (prevQ * valueI) - (prevI * valueQ);
		if ((cprod < 0) && (prevI < 0) && (valueI > 0))
        	crossProductResultValue = 1;
    	else if ((cprod > 0) && (prevI > 0) && (valueI < 0))
    	   	crossProductResultValue = -1;
		else 
			crossProductResultValue = 0;
		
		// this is the unwrap result if we apply the user's noise rejection value
		unwrappedPhaseCrossProduct += crossProductResultValue;	

		// here we keep track of the current point if it is not considered noise 
		prevQ = valueQ;
		prevI = valueI;
	}

	// computing cross product for zero noiseRejections
    cprod = (prevQZero * valueI) - (prevIZero * valueQ);
	if ((cprod < 0) && (prevIZero < 0) && (valueI > 0))
       	crossProductResultValue = 1;
    else if ((cprod > 0) && (prevIZero > 0) && (valueI < 0))
       	crossProductResultValue = -1;
	else 
		crossProductResultValue = 0;
		
	// this is the unwrap result if we apply zero noise rejection value
	unwrappedPhaseCrossProductZero += crossProductResultValue;	

	// here we keep track of the current point if it is not considered noise 
	prevQZero = valueQ;
	prevIZero = valueI;

	// computing cross product for MAX IQ rejection
	if ( abs(valueI) > MAX_IQ_REJECTION || abs(valueQ) > MAX_IQ_REJECTION ) {
    	cprod = (prevQMax * valueI) - (prevIMax * valueQ);
		if ((cprod < 0) && (prevIMax < 0) && (valueI > 0))
        	crossProductResultValue = 1;
    	else if ((cprod > 0) && (prevIMax > 0) && (valueI < 0))
    	   	crossProductResultValue = -1;
		else 
			crossProductResultValue = 0;
		
		// this is the unwrap result if we apply max noise rejection value
		unwrappedPhaseCrossProductMax += crossProductResultValue;	

		// here we keep track of the current point if it is not considered noise 
		prevQMax = valueQ;
		prevIMax = valueI;
	}
}

int getUnwrapMax(){
	return (abs(unwrappedPhaseCrossProductMax));
}

int getUnwrapZero(){
	return (abs(unwrappedPhaseCrossProductZero));
}

int getUnwrapMid(){
	return (abs(unwrappedPhaseCrossMid));
}
