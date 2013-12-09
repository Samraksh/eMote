using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using Microsoft.SPOT.Hardware;


namespace Samraksh.SPOT.Apps
{
    public abstract class PersistentStorage
    {
        abstract public bool Write(ushort[] data, UInt16 length);

        abstract public bool Write(byte[] data, UInt16 length);

        abstract public bool Read(byte[] data, UInt16 length);

        abstract public bool Read(ushort[] data, UInt16 length);

        abstract public bool WriteEof();

        abstract public bool eof();

    }

    public class SDStore : PersistentStorage
    {

        public static Samraksh.SPOT.Hardware.EmoteDotNow.SD.SDCallBackType sdResultCallBack;

        public static bool sdSuccessFlag = false;

        public static OutputPort sdFail = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN4, false);
        public static OutputPort sdSuccess = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN5, false);

        public static void mySdCallback(Samraksh.SPOT.Hardware.DeviceStatus status)
        {
            Debug.Print("Recieved SD Callback\n");

            sdSuccessFlag = true;

            sdSuccess.Write(false);
            sdSuccess.Write(true);

        }

        public SDStore()
        {
            sdResultCallBack = mySdCallback;

            Samraksh.SPOT.Hardware.EmoteDotNow.SD mysdCard = new Samraksh.SPOT.Hardware.EmoteDotNow.SD(sdResultCallBack);

            if (!Samraksh.SPOT.Hardware.EmoteDotNow.SD.Initialize())
            {
                Debug.Print("SD Card Initialization failed \n");
                //ADCTest.lcd.Write(Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_E, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_R, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_0, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_R);

                //cardAvailable = false;
                sdFail.Write(true);

                throw new InvalidOperationException("SD Card Initialization failed !!!");

            }

        }

        public override bool WriteEof()
        {
            throw new NotImplementedException();
        }

        public override bool eof()
        {
            throw new NotImplementedException();
        }

        public override bool Write(ushort[] data, ushort length)
        {
            byte[] dataInByte = new byte[512];

            bool result = true;

            int dataBufferCounter = 0;

            for (UInt16 i = 0; i < length; i++)
            {
                dataInByte[dataBufferCounter++] = (byte)(data[i]);
                dataInByte[dataBufferCounter++] = (byte)((data[i] >> 8) & 0xff);

                if (dataBufferCounter == 512)
                {
                    result &= Write(dataInByte, (ushort)dataBufferCounter);
                    dataBufferCounter = 0;
                }
            }

            return result;
        }

        public override bool Write(byte[] data, ushort length)
        {
            if (!Samraksh.SPOT.Hardware.EmoteDotNow.SD.Write(data, 0, length))
            {
                Debug.Print("Writing to SD Card Failed \n");
                return false;
            }

            return true;
        }

        public override bool Read(byte[] data, ushort length)
        {
            throw new NotImplementedException();
        }

        public override bool Read(ushort[] data, ushort length)
        {
            throw new NotImplementedException();
        }
    }

    public class NorStore : PersistentStorage
    {
        public const uint NorSize = 12 * 1024 * 1024;

        public NorStore()
        {
            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Initialize(NorSize);
        }

        public override bool Write(byte[] data, UInt16 length)
        {
            return false;
        }

        public override bool Write(ushort[] data, UInt16 length)
        {
            if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.IsFull())
            {
                return false;
            }

            // Writing to NOR flash
            //if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.StartNewRecord())
            //{
            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(data, length);



            //    Samraksh.SPOT.Hardware.EmoteDotNow.NOR.EndRecord();
            //}

            return true;

        }

        public override bool eof()
        {
            return Samraksh.SPOT.Hardware.EmoteDotNow.NOR.eof();
        }


        public override bool Read(byte[] data, UInt16 length)
        {
            return false;
        }

        public override bool Read(ushort[] data, UInt16 length)
        {
            if (!Samraksh.SPOT.Hardware.EmoteDotNow.NOR.eof())
            {
                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Read(data, length) != Samraksh.SPOT.Hardware.DeviceStatus.Success)
                {
                    Debug.Print("Read from NOR failed \n");
                    return false;
                }
            }
            else
                return false;

            return true;
        }

        public override bool WriteEof()
        {
            ushort[] eof = new ushort[512];

            for (UInt16 i = 0; i < eof.Length; i++)
            {
                eof[i] = 0x0c0c;
            }

            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(eof, (ushort)eof.Length);

            return true;
        }
    }

    public class BufferStorage
    {
        public ushort[] buffer;

        public Object bufferLock = new object();

        public bool bufferfull = false;

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

        public bool IsFull()
        {
            lock (bufferLock)
            {
                return bufferfull;
            }
        }

        public bool Persist(PersistentStorage storage)
        {
            lock (bufferLock)
            {
                if (!storage.Write(buffer, (ushort)buffer.Length))
                {
                    return false;
                }

                bufferfull = false;
            }

            return true;
        }



    }


    public class DataCollectorRadarSD
    {

        public const uint bufferSize = 1024;

        public const uint sampleTime = 1000;

        public ushort[] sampleBuffer1 = new ushort[bufferSize];
        public ushort[] sampleBuffer2 = new ushort[bufferSize];

        public Samraksh.SPOT.Hardware.EmoteDotNow.AdcCallBack adcCallbackPtr;

        public BufferStorage channelIBuffer;
        public BufferStorage channelQBuffer;


        //public BufferStorage transferBuffer;

        public static OutputPort callbackTime = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN3, false);

        public PersistentStorage storage;

        public PersistentStorage removableStorage;

        public static bool stopExperimentFlag = false;

        public static InterruptPort stopExperiment = new InterruptPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J11_PIN7, false, Port.ResistorMode.Disabled, Port.InterruptMode.InterruptEdgeLow);

        public static Samraksh.SPOT.Hardware.EmoteDotNow.EmoteLCD lcd;


        public DataCollectorRadarSD()
        {
            Debug.Print("Initializing LCD ....");

            lcd = new Samraksh.SPOT.Hardware.EmoteDotNow.EmoteLCD();

            lcd.Initialize();

            lcd.Clear();

            Debug.Print("Initializing ADC .....");

            channelIBuffer = new BufferStorage(bufferSize);
            channelQBuffer = new BufferStorage(bufferSize);

            adcCallbackPtr = AdcCallbackFn;
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitializeADC();
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitChannel(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.InitChannel(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel2);
            if (!Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.ConfigureContinuousModeDualChannel(sampleBuffer1, sampleBuffer2, bufferSize, sampleTime, AdcCallbackFn))
            {
                throw new InvalidOperationException("ADC Initialization failed \n");
            }

            Debug.Print("Initializing NOR ...");
            lcd.Write(Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_E, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_R, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_A, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_S);
            storage = new NorStore();

            Debug.Print("Initializing SD ...");
            removableStorage = new SDStore();

        }

        public void AdcCallbackFn(long threshold)
        {
            callbackTime.Write(true);
            callbackTime.Write(false);

            
            if (sampleBuffer1[0] == 0 && sampleBuffer1[1] == 0 && sampleBuffer1[2] == 0 && sampleBuffer1[3] == 0)
            {
                if (sampleBuffer2[0] == 4001 && sampleBuffer2[1] == 4001 && sampleBuffer2[2] == 4001 && sampleBuffer2[3] == 4001)
                {
                    Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.StopSampling();
                    Debug.Print("Stopping Experiment");
                    stopExperimentFlag = true;
                }
            }

            channelIBuffer.Copy(sampleBuffer1);
            channelQBuffer.Copy(sampleBuffer2);

        }

        public bool XferToSD()
        {

            UInt16[] tempBuffer = new UInt16[bufferSize];

            while (!storage.eof())
            {
                if (storage.Read(tempBuffer, (ushort)bufferSize))
                {
                    if (!removableStorage.Write(tempBuffer, (ushort)bufferSize))
                    {
                        Debug.Print("XferToSD : Write to SD Card failed \n");
                        return false;
                    }
                }
                else
                {
                    Debug.Print("XferToSD : Read from NOR failed \n");
                    return false;
                }

                //System.Threading.Thread.Sleep(20);
            }

            return true;

        }

        public void Run()
        {
            lcd.Clear();
            lcd.Write(Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_C, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_C, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_C, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_C);

            while (true)
            {


                if (channelIBuffer.IsFull())
                {
                    channelIBuffer.Persist(storage);
                }

                System.Threading.Thread.Sleep(10);

                if (channelQBuffer.IsFull())
                {
                    channelQBuffer.Persist(storage);
                }

                if (stopExperimentFlag)
                {
                    if (channelIBuffer.IsFull())
                    {
                        channelIBuffer.Persist(storage);
                    }

                    System.Threading.Thread.Sleep(10);

                    if (channelQBuffer.IsFull())
                    {
                        channelQBuffer.Persist(storage);
                    }

                    storage.WriteEof();
                    lcd.Write(Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_S, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_S, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_S, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_S);
                    XferToSD();
                    break;
                }
            }

            lcd.Write(Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_D, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_D, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_D, Samraksh.SPOT.Hardware.EmoteDotNow.LCD.CHAR_D);
            Debug.Print("Experiment Complete\n");
        }

        public static void Main()
        {
            Debug.EnableGCMessages(false);

            DataCollectorRadarSD dcrs = new DataCollectorRadarSD();

            dcrs.Run();
        }

    }
}
