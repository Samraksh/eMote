
#include <tinyhal.h>
#include "../../Log/Log.h"
#include <tim/netmf_timers.h>

#ifndef _Timer_Test_
#define _Timer_Test_ 1

class TimerTest
{
    UINT32   m_displayInterval;
    UINT32   m_timerDuration;
    CHAR     thorp[4];

public:

//--//
    		TimerTest ( UINT32 DisplayIntervalSeconds , UINT32 TimerDurationSeconds );

    BOOL    Execute( LOG_STREAM Stream );

};

#endif

