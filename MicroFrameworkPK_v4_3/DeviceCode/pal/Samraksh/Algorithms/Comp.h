/**
 * @file   Comp.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>

 For the moment we are not defining arithmetic for complex numbers.
 To do this right for fixed point requires more sophistication than
 we want to implement at first.

 The reason for doing this was to have this to avoid sucking in large
 portions of the Standard Template Library (STL).  Actually, some
 testing might be in order.  The STL complex class looks pretty streamlined.
 It might be that you don't suck in very much other code if you use
 <complex>.  Actually, this appears to be true for the MS compiler, but
 the GNU compiler seems to compile all the code in a file rather it is
 used or not.
 */

#ifndef _COMP_H_
#define _COMP_H_

#include "static_init.h"

#include "IntAttrib.h"

#if defined(SAM_IOSTREAM)
#include <iostream>
#endif

template<typename RealT>
class CompT {
public: // constructors
	CompT() : real(0), imag(0)
		{ /* Nothing */ };
	CompT(RealT Real, RealT Imag) : real(Real), imag(Imag)
		{ /* Nothing */ };

public: // assignment methods
	CompT<RealT>& operator = (CompT<RealT> const& Rhs);
	CompT<RealT>& operator <<= (int Shift);
	CompT<RealT>& operator >>= (int Shift);

public: // Accessor methods
	RealT Real() const
		{ return real; };
	RealT Imag() const
		{ return imag; };

	RealT& Real()
		{ return real; };
	RealT& Imag()
		{ return imag; };

public: // Logical operators
	bool operator == (CompT<RealT> const& Rhs) const
		{ return ((Rhs.real == real) && (Rhs.imag == imag)); };
	bool operator != (CompT<RealT> const& Rhs) const
		{ return ((Rhs.real != real) || (Rhs.imag != imag)); };

protected:
	RealT real, imag;
}; // CompT

#if defined(SAM_IOSTREAM)
/*
	Acquaintances (i.e., quasi-friends).
*/
template<typename RealT>
	inline std::ostream& operator <<
		(std::ostream &Stream, CompT<RealT> const& Value);

template<>
	inline std::ostream& operator << <Int8T>
		(std::ostream &Stream, CompT<Int8T> const& Value);

template<>
	inline std::ostream& operator << <UInt8T>
		(std::ostream &Stream, CompT<UInt8T> const& Value);
#endif // defined(SAM_IOSTREAM)

template<typename RealT>
	inline void ShiftData
		(CompT<RealT> Data[], unsigned Start, unsigned Len, int Shift);

/*
	Include inline code
*/
#include "Comp.hpp"

#endif /* _COMP_H_ */
