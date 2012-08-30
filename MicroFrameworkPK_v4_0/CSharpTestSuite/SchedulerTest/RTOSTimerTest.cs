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

using Samraksh.RealTime;
using CSharpTestSuite;

namespace CSharpTestSuite.SchedulerTestSuite
{
    public class RTOSTimerTest
    {
		private static OutputPort testPort0 = new OutputPort((Cpu.Pin)0, true);
		private static int x=0; 				
		
			
        public void Run()
        {
        	Samraksh.RealTime.Timer RT_Timer;
        	NativeEventHandler RT_EventHandler = new NativeEventHandler(RT_TimerCallback); //RT_Timer handler
					
            try
            {
				RT_Timer = new Samraksh.RealTime.Timer("RealTimeInteropTimer", 5000 , 0);
				RT_Timer.OnInterrupt += RT_EventHandler;				
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
        
        public void RT_TimerCallback( uint data1, uint data2, DateTime time)
        {
			Toggle0();
        }	
        
		static void Toggle0()
		{	
			testPort0.Write(true);					
		}
		
		 		
	}
}
