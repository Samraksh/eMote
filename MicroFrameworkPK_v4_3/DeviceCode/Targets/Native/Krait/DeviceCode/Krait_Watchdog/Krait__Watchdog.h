#ifndef _KRAIT_WATCHDOG_H_
#define _KRATI_WATCHDOG_H_

#include <tinyhal.h>

#define TCSR_WDT_CFG	0x30

#define WDT0_RST	0x38
#define WDT0_EN		0x40
#define WDT0_STS	0x44
#define WDT0_BARK_TIME	0x4C
#define WDT0_BITE_TIME	0x5C

#define WDT_HZ		32768

#define TIMER0_BASE 0x0


#endif
