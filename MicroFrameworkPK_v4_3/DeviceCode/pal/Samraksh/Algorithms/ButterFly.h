/**
 * @file   ButterFly.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 *
 * ButterFlyAccT is the most accurate version with respect to internal
 * rounding, but it is also the slowest.
*/

#ifndef _BUTTERFLY_H_
#define _BUTTERFLY_H_

#include "static_init.h"

#include "Comp.h"

template<typename RealT>
class ButterFlyAccT {
public:
	/**
	 * @return the extra shift
	 */
	int operator ()
	(CompT<RealT> &V0, CompT<RealT> &V1, CompT<RealT> const& W, unsigned Shift);
}; // ButterFlyAccT

/*
  Include code for inlining
*/
#include "ButterFly.hpp"

#endif /* _BUTTERFLY_H_ */
