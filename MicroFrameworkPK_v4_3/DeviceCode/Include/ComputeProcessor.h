#ifndef _COMPUTE_PROCESSOR_H_
#define _COMPUTE_PROCESSOR_H_

#include <tinyhal.h>

bool CP_Init(void);
bool CP_Present(void);
bool CP_UnInit(void);
void CP_SendMsgToCP(uint8_t* msg, int size);
void CP_Reload(void);

#endif // _COMPUTE_PROCESSOR_H_
