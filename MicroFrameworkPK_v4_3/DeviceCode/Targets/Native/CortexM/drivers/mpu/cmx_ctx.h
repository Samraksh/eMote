#ifndef _CMX_CTX_H_
#define _CMX_CTX_H_
//from here
//https://searchcode.com/file/115430949/platform/ChibiOS/os/rt/ports/ARMCMx/chcore_v7m.h
#include "exc_return.h"

typedef volatile UINT32 regarm_t;

//Cortex M3 cpu task/process context
typedef struct {
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      r12;
  regarm_t      lr_thd;
  regarm_t      pc;
  regarm_t      xpsr;
  regarm_t      r4;
  regarm_t      r5;
  regarm_t      r6;
  regarm_t      r7;
  regarm_t      r8;
  regarm_t      r9;
  regarm_t      r10;
  regarm_t      r11;
  regarm_t		stackframefp;
  //volatile UINT8 r4_r11[32];
#if CORTEX_USE_FPU
  regarm_t      s0;
  regarm_t      s1;
  regarm_t      s2;
  regarm_t      s3;
  regarm_t      s4;
  regarm_t      s5;
  regarm_t      s6;
  regarm_t      s7;
  regarm_t      s8;
  regarm_t      s9;
  regarm_t      s10;
  regarm_t      s11;
  regarm_t      s12;
  regarm_t      s13;
  regarm_t      s14;
  regarm_t      s15;
  regarm_t      fpscr;
  regarm_t      reserved;
#endif /* CORTEX_USE_FPU */
} task_ctx_t ;


#endif
