/**
 * @file   Algorithms.cpp
 * @author Michael Andrew McGrath <Michael.McGrath@Samraksh.com>
 * @date   Nov 21, 2014
 * @copyright (C) 2014 The Samraksh Company
 * @brief instantions of templated algorithms with access functions
 */

#include <Samraksh\Algorithms.h>

#include "Comp.h"
#include "IntAttrib.h"
#include "ButterFly.h"
#include "Fft.h"
#include "TwiddleWalk.h"
#include "RunningMean.h"



template class FftCompFixLenT<Int16T,ButterFlyAccT<Int16T>,BitWalkT<Int16T> >;


FftCompFixLenT<Int16T, ButterFlyAccT<Int16T>, BitWalkT<Int16T> >* Fft = NULL;

INT32 FftCompFixLenT_Int16T(INT16 (*complex_buffer)[2], INT32 ln2OfN)
{

    bool resize = false;
    if(Fft != NULL) {
        resize = (Fft->Ln2OfN() != ln2OfN);
        if(resize) {
            delete Fft;
        }
    }
    if(Fft == NULL || resize) {
        Fft = new FftCompFixLenT<Int16T,ButterFlyAccT<Int16T>,BitWalkT<Int16T> >((UINT32)ln2OfN);
        //hal_printf("WARNING: resizing FFT -- should be static!\n");
    }

    INT32 shift = Fft->operator ()(reinterpret_cast< CompT<Int16T>* >(complex_buffer));
    return shift;
}



template class RunningMeanT<Int16T, ALGORITHMS_N, Int32T>;

RunningMeanT<Int16T, ALGORITHMS_N, Int32T>* runMeanI16 = NULL;

INT16 RunningMeanT_Int16T(INT16* buffer, const UINT32 bufferLength)
{
#if defined(DEBUG)
    ASSERT(bufferLength > 0);
#endif
    INT16 fRet = 0;
    if(runMeanI16 == NULL) {
        runMeanI16 = new RunningMeanT<Int16T, ALGORITHMS_N, Int32T>(buffer[0]);
    }
    else {
        runMeanI16->Push(buffer[0]);
    }
    if(runMeanI16 != NULL) {
        for(int itr=0; itr < bufferLength; ++itr) {
            runMeanI16->Push(buffer[itr]);
        }
        fRet = runMeanI16->operator ()();
    }
    return fRet;
}
