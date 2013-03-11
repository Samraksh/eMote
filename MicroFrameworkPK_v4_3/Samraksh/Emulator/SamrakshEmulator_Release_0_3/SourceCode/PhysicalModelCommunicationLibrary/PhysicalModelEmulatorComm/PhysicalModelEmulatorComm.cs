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
        ADC, USART, SPI, I2C, USB, PMC
    } ;

    public class PhysicalModelEmulatorComm
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
       
        
        
        /// <summary>
        /// Send APIs
        /// </summary>
        /// <param name="mesg"></param>
        /// <param name="length"></param>
        public void SendToADC(ushort[] ADCValues, int NumberOfChannels)
        {
            byte[] OutBuffer = new byte[2 * NumberOfChannels];
            for (int i = 0; i < NumberOfChannels; i++)
            {
                OutBuffer[i * 2] = (byte)(ADCValues[i] >> 8);
                OutBuffer[i * 2 + 1] = (byte)ADCValues[i];
            }
            OutputToEmulator(VirtEmulatorInterface.ADC, OutBuffer, OutBuffer.Length);
        }
        public void SendToSerial(byte[] mesg, int length)
        {
            OutputToEmulator(VirtEmulatorInterface.USART, mesg, length);
        }
        public void SendToSPI(byte[] mesg, int length)
        {
            OutputToEmulator(VirtEmulatorInterface.SPI, mesg, length);
        }
        public void SendToI2C(byte[] mesg, int length)
        {
            OutputToEmulator(VirtEmulatorInterface.I2C, mesg, length);
        }
        public void SendToUSB(byte[] mesg, int length)
        {
            OutputToEmulator(VirtEmulatorInterface.USB, mesg, length);
        }


        void OutputToEmulator(VirtEmulatorInterface type, byte[] sbuffer, int length)
        {
            byte[] packet = new byte[length+1];
            switch (type)
            {
                case VirtEmulatorInterface.ADC:
                    packet[0] = (byte)VirtEmulatorInterface.ADC;
                    break;
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
                    //Console.WriteLine("Error: Unknown Emulator Interface: " + type.ToString());
                    break;
            }

            sbuffer.CopyTo(packet,1);
            //Console.WriteLine("Sending emulator packet of length: " + packet.Length.ToString());
            emulatorStream.Write(packet, 0, packet.Length);
        }

        public void ReadTimerHandler(Object o)
        {

            byte[] bb = new byte[100];
            if (emulatorStream != null )
            {
                if( emulatorClient.Available > 0){
                    int k = emulatorStream.Read(bb, 0, 100);
                    if (k > 0)
                    {
                        ParseInput(bb, k);
                        //Console.WriteLine("Read from emulator: " + k.ToString());
                    }
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
                case 84:
                    Console.WriteLine("Got Ack from Emulator");
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
