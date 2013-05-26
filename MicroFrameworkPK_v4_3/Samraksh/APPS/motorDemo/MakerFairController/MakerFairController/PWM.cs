using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Samraksh.SPOT.Net.Mac;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;
using Samraksh.SPOT.RealTime;

namespace MakerFairController
{
    class PWM
    {
        Samraksh.SPOT.RealTime.Timer RT_Timer;
        NativeEventHandler RT_EventHandler = new NativeEventHandler(RT_TimerCallback); //RT_Timer handler
        const uint Error = 109;	//Timer has a error of 109 microseconds

        static OutputPort ServoPort = new OutputPort((Cpu.Pin)24, true);  
        static bool Pulse = false;

        public PWM()
        {
            RT_Timer = new Samraksh.SPOT.RealTime.Timer("RealTimeInteropTimer", 5000, 0);
		    RT_Timer.OnInterrupt += RT_EventHandler;					

            ServoPort.Write(false);            
        }

        public void GeneratePulse(uint width)
        {
        	Pulse = true;
            Samraksh.SPOT.RealTime.Timer.Change(width - Error, 0); //Schedule a singleshot timer            
            //Samraksh.SPOT.RealTime.Timer.Change(0, width); //Schedule a singleshot timer            
            ServoPort.Write(true); 
        }

        static public void RT_TimerCallback(uint data1, uint data2, DateTime time)
        {
             try
            {
            if(Pulse) ServoPort.Write(false);
            Pulse=false;      
            /*Pulse = !Pulse;
            ServoPort.Write(Pulse);
            Pulse = !Pulse;
            ServoPort.Write(Pulse);*/
            }
             catch (Exception e)
             {
                 Debug.Print(e.ToString());
             }
        }
    }
}
