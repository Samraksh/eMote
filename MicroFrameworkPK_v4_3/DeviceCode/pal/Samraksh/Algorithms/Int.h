/**
 * @file   Int.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#ifndef _INT_H_
#define _INT_H_

// Will fit in smaller size
template<typename SmallT, typename BigT>
	unsigned ShiftToFitRound(BigT BigVal);
template<typename SmallT, typename BigT>
	unsigned ShiftToFitTrunc(BigT BigVal);

// Big shift a subset of an array
template<typename Type>
	void ShiftData(Type Data[], unsigned Start, unsigned Len, int Shift);

/*
	Include code for inlining
*/
#include "Int.hpp"

#endif /* _INT_H_ */


