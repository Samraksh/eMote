using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Text;
using System.Threading;

namespace Samraksh.PhysicalModel
{
    enum VirtEmulatorInterface
    {
        USART, SPI, I2C, USB,
    } ;

    class PhysicalModelEmulatorComm
    {

        int emulatorPort = 47000;
        int numberOfRetries = 10;
        //IPAddress emulatorIP = IPAddress.Parse("localhost");
        IPAddress emulatorIP = IPAddress.Loopback;
        TcpClient emulatorClient;
        Stream emulatorStream;
        Timer readTimer;

        //public delegate ReadCallback;
        public delegate void InterfaceCallback(byte[] Message);
        InterfaceCallback mySerialHandler, mySPIHandler, myI2CHandler, myUSBHandler;

        public PhysicalModelEmulatorComm()
        {

        }
        public PhysicalModelEmulatorComm(int EmulatorPort, String EmulatorIP)
        {
            this.emulatorPort = EmulatorPort;
            this.emulatorIP = IPAddress.Parse(EmulatorIP);
            //readTimer = new Timer(new TimerCallback(ReadTimerHandler), null, Timeout.Infinite, Timeout.Infinite);
        }

        public void InitializeSerial(InterfaceCallback Handler)
        {
            mySerialHandler = new InterfaceCallback(Handler);
        }
        public void InitializeSPI(InterfaceCallback Handler)
        {
            mySPIHandler = new InterfaceCallback(Handler);
        }
        public void InitializeI2C(InterfaceCallback Handler)
        {
            myI2CHandler = new InterfaceCallback(Handler);
        }
        public void InitializeUSB(InterfaceCallback Handler)
        {
            myUSBHandler = new InterfaceCallback(Handler);
        }
        public void SendToSerial(byte[] mesg)
        {
            OutputToEmulator(VirtEmulatorInterface.USART, mesg);
        }
        public void SendToSPI(byte[] mesg)
        {
            OutputToEmulator(VirtEmulatorInterface.SPI, mesg);
        }
        public void SendToI2C(byte[] mesg)
        {
            OutputToEmulator(VirtEmulatorInterface.I2C, mesg);
        }
        public void SendToUSB(byte[] mesg)
        {
            OutputToEmulator(VirtEmulatorInterface.USB, mesg);
        }


        void OutputToEmulator(VirtEmulatorInterface type, byte[] sbuffer)
        {
            byte[] packet = new byte[1];
            switch (type)
            {
                case VirtEmulatorInterface.USART:
                    packet[0] = (byte)VirtEmulatorInterface.USART;
                    break;
                case VirtEmulatorInterface.I2C:
                    packet[0] = (byte)VirtEmulatorInterface.I2C;
                    break;
                case VirtEmulatorInterface.SPI:
                    packet[0] = (byte)VirtEmulatorInterface.SPI;
                    break;
                case VirtEmulatorInterface.USB:
                    packet[0] = (byte)VirtEmulatorInterface.USB;
                    break;
                default:
                    Console.WriteLine("Error: Unknown Emulator Interface: " + type.ToString());
                    break;
            }

            packet[0] = (byte)VirtEmulatorInterface.USART;
            packet.Concat(sbuffer);
            Console.WriteLine("Sending emulator packet of length: " + packet.Length.ToString());
            emulatorStream.Write(packet, 0, packet.Length);
        }

        public void ReadTimerHandler(Object o)
        {

            byte[] bb = new byte[100];
            if (emulatorStream != null)
            {
                int k = emulatorStream.Read(bb, 0, 100);
                if (k > 0)
                {
                    ParseInput(bb, k);
                }
            }
            else
            {
                Console.WriteLine("Error: Emulator Stream not initialized");
            }
        }

        void ParseInput(byte[] inputPacket, int length)
        {
            switch (inputPacket[0])
            {
                case (byte)VirtEmulatorInterface.USART:
                    HandleSerialInput(inputPacket, length);
                    break;
                case (byte)VirtEmulatorInterface.I2C:
                    HandleI2CInput(inputPacket);
                    break;
                case (byte)VirtEmulatorInterface.SPI:
                    HandleSPIInput(inputPacket);
                    break;
                case (byte)VirtEmulatorInterface.USB:
                    HandleUSBInput(inputPacket);
                    break;
                default:
                    Console.WriteLine("Error: Unknown Emulator Interface: " + inputPacket[0].ToString());
                    break;
            }
        }

        void HandleSerialInput(byte[] input, int length)
        {
            //Console.WriteLine("Got a message from the emulator serial port, parsing it..");
            byte[] mesg = new byte[length - 1];
            for (int i = 0; i < length - 1; i++)
            {
                mesg[i] = input[i + 1];
            }
            mySerialHandler(mesg);
        }
        void HandleI2CInput(byte[] input)
        {

        }
        void HandleSPIInput(byte[] input)
        {

        }
        void HandleUSBInput(byte[] input)
        {

        }

        public bool ConnectToEmulator()
        {
            try
            {
                emulatorClient = new TcpClient();
                Console.WriteLine("Connecting.....");

                emulatorClient.Connect(emulatorIP, emulatorPort);
                // use the ipaddress as in the server program

                Console.WriteLine("Connected");
                // Console.Write("Enter the string to be transmitted : ");

                //String str = Console.ReadLine();
                emulatorStream = emulatorClient.GetStream();

                String str = "Hello From Client";
                ASCIIEncoding asen = new ASCIIEncoding();
                byte[] ba = asen.GetBytes(str);
                Console.WriteLine("Exchanging Handshake.....");

                emulatorStream.Write(ba, 0, ba.Length);

                /*byte[] readBuf = new byte[100];

                int rBytes =emulatorStream.Read(readBuf, 0, 100);
                if (rBytes > 0)
                {
                    Console.WriteLine("Got something..");
                }*/
                readTimer = new Timer(new TimerCallback(ReadTimerHandler), null, 0, 50);

                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine("Error..... " + e.StackTrace);
            }
            return false;
        }

        ~PhysicalModelEmulatorComm()
        {
            emulatorClient.Close();
        }


    }
}
