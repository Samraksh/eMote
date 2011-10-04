struct WatchDog_Driver
{
    static const UINT32 c_CountsPerMillisecond  = SYSTEM_CLOCK_HZ / 1000;
    static const UINT32 c_MaxMilliseconds       = (UINT32)0xFFFFF000 / c_CountsPerMillisecond;
    static const BOOL   c_WatchdogUsesInterrupt = TRUE;

    //--//

    static HRESULT Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context );

    static void Disable();

    static void ResetCounter();

    static void ResetCpu();

private:

    UINT32                      m_ResetCount;
    WATCHDOG_INTERRUPT_CALLBACK m_callback;
    void*                       m_context;

    //--//
    
    static void ISR( void* Param );
};

extern WatchDog_Driver g_WatchDog_Driver;
