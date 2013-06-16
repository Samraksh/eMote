using System;
using Microsoft.SPOT;
using System.IO.Ports;
using Microsoft.SPOT.Hardware;


namespace NORSDTransfer
{

    public class Program
    {
        public const UInt16 BufferSize = 768;
        public static Random rng = new Random();
        public static UInt16[] buffer1 = new UInt16[BufferSize];
        public static UInt16[] buffer2 = new UInt16[BufferSize];
        public static byte[] serialBuffer = new byte[BufferSize * 2];
        public static UInt16 serialBufferDataCounter = 0;
        public static SerialPort serialPort;
        public static byte[] serialDataStartDelimiter = new byte[4];
        public static byte[] serialDataEndDelimter = new byte[4];
        public static OutputPort error = new OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN1, false);

        public static bool sdSuccessFlag = false;

        public Samraksh.SPOT.Hardware.EmoteDotNow.SD.SDCallBackType sdResultCallBack;

        public static void mySdCallback(Samraksh.SPOT.Hardware.DeviceStatus status)
        {
            if (status == Samraksh.SPOT.Hardware.DeviceStatus.Success)
                sdSuccessFlag = true;
            else
                sdSuccessFlag = false;
        }

        public static void SerialPortHandler(Object sender, SerialDataReceivedEventArgs e)
        {

        }

        public Program()
        {
            // Set 8K as max nor size for testing
            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Initialize(8 * 1024);

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

            sdResultCallBack = mySdCallback;

            Samraksh.SPOT.Hardware.EmoteDotNow.SD mysdCard = new Samraksh.SPOT.Hardware.EmoteDotNow.SD(sdResultCallBack);

            Samraksh.SPOT.Hardware.EmoteDotNow.SD.Initialize();
        }

        public void ResetBuffers()
        {
            for (UInt16 i = 0; i < BufferSize; i++)
            {
                buffer1[i] = 0;
                buffer2[i] = 0;
            }

            for (UInt16 i = 0; i < (BufferSize * 2); i++)
            {
                serialBuffer[i] = 0;
            }
        }

        public bool StreamToHost(UInt16[] data, UInt16 length)
        {
            for (UInt16 i = 0; i < length; i++)
            {
                serialBuffer[serialBufferDataCounter++] = (byte) (data[i] & 0xff);
                serialBuffer[serialBufferDataCounter++] = (byte)((data[i] >> 8) & 0xff);

                if (serialPort.IsOpen)
                {
                    serialPort.Write(serialDataStartDelimiter, 0, 4);
                    serialPort.Write(serialBuffer, 0, serialBufferDataCounter);
                    serialPort.Write(serialDataEndDelimter, 0, 4);
                    serialPort.Flush();
                }
                else
                    return false;

            }

            return true;
        }

        public bool StreamToSDCard(UInt16[] data, UInt16 length)
        {
            for (UInt16 i = 0; i < length; i++)
            {
                serialBuffer[serialBufferDataCounter++] = (byte)(data[i] & 0xff);
                serialBuffer[serialBufferDataCounter++] = (byte)((data[i] >> 8) & 0xff);
            }

            Samraksh.SPOT.Hardware.EmoteDotNow.SD.Write(serialBuffer, 0, serialBufferDataCounter);

            while (!sdSuccessFlag) ;

            return true;
        }

        public bool store()
        {

            for (UInt16 i = 0; i < BufferSize; i++)
            {
                buffer1[i] = (UInt16) rng.Next(1 << 12);
                buffer2[i] = (UInt16)rng.Next(1 << 12);
            }

            if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.IsFull())
            {
                return false;
            }

            if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.StartNewRecord())
            {
                StreamToHost(buffer1, BufferSize);
                Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(buffer1, BufferSize);
                Samraksh.SPOT.Hardware.EmoteDotNow.NOR.EndRecord();
                
            }

            if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.StartNewRecord())
            {
                StreamToHost(buffer2, BufferSize);
                Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Write(buffer2, BufferSize);
                Samraksh.SPOT.Hardware.EmoteDotNow.NOR.EndRecord();
            }



            return true;
        }

        public bool XferToSD()
        {
            ResetBuffers();

            while (!Samraksh.SPOT.Hardware.EmoteDotNow.NOR.eof())
            {
                if (Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Read(buffer1, BufferSize) != Samraksh.SPOT.Hardware.DeviceStatus.Success)
                {
                    error.Write(true);
                    return false;
                }
                if (StreamToSDCard(buffer1, BufferSize))
                {
                    error.Write(true);
                    return false;
                }
            }

            

            return true;

        }

        


        public static void Main()
        {
            Program p = new Program();

            while (true)
            {
                if (!p.store())
                {
                    if (!p.XferToSD())
                    {
                        error.Write(true);
                    }
                }
            }
        }

    }
}
