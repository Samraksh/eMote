using System;
using System.IO.Ports;
using Microsoft.SPOT;
using Samraksh.eMote.DotNow;
using Samraksh.eMote.NonVolatileMemory;
using Microsoft.SPOT.Hardware;


namespace Samraksh.eMote.Apps
{
    public abstract class PersistentStorage
    {
        abstract public bool Write(ushort[] data, UInt16 length);
        abstract public bool Write(byte[] data, UInt16 length);
        abstract public ushort[] Read(int index, int bufferSize);
        abstract public bool Read(ushort[] data, UInt16 length);
        abstract public bool WriteEof(uint bufferSize);
        abstract public bool eof();
    }

    public class SDStore : PersistentStorage
    {
        public static Samraksh.eMote.DotNow.SD.SDCallBackType sdResultCallBack;
        public static bool sdSuccessFlag = false;

        public static OutputPort sdFail = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN4, false);
        public static OutputPort sdSuccess = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN5, false);

        public static void mySdCallback(Samraksh.eMote.DotNow.DeviceStatus status)
        {
            sdSuccessFlag = true;

            sdSuccess.Write(false);
            sdSuccess.Write(true);
        }

        public SDStore()
        {
            sdResultCallBack = mySdCallback;
            Samraksh.eMote.DotNow.SD mysdCard = new Samraksh.eMote.DotNow.SD(sdResultCallBack);

            if (!Samraksh.eMote.DotNow.SD.Initialize())
            {
                Debug.Print("SD Card Initialization failed \n");
                //ADCTest.lcd.Write(Samraksh.eMote.DotNow.LCD.CHAR_E, Samraksh.eMote.DotNow.LCD.CHAR_R, Samraksh.eMote.DotNow.LCD.CHAR_0, Samraksh.eMote.DotNow.LCD.CHAR_R);

                //cardAvailable = false;
                sdFail.Write(true);

                throw new InvalidOperationException("SD Card Initialization failed !!!");
            }
        }

        public override bool WriteEof(uint bufferSize)
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
            if (!Samraksh.eMote.DotNow.SD.Write(data, 0, length))
            {
                Debug.Print("Writing to SD Card Failed \n");
                return false;
            }
            return true;
        }

        public override ushort[] Read(int index, int bufferSize)
        {
            throw new NotImplementedException();
        }

        /*public override bool Read(byte[] data, ushort length)
        {
            throw new NotImplementedException();
        }*/

        public override bool Read(ushort[] data, ushort length)
        {
            throw new NotImplementedException();
        }
    }


    public class NorStore : PersistentStorage
    {
        public static OutputPort resultFailure = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J11_PIN3, false);
        public static OutputPort resultRWData = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN2, false);

        public const uint NorSize = 12 * 1024 * 1024;

        public const bool debugMode = false;
        public bool flag = true;
        public int bytesWritten = 0;
        public static int readCounter = 0;

        public ushort[] verfier = new ushort[1024];
        public byte[] verfierDS = new byte[1024];

        DataReference[] dataRefArray;
        DataStore dStore;
        DataReference dataDS;
        Type dataType = typeof(ushort);

        public NorStore()
        {
            //Samraksh.eMote.DotNow.NOR.Initialize(NorSize);
            dStore = DataStore.Instance(STORAGE_TYPE.NOR);
            //dStore.InitDataStore((int)StorageType.NOR);

            if (dStore.EraseAllData() == DATASTORE_RETURN_STATUS.Success)
                Debug.Print("Datastore succesfully erased");
        }

        public override bool Write(ushort[] data, UInt16 length)
        {
            resultRWData.Write(false);
            dataDS = new DataReference(dStore, data.Length, REFERENCE_DATA_TYPE.UINT16);
            if (dataDS.Write(data, data.Length) == DATASTORE_RETURN_STATUS.Success)
            {
                resultRWData.Write(true);
                //return true;
            }
            else
            {
                Debug.Print("Write to NOR failed\n");
                resultRWData.Write(false);
                resultFailure.Write(true);
                return false;
            }

            ////AnanthAtSamraksh - this is not needed
            /*if (debugMode)
            {
                //verfierDS = Read(0, 1024);

                if (dataDS.Read(verfierDS) != DataStatus.Success)
                {
                    Debug.Print("Read from NOR failed during verification\n");
                    resultRWData.Write(false);
                    resultFailure.Write(true);
                    return false;
                }

                for (UInt16 index = 0; index < verfierDS.Length; index++)
                {
                    if (verfierDS[index] != data[index])
                    {
                        Debug.Print("Write Failed");
                        resultRWData.Write(false);
                        resultFailure.Write(true);
                        flag = false;
                        return false;
                    }
                }
                if (flag == true)
                    Debug.Print("Write and read succeeded");
            }*/
            return true;
        }

        ////AnanthAtSamraksh - similar to the write function that uses ushort[]
        public override bool Write(byte[] data, UInt16 length)
        {
            if (Samraksh.eMote.DotNow.NOR.IsFull())
            {
                return false;
            }

            // Writing to NOR flash
            //if (Samraksh.eMote.DotNow.NOR.StartNewRecord())
            //{
            
            ////Samraksh.eMote.DotNow.NOR.Write(data, length);

            //    Samraksh.eMote.DotNow.NOR.EndRecord();
            //}

            return true;
        }


        public override bool eof()
        {
            return Samraksh.eMote.DotNow.NOR.eof();
        }

        public override ushort[] Read(int readIndex, int bufferSize)
        {
            if (readCounter != 0)
                readCounter = 0;

            //if (readCounter == 0)
            //{
                dataRefArray = new DataReference[10];
                dStore.ReadAllDataReferences(dataRefArray, 0);      //Get the data references into dataRefArray.
                ++readCounter;
            //}

            ushort[] readBuffer = new ushort[bufferSize];
            if ( readIndex < 10 )
            {
                if ((dataRefArray[readIndex].Read(readBuffer, 0, readBuffer.Length)) != DATASTORE_RETURN_STATUS.Success)
                {
                    Debug.Print("Read from NOR failed during verification\n");
                    resultRWData.Write(false);
                    resultFailure.Write(true);
                }
            }
            return readBuffer;
        }

        /*public override bool Read(byte[] data, UInt16 length)
        {
            return false;
        }*/

        public override bool Read(ushort[] data, UInt16 length)
        {
            if (readCounter != 0)
                readCounter = 0;

            //if (readCounter == 0)
            //{
                dataRefArray = new DataReference[10];
                dStore.ReadAllDataReferences(dataRefArray, 0);      //Get the data references into dataRefArray.
                ++readCounter;
            //}

            //ushort[] readBuffer = new ushort[length];
            int readIndex = 0;
            while (readIndex < 10)
            {
                if ((dataRefArray[readIndex].Read(data, 0, data.Length)) != DATASTORE_RETURN_STATUS.Success)
                {
                    Debug.Print("Read from NOR failed during verification\n");
                    resultRWData.Write(false);
                    resultFailure.Write(true);
                    return false;
                }
                
                ++readIndex;
            }
            
            return true;
        }

        public override bool WriteEof(uint bufferSize)
        {
            ushort[] eof = new ushort[bufferSize];

            for (UInt16 i = 0; i < eof.Length; i++)
            {
                eof[i] = 0x0c0c;
            }

            //Samraksh.eMote.DotNow.NOR.Write(eof, (ushort)eof.Length);
            dataDS = new DataReference(dStore, eof.Length, REFERENCE_DATA_TYPE.UINT16);
            if (dataDS.Write(eof, eof.Length) == DATASTORE_RETURN_STATUS.Success)
                return true;
            else
                return false;
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
        public const uint bufferSize = 512;
        public const uint sampleTime = 1000;

        public ushort[] sampleBuffer = new ushort[bufferSize];
        public byte[] ibufferByte = new byte[bufferSize * 2];
        public byte[] qbufferByte = new byte[bufferSize * 2];
        public ushort[] ibuffer = new ushort[bufferSize];
        public ushort[] qbuffer = new ushort[bufferSize];

        public System.IO.Ports.SerialPort serialPort;
        public BufferStorage buffer;

        public Samraksh.eMote.DotNow.AdcCallBack adcCallbackPtr;

        public BufferStorage channelIBuffer;
        public BufferStorage channelQBuffer;

        //public BufferStorage transferBuffer;

        public static OutputPort callbackTime = new OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN3, false);
        public PersistentStorage storage;
        public PersistentStorage removableStorage;

        public int dataCollected = 0;
        public static bool stopExperimentFlag = false;

        public static InterruptPort stopExperiment = new InterruptPort(Samraksh.eMote.DotNow.Pins.GPIO_J11_PIN7, false, Port.ResistorMode.Disabled, Port.InterruptMode.InterruptEdgeLow);
        public static Samraksh.eMote.DotNow.EmoteLCD lcd;


        public DataCollectorRadarSD()
        {
            buffer = new BufferStorage(bufferSize);

            Debug.Print("Initializing Serial ....");
            serialPort = new SerialPort("COM1");
            serialPort.BaudRate = 115200;
            serialPort.Parity = System.IO.Ports.Parity.None;
            serialPort.StopBits = StopBits.One;
            serialPort.DataBits = 8;
            serialPort.Handshake = Handshake.None;

            Debug.Print("Initializing LCD ....");
            lcd = new Samraksh.eMote.DotNow.EmoteLCD();
            lcd.Initialize();
            lcd.Clear();

            Debug.Print("Initializing ADC .....");

            stopExperiment.OnInterrupt += stopExperiment_OnInterrupt;

            channelIBuffer = new BufferStorage(bufferSize);
            channelQBuffer = new BufferStorage(bufferSize);

            adcCallbackPtr = AdcCallbackFn;
            Samraksh.eMote.DotNow.AnalogInput.InitializeADC();
            Samraksh.eMote.DotNow.AnalogInput.InitChannel(Samraksh.eMote.DotNow.ADCChannel.ADC_Channel1);
            Samraksh.eMote.DotNow.AnalogInput.InitChannel(Samraksh.eMote.DotNow.ADCChannel.ADC_Channel2);
            if (!Samraksh.eMote.DotNow.AnalogInput.ConfigureContinuousModeDualChannel(ibuffer, qbuffer, bufferSize, sampleTime, AdcCallbackFn))
            {
                throw new InvalidOperationException("ADC Initialization failed \n");
            }

            Debug.Print("Initializing SD ...");
            removableStorage = new SDStore();

            Debug.Print("Initializing NOR ...");
            lcd.Write(Samraksh.eMote.DotNow.LCD.CHAR_E, Samraksh.eMote.DotNow.LCD.CHAR_R, Samraksh.eMote.DotNow.LCD.CHAR_A, Samraksh.eMote.DotNow.LCD.CHAR_S);
            storage = new NorStore();
        }

        void stopExperiment_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            Samraksh.eMote.DotNow.AnalogInput.StopSampling();
            stopExperimentFlag = true;
        }

        public void AdcCallbackFn(long threshold)
        {
            callbackTime.Write(true);
            callbackTime.Write(false);

            Debug.Print(" I " + ibuffer[200].ToString() + " Q " + qbuffer[200].ToString() + "\n");
            dataCollected += (sampleBuffer.Length * 2);

            if (ibuffer[0] == 0 && ibuffer[1] == 0 && ibuffer[2] == 0 && ibuffer[3] == 0)
            {
                if (qbuffer[0] == 0 && qbuffer[1] == 0 && qbuffer[2] == 0 && qbuffer[3] == 0)
                {
                    Samraksh.eMote.DotNow.AnalogInput.StopSampling();
                    Debug.Print("Stopping Experiment");
                    stopExperimentFlag = true;
                }
            }

            channelIBuffer.Copy(ibuffer);
            channelQBuffer.Copy(qbuffer);
        }


        public bool XferToSD()
        {
            ushort[] tempBuffer = new ushort[bufferSize];
            int readIndex = 1;

            while (true)
            {
                tempBuffer = storage.Read(readIndex, (ushort)bufferSize);
                if (tempBuffer[0] == 0x0c0c && tempBuffer[1] == 0x0c0c && tempBuffer[2] == 0x0c0c && tempBuffer[3] == 0x0c0c)
                {
                    Debug.Print("XferToSD 0x0c: Writing to SD Card complete " + tempBuffer[0] + "\n");
                    return true;
                }
                else if (tempBuffer[0] == 0x0 && tempBuffer[1] == 0x0 && tempBuffer[2] == 0x0 && tempBuffer[3] == 0x0)
                {
                    Debug.Print("XferToSD 0x0: Writing to SD Card complete " + tempBuffer[0] + "\n");
                    return true;
                }
                else
                {
                    if (!removableStorage.Write(tempBuffer, (ushort)bufferSize))
                    {
                        Debug.Print("XferToSD : Write to SD Card failed \n");
                        return false;
                    }
                }
                ++readIndex;
                Array.Clear(tempBuffer, 0, tempBuffer.Length);
            }
        }

        public void Run()
        {
            lcd.Clear();
            lcd.Write(Samraksh.eMote.DotNow.LCD.CHAR_C, Samraksh.eMote.DotNow.LCD.CHAR_C, Samraksh.eMote.DotNow.LCD.CHAR_C, Samraksh.eMote.DotNow.LCD.CHAR_C);

            while (true)
            {
                if (channelIBuffer.IsFull())
                {
                    channelIBuffer.Persist(storage);
                }

                if (channelQBuffer.IsFull())
                {
                    channelQBuffer.Persist(storage);
                }

                System.Threading.Thread.Sleep(1000);

                if (stopExperimentFlag)
                {
                    storage.WriteEof(bufferSize);
                    lcd.Write(Samraksh.eMote.DotNow.LCD.CHAR_S, Samraksh.eMote.DotNow.LCD.CHAR_S, Samraksh.eMote.DotNow.LCD.CHAR_S, Samraksh.eMote.DotNow.LCD.CHAR_S);
                    XferToSD();
                    break;
                }
            }

            lcd.Write(Samraksh.eMote.DotNow.LCD.CHAR_D, Samraksh.eMote.DotNow.LCD.CHAR_D, Samraksh.eMote.DotNow.LCD.CHAR_D, Samraksh.eMote.DotNow.LCD.CHAR_D);
            
            Debug.Print("Total number of bytes collected : " + dataCollected.ToString());
            Debug.Print("Experiment Complete\n");
        }

        public static void Main()
        {
            Debug.EnableGCMessages(false);
            
            Debug.Print("Sleeping for 10s");
            System.Threading.Thread.Sleep(10000);

            Debug.Print("Starting ..");

            DataCollectorRadarSD dcrs = new DataCollectorRadarSD();

            Debug.Print("Calling Run ..");
            dcrs.Run();
        }

    }
}
