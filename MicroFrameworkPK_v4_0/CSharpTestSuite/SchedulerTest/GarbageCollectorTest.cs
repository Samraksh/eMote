// Author : Mukundan Sridharan
// Date: Dec-22-2011
// Company : The Samraksh Company
// Description : Simple test to check if the Jitter of AsyncTimer
// Requirements: Requires OScilloscope/Logic Analyser to be connected on Pin 0

using System;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;

//using AsyncTimer;
using CSharpTestSuite;

namespace CSharpTestSuite.SchedulerTestSuite
{
	class ReferenceTypeObject {
		public ushort[] fft_buf = new ushort[512];
	}


    public class GarbageCollectorTest
    {
		//Debug.Print("Executing now.. ");
        //private OutputPort testPort = new OutputPort(Pins.GPIO_PORT_PIN_0, true);
		public static bool state0 = false;
		public static bool state1 = false;
		public static int timerCount=0;
		public static int t1=0;
		public static int t2=0;
		private static OutputPort testPort0 = new OutputPort(EmotePins.GPIO_PIN_0, true); //Emote Pin 0 is PA9 on the board	 
		private static OutputPort testPort1 = new OutputPort(EmotePins.GPIO_PIN_1, true); //Emote Pin 1 is PA10 on the board
		private static OutputPort testPort2 = new OutputPort(EmotePins.GPIO_PIN_3, true); //Emote Pin 3 is PA0 on the board
		//ushort[] fft_buf;
		
		Random random = new Random();		
        Timer TestTimer;
        //Timer gcTimer;					
			
        public void Run()
        {
        	try {
        			TestTimer = new Timer(new TimerCallback(TestTimerCallback),null,0 ,5 );
        	   }
            catch (Exception)
            {
            	 while (true)
	            {
	            	testPort2.Write(true);
	            	testPort2.Write(false);
					Thread.Sleep(2000);
	            }    
            }
		
            while (true)
            {
            	Thread.Sleep(60000);
            }		
        }
        
        
        void TestTimerCallback(Object o)
        {               
            int[] fft_buf = new int[512];
            //ReferenceTypeObject RTO= new ReferenceTypeObject();	
            
            for(int i=0; i<512; i++){
            	fft_buf[i]=i;
            	//RTO.fft_buf[i]=i;
            }
			Toggle0();
				
						
			
			//Debug.Print( "AT: T1:" + t1.ToString() + " T2:" + t2.ToString() );
			/*if(timerCount==17){
				timerCount=0;
				gcTimer = new Timer(new TimerCallback(CleanUpGC),null, 1, Timeout.Infinite );
			}*/
        }	
        
        void CleanUpGC(Object o){
        	testPort1.Write(true);      	
        	Debug.GC(true);
        	testPort1.Write(false);
        }
        
		static void Toggle0()
		{	
			testPort0.Write(true);		
			testPort0.Write(false);
		}
	}
}
