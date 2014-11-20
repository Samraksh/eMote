/**
 * @file   TwiddleWalk.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#ifndef _TWIDDLE_WALK_H_
#define _TWIDDLE_WALK_H_

#include "Comp.h"

template<typename RealT>
class TwidWalkT {
public: // Core OO interface
	TwidWalkT(unsigned Ln2OfN);

	virtual void Start() = 0;
	virtual void Next() = 0;

	virtual CompT<RealT> operator () () const = 0;

public: // Accessor method
	unsigned Ln2OfN() const
		{ return ln2OfN; };

protected:
	unsigned ln2OfN;
}; // TwidWalkT

template<typename RealT>
class BitWalkT : public virtual TwidWalkT<RealT> {
public:
	BitWalkT(unsigned Ln2OfN);
	~BitWalkT();

public: // virtual functions
	void Start();
	void Next();
	CompT<RealT> operator () () const;

private:
	unsigned Shift() const;
	RealT Scale() const;

	// Should eventually be replaced by funciton templates
	void Prod
		(CompT<RealT> const& fact0, CompT<RealT> const& fact1,
		 CompT<RealT> &prod) const;

protected:
	bool *bit;
	CompT<RealT> *partial;
	CompT<RealT> *wToPowOf2;
}; // BitWalkT


/*
	Include code for inlining
*/
#include "TwiddleWalk.hpp"

#endif /* _TWIDDLE_WALK_H_ */
