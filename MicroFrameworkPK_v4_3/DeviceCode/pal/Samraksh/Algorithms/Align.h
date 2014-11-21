/**
 * @file   Align.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#ifndef _ALIGN_H_
#define _ALIGN_H_

#include "static_init.h"

#include "Comp.h"





template <typename RealT>
class AlignT {
public:
	AlignT(unsigned N);

	/**
	 * @return shift of FFT
	 */
	virtual int operator () (RealT Ref[], RealT Data[]) = 0;

protected:
	~AlignT();  //MAM: move dtor to protected b/c a base class destructor should be either public and virtual, or protected and non-virtual.

	unsigned n;
}; // AlignT

template <typename RealT, typename InvFftT>
class FftShiftT : public AlignT<RealT> {
public:
	FftShiftT(unsigned N, unsigned Ln2OfPadN);
	~FftShiftT();

	int operator () (RealT Ref[], RealT Data[]);

protected:
	unsigned ln2OfPadN;
	InvFftT invFft;
	CompT<RealT> *padData, *padRef, *prod;
}; // FftShift

#include "Align.hpp"

#endif /* _ALIGN_H_ */
