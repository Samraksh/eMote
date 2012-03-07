// Author : Mukundan Sridharan
// Date: Dec-22-2011
// Company : The Samraksh Company
// Description : Test to profile the jitter of the AsyncTimer/RTOS-Scheduler 
// Requirements: Requires OScilloscope/Logic Analyser to be connected on Pin 0
//

using System;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;
//
using AsyncTimer;
using CSharpTestSuite;

namespace CSharpTestSuite
{
    public class RTOSTimerTest
    {
		private static OutputPort testPort0 = new OutputPort((Cpu.Pin)56, true);
		private static int x=0; 				
		
			
        public void Run()
        {
        	AsyncTimer.AsyncTimer atimer = new AsyncTimer.AsyncTimer("AsyncInteropTimer", 5000, 0);
			NativeEventHandler   eventHandler = new NativeEventHandler( AsyncCallback );
					
            try
            {
				atimer.OnInterrupt += eventHandler;					
            }
            catch (Exception e)
            {
                //Send any exception to the debug port
                Debug.Print("Exception: " + e.ToString());
            }
            while (true)
                {       	
                	x++;
                	x--;
                	Thread.Sleep(10000);
                }		
        }
        
        public void AsyncCallback( uint data1, uint data2, DateTime time)
        {
			Toggle0();
        }	
        
		static void Toggle0()
		{	
			testPort0.Write(true);					
		}
		
		 		
	}
}
