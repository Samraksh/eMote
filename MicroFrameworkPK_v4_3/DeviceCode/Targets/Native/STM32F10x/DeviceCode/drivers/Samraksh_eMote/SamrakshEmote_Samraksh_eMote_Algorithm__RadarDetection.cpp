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

using namespace Samraksh::eMote;
static double threshold;
static INT32 noiseRejection;
static INT16 medianI = 2040, medianQ = 2040;
static UINT16 debugVal = 0;

INT8 Algorithm_RadarDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, CLR_RT_TypedArray_UINT16 param2, INT32 param3, CLR_RT_TypedArray_INT16 param4, HRESULT &hr )
{
    INT8 retVal = 0;
	
	// Find median I
	medianI = findMedian(param0.GetBuffer(), param3);

	// Find median Q
	medianQ = findMedian(param1.GetBuffer(), param3);

	// copying to temp buffer so I don't modify original I/Q buffers in case I want to save them to NOR
	retVal = calculatePhase(param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer(), param3, medianI, medianQ, param4.GetBuffer(), threshold, noiseRejection, debugVal);

    return retVal;
}

INT8 Algorithm_RadarDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, INT32 param2, CLR_RT_TypedArray_INT16 param3, HRESULT &hr )
{
    INT8 retVal = 0;
	UINT16 tempUnwrap[param2];
	
	// Find median I
	medianI = findMedian(param0.GetBuffer(), param2);

	// Find median Q
	medianQ = findMedian(param1.GetBuffer(), param2);

	// copying to temp buffer so I don't modify original I/Q buffers in case I want to save them to NOR
	retVal = calculatePhase(param0.GetBuffer(), param1.GetBuffer(), tempUnwrap, param2, medianI, medianQ, param3.GetBuffer(), threshold, noiseRejection, debugVal);

    return retVal;
}

INT8 Algorithm_RadarDetection::SetDetectionParameters( CLR_RT_HeapBlock* pMngObj, double param0, INT32 param1, UINT16 param2, HRESULT &hr )
{
    INT8 retVal = 0; 

	threshold = param0;
	noiseRejection = param1;
	debugVal = param2;

    return retVal;
}
