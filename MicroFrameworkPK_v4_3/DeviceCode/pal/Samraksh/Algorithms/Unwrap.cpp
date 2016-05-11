#include <tinyhal.h>
#include "Samraksh\Unwrap.h"

static int wPhase = 0, uwPhase = 0, wPhase_prev = 0, uwPhase_prev = 0;
static int wPhaseMax = 0, uwPhaseMax = 0, wPhase_prevMax = 0, uwPhase_prevMax = 0;
static int unwrapMax = 0;
static int wPhaseZero = 0, uwPhaseZero = 0, wPhase_prevZero = 0, uwPhase_prevZero = 0;
static int unwrapZero = 0;

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

int calculatePhase(UINT16* bufferI, UINT16* bufferQ, UINT16* bufferUnwrap, INT32 length, INT16 medianI, INT16 medianQ, INT16* arcTan, INT32 noiseRejection, UINT16 debugVal, UINT16 IDNumber, UINT16 versionNumber)
{
	int i;
	int unwrappedPhase;
	int unwrappedPhaseZero;
	int unwrappedPhaseMax;
	int minPhase, maxPhase;
	int minPhaseZero, maxPhaseZero;
	int minPhaseMax, maxPhaseMax;
	static BOOL detection = false;
	static UINT16 markerPrimary = 0xa5a5;
	static UINT16 markerRepeat = 0xf0f0;
	static UINT16 countPrimary = 1;
	static UINT16 countRepeat = 0;
	static UINT16 checksumPrimary = 0;
	static UINT16 checksumRepeat = 0;
	static UINT16 prevBufferI[250];
	static UINT16 prevBufferQ[250];
	static UINT8 uartPort = 0;
	INT16 iBufferI[length];
	INT16 iBufferQ[length];
	UINT16 dTrue = 1;
	UINT16 dFalse = 0;

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

	//hal_printf("using %d\r\n",noiseRejection);
	for (i=0; i<length; i++){
		if ((debugVal == 1) || (debugVal == 2)){
			USART_Write( uartPort, (char *)&bufferI[i], 2 );
			checksumPrimary += bufferI[i];
			USART_Write( uartPort, (char *)&bufferQ[i], 2 );
			checksumPrimary += bufferQ[i];
		}
		iBufferI[i] = (INT16)bufferI[i] - medianI;
		iBufferQ[i] = (INT16)bufferQ[i] - medianQ; 
		unwrappedPhase = (unwrapPhase(iBufferI[i], iBufferQ[i], arcTan, noiseRejection) >> 12);	// divide by 4096
		bufferUnwrap[i] = (UINT16)unwrappedPhase;

		if (debugVal == 1){
			USART_Write( uartPort, (char *)&bufferUnwrap[i], 2 );
			if (detection == true)
				USART_Write( uartPort, (char *)&dTrue, 2 );
			else
				USART_Write( uartPort, (char *)&dFalse, 2 );
		}

		unwrappedPhaseZero = uwPhaseZero >> 12;
		unwrappedPhaseMax = uwPhaseMax >> 12;

		if (i == 0) {
			minPhase = maxPhase = unwrappedPhase;
			minPhaseZero = maxPhaseZero = unwrappedPhaseZero;
			minPhaseMax = maxPhaseMax = unwrappedPhaseMax;
		}

    	if (unwrappedPhase < minPhase) minPhase = unwrappedPhase;
    	else if (unwrappedPhase > maxPhase) maxPhase = unwrappedPhase;

		if (unwrappedPhaseMax < minPhaseMax) minPhaseMax = unwrappedPhaseMax;
    	else if (unwrappedPhaseMax > maxPhaseMax) maxPhaseMax = unwrappedPhaseMax;

		if (unwrappedPhaseZero < minPhaseZero) minPhaseZero = unwrappedPhaseZero;
    	else if (unwrappedPhaseZero > maxPhaseZero) maxPhaseZero = unwrappedPhaseZero;
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
		for (i=0; i<length; i++){
			USART_Write( uartPort, (char *)&prevBufferI[i], 2 );
			checksumRepeat += prevBufferI[i];
			USART_Write( uartPort, (char *)&prevBufferQ[i], 2 );
			checksumRepeat += prevBufferQ[i];
		}
		USART_Write( uartPort, (char *)&checksumRepeat, 2 );

		memcpy(prevBufferI, bufferI, 500);
		memcpy(prevBufferQ, bufferQ, 500);
	}

	//hal_printf("%d %d %d\r\n",(maxPhase - minPhase),unwrapMax,unwrapZero);
	unwrapMax = maxPhaseMax - minPhaseMax;
	unwrapZero = maxPhaseZero - minPhaseZero;

	return (maxPhase - minPhase);
}

int getUnwrapMax(){
	return unwrapMax;
}

int getUnwrapZero(){
	return unwrapZero;
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
	int phase_diffMax;
	int phase_diffZero;
    int newPhase = 0;
    int newPhaseMax = 0;
    int newPhaseZero = 0;

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

	newPhaseZero = newPhase;
	newPhaseMax = newPhase;

	// Ignore small changes
	if ( abs(valueI) < noiseRejection && abs(valueQ) < noiseRejection ) {
		newPhase = wPhase_prev;
	}
	// always keeping track of phase with max IQ rejection
	if ( abs(valueI) < MAX_IQ_REJECTION && abs(valueQ) < MAX_IQ_REJECTION ) {
		newPhaseMax = wPhase_prevMax;
	}

	// phase unwrap with currently used IQ rejection parameter
	wPhase = newPhase;
    phase_diff = wPhase - wPhase_prev + (int)FULL;
    if (phase_diff < 0)
    	phase_diff += (int)TWO;
    else if (phase_diff > (int)TWO)
    	phase_diff -= (int)TWO;
    uwPhase = uwPhase_prev + phase_diff - (int)FULL;

	wPhase_prev = wPhase;
    uwPhase_prev = uwPhase;

	// phase unwrap with currently used IQ rejection of zero
	wPhaseZero = newPhaseZero;
	
    phase_diffZero = wPhaseZero - wPhase_prevZero + (int)FULL;
    if (phase_diffZero < 0)
    	phase_diffZero += (int)TWO;
    else if (phase_diffZero > (int)TWO)
    	phase_diffZero -= (int)TWO;
    uwPhaseZero = uwPhase_prevZero + phase_diffZero - (int)FULL;

	wPhase_prevZero = wPhaseZero;
    uwPhase_prevZero = uwPhaseZero;

	// phase unwrap with currently used IQ rejection of MAX
	wPhaseMax = newPhaseMax;
	
    phase_diffMax = wPhaseMax - wPhase_prevMax + (int)FULL;
    if (phase_diffMax < 0)
    	phase_diffMax += (int)TWO;
    else if (phase_diffMax > (int)TWO)
    	phase_diffMax -= (int)TWO;
    uwPhaseMax = uwPhase_prevMax + phase_diffMax - (int)FULL;

	wPhase_prevMax = wPhaseMax;
    uwPhase_prevMax = uwPhaseMax;

    return uwPhase;
}
