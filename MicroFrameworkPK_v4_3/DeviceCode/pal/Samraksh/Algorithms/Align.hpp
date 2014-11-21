/**
 * @file   Align.hpp
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */


#include "Align.h"
#include "Comp.h"
#include "Int.h"
#include "IntAttrib.h"
#include "DataTypes.h"
#include "Util.h"

/*
	Helpers.	Required for circa 2001 compiler.
*/
inline UInt16T Sqr(Int8T Num)
	{ return Num*Num; }
inline UInt32T Sqr(Int16T Num)
	{ return Num*Num; }

inline UInt16T MagSqr(CompT<Int8T> Num)
	{ return (Sqr(Num.Real()) + Sqr(Num.Imag())); }
inline UInt32T MagSqr(CompT<Int16T> Num)
	{ return (Sqr(Num.Real()) + Sqr(Num.Imag())); }

/*
	AlignT
*/
template <typename RealT>
	AlignT<RealT>::AlignT(unsigned N) : n(N)
	{ /* Nothing */ };

template <typename RealT>
	AlignT<RealT>::~AlignT()
	{ /* Nothing */ };

/*
	FftShiftT
*/
template <typename RealT, typename InvFftT>
	FftShiftT<RealT, InvFftT>::FftShiftT(unsigned N, unsigned Ln2OfPadN) :
	AlignT<RealT>(N), ln2OfPadN(Ln2OfPadN), invFft(Ln2OfPadN)
{
	const unsigned padN = (1 << ln2OfPadN);

	padData = new CompT<RealT>[padN];
	padRef = new CompT<RealT>[padN];
	prod = new CompT<RealT>[padN];
}; // FftShiftT

template <typename RealT, typename InvFftT>
	FftShiftT<RealT, InvFftT>::~FftShiftT()
{
	delete [] padData;
	delete [] padRef;
	delete [] prod;
}; // ~FftShiftT

template <typename RealT, typename InvFftT>
	int FftShiftT<RealT, InvFftT>::operator () (RealT Ref[], RealT Data[])
{
	const unsigned padN = (1 << ln2OfPadN);

	// Do zero padding
	unsigned i;
	for (i=0; i < AlignT<RealT>::n; i++) {  // MAM: in GCC 3.4+, in a template definition, unqualified names do not reference base class members.  Right now this n depends on the class constructor instantiating the base class.
		padData[i] = CompT<RealT>(Data[i], 0);
		padRef[i] = CompT<RealT>(Ref[i], 0);
	}

	const CompT<RealT> zero = CompT<RealT>(0,0);
	for (; i < padN; i++) {
		padData[i] = zero;
		padRef[i] = zero;
	}
//	OutPutArray(std::cout, padData,0, padN, "PadData");
//	OutPutArray(std::cout, padRef,0, padN, "PadRef");

	int shiftData = invFft.For(padData);
//	OutPutArray(std::cout, padData,shiftData,padN, "Trans of PadData");

	int shiftRef = invFft.For(padRef);
//	OutPutArray(std::cout, padRef,shiftRef,padN, "Trans of PadRef");

	// Do product
	typedef typename IntAttribT<RealT>::MultT MultT;

	unsigned shift = 0;
	MultT half = 0;
	const MultT maxProd = Sqr(IntAttribT<RealT>::Min());

	for (i=0; i < padN; i++) {
		// R[i] * conj(D[i]) = (Rr*Dr + Ri*Di) + i*(-Rr*Di + Ri*Dr)
		MultT realProd = padRef[i].Real() * padData[i].Real();
		MultT imagProd = padRef[i].Imag() * padData[i].Imag();
		MultT crossProd0 = padRef[i].Real() * padData[i].Imag();
		MultT crossProd1 = padRef[i].Imag() * padData[i].Real();

		// check for special case
		MultT ansReal = 0, ansImag = 0;
		if ((realProd == maxProd) && (imagProd == maxProd)) {
			shift++;
			ansReal = (realProd >> 1) + (imagProd >> 1);
			ansImag = (-crossProd0 >> 1) + (crossProd1 >> 1);
		}
		else {
			ansReal = realProd + imagProd;
			ansImag = -crossProd0 + crossProd1;
		}

		MultT ansRealRound = ((ansReal + half) >> shift);
		MultT ansImagRound = ((ansImag + half) >> shift);

		if (InRange<RealT>(ansImagRound) && InRange<RealT>(ansRealRound)) {
			#pragma warning(push)
			#pragma warning(disable:4244)
			prod[i] = CompT<RealT>(ansRealRound,ansImagRound);
			#pragma warning(pop)
		}
		else {
			unsigned extraShiftReal = ShiftToFitRound<RealT>(ansReal);
			unsigned extraShiftImag = ShiftToFitRound<RealT>(ansImag);
			unsigned extraShift = Max(extraShiftReal,extraShiftImag) - shift;

			for (unsigned j=0; j < i; j++) {
				prod[j].Real() >>= extraShift;
				prod[j].Imag() >>= extraShift;
			}

			shift += extraShift;
			half = (1 << (shift - 1));

			prod[i] = CompT<RealT>((ansReal + half) >> shift, (ansImag + half) >> shift);
		}
	}
//	OutPutArray(std::cout, prod,shift,padN, "Prod");

	// Compute inverse
	int shiftInv = invFft.Inv(prod);
//	OutPutArray(std::cout, prod, shift + shiftInv, padN, "Trans of Prod");

	// Find Max
	unsigned maxIndex = 0;
	RealT max = prod[maxIndex].Real();

	for (i=1; i < padN; i++) {
		RealT mag = prod[i].Real();
		if (max < mag) {
			maxIndex = i;
			max = mag;
		}
	}
//	std::cout << "MaxIndex = " << maxIndex << "\n\n";

	// Compute shift and shift the Data
	unsigned mid = (1 << (ln2OfPadN - 1));

	int dataShift = 0;
	if ((0 < maxIndex) && (maxIndex < mid)) {
		dataShift = maxIndex;

		for (i = AlignT<RealT>::n; maxIndex < i; ) {
			i--;
			Data[i] = Data[i - dataShift];
		}
		for ( ; 0 < i; ) {
			i--;
			Data[i] = 0;
		}
	}
	else if (mid < maxIndex) {
		dataShift = maxIndex - padN;

		for (i=0; i < AlignT<RealT>::n + maxIndex - padN; i++) {
			Data[i] = Data[i - dataShift];
		}
		for ( ; i < AlignT<RealT>::n; i++) {
			Data[i] = 0;
		}
	}
	else {// maxIndex == 0
		dataShift = 0;
	}

	return dataShift;
}; // FftShiftT
