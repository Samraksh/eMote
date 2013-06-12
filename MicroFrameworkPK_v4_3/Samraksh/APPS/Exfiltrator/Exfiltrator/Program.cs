using System;
using Microsoft.SPOT;
using System.IO.Ports;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Exfiltrator
{
    public class Program
    {
        public static void Main()
        {

            UInt16[] m_sendBuffer = new UInt16[256];
            byte[] m_serialBuffer = new byte[512];

            UInt16 counter = 0;

            SerialPort serialPort = new SerialPort("COM1");
            serialPort.BaudRate = 115200;
            serialPort.Parity = System.IO.Ports.Parity.None;
            serialPort.StopBits = StopBits.One;
            serialPort.DataBits = 8;
            serialPort.Handshake = Handshake.None;

            serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPortHandler);

            serialPort.Open();

            Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Initialize();


            while (true)
            {
                Samraksh.SPOT.Hardware.EmoteDotNow.NOR.Read(m_sendBuffer, (ushort) 256);

                for (UInt16 i = 0; i < 256; i++)
                {
                    m_serialBuffer[counter++] = (byte) (m_sendBuffer[i] & 0xff);
                    m_serialBuffer[counter++] = (byte)((m_sendBuffer[i] >> 8) & 0xff);
                }

                serialPort.Write(m_serialBuffer, 0, 512);

                Thread.Sleep(200);
                
            }
        }

        static void SerialPortHandler(object sender, SerialDataReceivedEventArgs e)
        {
            byte[] m_recvBuffer = new byte[100];
            SerialPort serialPort = (SerialPort)sender;

            int numBytes = serialPort.BytesToRead;
            serialPort.Read(m_recvBuffer, 0, numBytes);
            serialPort.Write(m_recvBuffer, 0, numBytes);
            serialPort.Flush();

        }

    }
}
