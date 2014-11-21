/**
 * @file   Comp.hpp
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#include "Comp.h"

/*
	Assignment
*/
template<typename RealT>
	inline CompT<RealT>& CompT<RealT>::operator = (CompT<RealT> const& Rhs)
{
	real = Rhs.real;
	imag = Rhs.imag;

	return *this;
}; // operator =

/*
	Bit shift operators
*/
template<typename RealT>
	inline CompT<RealT>& CompT<RealT>::operator <<= (int Shift)
{
	real <<= Shift;
	imag <<= Shift;

	return *this;
}; // operator <<=

template<typename RealT>
	inline CompT<RealT>& CompT<RealT>::operator >>= (int Shift)
{
	real >>= Shift;
	imag >>= Shift;

	return *this;
}; // operator >>=

#if defined(SAM_IOSTREAM)
/*
	Stream operators
*/
template<typename RealT>
	inline std::ostream& operator << (std::ostream &Stream, CompT<RealT> const& Value)
{
	Stream << '(' << int(Value.Real()) << ',' << int(Value.Imag()) << ')';
	return Stream;
}; // operation <<

// Need formating for char types to print them as integers
template<>
	inline std::ostream& operator << <Int8T>
		(std::ostream &Stream, CompT<Int8T> const& Value)
{
	Stream << '(' << int(Value.Real()) << ',' << int(Value.Imag()) << ')';
	return Stream;
}; // operation <<

template<>
	inline std::ostream& operator << <UInt8T>
		(std::ostream &Stream, CompT<UInt8T> const& Value)
{
	Stream << '(' << unsigned(Value.Real()) << ',' << unsigned(Value.Imag()) << ')';
	return Stream;
}; // operation <<
#endif // defined(SAM_IOSTREAM)

