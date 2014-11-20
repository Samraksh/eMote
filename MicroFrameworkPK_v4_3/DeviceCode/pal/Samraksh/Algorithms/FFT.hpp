/**
 * @file   FFT.hpp
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @brief  Inline code for the Fft module.
 */

#ifndef _FFT_HPP_
#define _FFT_HPP_

#include "static_init.h"

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"

//#include <algorithm>
#include <assert.h>
#include <stdio.h>

/*
	Constructors
*/
template <typename RealT, typename ButterFlyT, typename TwidleWalkT>
	FftCompFixLenT<RealT,ButterFlyT,TwidleWalkT>::FftCompFixLenT(unsigned Ln2OfN) :
		twidleWalk(Ln2OfN)
	{ /* Nothing */ };

 /*
	Core Computation
*/
template <typename RealT, typename ButterFlyT, typename TwidleWalkT>
	int FftCompFixLenT<RealT,ButterFlyT,TwidleWalkT>::operator () (CompT<RealT> Data[])
{
	int shift = 0;

	for (unsigned i=0; i < Ln2OfN(); i++) {
		int incShift = CompStage(Data, i+1);
		shift += incShift;
	}
	UnScram(Data);

	return shift;
}; // operator ()

template <typename RealT, typename ButterFlyT, typename TwidleWalkT>
	int FftCompFixLenT<RealT,ButterFlyT,TwidleWalkT>::CompStage
		(CompT<RealT> Data[], unsigned OutStageNum)
{
	const unsigned numGroup = (1 << (OutStageNum - 1));
	const unsigned groupSize = (1 << (twidleWalk.Ln2OfN() - (OutStageNum - 1)));

	int shift = 0;
	twidleWalk.Start();
	CompGroup(Data, 0,groupSize, shift);

	for (unsigned group = 1; group < numGroup; group++) {
		twidleWalk.Next();
		CompGroup(Data, group,groupSize, shift);
	}

	return shift;
}; // CompStage

template <typename RealT, typename ButterFlyT, typename TwidleWalkT>
	void FftCompFixLenT<RealT,ButterFlyT,TwidleWalkT>::CompGroup
		(CompT<RealT> Data[], unsigned Group, unsigned GroupSize, int &Shift)
{
	const unsigned opPerGroup = (GroupSize >> 1); // ButterFly operations
	const unsigned halfGroupSize = opPerGroup;

	ButterFlyT ButterFly;
	CompT<RealT> twidle = twidleWalk();

	unsigned groupStart = Group*GroupSize;
	for (unsigned opNum=0; opNum < opPerGroup; opNum++) {
		unsigned start = groupStart + opNum;

		int incShift =
			ButterFly(Data[start], Data[start + halfGroupSize], twidle, Shift);

		if (0 < incShift) {
			ShiftData<CompT<RealT> >(Data, 0,start, incShift); // both
			ShiftData<CompT<RealT> >(Data, groupStart + halfGroupSize,opNum, incShift);
			Shift += incShift;
		}
	}
} // CompGroup

template <typename RealT, typename ButterFlyT, typename TwidleWalkT>
	void FftCompFixLenT<RealT,ButterFlyT,TwidleWalkT>::UnScram(CompT<RealT> Data[])
{
	unsigned ln2OfN = twidleWalk.Ln2OfN();

	bool *bit = new bool[ln2OfN];
	unsigned *partial = new unsigned[ln2OfN];
	unsigned *rev = &(partial[0]);

	// Initialize
	for (unsigned i=0; i < ln2OfN; i++) {
		bit[i] = false;
		partial[i] = 0;
	}

	// Count
	const unsigned n = (1 << ln2OfN);
	for (unsigned i=1; i < n; i++) {
		unsigned rollOverPoint = 0;
		while (bit[rollOverPoint])
			rollOverPoint++;
		assert(rollOverPoint < ln2OfN);

		bit[rollOverPoint] = true;
		*rev = partial[rollOverPoint] + (1 << (ln2OfN - rollOverPoint - 1));

		for (unsigned j = 0; j < rollOverPoint; j++)
			bit[j] = false;
		for (unsigned j = 1; j < rollOverPoint; j++)
			partial[j] = *rev;

		if (i < *rev)
			Swap(Data[i], Data[*rev]);
	}

	delete [] bit;
	delete [] partial;
} // UnScram

/**
 *	Inverse FFT, fixed length
*/
template <typename RealT, typename FftT>
	InvFftCompFixLenT<RealT,FftT>::InvFftCompFixLenT(unsigned Ln2OfN) :
		fft(Ln2OfN)
	{ /* Nothing; */ };

template <typename RealT, typename FftT>
	int InvFftCompFixLenT<RealT,FftT>::For(CompT<RealT> Data[])
{
	int shift = fft(Data);
	return shift;
} // For

template <typename RealT, typename FftT>
	int InvFftCompFixLenT<RealT, FftT>::Inv(CompT<RealT> Data[])
{
	int shift = Conj(Data);
	shift += fft(Data);
	shift += Conj(Data);

	return shift;
} // Inv

template <typename RealT, typename FftT>
	int InvFftCompFixLenT<RealT,FftT>::Conj(CompT<RealT> Data[])
{
	const unsigned n = (1 << fft.Ln2OfN());

	int shift = 0;
//	typedef typename RealT RealT;
	const RealT min = (1 << (IntAttribT<RealT>::NumBits() - 1));

	unsigned i;
	for (i = 0; i < n; i++) {
		if (Data[i].Imag() == min) {
			shift = 1;
			for (unsigned j=0; j < i; j++)
				Data[j].Imag() >>= 1;
			Data[i].Imag() = -(Data[i].Imag() >> 1);
			break; // Not using a goto changes loop from 4 ops to 5 ops.
		}

		Data[i].Imag() = -Data[i].Imag();
	}

	for (i++ ; i < n; i++)
		Data[i].Imag() = -(Data[i].Imag() >> shift);

	return shift;
} // Conj

#endif
