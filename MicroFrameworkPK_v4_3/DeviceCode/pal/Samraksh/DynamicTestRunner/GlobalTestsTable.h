/**
 * @file      GlobalTestsTable.h
 * @author    Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date      Nov 4, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief     API enum for MATLAB to refer to functions by index.
 *
 * TODO: MSBUILD adds structs matching pattern "EmoteDynamicTest_*" to this file.
 */

#ifndef GLOBALTESTSTABLE_H_
#define GLOBALTESTSTABLE_H_

typedef void* (*generic_fptr)();

struct TestEntry {
    generic_fptr functionAddress;
    const char* functionName;
};

enum TestIndex {
	AddUINT8 = 0,
	AddUINT32 = 1,
	SubUINT8 = 2,
	SubUINT32 = 3,
	ARMFFTR4INITQ16 = 4,
	ARMFFTR4Q15 = 5,
	TWIDWALKCI16 = 6,
	RUNMEANI16 = 7,
	RUNMEDIANI16 = 8,
};

extern struct TestEntry GlobalTestTable[];
extern unsigned int GlobalTestCount;

#endif /* GLOBALTESTSTABLE_H_ */
