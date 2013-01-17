using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using System.Threading;

namespace ADCExample
{
    public class Program
    {
        public static Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput adc = new Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput();

        public static void Main()
        {
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Init(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.ADC_Channel1);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Init(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.ADC_Channel2);

            Timer mytime = new Timer(callback, null, 0, 1000);

            while (true)
            {
                Thread.Sleep(1000);
            }

        }

        public static void callback(Object state)
        {
            double adcval1 = Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Read(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.ADC_Channel1);
            double adcval2 = Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Read(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.ADC_Channel2);

            Debug.Print("Channel 1" + adcval1.ToString());
            Debug.Print("Channel 2" + adcval2.ToString());
        }

    }
}
