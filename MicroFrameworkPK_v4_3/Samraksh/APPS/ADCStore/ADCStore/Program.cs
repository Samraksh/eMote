using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Hardware;
using Microsoft.SPOT.Hardware;
using System.Threading;
using System.IO.Ports;

namespace ADCStore
{
    public class ADCS
    {
        const UInt16 BufferSize = 750;

        Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput adc = new Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput();
        public static OutputPort alive = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);
        public static OutputPort norflashFull = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN2, false);
        public static UInt16[] IValue = new UInt16[BufferSize];
        public static UInt16[] QValue = new UInt16[BufferSize];
        public static UInt16[] channelValues = new UInt16[2];
        public static UInt16 IvalueCounter = 0;
        public static UInt16 QvalueCounter = 0;
        public static SerialPort serialPort;
        public static byte[] IValueSerialBuffer = new byte[BufferSize * 2];
        public static byte[] QValueSerialBuffer = new byte[BufferSize * 2];
        public static UInt16 ISerialBuffervalueCounter = 0;
        public static UInt16 QSerialBuffervalueCounter = 0;
        public static Timer sampler;
        public static bool norFlashFlag = false;
        public static bool aliveFlag = false;
        public static byte[] serialDataStartDelimiter = new byte[4];
        public static byte[] serialDataEndDelimter = new byte[4];

        public ADCS()
        {
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Init(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel1);
            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.Init(Samraksh.SPOT.Hardware.EmoteDotNow.ADCChannel.ADC_Channel2);
            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Initialize(8 * 750);

            serialPort = new SerialPort("COM1");
            serialPort.BaudRate = 115200;
            serialPort.Parity = Parity.None;
            serialPort.StopBits = StopBits.One;
            serialPort.DataBits = 8;
            serialPort.Handshake = Handshake.None;

            serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPortHandler);

            serialPort.Open();
            serialDataStartDelimiter[0] = 0xf0;
            serialDataStartDelimiter[1] = 0x0f;
            serialDataStartDelimiter[2] = 0xe0;
            serialDataStartDelimiter[3] = 0x0e;

            serialDataEndDelimter[0] = 0xc0;
            serialDataEndDelimter[1] = 0x0c;
            serialDataEndDelimter[2] = 0xd0;
            serialDataEndDelimter[3] = 0x0d;

        }

        public static void SerialPortHandler(Object sender, SerialDataReceivedEventArgs e)
        {

        }

        public bool ResetSerialBuffers()
        {
            for (UInt16 i = 0; i < (BufferSize * 2); i++)
            {
                IValueSerialBuffer[i] = 0;
                QValueSerialBuffer[i] = 0;
            }

            ISerialBuffervalueCounter = 0;
            QSerialBuffervalueCounter = 0;

            return true;
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
            if (IvalueCounter % 50 == 0)
            {
                if (aliveFlag == true)
                {
                    alive.Write(aliveFlag);
                    aliveFlag = false;
                }
                else
                {
                    alive.Write(aliveFlag);
                    aliveFlag = true;
                }
            }
                    

            Samraksh.SPOT.Hardware.EmoteDotNow.AnalogInput.DualChannelRead(channelValues);


            if (IvalueCounter >= BufferSize)
            {
                Debug.Print("Buffer Full \n");

                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.IsFull())
                {
                    Debug.Print("Nor Full \n");
                    norflashFull.Write(true);
                    serialPort.Close();
                    return false;
                }

                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.StartNewRecord())
                {

                    for (UInt16 i = 0; i < IvalueCounter; i++)
                    {
                        IValueSerialBuffer[ISerialBuffervalueCounter++] = (byte)(IValue[i] & 0xff);
                        IValueSerialBuffer[ISerialBuffervalueCounter++] = (byte)((IValue[i] >> 8) & 0xff);
                        QValueSerialBuffer[QSerialBuffervalueCounter++] = (byte)(QValue[i] & 0xff);
                        QValueSerialBuffer[QSerialBuffervalueCounter++] = (byte)((QValue[i] >> 8) & 0xff);
                    }
                    if (serialPort.IsOpen)
                    {
                        serialPort.Write(serialDataStartDelimiter, 0, 4);
                        serialPort.Write(IValueSerialBuffer, 0, IvalueCounter);
                        serialPort.Write(serialDataEndDelimter, 0, 4);
                        serialPort.Flush();
                    }
                    Debug.Print("Writing to NOR Flash I Values \n");
                    Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(IValue, IvalueCounter);
                    if (serialPort.IsOpen)
                    {
                        serialPort.Write(serialDataStartDelimiter, 0, 4);
                        serialPort.Write(QValueSerialBuffer, 0, QvalueCounter);
                        serialPort.Write(serialDataEndDelimter, 0, 4);
                        serialPort.Flush();
                    }
                    Debug.Print("Writing to NOR Flash Q Values \n");
                    Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(QValue, QvalueCounter);
                    Samraksh.SPOT.Hardware.EmoteDotNow.NOR.EndRecord();
                }
                ResetBuffers();
                ResetSerialBuffers();

            }
            
            if(IvalueCounter < BufferSize)
            {
                IValue[IvalueCounter++] = channelValues[0];
                QValue[QvalueCounter++] = channelValues[1];
            }


            //Debug.Print("Sampling  I : " + channelValues[0].ToString() + " Q : " + channelValues[1].ToString());

            return true;

        }

        void sampleCallback(Object state)
        {
            if (!Sample())
            {
                sampler.Dispose();
            }
                
        }

        public void Run()
        {
            sampler = new Timer(sampleCallback, null, 1000, 50);

            while (true)
            {
                Thread.Sleep(25);
            }
        }
    }

    public class Program
    {
        public static void Main()
        {
            Debug.EnableGCMessages(false);

            ADCS adcTest = new ADCS();

            adcTest.Run();
        }

    }
}
