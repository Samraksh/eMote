/*
 * @file   RunningMedian.hpp
 * @author Michael Andew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 6, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief Maintains a running median using a min heap and a max heap, with a circular buffer reference to the original order. Created per Kenneth Parker's API design document.
 */

#ifndef _RUNNINGMEDIAN_HPP_
#define _RUNNINGMEDIAN_HPP_

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"
#include "Comp.h"

#include "Heap.h"
#include "CircularBuffer.h"

#include "RunningMedian.h"

/*
 * Constructors
 */
template <typename IntElemT, size_t N, typename IntSumT>
  RunningMedianT<IntElemT,N,IntSumT>::RunningMedianT(IntElemT InitVal)
{
    m_length = 0;
    m_MinHeap = new MinHeapT<IntElemT,IntElemT,N/2>::MinHeapT();
    m_MaxHeap = new MaxHeapT<IntElemT,IntElemT,N/2>::MinHeapT();
    m_CircularBuffer = new CircularBufferT<struct HeapDataT<IntElemT,IntElemT>*>::CircularBufferT(N);
};

template <typename IntElemT, size_t N, typename IntSumT>
  RunningMedianT<IntElemT,N,IntSumT>::RunningMedianT(IntElemT InitVal)
{
    RunningMedianT();
    Add(InitVal);
};

/**
 * @todo use the heapify algorithm in CLRS Introduction to Algorithms to make
 * this O(N) instead of O(N*lg(N)).  Except here we have two heaps.  Ideally,
 * it may be faster to roughly approximate the partitioning knowing that we
 * have an expected median, then partition using the O(N) heapify and repeat,
 * however need to profile the worst-case which could be O(N^2)?
 */
template <typename IntElemT, size_t N, typename IntSumT>
  RunningMedianT<IntElemT,N,IntSumT>::RunningMedianT(IntElemT const InitVal[N])
{
    RunningMedianT(InitVal[0]);
    for(int itr = 1; itr < N; ++itr) {
        Add(InitVal[itr]);
    }
};

/**
 * Core Computation
 */
template <typename IntElemT, size_t N, typename IntSumT>
  void RunningMedianT<IntElemT,N,IntSumT>::Add(IntElemT val)
{
    size_t size_max = m_MaxHeap->m_size;
    size_t size_min = m_MinHeap->m_size;
    IntElemT median;
    struct HeapDataT<IntElemT,IntElemT> temp_heap;
    struct HeapDataT<IntElemT,IntElemT>* ptr_heap;
    struct HeapDataT<IntElemT,IntElemT>** ptr_cbuf_heap;
    if(m_length == 0)
    {
        m_MaxHeap->Insert(val, ptr_heap);
        m_CircularBuffer->Push(ptr_heap, ptr_cbuf_heap);
        ptr_heap->ptr = ptr_cbuf_heap;
        ++m_length;
    }
    else if(m_length >= N) {
        //get lowest node on the graph.
        //remove node based on circular buffer.
        //replace with lowest node on the graph unless it was lowest node)
        // call MinHeapifyParent or MinHeapify Children
        // do some check on whether node needs to jump across heaps.
        // finally insert?
        m_CircularBuffer->Pop(ptr_heap);
        IntElemT deleted_val = ptr_heap->val;
        ptr_heap->val = val;
        m_CircularBuffer->Push(ptr_heap, ptr_cbuf_heap);
        ptr_heap->ptr = ptr_cbuf_heap;
        //m_length = m_length;

        if(m_MinHeap->isMember(ptr_heap)) {
            if(ptr_heap->val > deleted_val) {
                m_MinHeap->MinRuleCheckChildren(ptr_heap);
            }
            else if(ptr_heap->val < deleted_val) {
                m_MinHeap->MinRuleCheckParent(ptr_heap);
            }
            // do nothing if equal.
        }
        else {
            //TODO: ASSERT(m_MaxHeap->isMember(ptr_heap))
            if(ptr_heap->val < deleted_val) {
                m_MaxHeap->MaxRuleCheckChildren(ptr_heap);
            }
            else if(ptr_heap->val > deleted_val) {
                m_MaxHeap->MaxRuleCheckParent(ptr_heap);
            }
        }
    }
    else {
        /** insert based on where it should go, then balance. in this model,
         * there are two extra children allocated so the inserted node can
         * fall all the way to the bottom of the heap before being corrected
         * by the balance operation.
         ** In the API design document, the correction happens inside
         * this function. That should be explored for better performance, but
         * right now I'm playing for implementation speed.
         */
        median = this();
        if(val < median) {
            m_MaxHeap->InsertMax(val, ptr_heap);
            m_CircularBuffer->Push(ptr_heap, ptr_cbuf_heap);
            ptr_heap->ptr = ptr_cbuf_heap;
            ++m_length;
        }
        else {
            m_MinHeap->InsertMin(val, ptr_heap);
            m_CircularBuffer->Push(ptr_heap, ptr_cbuf_heap);
            ptr_heap->ptr = ptr_cbuf_heap;
            ++m_length;
        }
        Balance();
    }
};

template <typename IntElemT, size_t N, typename IntSumT>
  IntElemT const& RunningMedianT<IntElemT,N,IntSumT>::operator () (void)
{
    size_t size_max = m_MaxHeap->m_size;
    size_t size_min = m_MinHeap->m_size;
    IntElemT median;
    if(size_max > size_min)
    {
        m_MaxHeap->getMax(median);
    }
    else if(size_min > size_max){
        m_MinHeap->getMin(median);
    }
    else {
        //TODO: varies depending on chosen mathematical definition of "median".
        IntElemT maxHeapVal;
        m_MaxHeap->getMax(maxHeapVal);
        IntElemT minHeapVal;
        m_MinHeap->getMin(minHeapVal);
        median = (maxHeapVal + minHeapVal)/2;
    }
    return median;
};

template <typename IntElemT, size_t N, typename IntSumT>
  bool RunningMedianT<IntElemT,N,IntSumT>::Balance (void)
{
    bool fRet = true;
    size_t size_max = m_MaxHeap->m_size;
    size_t size_min = m_MinHeap->m_size;
    struct HeapDataT<IntElemT,IntElemT>* ptr_heap;
    struct HeapDataT<IntElemT,IntElemT>** ptr_cbuf_heap;
    if(abs(size_max-size_min)>=2) {
        if(size_max > size_min)
        {
            struct HeapDataT<IntElemT,IntElemT> s_data;
            m_MaxHeap->DeleteMax(s_data);
            ptr_heap = m_MinHeap->Insert(s_data.val);
            *(s_data->ptr) = ptr_heap;
            ptr_heap->ptr = s_data->ptr;
        }
        else {
            struct HeapDataT<IntElemT,IntElemT> s_data;
            m_MinHeap->DeleteMin(s_data);
            ptr_heap = m_MaxHeap->Insert(s_data.val);
            *(s_data->ptr) = ptr_heap;
            ptr_heap->ptr = s_data->ptr;
        }
    }
    else {
        //Do nothing.
    }
    //TODO: integrity assertions.
    return fRet;
};

template <typename IntElemT, size_t N, typename IntSumT>
  void RunningMedianT<IntElemT,N,IntSumT>::Reset (void)
{
    m_length = 0;
    m_MinHeap->Reset();
    m_MaxHeap->Reset();
    m_CircularBuffer->Reset();
}

#endif /* _RUNNINGMEDIAN_HPP_ */
