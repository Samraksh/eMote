#include <tinyhal.h>
#include "Samraksh\Unwrap.h"

static int unwrappedPhaseCrossProduct = 0;
static int prevQ = 0, prevI = 0;

static int unwrappedPhaseCrossProductMax = 0;
static int prevQMax = 0, prevIMax = 0;

static int unwrappedPhaseCrossProductZero = 0;
static int prevQZero = 0, prevIZero = 0;

// displacementEntireWindow is unwrappedPhaseCrossProduct
static int displacementFirstHalf = 0;
static int displacementSecondHalf = 0;

static int absoluteDisplFirstHalf = 0;
static int absoluteDisplSecondHalf = 0;
static int absoluteDisplEntire = 0;

static int minDisplacementFirstHalf = 0;
static int minDisplacementSecondHalf = 0;
static int minDisplacementEntire = 0;

static int maxDisplacementFirstHalf = 0;
static int maxDisplacementSecondHalf = 0;
static int maxDisplacementEntire = 0;

static INT16 absOffsetQ =0;
static INT16 absOffsetI =0;

static INT16 countOverTarget = 0;

enum PI
{
    HALF = 6434,
    FULL = 12868,
    NEG_FULL = -12868,
    TWO = 25736,
    NEG_HALF = -6434
};


    enum SAMPLE_WINDOW_PORTION
    {
        SAMPLE_WINDOW_FULL,
        SAMPLE_WINDOW_FIRST_HALF,
        SAMPLE_WINDOW_SECOND_HALF
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

int calculatePhase(UINT16* bufferI, UINT16* bufferQ, UINT16* bufferUnwrap, INT32 length, INT16 medianI, INT16 medianQ, INT32 noiseRejection, UINT16 debugVal, UINT16 IDNumber, UINT16 versionNumber, UINT16 classifierTarget)
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
	INT16 maxOffsetQ = 2048;
	INT16 maxOffsetI = 2048;
	INT16 minOffsetQ = 2048;
	INT16 minOffsetI = 2048;
	int midPoint = (int)length>>1;
	int sampleDisplacement = 0;

	countOverTarget = 0;

	unwrappedPhaseCrossProduct = 0;
	unwrappedPhaseCrossProductMax = 0;
	unwrappedPhaseCrossProductZero = 0;

	displacementFirstHalf = 0;
 	displacementSecondHalf = 0;

	absoluteDisplFirstHalf = 0;
	absoluteDisplSecondHalf = 0;
	absoluteDisplEntire = 0;

	minDisplacementFirstHalf = 0;
	minDisplacementSecondHalf = 0;
	minDisplacementEntire = 0;

	maxDisplacementFirstHalf = 0;
	maxDisplacementSecondHalf = 0;
	maxDisplacementEntire = 0;

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
		if ((debugVal == 1) || (debugVal == 2) || (debugVal == 3) || (debugVal == 6)){
			USART_Write( uartPort, (char *)&bufferI[i], 2 );
			checksumPrimary += bufferI[i];
			USART_Write( uartPort, (char *)&bufferQ[i], 2 );
			checksumPrimary += bufferQ[i];
		}
		iBufferI[i] = (INT16)bufferI[i] - medianI;
		if (iBufferI[i] > maxOffsetI)
			maxOffsetI = iBufferI[i];
		if (iBufferI[i] < minOffsetI)
			minOffsetI = iBufferI[i];
		iBufferQ[i] = (INT16)bufferQ[i] - medianQ; 
		if (iBufferQ[i] > maxOffsetQ)
			maxOffsetQ = iBufferQ[i];
		if (iBufferQ[i] < minOffsetQ)
			minOffsetQ = iBufferQ[i];

		if (abs( iBufferI[i] + iBufferQ[i]) > classifierTarget){
			countOverTarget++;
		}
		
		// sampleDisplacement contains the current samples displacement
		sampleDisplacement = unwrapCrossProduct(iBufferI[i], iBufferQ[i], noiseRejection);
		
		// unwrappedPhaseCrossProduct contains the cumlative sum of sampleDisplacement
		// this is done within unwrapCrossProduct()

		// saving data points to be used in classifiers
		// calculating 1st half window data points
		if (i < midPoint){
			displacementFirstHalf += sampleDisplacement;
			absoluteDisplFirstHalf += abs(sampleDisplacement);
			if (unwrappedPhaseCrossProduct < minDisplacementFirstHalf)
				minDisplacementFirstHalf = unwrappedPhaseCrossProduct;
			if (unwrappedPhaseCrossProduct > maxDisplacementFirstHalf)
				maxDisplacementFirstHalf = unwrappedPhaseCrossProduct;
		}
		// calculating 2nd half window data points
		if (i >= midPoint){
			displacementSecondHalf += sampleDisplacement;
			absoluteDisplSecondHalf += abs(sampleDisplacement);
			if (unwrappedPhaseCrossProduct < minDisplacementSecondHalf)
				minDisplacementSecondHalf = unwrappedPhaseCrossProduct;
			if (unwrappedPhaseCrossProduct > maxDisplacementSecondHalf)
				maxDisplacementSecondHalf = unwrappedPhaseCrossProduct;
		}
		// calculating data for entire window
		absoluteDisplEntire += abs(sampleDisplacement);
		if (unwrappedPhaseCrossProduct < minDisplacementEntire)
				minDisplacementEntire = unwrappedPhaseCrossProduct;
		if (unwrappedPhaseCrossProduct > maxDisplacementEntire)
				maxDisplacementEntire = unwrappedPhaseCrossProduct;


		bufferUnwrap[i] = (UINT16)unwrappedPhaseCrossProduct;

		if (debugVal == 1){
			USART_Write( uartPort, (char *)&bufferUnwrap[i], 2 );
		}
	}

	// saving data points to be used in classifier
	// calculating final data
	

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

	if ((maxOffsetI - 2048) > (2048 - minOffsetI))
		absOffsetI = abs(maxOffsetI);
	else
		absOffsetI = abs(minOffsetI);

	if ((maxOffsetQ - 2048) > (2048 - minOffsetQ))
		absOffsetQ = abs(maxOffsetQ);
	else
		absOffsetQ = abs(minOffsetQ);

	return (abs(unwrappedPhaseCrossProduct));
}

int unwrapCrossProduct(INT16 valueI, INT16 valueQ, INT32 noiseRejection)
{
	int crossProductResultValue = 0;
	int normalSampleResult = 0;
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

		normalSampleResult = crossProductResultValue;

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

	return normalSampleResult;
}

int getUnwrapMax(){
	return (abs(unwrappedPhaseCrossProductMax));
}

int getUnwrapZero(){
	return (abs(unwrappedPhaseCrossProductZero));
}

int getDisplacement(INT32 portion){
	if (portion == SAMPLE_WINDOW_FULL){
		return (unwrappedPhaseCrossProduct);
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return (displacementFirstHalf);
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return (displacementSecondHalf);
	}
}

int getAbsoluteDisplacement(INT32 portion){
	if (portion == SAMPLE_WINDOW_FULL){
		return absoluteDisplEntire;
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return absoluteDisplFirstHalf;
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return absoluteDisplSecondHalf;
	}
}

int getRange(INT32 portion){
	if (portion == SAMPLE_WINDOW_FULL){
		return (maxDisplacementEntire - minDisplacementEntire);
	} else if (portion == SAMPLE_WINDOW_FIRST_HALF){
		return (maxDisplacementFirstHalf - minDisplacementFirstHalf);
	} else {
		// SAMPLE_WINDOW_SECOND_HALF
		return (maxDisplacementSecondHalf - minDisplacementSecondHalf);
	}
}

INT16 getAbsOffsetQ(){
	return absOffsetQ;
}

INT16 getAbsOffsetI(){
	return absOffsetI;
}

INT16 getCountOverTarget(){
	return countOverTarget;
}
