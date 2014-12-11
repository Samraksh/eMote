/**
 * @file      DynamicTestRunnerTests.h
 * @author    Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date      Nov 14, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief Expose instantiated algorithms templates to MATLAB by using void* API.
 */

#ifndef _DYNAMICTESTRUNNERTESTS_H_
#define _DYNAMICTESTRUNNERTESTS_H_



namespace Emote_DynamicTestRunner_Tests {

template <typename T> void* Add(void* ptr_int_length, void* ptr_u8_a_in, void* ptr_u8_b_in, void* ptr_u8_c_result);

template <typename T> void* Subtract(void* ptr_int_length, void* ptr_T_a_in, void* ptr_T_b_in, void* ptr_T_c_result);

void* FftCompFixLenT_Int16T_Test(void* complex_buffer, void* ln2OfN);

void* RunningMeanT_Int16T_Test(void* buffer, void* bufferLength);

void* RunningMedianT_Int16T_Test(void* buffer, void* bufferLength);
}

#include "DynamicTestRunnerTests.hpp"

namespace Emote_DynamicTestRunner_Tests {

extern template void* Add<UINT8>(void*,void*,void*,void*);
extern template void* Subtract<UINT8>(void*,void*,void*,void*);
extern template void* Add<UINT32>(void*,void*,void*,void*);
extern template void* Subtract<UINT32>(void*,void*,void*,void*);

}

#endif /* _DYNAMICTESTRUNNERTESTS_H_ */
