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
static UINT16 IDNumber=0;
static UINT16 codeVersion=0;

INT8 Algorithm_RadarDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, CLR_RT_TypedArray_UINT16 param1, CLR_RT_TypedArray_UINT16 param2, INT32 param3, CLR_RT_TypedArray_INT16 param4, HRESULT &hr )
{
    INT8 retVal = 0;
	
	// Find median I
	medianI = findMedian(param0.GetBuffer(), param3);

	// Find median Q
	medianQ = findMedian(param1.GetBuffer(), param3);

	// copying to temp buffer so I don't modify original I/Q buffers in case I want to save them to NOR
	retVal = calculatePhase(param0.GetBuffer(), param1.GetBuffer(), param2.GetBuffer(), param3, medianI, medianQ, param4.GetBuffer(), threshold, noiseRejection, debugVal, IDNumber, codeVersion);

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
	retVal = calculatePhase(param0.GetBuffer(), param1.GetBuffer(), tempUnwrap, param2, medianI, medianQ, param3.GetBuffer(), threshold, noiseRejection, debugVal, IDNumber, codeVersion);

    return retVal;
}

INT8 Algorithm_RadarDetection::SetDetectionParameters( CLR_RT_HeapBlock* pMngObj, double param0, INT32 param1, UINT16 param2, UINT16 param3, HRESULT &hr )
{
    INT8 retVal = 0; 

	threshold = param0;
	noiseRejection = param1;
	debugVal = param2;
	codeVersion =  param3;

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
