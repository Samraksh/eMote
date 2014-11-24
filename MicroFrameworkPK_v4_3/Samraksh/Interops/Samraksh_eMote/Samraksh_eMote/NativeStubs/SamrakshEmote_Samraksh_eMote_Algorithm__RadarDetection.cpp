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
static INT32 m, n, threshold;
static INT16 medianI = 2040, medianQ = 2040;

INT8 Algorithm_RadarDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, CLR_RT_TypedArray_UINT16 param2, INT32 param3, CLR_RT_TypedArray_INT16 param4, HRESULT &hr )
{
    INT8 retVal = 0;
	INT16 tempIBuffer[param3];
	INT16 tempQBuffer[param3];
	
	// Find median I
	memcpy(tempIBuffer, param0.GetBuffer(), param3 * sizeof(UINT16));
	medianI = findMedian(tempIBuffer, param3);

	// Find median Q
	memcpy(tempQBuffer, param1.GetBuffer(), param3 * sizeof(UINT16));
	medianQ = findMedian(tempQBuffer, param3);

	// copying to temp buffer so I don't modify original I/Q buffers in case I want to save them to NOR
	memcpy(tempIBuffer, param0.GetBuffer(), param3 * sizeof(UINT16));
	memcpy(tempQBuffer, param1.GetBuffer(), param3 * sizeof(UINT16));
	retVal = calculatePhase(tempIBuffer, tempQBuffer, param2.GetBuffer(), param3, medianI, medianQ, param4.GetBuffer(), m, n, threshold);

    return retVal;
}

INT8 Algorithm_RadarDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, INT32 param2, CLR_RT_TypedArray_INT16 param3, HRESULT &hr )
{
    INT8 retVal = 0;
	INT16 tempIBuffer[param2];
	INT16 tempQBuffer[param2];
	UINT16 tempUnwrap[param2];
	
	// Find median I
	memcpy(tempIBuffer, param0.GetBuffer(), param2 * sizeof(UINT16));
	medianI = findMedian(tempIBuffer, param2);

	// Find median Q
	memcpy(tempQBuffer, param1.GetBuffer(), param2 * sizeof(UINT16));
	medianQ = findMedian(tempQBuffer, param2);

	// copying to temp buffer so I don't modify original I/Q buffers in case I want to save them to NOR
	memcpy(tempIBuffer, param0.GetBuffer(), param2 * sizeof(UINT16));
	memcpy(tempQBuffer, param1.GetBuffer(), param2 * sizeof(UINT16));
	retVal = calculatePhase(tempIBuffer, tempQBuffer, tempUnwrap, param2, medianI, medianQ, param3.GetBuffer(), m, n, threshold);

    return retVal;
}

INT8 Algorithm_RadarDetection::SetDetectionThreshold( CLR_RT_HeapBlock* pMngObj, INT32 param0, HRESULT &hr )
{
    INT8 retVal = 0; 

	m = (INT32)param0;
	n = (INT32)param1;
	threshold = param2;

    return retVal;
}
