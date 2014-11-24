/*
 * @file   RunningMedian.h
 * @author Michael Andrew McGrath (michael.mcgrath@samraksh.com)
 * @date   Nov 6, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief Maintains a running median using a min heap and a max heap, with a circular buffer reference to the original order. Created per Kenneth Parker's API design document.
 */

#ifndef _RUNNINGMEDIAN_H_
#define _RUNNINGMEDIAN_H_

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"
#include "Comp.h"

#include "Heap.h"
#include "CircularBuffer.h"

template <typename IntElemT, size_t N, typename IntSumT>
class RunningMedianT {
private:
    size_t m_length;
    MinHeapT<IntElemT,IntElemT,N/2>* m_MinHeap;
    MaxHeapT<IntElemT,IntElemT,N/2>* m_MaxHeap;
    CircularBufferT<struct HeapDataT<IntElemT,IntElemT>*>* m_CircularBuffer;

    bool Balance(void);

public: // constructors
  RunningMedianT();
  RunningMedianT(IntElemT InitVal);
  RunningMedianT(IntElemT const InitVal[N]);

  void Add(IntElemT val);

  /**
   * returns median value.
   */
  IntElemT const& operator () (void);

  void Reset();
}; //RunningMedianT


#include "RunningMedian.hpp"

#endif /* _RUNNINGMEDIAN_H_ */
