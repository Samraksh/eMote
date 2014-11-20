/**
 * @file      DynamicTestRunnerTests
 * @author    Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date      Oct 23, 2014
 * @copyright (C) 2014 The Samraksh Company
 *
 */

#include <tinyhal_types.h>

#include "DynamicTestRunnerTests.h"

namespace Emote_DynamicTestRunner_Tests {
template <typename T>
void* Add(void* ptr_int_length, void* ptr_u8_a_in, void* ptr_u8_b_in, void* ptr_u8_c_result) {

	int length = *((int*)ptr_int_length);
	T* a = (T*)ptr_u8_a_in;
	T* b = (T*)ptr_u8_b_in;
	T* c = (T*)ptr_u8_c_result;
	for(int itr=0; itr < length; ++itr) {
		c[itr] = a[itr] + b[itr];
	}
	return (void*)c;
}

template <typename T>
void* Subtract(void* ptr_int_length, void* ptr_T_a_in, void* ptr_T_b_in, void* ptr_T_c_result) {
	int length = *((int*)ptr_int_length);
	T* a = (T*)ptr_T_a_in;
	T* b = (T*)ptr_T_b_in;
	T* c = (T*)ptr_T_c_result;
	for(int itr=0; itr < length; ++itr) {
		c[itr] = a[itr] - b[itr];
	}
	return (void*)c;
}

}
