#ifndef _ACOUSTIC_H_INCLUDED_
#define _ACOUSTIC_H_INCLUDED_

#include <tinyhal_types.h>


INT8 testFunction(UINT16* buffer, INT32 length, double* processingOutput, bool historyUpdateControl, INT32 Pa, INT32 Pm);

INT8 DetectionCalculationNative(UINT16* buffer, INT32 length, double* processingOutput, bool historyUpdateControl, INT32 Pa, INT32 Pm);

#endif
