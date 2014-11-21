/**
 * @file   Util.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @brief  These are small helper functions that are meant to be in-lined.
*/

#ifndef _UTIL_H_
#define _UTIL_H_

#define _USE_MATH_DEFINES
#include <math.h>

/*
	Multiple argument Max
*/
template<typename T>
	inline T Max(T A, T B)
		{ return (A < B) ? B : A; }

template<typename T>
	inline T Max(T A, T B, T C)
{
	T abMax = Max(A,B);
	return (abMax < C) ? C : abMax;
} // Max(T,T,T)

template<typename T>
	inline T Max(T A, T B, T C, T D)
{
	T abcMax = Max(A,B,C);
	return (abcMax < D) ? D : abcMax;
} // Max(T,T,T,T)

/*
	Multiple argument Min
*/
template<typename T>
	inline T Min(T A, T B)
		{ return (A < B) ? A : B; }

template<typename T>
	inline T Min(T A, T B, T C)
{
	T abMin = Min(A,B);
	return (abMin < C) ? abMin : C;
} // Min(T,T,T)

template<typename T>
	inline T Min(T A, T B, T C, T D)
{
	T abcMin = Min(A,B,C);
	return (abcMin < D) ? abcMin : D;
} // Min(T,T,T,T)

/*
	Range function
*/
template<typename T>
	inline bool InRangeClose(T Num, T Low, T High)
		{ return (Low <= Num) && (Num <= High); }

template<typename T>
	inline bool InRangeOpen(T Num, T Low, T High)
		{ return (Low < Num) && (Num < High); }

/*
	Math functions
*/
template <typename T>
	T Abs(T Num)
		{ return (Num < 0) ? -Num : Num; }

/*
	Swap
*/
template<typename T>
	inline void Swap(T &A, T &B)
{
	T temp = A;
	A = B;
	B = temp;
} // Swap

/*
	Still to be done
*/
template<typename ElemT>
	int IndexOfMax(ElemT *Data, unsigned Len);
template<typename ElemT>
	int IndexOfMin(ElemT *Data, unsigned Len);

template<typename SampT, typename FracT>
	void QuadPeakEst(SampT Left, SampT Peak, SampT Right, FracT &Shift);

#endif
