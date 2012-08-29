// Name : Nived Sivadas
// Company : The Samraksh Company
// Description : Simple test designed to check if the timer sleeps for a given period of time
// Requirements: Requires OScilloscope/Logic Analyser to be connected on Pin 0
//




using System;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;
using CSharpTestSuite;

namespace CSharpTestSuite
{
    public class TimerTest
    {
		
		public static bool state = false;
		private static OutputPort testPort = new OutputPort(EmotePins.GPIO_PIN_0, true);		
		private static OutputPort testPort1 = new OutputPort(EmotePins.GPIO_PIN_1, true);
        //Log log = new Log();

        public void Run()
        {
            try
            {	
				Timer tmr = new Timer(new TimerCallback(Callback), null, 0, 1000);								
				//PowerState.ChangePowerLevel(PowerLevel.Low);
				while (true)
                {
					Thread.Sleep(100);
                }				
            }
            catch (Exception)
            {	
            	while(true){
            		testPort1.Write(true);						
					testPort1.Write(false);
            		Thread.Sleep(2000);
            	}
                
            }
        }
        private void Callback(Object state)
        {
            testPort.Write(true);						
			testPort.Write(false);
        }	

    }
}
