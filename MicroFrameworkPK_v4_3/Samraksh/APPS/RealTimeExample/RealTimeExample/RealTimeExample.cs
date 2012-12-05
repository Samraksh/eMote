using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.RealTime;

namespace Samraksh.SPOT.RealTime
{
    public class RealTimeExample
    {
        Timer RT_Timer;
       static bool RT_Timer_State;
        NativeEventHandler RT_EventHandler = new NativeEventHandler(RT_TimerCallback);
        static OutputPort Output = new OutputPort((Cpu.Pin) 30, true);
        static OutputPort Error = new OutputPort((Cpu.Pin) 31, true);

        RealTimeExample()
        {
            try
            {
            	RT_Timer = new Timer("RealTimeInteropTimer", 500 , 0);
				RT_Timer.OnInterrupt += RT_EventHandler;					
            }
            catch (Exception)
            {
                Debug.Print("Error initializing Realtime timer");
                while (true){
                    Error.Write(true);
                    Error.Write(false);
                }
            }
        }

        private static void RT_TimerCallback(uint data1, uint data2, DateTime time)
        {
            RT_Timer_State = !RT_Timer_State;
            Output.Write(RT_Timer_State);
            RT_Timer_State = !RT_Timer_State;
            Output.Write(RT_Timer_State);
        } //RT_Timer handler

        public static void Main()
        {
            Debug.Print("Starting the RT timer");
            RealTimeExample rt = new RealTimeExample();
        }

    }
}
