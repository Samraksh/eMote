/**
 * @file      GlobalTestTable.cpp
 * @author    Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date      Oct 23, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief     TODO: MSBUILD adds structs matching pattern "EmoteDynamicTest_*" to this file.
 *
 */

#include <Samraksh\DynamicTestRunner.h>
#include "GlobalTestsTable.h"
#include "DynamicTestRunnerTests.h"

#include <Samraksh\Acoustic.h>

#include "..\..\..\Targets\Native\STM32F10x\DeviceCode\math\Include\arm_math.h"

/* use the string as a way to communicate to the caller which instance of a
 * templated function the pointer is to, otherwise each function would have
 * to use __PRETTY_FUNCTION__ at runtime to update its string pointer, and
 * only after the function was called.
 * @todo: is this only valid for C?
 */
struct TestEntry GlobalTestTable[] = {
	[AddUINT8] = {(generic_fptr)&Emote_DynamicTestRunner_Tests::Add<UINT8>,"TemplatedAddUINT8"},
	[AddUINT32] = {(generic_fptr)&Emote_DynamicTestRunner_Tests::Add<UINT32>,"TemplatedAddUINT32"},
	[SubUINT8] = {(generic_fptr)&Emote_DynamicTestRunner_Tests::Subtract<UINT8>,"TemplatedSubtractUINT8"},
	[SubUINT32] = {(generic_fptr)&Emote_DynamicTestRunner_Tests::Subtract<UINT32>,"TemplatedSubtractUINT32"},
	[ARMFFTR4INITQ16] = {(generic_fptr)&arm_cfft_radix4_init_q15,"arm_cfft_radix4_init_q15"},
	[ARMFFTR4Q15] = {(generic_fptr)&arm_cfft_radix4_q15,"arm_cfft_radix4_q15"},
	[TWIDWALKCI16] = {(generic_fptr)&FftCompFixLenT_Int16T,"FftCompFixLenT_Int16T"},
};

unsigned int GlobalTestCount = sizeof(GlobalTestTable) / sizeof(GlobalTestTable[0]);
