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
#include "SamrakshEmote_Samraksh_eMote_Algorithm__AcousticDetection.h"
#include "Samraksh\Acoustic.h"

using namespace Samraksh::eMote;
static INT32 Pa = 1, Pm = 1;

UINT16 Algorithm_AcousticDetection::DetectionCalculation( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT16 param0, INT32 param1, CLR_RT_TypedArray_double param2, INT8 param3, HRESULT &hr )
{
    INT8 retVal = 0; 

	retVal = testFunction(param0.GetBuffer(), param1, param2.GetBuffer(), param3, Pa, Pm);

    return retVal;
}

INT8 Algorithm_AcousticDetection::SetDetectionParameters( CLR_RT_HeapBlock* pMngObj, INT32 param0, INT32 param1, HRESULT &hr )
{
    INT8 retVal = 0; 

	Pa = param0;
	Pm = param1;

    return retVal;
}

