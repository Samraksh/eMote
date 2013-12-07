using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using Microsoft.SPOT.Hardware;
using System.IO.Ports;
using System.Threading;


/*
 *  This test is designed to check how quickly adc callbacks happen and to measure the jitter between each sample in the callback. Ensure that you have  
 *  the native ADC_DEBUG_ENABLED macro to check the jitter between each sample. If this is not enabled you will not have pin 25 on the emote toggling on 
 *  each timer callback
 * 
 */
 
namespace Samraksh.SPOT.APPS
{

    public class BufferStorage
    {
        public ushort[] buffer;

        public Object bufferLock = new object();

        public bool bufferfull = true;

        public BufferStorage(uint BufferSize)
        {
            buffer = new ushort[BufferSize];
        }

        public void Copy(ushort[] inpArray)
        {
            lock (bufferLock)
            {
                inpArray.CopyTo(buffer, 0);
                bufferfull = true;
            }
        }

    }

    public class DataCollectorRam
    {
        public const uint bufferSize = 1024;

        public const uint sampleTime = 1000;

        public ushort[] sampleBuffer = new ushort[bufferSize];

        public Samraksh.SPOT.Hardware.EmoteDotNow.AdcCallBack adcCallbackPtr;

        public static bool callbackRecieved = false;

        public BufferStorage buffer;

        public System.IO.Ports.SerialPort serialPort;

        public static OutputPort callbackTime = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN3, false);

        public void AdcCallbackFn(long threshold)
        {
            callbackTime.Write(true);
            callbackTime.Write(false);
            buffer.Copy(sampleBuffer);
            
        }


        public DataCollectorRam()
        {

            Debug.Print("Initializing Serial ....");

            serialPort = new SerialPort("COM1");
            serialPort.BaudRate = 115200;
            serialPort.Parity = System.IO.Ports.Parity.None;
            serialPort.StopBits = StopBits.One;
            serialPort.DataBits = 8;
            serialPort.Handshake = Handshake.None;

            Debug.Print("Initializing ADC .....");

            adcCallbackPtr = AdcCallbackFn;
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitializeADC();
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitChannel(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.ConfigureContinuousMode(sampleBuffer, Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1, bufferSize, sampleTime, AdcCallbackFn);

            buffer = new BufferStorage(bufferSize);
        }

        public static void Main()
        {
            Debug.EnableGCMessages(false);

            DataCollectorRam dcr = new DataCollectorRam();

            while (true)
            {
                Thread.Sleep(2000);
            }



        }

    }
}
