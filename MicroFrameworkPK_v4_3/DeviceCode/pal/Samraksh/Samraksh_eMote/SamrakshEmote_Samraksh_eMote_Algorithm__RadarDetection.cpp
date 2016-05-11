//-----------------------------------------------------------------------------
//
//                   ** WARNING! ** 
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "SamrakshEmote.h"
#include "SamrakshEmote_Samraksh_eMote_Algorithm__RadarDetection.h"
#include "Samraksh\Unwrap.h"
#include "Samraksh\meanTracker.h"


static HeapTrack* unwrapMedian;
static HeapTrack* unwrapMedianZero;
static HeapTrack* unwrapMedianMax;
static HeapTrack* radarQ;
using namespace Samraksh::eMote;
static double threshold;
static double noiseRejectionPassedParameter;
static INT16 medianI = 2040, medianQ = 2040;
static UINT16 debugVal = 0;
static UINT16 IDNumber=0;
static UINT16 codeVersion=0;
static INT32 radarNoiseCtrl=0;
static UINT16 M=0;
static UINT16 N=0;
static bool initialized = false;
static int lastUnwrap = 0;
static int lastUnwrapMax = 0;
static int lastUnwrapZero = 0;
#define IQ_ADJUSTMENT_PERIOD 80
static int adjustmentIQPeriod = IQ_ADJUSTMENT_PERIOD;
static int noiseTest = 0;
static INT32 IQRejectionToUse = 50;
static double callbacksPerSecond = 0;
// the initial adjustment will be made every 10 samples (so at 2 samples a second we will make fast adjustments for the first 50 seconds)
static int initialAdjustmentCnt = 100;

enum RADAR_NOISE_CONTROL
{
	FIXED_NOISE_REJECTION,
	SCALING_NOISE_REJECTION_ADD,
	SCALING_NOISE_REJECTION_MULTIPLY,
	SCALING_NOISE_REJECTION_TARGET
};

enum RADAR_NOISE_REQUEST
{
        IQ_REJECTION_ZERO,
        IQ_REJECTION_MAX,
        IQ_REJECTION_CURRENTLY_USED,
		RADAR_Q
};

// Counter for storing shared state
        class Counter
        {
			public:
			   int count;
        };
        // M-out-of-N detector to detect Displacement Detection
        // state is the current state of the detector
        // state = 1 iff there are M hits in the last N seconds
        class MoutOfNDetector
        {
			public:
            int state;
            int prevstate;

            int M, N, End;
            int* Buff;
			bool initialized = false;
			//int Buff[3];

            void Init(int x, int y)
            {
				initialized = true;
				state = 0;
				prevstate = 0;
                M = x;
                N = y;
                End = 0;
				//Buff = new int[M];
				Buff = (int*) private_malloc(M * sizeof(int));
                int i;

                for (i = 0; i < M; i++)
                    Buff[i] = -N;
            }

			void Uninit()
            {
				private_free(Buff);
				initialized = false;
            }

            void Update(int index, int detect)
            {
                prevstate = state;

                if (detect == 1)
                {
                    Buff[End] = index;
                    End = (End + 1) % M;
                }

                if (index - Buff[End] < N)
                    state = 1;
                else
                    state = 0;
            }
        };
static Counter mOfnCounter;
static MoutOfNDetector mOfnDetector;

INT8 processPhase(UINT16* bufferI, UINT16* bufferQ, UINT16* bufferUnwrap, INT32 length, INT16* arcTan)
{
	INT8 detection = 0;	
	int unwrap;
	INT32 backgroundDisplacementNoise=0;
	INT32 currentDisplacementNoise=0;
	INT32 currentRadarNoise=0;
	
	// Find median I
	medianI = findMedian(bufferI, length);
	// Find median Q
	medianQ = findMedian(bufferQ, length);
	HeapTrackInsert(radarQ,medianQ);

	backgroundDisplacementNoise = HeapTrackMedian(unwrapMedianZero);
	currentDisplacementNoise = HeapTrackMedian(unwrapMedian);
	currentRadarNoise = HeapTrackMedian(radarQ);

	if (radarNoiseCtrl == SCALING_NOISE_REJECTION_ADD){
		IQRejectionToUse = noiseRejectionPassedParameter + backgroundDisplacementNoise;
	} else if (radarNoiseCtrl == SCALING_NOISE_REJECTION_MULTIPLY){
		IQRejectionToUse = noiseRejectionPassedParameter * backgroundDisplacementNoise;
	} else if (radarNoiseCtrl == SCALING_NOISE_REJECTION_TARGET){
		if (initialAdjustmentCnt > 0){
			// this section will make large initial adjustments to the IQ rejection parameter
			initialAdjustmentCnt--;
			// we'll only make adjustments every 10 samples
			if ((initialAdjustmentCnt%10) == 0){
				if (currentDisplacementNoise < (noiseRejectionPassedParameter-1))
					IQRejectionToUse = IQRejectionToUse - 10;
				else if (currentDisplacementNoise > (noiseRejectionPassedParameter+1))
					IQRejectionToUse = IQRejectionToUse + 10;
				else
					// we are close enough to where we want to be so the initial large adjustment period is over
					initialAdjustmentCnt = 0;
				ResetHeapTrack(unwrapMedian, 300);
				ResetHeapTrack(unwrapMedianZero, 300);
				ResetHeapTrack(unwrapMedianMax, 300);	
			}
		} else {
			// this section will make small adjustments every IQ_ADJUSTMENT_PERIOD
			adjustmentIQPeriod--;
			// only adjusted every IQ_ADJUSTMENT_PERIOD calls
			if (adjustmentIQPeriod <= 0){
				if (currentDisplacementNoise < (noiseRejectionPassedParameter-1))
					IQRejectionToUse--;
				else if (currentDisplacementNoise > (noiseRejectionPassedParameter+1))
					IQRejectionToUse++;
				adjustmentIQPeriod = IQ_ADJUSTMENT_PERIOD;
			}
		}
		// capping the noise rejection to a max or min
		if (IQRejectionToUse < 0)
			IQRejectionToUse = 0;
		else if (IQRejectionToUse > MAX_IQ_REJECTION)
			IQRejectionToUse = MAX_IQ_REJECTION;
	} else {
		// radarNoiseCtrl == FIXED_NOISE_REJECTION
		IQRejectionToUse = noiseRejectionPassedParameter;
	}

	// copying to temp buffer so I don't modify original I/Q buffers in case I want to save them to NOR
	unwrap = calculatePhase(bufferI, bufferQ, bufferUnwrap, length, medianI, medianQ, arcTan, IQRejectionToUse, debugVal, IDNumber, codeVersion);

	lastUnwrap = unwrap;
	lastUnwrapZero = getUnwrapZero();
	lastUnwrapMax = getUnwrapMax();

	// if the AD callbacks per second is not yet set we will attempt to read the correct value now and then adjust the threshold appropriately
	if (callbacksPerSecond == 0){
		callbacksPerSecond = AD_GetCallbacksPerSecond();
		if (callbacksPerSecond != 0) {
			threshold = threshold / callbacksPerSecond;
		}
	}
	// threshold passed is given in rotations, converting to radians here
	double thresholdRadians = threshold * 2 * 3.14159;

	mOfnCounter.count += 1;
	if (unwrap > thresholdRadians)
    {
		mOfnDetector.Update(mOfnCounter.count, 1);
    }
    else
    {
		mOfnDetector.Update(mOfnCounter.count, 0);
		// we are only adding the current unwrap value to the background noise tracking if we did not detect anything
		HeapTrackInsert(unwrapMedian,lastUnwrap);
		HeapTrackInsert(unwrapMedianZero,lastUnwrapZero);
		HeapTrackInsert(unwrapMedianMax,lastUnwrapMax);
	}
	if (mOfnDetector.state == 1 && mOfnDetector.prevstate == 0)
    {
		//hal_printf("Detection\r\n");
		detection = 1;
	} else {
		detection = 0;
	}
	
	if (debugVal == 6)
		hal_printf("%d %d %d %d %d %d %d\r\n",getUnwrapMax(),unwrap,getUnwrapZero(),HeapTrackMedian(unwrapMedianMax),HeapTrackMedian(unwrapMedian),HeapTrackMedian(unwrapMedianZero),IQRejectionToUse);

	return detection;
}

INT8 Algorithm_RadarDetection::Initialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 

	if (initialized == false){
		unwrapMedian = HeapTrackNew(300);
		unwrapMedianZero = HeapTrackNew(300);
		unwrapMedianMax = HeapTrackNew(300);
		radarQ = HeapTrackNew(300);
		initialized = true;
		HeapTrackInsert(unwrapMedian,0);
		HeapTrackInsert(unwrapMedianZero,0);
		HeapTrackInsert(unwrapMedianMax,0);
		HeapTrackInsert(radarQ,0);
		initialAdjustmentCnt = 100;
	}

	retVal = 1;

    return retVal;
}

INT8 Algorithm_RadarDetection::Uninitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    INT8 retVal = 0; 
    return retVal;
}

INT8 Algorithm_RadarDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, CLR_RT_TypedArray_UINT16 param2, INT32 param3, CLR_RT_TypedArray_INT16 param4, HRESULT &hr )
{
    INT8 detection = 0;
	
	detection = processPhase(param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer(), param3, param4.GetBuffer());

    return detection;
}

INT8 Algorithm_RadarDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, INT32 param2, CLR_RT_TypedArray_INT16 param3, HRESULT &hr )
{
    INT8 detection = 0;
	UINT16 tempUnwrap[param2];

	detection = processPhase(param0.GetBuffer(), param1.GetBuffer(), tempUnwrap, param2, param3.GetBuffer());

    return detection;
}

INT8 Algorithm_RadarDetection::SetDetectionParameters( CLR_RT_HeapBlock* pMngObj, INT32 param0, double param1, double param2, UINT16 param3, UINT16 param4, UINT16 param5, UINT16 param6, HRESULT &hr )
{
    INT8 retVal = 0; 

	radarNoiseCtrl = param0;
	threshold = param1;
	noiseRejectionPassedParameter = param2;
	if ( mOfnDetector.initialized == true ){
		M = param3;
		N = param4;
		mOfnDetector.Uninit();
		hal_printf("Uninit M, N\r\n");
	}
	if (M == 0){
		M = 1;
	}
	hal_printf("setting m %d of n %d\r\n",M,N);
	mOfnDetector.Init(M,N);
	mOfnCounter.count = 0;

	debugVal = param5;
	codeVersion =  param6;

	UINT8 cpuserial[12];
	UINT32 Device_Serial0;UINT32 Device_Serial1; UINT32 Device_Serial2;
	Device_Serial0 = *(UINT32*)(0x1FFFF7E8);
	Device_Serial1 = *(UINT32*)(0x1FFFF7EC);
	Device_Serial2 = *(UINT32*)(0x1FFFF7F0);

	cpuserial[0] = (UINT8)(Device_Serial0 & 0x000000FF);
	cpuserial[1] = (UINT8)((Device_Serial0 & 0x0000FF00) >> 8);
	cpuserial[2] = (UINT8)((Device_Serial0 & 0x00FF0000) >> 16);
	cpuserial[3] = (UINT8)((Device_Serial0 & 0xFF000000) >> 24);
    cpuserial[4] = (UINT8)(Device_Serial1 & 0x000000FF);
    cpuserial[5] = (UINT8)((Device_Serial1 & 0x0000FF00) >> 8);
    cpuserial[6] = (UINT8)((Device_Serial1 & 0x00FF0000) >> 16);
    cpuserial[7] = (UINT8)((Device_Serial1 & 0xFF000000) >> 24);
    cpuserial[8] = (UINT8)(Device_Serial2 & 0x000000FF);
    cpuserial[9] = (UINT8)((Device_Serial2 & 0x0000FF00) >> 8);
    cpuserial[10] = (UINT8)((Device_Serial2 & 0x00FF0000) >> 16);
    cpuserial[11] = (UINT8)((Device_Serial2 & 0xFF000000) >> 24);

	
	UINT16 * temp = (UINT16 *) cpuserial;
	IDNumber = 0;
	for (int i=0; i< 6; i++){
		IDNumber=IDNumber ^ temp[i]; //XOR 72-bit number to generate 16-bit hash
	}

    return retVal;
}

INT32 Algorithm_RadarDetection::GetBackgroundNoiseLevel( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
	if (param0 == IQ_REJECTION_ZERO){
		return HeapTrackMedian(unwrapMedianZero);
	} else if (param0 == IQ_REJECTION_MAX){
		return HeapTrackMedian(unwrapMedianMax);
	} else {
		// param0 == IQ_REJECTION_CURRENTLY_USED
		return HeapTrackMedian(unwrapMedian);
	}		
}

INT8 Algorithm_RadarDetection::ResetBackgroundNoiseTracking( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    ResetHeapTrack(unwrapMedian, 300);
	ResetHeapTrack(unwrapMedianZero, 300);
	ResetHeapTrack(unwrapMedianMax, 300);		 
    return true;
}

INT32 Algorithm_RadarDetection::GetIQRejectionLevel( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return IQRejectionToUse;
}

INT32 Algorithm_RadarDetection::GetLastUnwrap( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
	if (param0 == IQ_REJECTION_ZERO){
		return lastUnwrapZero;
	} else if (param0 == IQ_REJECTION_MAX){
		return lastUnwrapMax;
	} else {
		// param0 == IQ_REJECTION_CURRENTLY_USED
		return lastUnwrap;
	}
}
