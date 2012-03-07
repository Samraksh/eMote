// Author : Mukundan Sridharan
// Date: Dec-22-2011
// Company : The Samraksh Company
// Description : Simple test to check if the Jitter of AsyncTimer
// Requirements: Requires OScilloscope/Logic Analyser to be connected on Pin 0
//




using System;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;
//
using AsyncTimer;
using CSharpTestSuite;

namespace SchedulerTestSuite
{
    public class RTOSTimerTest
    {
		//Debug.Print("Executing now.. ");
        //private OutputPort testPort = new OutputPort(Pins.GPIO_PORT_PIN_0, true);
		public static bool state0 = false;
		public static bool state1 = false;
		public static int t1=0;
		public static int t2=0;
		private static OutputPort testPort0 = new OutputPort(EmotePins.GPIO_PIN_3, true); //Emote Pin 3 is PA0 on the board	 
		private static OutputPort testPort1 = new OutputPort(EmotePins.GPIO_PIN_1, true); //Emote Pin 3 is PA10 on the board
		private static OutputPort testPort2 = new OutputPort(EmotePins.GPIO_PIN_2, true); //Emote Pin 3 is PB6 on the board				
			
        public void Run()
        {
        	
        	AsyncTimer.AsyncTimer atimer = new AsyncTimer.AsyncTimer("AsyncInteropTimer", 5000, 0);
			NativeEventHandler   eventHandler = new NativeEventHandler( AsyncCallback );
			
			
			Thread thread1 = new Thread(T1Activity); 
            Thread thread2 = new Thread(T2Activity);
			thread1.Priority = ThreadPriority.AboveNormal;
			
            thread1.Start(); 
            thread2.Start(); 
		        
            try
            {
				atimer.OnInterrupt += eventHandler;					
            }
            catch (Exception e)
            {
                // Will enable harness or external hardware to check accurate times
                Debug.Print("Exception: " + e.ToString());
            }
            while (true)
                {
					Thread.Sleep(50000);
                }		
        }
        
        public void AsyncCallback( uint data1, uint data2, DateTime time)
        {
            //testPort.Write(true);						
			//testPort.Write(false);
            //log.log("Time :\n" + DateTime.Now);
            //log.log("Call Back Happening\n");
			Toggle0();
			t1--;
			t2--;
			Debug.Print( "AT: T1:" + t1.ToString() + " T2:" + t2.ToString() );
        }	
        
		static void Toggle0()
		{	
			testPort0.Write(true);		
			/*if(state0 == true)
			{
				testPort0.Write(false);
				state0 = false;
			}
			else
			{
				testPort0.Write(true);
				state0 = true;
			}	*/		
		}
		 static void T1Activity() 
        {          
            while (true) 
            { 
            	testPort1.Write(true);
            	testPort1.Write(false);	
                for (int i = 0; i < 200; i++) 
                { 
                    t1++; 
                }
                Debug.Print(" T1: "+ t1.ToString());
            } 
        } 
        static void T2Activity() 
        { 
            while (true) 
            { 
            	testPort2.Write(true);
            	testPort2.Write(false);
                for (int i = 0; i < 200; i++) 
                { 
                    t2++; 
                }
                Debug.Print("  T2: " + t2.ToString());
            }  
        } 
		
	}
}
