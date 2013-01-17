//////////////////////////////////////////////////////////////////////////////
// TIMER driver
//
struct AT91_TIMER_Driver
{
    static const UINT32 c_SystemTimer   = 0;
    static const UINT16 c_MaxTimerValue = 0xFFFF; 
    
    //--//
    
    static BOOL Initialize   ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL Uninitialize ( UINT32 Timer );
    static void ISR_TIMER( void* param );

    static void EnableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);


        (void) TC.TC_SR;
        TC.TC_IER = AT91_TC::TC_CPCS;
        TC.TC_CCR = (AT91_TC::TC_SWTRG  | AT91_TC::TC_CLKEN) ;
    }

    static void DisableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);

        TC.TC_IDR = AT91_TC::TC_CPCS; 
    }

    static void ForceInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        ASSERT_IRQ_MUST_BE_OFF();

        AT91_AIC_Driver::ForceInterrupt( AT91C_ID_TC0 + Timer );
    }

    static void SetCompare( UINT32 Timer, UINT16 Compare )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);

        TC.TC_RC = Compare;
    }

    static UINT16 ReadCounter( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);
        return    TC.TC_CV;
    }

    static BOOL DidTimerOverFlow( UINT32 Timer )
    {
        ASSERT(Timer < AT91_MAX_TIMER);
        AT91_TC &TC = AT91::TIMER(Timer);
        
        return (TC.TC_SR & AT91_TC::TC_COVFS) != 0;        
    }

//--//

private:
    struct Descriptors
    {
        HAL_CALLBACK_FPN isr;
        void* arg;
        BOOL configured;
    };
    Descriptors m_descriptors[AT91_MAX_TIMER];

   
};

extern AT91_TIMER_Driver g_AT91_TIMER_Driver;
