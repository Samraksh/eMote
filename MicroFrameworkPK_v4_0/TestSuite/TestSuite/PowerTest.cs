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
    public class PowerTest
    {
		
		public static bool state = false;
		private static OutputPort testPort = new OutputPort(Pins.GPIO_PORT_PIN_0, true);		
        Log log = new Log();

        public void Run()
        {
			
            try
            {
				PowerState.ChangePowerLevel( PowerLevel.Low );
				Timer tmr = new Timer(new TimerCallback(Callback), null, 0, 5);
				while (true)
                {
					Thread.Sleep(1000);
                }				
            }
            catch (Exception)
            {
                
            }
        }
        private void Callback(Object state)
        {
            testPort.Write(true);						
			testPort.Write(false);
        }	

    }
}
