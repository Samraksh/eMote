/**
 * @file   FFT.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#ifndef _FFT_H_
#define _FFT_H_

#include "Comp.h"

/*
	Complex valued FFTs, fixed length
*/
template <typename RealT, typename ButterFlyT, typename TwidleWalkT>
class FftCompFixLenT {
public: // Accessor and type functions for users of this class
	unsigned Ln2OfN() const
		{ return twidleWalk.Ln2OfN(); };

public: // The constructor
	FftCompFixLenT(unsigned Ln2OfN);

public: // The main operations
	int operator () (CompT<RealT> Data[]); // returns the shift

private: // Internal operations
	int CompStage(CompT<RealT> Data[], unsigned outStageNum); // returns incremental shift
	void CompGroup(CompT<RealT> Data[], unsigned group, unsigned groupSize, int &Shift);
	void UnScram(CompT<RealT> Data[]);

protected: // Data
	TwidleWalkT twidleWalk;
}; // FftCompFixLenT

/*
	Inverse FFT, fixed length
*/
template <typename RealT, typename FftT>
class InvFftCompFixLenT {
public:
	InvFftCompFixLenT(unsigned Ln2OfN);

public:
	// return bit-shift
	int For(CompT<RealT> Data[]);
	int Inv(CompT<RealT> Data[]);

protected:
	// return bit-shift
	int Conj(CompT<RealT> Data[]);

protected:
	FftT fft;
}; // InvFftCompFixLenT

/*
	Include code for inlining
*/
#include "FFT.hpp"

#endif /* _FFT_H_ */
