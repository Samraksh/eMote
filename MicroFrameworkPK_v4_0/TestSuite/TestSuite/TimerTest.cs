// Name : Nived Sivadas
// Company : The Samraksh Company
// Description : Simple test designed to check if the timer sleeps for a given period of time
// Requirements: Requires OScilloscope/Logic Analyser to be connected on Pin 0
//




using System;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;
using TestSuite;

namespace TestSuite
{
    public class TimerTest
    {
		//Debug.Print("Executing now.. ");
        //private OutputPort testPort = new OutputPort(Pins.GPIO_PORT_PIN_0, true);
		public static bool state = false;
		private static OutputPort testPort = new OutputPort(Pins.GPIO_PORT_PIN_57, true);		
        Log log = new Log();

        public void Run()
        {
			
			
            try
            {
			
				Timer tmr = new Timer(new TimerCallback(Callback), null, 0, 5);								
				while (true)
                {
					//Thread.Sleep(10000);
                   //DateTime start = DateTime.Now;
                    //log.log("Time :" + start + "\n");
                    // Will enable harness or external hardware to check accurate times
					testPort.Write(true);
                    //Thread.Sleep(1000);
					testPort.Write(false);
                //   log.log("Test Complete \n");
                }				
            }
            catch (Exception)
            {
                // Will enable harness or external hardware to check accurate times
                
                log.log("Unexpected Exception During Timer Test\n");
            }
        }
        private void Callback(Object state)
        {
            testPort.Write(true);						
			testPort.Write(false);
            //log.log("Time :\n" + DateTime.Now);
            //log.log("Call Back Happening\n");
			//Toggle();
        }	
        
		private void Toggle()
		{			
			if(state == true)
			{
				testPort.Write(false);
				state = false;
			}
			else
			{
				testPort.Write(true);
				state = true;
			}			
		}


    }
}
