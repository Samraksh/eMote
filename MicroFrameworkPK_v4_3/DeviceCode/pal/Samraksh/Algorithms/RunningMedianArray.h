/**
 * @file   RunningMedianArray.h
 * @date   Dec 8, 2014
 * @copyright (C) 2014 The Samraksh Company
 */

#ifndef RUNNINGMEDIANARRAY_H_
#define RUNNINGMEDIANARRAY_H_

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"
#include "Comp.h"

#include <stddef.h>


template <typename IntElemT, size_t N, typename IntSumT>
class RunningMedianArrayT {
public:
    short len;
    IntElemT data;
    UINT32* indexToData;
    UINT32* dataToIndex;

    // constructors
    RunningMedianArrayT();
    RunningMedianArrayT(IntElemT InitVal);
    RunningMedianArrayT(IntElemT const InitVal[N]);

    void Add(IntElemT val);
    void Add(UINT32 dataPos, IntElemT val);

    /**
     * returns median value.
     */
    IntElemT const& operator () (void);


    /**
     * Index
     */
    void InitIndex();
    void Swap(UINT32* arr, UINT32 pos0, UINT32 pos1);
    void swapIndex(UINT32 indexPos0, UINT32 indexPos1);
    bool outOfOrder(UINT32 indexPos0, UINT32 indexPos1);
    UINT32 Parent(UINT32 i);
    UINT32 FirstChild(UINT32 i);
    UINT32 MoveUp(UINT32 indexPos);
    UINT32 MoveDown(UINT32 indexPos);
};

#include "RunningMedianArray.hpp"

#endif /* RUNNINGMEDIANARRAY_H_ */
