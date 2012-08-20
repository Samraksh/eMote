﻿using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT;
using System.Threading;

namespace ADCExtendedTest
{
    public class Program
    {
        private static OutputPort testPort = new OutputPort((Cpu.Pin) 0, true);
        static ushort[] sampleBuffer = new ushort[1000];
        static byte[] value = new byte[2];
        
        public static void Main()
        {
            
            Samraksh.SPOT.ADC adcHandler = new Samraksh.SPOT.ADC();

            Samraksh.SPOT.ADC.Init(Samraksh.SPOT.AdcSampleTime.ADC_SampleTime_1_5_Cycles, 2);

            Samraksh.SPOT.ADC.ConfigureContinuousMode(sampleBuffer, 0, 2,1000, 25, Adc_Call);

            while (true)
            {
                Thread.Sleep(100);
            }
        }

        static void Adc_Call(bool state)
        {
            testPort.Write(true);
            value[0] = (byte) (sampleBuffer[998] & 0xff);
            value[1] = (byte) ((sampleBuffer[998] >> 8) & 0xff);
            Samraksh.SPOT.USB.Write(value);
            testPort.Write(false);
        }
    }
}
