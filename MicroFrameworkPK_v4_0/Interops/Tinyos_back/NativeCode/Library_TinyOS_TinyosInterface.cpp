//-----------------------------------------------------------------------------
//
//                   ** WARNING! **
//    This file was generated automatically by a tool.
//    Re-running the tool will overwrite this file.
//    You should copy this file to a custom location
//    before adding any customization in the copy to
//    prevent loss of your changes when the tool is
//    re-run.
//
//-----------------------------------------------------------------------------


#include "Library.h"
#include "Library_TinyOS_TinyosInterface.h"
#include <rcc/stm32f10x_rcc.h>


typedef uint8_t error_t  ;
extern int main_tinyos();
extern error_t RealMainP$PlatformInit$init(void);
//extern void RealMainP__Scheduler__taskLoop(void);
extern void RealMainP$Boot$booted(void);
extern error_t RealMainP$Scheduler$init(void);
extern error_t RealMainP$SoftwareInit$init(void);
//extern void RealMainP__Scheduler__init(void);
extern bool RealMainP$Scheduler$runNextTask(void);
//extern void printSchedulerContents();
int globalCheck = 0;
typedef uint32_t __nesc_atomic_t;
extern __nesc_atomic_t __nesc_atomic_start(void )  ;
extern void __nesc_atomic_end(__nesc_atomic_t oldState)  ;
extern void __nesc_enable_interrupt(void);
extern void __nesc_disable_interrupt(void);
extern void RealMainP$Scheduler$taskLoop(void);

using namespace TinyOS;



void TinyosInterface::execute( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 param0, HRESULT &hr )
{
	hal_printf("About to enter tinyos\n");

//	uint8_t sys_clk = RCC_GetSYSCLKSource();
	//main_tinyos();
    /*
	{__nesc_atomic_t __nesc_atomic = __nesc_atomic_start();
	if(globalCheck == 0)
	{
	    hal_printf("First Time Init \n");
		RealMainP$Scheduler$init();

		hal_printf("Initializing Platform\n");
		RealMainP$PlatformInit$init();

		while (RealMainP$Scheduler$runNextTask()) ;
		//printSchedulerContents();

		RealMainP$SoftwareInit$init();
		while (RealMainP$Scheduler$runNextTask()) ;
		//printSchedulerContents();

		globalCheck++;
	}
	__nesc_atomic_end(__nesc_atomic); }
	
	__nesc_enable_interrupt();
	
	
    if(globalCheck == 1)
    {	
	    hal_printf("Booted\n");
		RealMainP$Boot$booted();
		globalCheck++;
     //   RealMainP$Scheduler$taskLoop();
    }
	#endif
	
	*/
	hal_printf("Main Work Done Here\n");
	//RealMainP$Scheduler$taskLoop();
     //RealMainP$Boot$booted();
	//RealMainP$Scheduler$taskLoop();
	// Nived : Calling main in tinyos
	

}

