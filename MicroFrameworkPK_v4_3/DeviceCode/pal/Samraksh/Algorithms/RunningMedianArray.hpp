/**
 * @file   RunningMedianArray.hpp
 * @date   Dec 8, 2014
 * @copyright (C) 2014 The Samraksh Company
 */

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"
#include "Comp.h"

#include <stddef.h>

#include "RunningMedianArray.h"

template <typename IntElemT, size_t N, typename IntSumT>
  void RunningMedianArrayT<IntElemT, N, IntSumT>::InitIndex()
{
    for (UINT32 i = 0; i < N; ++i)
    {
        indexToData[i] = dataToIndex[i] = i;
        data[i] = 0;
    }
}

template <typename IntElemT, size_t N, typename IntSumT>
  void RunningMedianArrayT<IntElemT, N, IntSumT>::Swap(UINT32* arr, UINT32 pos0, UINT32 pos1)
{
    IntElemT temp;
    temp = arr[pos0];
    arr[pos0] = arr[pos1];
    arr[pos1] = temp;
}

template <typename IntElemT, size_t N, typename IntSumT>
  void RunningMedianArrayT<IntElemT, N, IntSumT>::swapIndex(UINT32 indexPos0, UINT32 indexPos1)
{
    Swap(indexToData, indexPos0, indexPos1);
    Swap(dataToIndex, indexToData[indexPos0], indexToData[indexPos1]);
}

template <typename IntElemT, size_t N, typename IntSumT>
  bool RunningMedianArrayT<IntElemT, N, IntSumT>::outOfOrder(UINT32 indexPos0, UINT32 indexPos1)
{
    bool result;
    UINT32 dataPos0, dataPos1;
    IntElemT value0, value1;

    dataPos0 = indexToData[indexPos0];
    dataPos1 = indexToData[indexPos1];
    value0 = data[dataPos1];
    value1 = data[dataPos1];

    result = (value0 > value1);
    return result;
}

template <typename IntElemT, size_t N, typename IntSumT>
UINT32  RunningMedianArrayT<IntElemT, N, IntSumT>::Parent(UINT32 i)
{
    return (((i + 1) >> 1) - 1);
}

template <typename IntElemT, size_t N, typename IntSumT>
UINT32  RunningMedianArrayT<IntElemT, N, IntSumT>::FirstChild(UINT32 i)
{
    return (((i + 1) >> 1) - 1);
}

template <typename IntElemT, size_t N, typename IntSumT>
UINT32  RunningMedianArrayT<IntElemT, N, IntSumT>::MoveUp(UINT32 indexPos)
{
    short upIndex0, upIndex1, upIndex, newIndexPos, minUpper, maxLower;
    bool outOfOrder0, outOfOrder1;

    //TODO: static assert N > 2?
    minUpper = (UINT32)(N >> 1);
    maxLower = (UINT32)((N - 1) >> 1);

    if (indexPos < minUpper)
    {
        if (indexPos == maxLower)
            upIndex = indexPos++;
        else
            upIndex = (short)(maxLower - Parent((short)(maxLower - indexPos)));

        if (outOfOrder(indexPos, upIndex))
        {
            swapIndex(indexPos, upIndex);
            newIndexPos = upIndex;
        }
        else
            newIndexPos = indexPos;
    }

    else
    { // (minUppper <= indexPos)
        upIndex0 = (short)(FirstChild((short)(indexPos - minUpper)) + minUpper);
        upIndex1 = upIndex0++;

        outOfOrder0 = ((upIndex0 < N) && outOfOrder(indexPos, upIndex0));
        outOfOrder1 = ((upIndex1 < N) && outOfOrder(indexPos, upIndex1));

        if (outOfOrder0 && outOfOrder1)
        {
            upIndex = outOfOrder(upIndex0, upIndex1) ? upIndex1 : upIndex0;
            swapIndex(indexPos, upIndex);
            newIndexPos = upIndex;
        }
        else if (outOfOrder0)
        {
            swapIndex(indexPos, upIndex0);
            newIndexPos = upIndex0;
        }
        else if (outOfOrder1)
        {
            swapIndex(indexPos, upIndex1);
            newIndexPos = upIndex1;
        }
        else
            newIndexPos = indexPos;
    }

    return newIndexPos;
}

template <typename IntElemT, size_t N, typename IntSumT>
UINT32  RunningMedianArrayT<IntElemT, N, IntSumT>::MoveDown(UINT32 indexPos)
{
    UINT32 downIndex0, downIndex1, downIndex, newIndexPos, minUpper, maxLower;
    bool outOfOrder0, outOfOrder1;

    minUpper = (UINT32)(N >> 1);
    maxLower = (UINT32)((N - 1) >> 1);

    if (maxLower < indexPos)
    {
        if (indexPos == minUpper)
            downIndex = indexPos--;
        else
            downIndex = (short)(Parent((short)(indexPos - minUpper)) + minUpper);

        if (outOfOrder(downIndex,indexPos))
        {
            swapIndex(downIndex,indexPos);
            newIndexPos = downIndex;
        }
        else
            newIndexPos = indexPos;
    }
    else
    { // (indexPos <= maxLower)
        downIndex0 = (UINT32)(maxLower - FirstChild((UINT32)(maxLower - indexPos)));
        downIndex1 = downIndex0--;

        outOfOrder0 = ((0 <= downIndex0) && outOfOrder(downIndex0,indexPos));
        outOfOrder1 = ((0 <= downIndex1) && outOfOrder(downIndex1,indexPos));

        if ( outOfOrder0 && outOfOrder1 )
        {
            downIndex = outOfOrder(downIndex0,downIndex1) ? downIndex0 : downIndex1;
            swapIndex(indexPos,downIndex);
            newIndexPos = downIndex;
        }
        else if ( outOfOrder0 )
        {
            swapIndex(indexPos,downIndex0);
            newIndexPos = downIndex0;
        }
        else if ( outOfOrder1 )
        {
            swapIndex(indexPos,downIndex1);
            newIndexPos = downIndex1;
        }
        else
            newIndexPos = indexPos;
    }

    return newIndexPos;
}

/*
 * Constructors
 */
template <typename IntElemT, size_t N, typename IntSumT>
  RunningMedianArrayT<IntElemT,N,IntSumT>::RunningMedianArrayT()
{
    len = N;
    data = new IntElemT[N];
    dataToIndex = new UINT32[N];
    indexToData = new UINT32[N];
    InitIndex();
}

/**
 * Core Computation
 */
template <typename IntElemT, size_t N, typename IntSumT>
  void RunningMedianArrayT<IntElemT,N,IntSumT>::Add(UINT32 dataPos, IntElemT val)
{
    UINT32 indexPos, newIndexPos;
    data[dataPos] = val;
    indexPos = dataToIndex[dataPos];
    newIndexPos = MoveUp(indexPos);
    if (newIndexPos != indexPos)
    {
        do
        {
            indexPos = newIndexPos;
            newIndexPos = MoveUp(indexPos);
        } while (indexPos != newIndexPos);
    }
    else
    {
        newIndexPos = MoveDown(indexPos);
        while (newIndexPos != indexPos)
        {
            indexPos = newIndexPos;
            newIndexPos = MoveDown(indexPos);
        }
    }
}

/**
 * getMedian
 */
template <typename IntElemT, size_t N, typename IntSumT>
  IntElemT const& RunningMedianArrayT<IntElemT,N,IntSumT>::operator () (void)
{
    IntElemT med;
    UINT32 dataPos, dataPos0, dataPos1;
    IntElemT result;

    med = (IntElemT)(N/2);
    if (( N % 2) == 1 )
    {
        dataPos = indexToData[med];
        result = (data[dataPos]);
    }
    else
    {
        dataPos0 = indexToData[med];
        dataPos1 = indexToData[med - 1];
        result = (IntElemT)((data[dataPos0] + data[dataPos1])/2);
    }
    return result;
}

