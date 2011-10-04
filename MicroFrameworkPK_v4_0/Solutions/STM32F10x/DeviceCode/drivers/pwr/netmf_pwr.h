struct Power_Driver
{
    static const UINT32 c_SystemTime_Timer         = 2;
	//We have not defined any watch dog timers yet
    //static const UINT32 c_Watchdog_Timer           = 3;
    //static const UINT32 c_PerformanceCounter_Timer = 4;

#if !defined(BUILD_RTM)
    volatile UINT32* m_PerformanceCounter;
#endif

    //--//

    static BOOL Initialize();

    static void Pause();

    static void Sleep();

    static void Halt();

    static void Reset();

    static void Shutdown();

    static void Hibernate();

#if !defined(BUILD_RTM)
    static void   PerformanceCounter_Initialize  ();
    static void   PerformanceCounter_Uninitialize();
    static UINT32 PerformanceCounter             ();
#endif
};

extern Power_Driver g_Power_Driver;