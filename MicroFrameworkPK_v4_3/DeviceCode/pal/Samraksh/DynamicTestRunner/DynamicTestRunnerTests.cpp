/**
 * @file   DynamicTestRunnerTests.cpp
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 21, 2014
 * @copyright (C) 2014 The Samraksh Company
 */

#include "DynamicTestRunnerTests.h"

#include <Samraksh\Algorithms.h>

void* Emote_DynamicTestRunner_Tests::FftCompFixLenT_Int16T_Test(void* complex_buffer, void* ln2OfN) {
    INT32 fRet_shift = 0;
    INT16 (*param_complex_buffer)[2] = static_cast< INT16(*)[2] >(complex_buffer);
    INT32 param_ln2OfN = *( static_cast< INT32* >(ln2OfN) );
    fRet_shift = FftCompFixLenT_Int16T( param_complex_buffer, param_ln2OfN);
    return reinterpret_cast< void* >(fRet_shift);
}

void* Emote_DynamicTestRunner_Tests::RunningMeanT_Int16T_Test(void* buffer, void* bufferLength) {
    INT16 fRet_mean = 0;
    INT16* param_buffer = static_cast<INT16*>(buffer);
    UINT32 param_bufferLength = *static_cast<UINT32*>(bufferLength);
    fRet_mean = RunningMeanT_Int16T(param_buffer, param_bufferLength);
    return reinterpret_cast<void*>(fRet_mean);
}

void* Emote_DynamicTestRunner_Tests::RunningMedianT_Int16T_Test(void* buffer, void* bufferLength) {
    INT16 fRet_mean = 0;
    INT16* param_buffer = static_cast<INT16*>(buffer);
    UINT32 param_bufferLength = *static_cast<UINT32*>(bufferLength);
    fRet_mean = RunningMedianT_Int16T(param_buffer, param_bufferLength);
    return reinterpret_cast<void*>(fRet_mean);
}