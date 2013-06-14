using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;

namespace ADCStore
{
    public class ADCS
    {
        const UInt16 BufferSize = 750;

        Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput adc = new Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput();
        public static UInt16[] IValue = new UInt16[BufferSize];
        public static UInt16[] QValue = new UInt16[BufferSize];
        public static UInt16[] channelValues = new UInt16[2];
        public static UInt16 IvalueCounter = 0;
        public static UInt16 QvalueCounter = 0;

        public ADCS()
        {
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Init(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Init(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel2);
        }


        public bool ResetBuffers()
        {
            for (UInt16 i = 0; i < BufferSize; i++)
            {
                IValue[i] = 0;
                QValue[i] = 0;
            }

            IvalueCounter = 0;
            QvalueCounter = 0;

            return true;

        }

        public bool Sample()
        {
            long currentTimestamp = 0;
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.DualChannelRead(channelValues);


            if (IvalueCounter == BufferSize)
            {
                currentTimestamp = DateTime.Now.Ticks;

                ResetBuffers();
            }

            IValue[IvalueCounter++] = channelValues[0];
            QValue[QvalueCounter++] = channelValues[1];

            return true;

        }
    }

    public class Program
    {
        public static void Main()
        {
           
        }

    }
}
