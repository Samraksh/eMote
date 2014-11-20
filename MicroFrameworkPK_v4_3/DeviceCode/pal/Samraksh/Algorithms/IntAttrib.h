/**
 * @file   IntAttrib.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#ifndef _INTATTRIB_H_
#define _INTATTRIB_H_

#include "static_init.h"

/*
	Rename the native integer types
*/
typedef signed char Int8T;
typedef unsigned char UInt8T;
typedef short Int16T;
typedef unsigned short UInt16T;
typedef int Int32T;
typedef unsigned UInt32T;

/*
	Define the virtual base classes for the integer attribute classes
*/
template <typename NativeT>
class IntAttribT;

template <typename NativeT, unsigned NumBitArg>
class SignedAttribT {
public:
	static bool Signed()
		{ return true; };
	static NativeT Min()
		{ return ( -(1 << (NumBits() - 1)) ); };
	static NativeT Max()
		{ return ((1 << (NumBits() -1)) - 1); };
	static unsigned NumBits()
		{ return NumBitArg; }
}; // SignedAttribT

template <typename NativeT, unsigned NumBitArg>
class UnSignedAttribT {
public:
	static bool Signed()
		{ return false; };
	static NativeT Min()
		{ return (0); };
	static NativeT Max()
		{ return (1 << NumBits() - 1); };
	static unsigned NumBits()
		{ return NumBitArg; }
}; // UnSignedAttribT

/*
	Do the specializations
*/
// Int8T
template <>
class IntAttribT<Int8T> : public SignedAttribT<Int8T,8> {
public: // Related Types
	typedef Int16T MultT;
	typedef Int16T AddT;
	typedef Int16T NextT;
};

// UInt8T
template <>
class IntAttribT<UInt8T> : public UnSignedAttribT<Int8T,8> {
public: // Related Types
	typedef UInt16T MultT;
	typedef UInt16T AddT;
	typedef UInt16T NextT;
};

// Int16T
template <>
class IntAttribT<Int16T> : public SignedAttribT<Int16T,16> {
public: // Related Types
	typedef Int32T MultT;
	typedef Int32T AddT; // Would like an Int24T
	typedef Int32T NextT;
};

// UInt16T
template <>
class IntAttribT<UInt16T> : public UnSignedAttribT<Int16T,16> {
public: // Related Types
	typedef UInt32T MultT;
	typedef UInt32T AddT;
	typedef UInt32T NextT;
};

// Int32T
template<>
class IntAttribT<Int32T> : public SignedAttribT<Int32T,32> {
public: // Related Types
	typedef long long MultT;
	typedef long long AddT; // Would like an Int36T
	typedef long long NextT;
};

// UInt32T
template <>
class IntAttribT<UInt32T> : public UnSignedAttribT<Int32T,32> {
public: // Related Types
	typedef unsigned long long MultT;
	typedef unsigned long long AddT;
	typedef unsigned long long Next;
};

#endif /* _INTATTRIB_H_ */


