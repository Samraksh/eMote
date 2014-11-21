#ifndef _ACOUSTIC_H_INCLUDED_
#define _ACOUSTIC_H_INCLUDED_

INT8 testFunction(UINT16* buffer, INT32 length, double* processingOutput, bool historyUpdateControl, INT32 Pa, INT32 Pm);

INT32 FftCompFixLenT_Int16T(INT16* complex_buffer, INT32 ln2OfN);

#endif
