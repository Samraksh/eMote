using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.RealTime;
using System.Threading;

namespace RealTimeExample
{
    public class RealTimeExample
    {
        Samraksh.SPOT.RealTime.Timer RT_Timer;
        static string outstr = "RealTime Handler";
        static bool RT_Timer_State;
        NativeEventHandler RT_EventHandler = new NativeEventHandler(RT_TimerCallback);
        static OutputPort Output = new OutputPort((Cpu.Pin)30, true);
        static OutputPort Error = new OutputPort((Cpu.Pin)31, true);

        RealTimeExample()
        {
            try
            {
                RT_Timer = new Samraksh.SPOT.RealTime.Timer("RealTimeInteropTimer", 10000, 0);
                RT_Timer.OnInterrupt += RT_EventHandler;
            }
            catch (Exception)
            {
                Debug.Print("Error initializing Realtime timer");
                while (true)
                {
                    Error.Write(true);
                    Thread.Sleep(100);
                    Error.Write(false);
                }
            }
        }

        private static void RT_TimerCallback(uint data1, uint data2, DateTime time)
        {
            //Debug.Print(outstr);
            RT_Timer_State = !RT_Timer_State;
            Output.Write(RT_Timer_State);
            RT_Timer_State = !RT_Timer_State;
            Output.Write(RT_Timer_State);
        } 

        public static void Main()
        {
            Debug.Print("Starting the RT timer");
            RealTimeExample rt = new RealTimeExample();
        }

    }
}
