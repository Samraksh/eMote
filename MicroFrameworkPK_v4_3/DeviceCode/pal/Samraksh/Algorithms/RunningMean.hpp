/**
 * @file   RunningMean.hpp
 * @date   Nov 6, 2014
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @copyright (c) 2014 The Samraksh Company
 * @brief Maintains running mean on input of size N. Created per Kenneth Parker's API design document.
 */

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"

#include "RunningMean.h"

/*
 * Constructors
 */
template <typename IntElemT, size_t N, typename IntSumT>
  RunningMeanT<IntElemT,N,IntSumT>::RunningMeanT(IntElemT InitVal)
{
    m_CircularBuffer = new CircularBufferT<IntElemT>(N);
    m_CircularBuffer->Push(InitVal);
    m_sum = InitVal;
    m_length = 0; //BK: Start counting zero since we pop element when it reaches N
};

template <typename IntElemT, size_t N, typename IntSumT>
  RunningMeanT<IntElemT,N,IntSumT>::RunningMeanT(IntElemT const (&InitVal)[N])
{
    m_CircularBuffer = new CircularBufferT<IntElemT>(N);
    m_sum = 0;
    for(int itr = 0; itr < N; ++itr) {
        m_sum += InitVal[itr];
        m_CircularBuffer->Push(InitVal[itr]);
    }
    m_length = N;
};

template <typename IntElemT, size_t N, typename IntSumT>
  RunningMeanT<IntElemT,N,IntSumT>::~RunningMeanT() {
    delete m_CircularBuffer;
}

/*
 * Core Computation
 */
template <typename IntElemT, size_t N, typename IntSumT>
  void RunningMeanT<IntElemT,N,IntSumT>::Push(IntElemT elem)
{
    if(m_length == N) {
        IntElemT lastElem;
        m_CircularBuffer->Pop(lastElem);
        m_sum -= lastElem;
    }
    m_CircularBuffer->Push(elem);
    m_sum += elem;
    if(m_length < N) {
        ++m_length;
    }
};

template <typename IntElemT, size_t N, typename IntSumT>
  const IntElemT& RunningMeanT<IntElemT,N,IntSumT>::operator () (void)
{
    m_mean = (IntElemT)(m_sum / m_length);
    return m_mean;
};

