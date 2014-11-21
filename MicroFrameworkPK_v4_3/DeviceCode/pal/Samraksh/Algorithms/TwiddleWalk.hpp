/**
 * @file   TwiddleWalk.hpp
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */


#include "TwiddleWalk.h"

#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif
/*
	Helper functions
*/
template <typename RealT>
	inline CompT<RealT> PowOfW(unsigned K, unsigned N, double Scale)
{
	// Why is there not a native floor?
	#pragma warning(push)
	#pragma warning(disable:4244)
	// very very slow, crazzy slow, but OK because you only do it once
	RealT real = floor(Scale * cos(-2*M_PI * K / double(N)) + 0.5);
	RealT imag = floor(Scale * sin(-2*M_PI * K / double(N)) + 0.5);
	#pragma warning(pop)

	return CompT<RealT>(real,imag);
}; // PowOfW

/*
	Constructor
*/
template<typename RealT>
	TwidWalkT<RealT>::TwidWalkT(unsigned Ln2OfN) : ln2OfN(Ln2OfN)
		{ /* Nothing */ };

/*
	Bit walk code
*/
template<typename RealT>
	BitWalkT<RealT>::BitWalkT(unsigned Ln2OfN) : TwidWalkT<RealT>(Ln2OfN)
{
//	typedef typename TwidWalkT<RealT> BT; // For circa 2001 compilers

	wToPowOf2 = new CompT<RealT>[TwidWalkT<RealT>::ln2OfN];
	partial = new CompT<RealT>[TwidWalkT<RealT>::ln2OfN];
	bit = new bool[TwidWalkT<RealT>::ln2OfN];

	for (unsigned powOf2 = 0; powOf2 < TwidWalkT<RealT>::ln2OfN; powOf2++)
		wToPowOf2[powOf2] = PowOfW<RealT>((1 << powOf2), (1 << TwidWalkT<RealT>::ln2OfN), Scale());
}; // BitWalkT

template<typename RealT>
	BitWalkT<RealT>::~BitWalkT()
{
	delete [] bit;
	delete [] partial;
	delete [] wToPowOf2;
}; // ~BitWalkT

template<typename RealT>
	CompT<RealT> BitWalkT<RealT>::operator () () const
		{ return partial[0]; };

/*
	Without expresion templates P = F1*F2 becomes inefficent for a simple
	complex pair.	This is probably a good reason to use the STL complex.
*/
template<typename RealT>
	void BitWalkT<RealT>::Prod
		(CompT<RealT> const& fact0, CompT<RealT> const& fact1, CompT<RealT> &prod) const
{
	typedef typename IntAttribT<RealT>::MultT MultT;

	MultT prodReal = MultT(fact0.Real()) * MultT(fact1.Real());
	MultT prodImag = MultT(fact0.Imag()) * MultT(fact1.Imag());

	MultT crossAReal = MultT(fact0.Real()) * MultT(fact1.Imag());
	MultT crossAImag = MultT(fact0.Imag()) * MultT(fact1.Real());

	prod.Real() = (prodReal - prodImag) >> Shift();
	prod.Imag() = (crossAReal + crossAImag) >> Shift();
}; // Prod

template<typename RealT>
void BitWalkT<RealT>::Start()
{
// typedef typename TwidWalkT<RealT> BT; // For circa 2001 compilers

	for (unsigned i=0; i < TwidWalkT<RealT>::ln2OfN; i++) {
		bit[i] = false;
		partial[i] = CompT<RealT>(Scale(),0);
	}
} // Start

/*
	Need bounds check.
*/
template<typename RealT>
	void BitWalkT<RealT>::Next()
{
	CompT<RealT> &w = *partial;

	unsigned rollOverPoint = 0;
	while ( bit[rollOverPoint] )
		rollOverPoint++;

//	typedef typename TwidWalkT<RealT> BT; // For circa 2001 compilers
	assert(rollOverPoint < (TwidWalkT<RealT>::ln2OfN - 1));	// bounds check

	bit[rollOverPoint] = true;
	Prod(partial[rollOverPoint], wToPowOf2[TwidWalkT<RealT>::ln2OfN - rollOverPoint - 2], w);

	for (unsigned i=0; i < rollOverPoint; i++)
		bit[i] = false;
	for (unsigned i=1; i < rollOverPoint; i++)
		partial[i] = w;
}; // Next

template<typename RealT>
	unsigned BitWalkT<RealT>::Shift() const
		{ return IntAttribT<RealT>::NumBits() - 2; };

template<typename RealT>
	RealT BitWalkT<RealT>::Scale() const
		{ return RealT(1 << Shift()); };

