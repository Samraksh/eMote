/**
 * @file   ButterFly.hpp
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */


#include "ButterFly.h"
#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"


template<typename RealT>
	int ButterFlyAccT<RealT>::operator ()
		(CompT<RealT> &V0, CompT<RealT> &V1, CompT<RealT> const& W, unsigned Shift)
{
	typedef typename IntAttribT<RealT>::MultT MultT;
	unsigned wShift = IntAttribT<RealT>::NumBits() - 2;
	MultT wHalf = (1 << (wShift - 1));

	MultT RealProd = (MultT(W.Real()) * MultT(V1.Real()) + wHalf) >> wShift;
	MultT ImagProd = (MultT(W.Imag()) * MultT(V1.Imag()) + wHalf) >> wShift;

	MultT CrossRealW = (MultT(W.Real()) * MultT(V1.Imag()) + wHalf) >> wShift;
	MultT CrossImagW = (MultT(W.Imag()) * MultT(V1.Real()) + wHalf) >> wShift;

	// Compute but don't round yet ... Should only round once ...
	MultT Ans0Real = V0.Real() + RealProd - ImagProd;
	MultT Ans0Imag = V0.Imag() + CrossRealW + CrossImagW;
	MultT Ans1Real = V0.Real() - RealProd + ImagProd;
	MultT Ans1Imag = V0.Imag() - CrossRealW - CrossImagW;

	// Need a fast check with a slow recovery
	MultT temp = (1 << (Shift + IntAttribT<RealT>::NumBits() - 1));
	MultT low = -temp, high = temp - 1;

	int extraShift = 0;
	if (!InRangeOpen(Ans0Real,low,high) || !InRangeOpen(Ans0Imag,low,high) ||
			!InRangeOpen(Ans1Real,low,high) || !InRangeOpen(Ans1Imag,low,high)) {

		unsigned S0R = ShiftToFitRound<RealT>(Ans0Real);
		unsigned S0I = ShiftToFitRound<RealT>(Ans0Imag);
		unsigned S1R = ShiftToFitRound<RealT>(Ans1Real);
		unsigned S1I = ShiftToFitRound<RealT>(Ans1Imag);

		extraShift = Max(S0R,S0I,S1R,S1I) - Shift;
	}

	// Do final shift and rounding.
	unsigned finalShift = Shift + extraShift;

	if (0 < finalShift) {
		MultT finalHalf = (1 << (finalShift - 1));

		#pragma warning(push)
		#pragma warning(disable:4244)
		V0.Real() = RealT((Ans0Real + finalHalf) >> finalShift);
		V0.Imag() = RealT((Ans0Imag + finalHalf) >> finalShift);
		V1.Real() = RealT((Ans1Real + finalHalf) >> finalShift);
		V1.Imag() = RealT((Ans1Imag + finalHalf) >> finalShift);
		#pragma warning(pop)

	} else {
		#pragma warning(push)
		#pragma warning(disable:4244)
		V0.Real() = RealT(Ans0Real);
		V0.Imag() = RealT(Ans0Imag);
		V1.Real() = RealT(Ans1Real);
		V1.Imag() = RealT(Ans1Imag);
		#pragma warning(pop)

	}

	return extraShift;
}; // operator ()

