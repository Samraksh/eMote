/**
 * @file   Int.hpp
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#ifndef _INT_HPP_
#define _INT_HPP_

#include "static_init.h"

#include "Comp.h"
#include "IntAttrib.h"
#include "Util.h"

/*
	Array Shift
*/
template<typename Type>
inline void ShiftData(Type Data[], unsigned Start, unsigned Len, int Shift)
{
	for (unsigned i=Start; i < Start + Len; i++)
		if (Shift < 0)
			Data[i] <<= (-Shift);
		else
			Data[i] >>= Shift;
} // ShiftData

/*
	Log2 like methods

	For the moment these methods assume conversion from signed to signed.
	Originally written to do efficient bisection, these just aren't intended
	to be used that much and the linear algorithms are easier to maintain.
*/

template<typename SmallT, typename BigT>
	unsigned ShiftToFitRound(BigT BigVal)
{
	unsigned smallBits = IntAttribT<SmallT>::NumBits();
	unsigned shift = 0;
	BigT limit = (1 << (smallBits - 1));

	if ( !InRangeClose<BigT>(BigVal, -limit,limit - 1) ) {
		shift = 1;
		limit = (limit << 1);
		BigT half = 1;
		unsigned maxShift = IntAttribT<BigT>::NumBits() - smallBits;

		while ((shift < maxShift) && !InRangeClose<BigT>(BigVal, -limit,limit - half - 1)) {
			half = (half << 1);
			limit = (limit << 1);
			shift++;
		}
	}

	return shift;
}; // ShiftToFitRound

template<typename SmallT, typename BigT>
	unsigned ShiftToFitTrunc(BigT BigVal)
{
	unsigned smallBits = IntAttribT<SmallT>::NumBits();
	unsigned shift = 0;
	BigT limit = (1 << (smallBits - 1));

	if ( !InRangeClose(BigVal, -limit,limit - 1) ) {
		shift = 1;
		limit = (limit << 1);
		unsigned maxShift = BigT::Bits() - smallBits;

		while ((shift < maxShift) && !InRangeClose<BigT>(BigVal, -limit,limit - 1)) {
			limit = (limit << 1);
			shift++;
		}
	}

	return shift;
}; // ShiftToFitTrunc

template <typename TargT, typename SourceT>
	inline bool InRange(SourceT Source)
{
	return (IntAttribT<TargT>::Min() <= Source) && (Source <= IntAttribT<TargT>::Max());
}; // InRange

#endif /* _INT_HPP_ */
