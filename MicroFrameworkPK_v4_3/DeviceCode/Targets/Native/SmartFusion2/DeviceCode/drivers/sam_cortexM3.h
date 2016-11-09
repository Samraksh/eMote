
/*

Dumping cortex m3 specific code here, that is locally sourced (not CMSIS or 3rd party).

Maybe this should go somewhere else.
Probably an isolated driver is needed, maybe this should be attached to stmlib.
This is currently "attached" to the PWR driver .proj file.

Nathan Stohs
nathan.stohs@samraksh.com
2015-10-26

*/

#ifndef SAM_CORTEXM3_H
#define SAM_CORTEXM3_H

// Returns TRUE if currently in interrupt context
inline BOOL isInterrupt()
{
    return ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0);
}
#endif
