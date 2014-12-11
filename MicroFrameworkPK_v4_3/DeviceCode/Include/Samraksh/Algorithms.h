/**
 * @file   Algorithms.h
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 24, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief  Expose instantiated Algorithms templates to other libraries by using fundamental types in the API.
 */

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <tinyhal_types.h>

#define ALGORITHMS_N 64

INT32 FftCompFixLenT_Int16T(INT16 (*complex_buffer) [2], INT32 ln2OfN);

INT16 RunningMeanT_Int16T(INT16* buffer, const UINT32 bufferLength);

INT16 RunningMedianT_Int16T(INT16* buffer, const UINT32 bufferLength);

#endif /* ALGORITHMS_H_ */
