/**
 * @file   DataTypes.h
 * @author Kenneth Parker
 * @date   before Aug 29, 2007
 * @copyright (C) 2014 The Samraksh Company
 * @par    Maintainer: Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 */

#ifndef _DATATYPES_H_
#define _DATATYPES_H_

#include "static_init.h"

#include "IntAttrib.h"

#define MAXNUMENDS NUMSAMPLES
//#define NUMSAMPLES 704  //-6.4:16
//#define NUMSAMPLES 1568 //-10:30
//#define NUMSAMPLES 2528 //-10:40
//#define NUMSAMPLES 1888 //-10:50
#define NUMSAMPLES 2208 //-10:60
//#define NUMSAMPLES 2816 //-10:80
//#define NUMSAMPLES 3456 //-10:100
//#define NUMSAMPLES 5024 //-10:150
//#define NUMSAMPLES 50 //for testing

extern "C" {
typedef struct out{
	UInt16T netstart; //starting point of bin
	UInt16T netend; //end point of track
	UInt16T displaypath;
	UInt16T minhist;
	UInt16T maxhist;
	UInt16T numends;
	UInt16T end[MAXNUMENDS];
} displaytrack;
}
#endif /* _DATATYPES_H_ */
