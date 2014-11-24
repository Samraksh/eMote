/**
 * @file   RunningMean.h
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 6, 2014
 * @copyright (c) 2014 The Samraksh Company
 * @brief Maintains running mean on input of size N. Created per Kenneth Parker's API design document.
 */

#ifndef _RUNNINGMEAN_H_
#define _RUNNINGMEAN_H_

#include "Int.h"
#include "IntAttrib.h"
#include "Util.h"
#include "Comp.h"

#include "CircularBuffer.h"

template<typename IntElemT, size_t N, typename IntSumT>
class RunningMeanT {
private:
    CircularBufferT<IntElemT> m_CircularBuffer;
    IntSumT m_sum;
    size_t m_length;
    IntElemT m_mean;
public: // constructors
    RunningMeanT(IntElemT InitVal);
    RunningMeanT(IntElemT const (&InitVal)[N]);
    ~RunningMeanT();
public:
    /**
    * insert new element at end of the circular buffer, drop trailing element.
    */
    void Push(IntElemT elem);

    /**
    * return the mean without modifying the object
    */
    const IntElemT& operator () (void);
};

#include "RunningMean.hpp"

#endif /* _RUNNINGMEAN_H_ */
