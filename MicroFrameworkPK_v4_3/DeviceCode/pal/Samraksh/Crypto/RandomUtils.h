#ifndef _RANDOM_UTILS_H_
#define _RANDOM_UTILS_H_

#include <tinyhal_types.h>

BOOL GetCPUSerial(UINT8 * ptr, UINT16 num_of_bytes );
UINT16 GetNodeID();
//Very very bad implementation of random byte stream
int GetRandomBytes (unsigned char *buf, UINT16 length);
int GetRandomSeed (unsigned char *buf, UINT16 length);

#endif //_RANDOM_UTILS_H_
