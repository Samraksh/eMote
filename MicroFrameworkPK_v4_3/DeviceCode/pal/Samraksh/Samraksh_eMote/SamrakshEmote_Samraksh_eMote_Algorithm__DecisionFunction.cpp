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
#include "SamrakshEmote_Samraksh_eMote_Algorithm__DecisionFunction.h"
#include "Samraksh\DecisionFunction.h"
#include <stm32f10x.h>

using namespace Samraksh::eMote;

DecisionFunction decisionFunc;

void Algorithm_DecisionFunction::NormalizeFeatureVector( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_float param0, CLR_RT_TypedArray_INT32 param1, HRESULT &hr )
{
    decisionFunc.Normalize(param0.GetBuffer(), param1.GetBuffer()); 
    return;
}

float Algorithm_DecisionFunction::Decide( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT32 param0, HRESULT &hr )
{
    float retVal = decisionFunc.Decide(param0.GetBuffer()); 
    return retVal;
}

