using System;
using Microsoft.SPOT;
using Samraksh;
using Microsoft.SPOT.Hardware;

namespace BaseStation
{
    public class Program
    {
        public static OutputPort Gpio0 = new OutputPort((Cpu.Pin)9, false);
        public static bool state = false;

        public static void Main()
        {
			MessageLayer.Init();
            ReceiveDoneEvent rEvent = new ReceiveDoneEvent("RadioCallback", 0, 100);
            NativeEventHandler eventHandler = new NativeEventHandler(OnNativeEvent);
            rEvent.OnInterrupt += eventHandler;
			while(true)
			{
				System.Threading.Thread.Sleep(10000);
			}
        }

        static void OnNativeEvent(uint data1, uint data2, DateTime time)
        {
            if(state == false)
                Gpio0.Write(true);
            else
            {
                Gpio0.Write(false);
            }
        }
    }
}
