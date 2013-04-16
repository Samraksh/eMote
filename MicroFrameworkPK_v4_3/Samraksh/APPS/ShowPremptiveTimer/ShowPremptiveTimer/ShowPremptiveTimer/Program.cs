using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;


namespace ShowPremptiveTimer
{
    public class Program
    {
        static string msgString = "RealTime.Timer";
        public static void Main()
        {
            Samraksh.SPOT.RealTime.Timer rt_timer;
            rt_timer = new Samraksh.SPOT.RealTime.Timer("RealTimeInteropTimer", 500,0);
            NativeEventHandler RT_EventHandler = new NativeEventHandler(RT_TimerCallback);
            rt_timer.OnInterrupt += RT_EventHandler;

            Thread thread1 = new Thread(XActivity);
            Thread thread2 = new Thread(YActivity);

            thread1.Start();
            thread2.Start(); 

        }
        static void RT_TimerCallback (uint data1, uint data2, DateTime time)
        {
            Debug.Print(msgString);
        }


         static void XActivity() 
        { 
            int j; 
            while (true) 
            { 
                Debug.Print("X"); 
                for (int i = 0; i < 50; i++) 
                { 
                    j = i * 3; 
                } 
            } 
        }
         static void YActivity()
         {
             int j;
             while (true)
             {
                 Debug.Print(" Y");
                 for (int i = 0; i < 50; i++)
                 {
                     j = i * 3;
                 }
             }
         }
    }
}
