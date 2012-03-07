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
//using AsyncTimer;
using CSharpTestSuite;

namespace CSharpTestSuite
{
    public class USBSpeedTest
    {
		//Debug.Print("Executing now.. ");
        
        static int[] dataBuffer= new int[500];
		public static bool state0;
		private static OutputPort testPort0 = new OutputPort(EmotePins.GPIO_PIN_3, true); //Emote Pin 3 is PA0 on the board	 
		public void Run()
        {
        		for (int i=0; i<500; i++){
        		dataBuffer[i]=i;
        		}
                while (true)
                {
					Thread.Sleep(1000);
					SendData();
                }		
        }
        
        public void SendData()
        {
			//string output="";
			Toggle0();	
			for (int i=0; i<250; i++){
				Debug.Print(i.ToString() +","+ dataBuffer[i*2].ToString()+ "," + dataBuffer[i*2+1].ToString()+"\n");
					//output+=(i.ToString());
			}
			 		
			//Debug.Print( output );
        }	
        
		static void Toggle0()
		{	
			if(state0 == true)
			{
				testPort0.Write(false);
				state0 = false;
			}
			else
			{
				testPort0.Write(true);
				state0 = true;
			}			
		}
				
	}
}
