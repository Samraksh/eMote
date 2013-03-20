using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Messaging;
using System.Threading;
using System.IO.Ports;

namespace CarDriver
{
    class CarModelInput
    {
        public short SteeringAngle; //-128to127
        public sbyte Acceleration;//int8 -63 to 63
        public byte State; //Special instructions 
    }

    class CarModelOutput
    {
        public ushort X; //0-65535
        public ushort Y; //0-65535
        public byte speed;//0-255
        public short direction; //degrees 
    }

    enum CarCommands
    {
        START, STOP
    };

    public class Program
    {

        SerialPort phyiscalModelPort;
        CarModelInput carInput;

        Program()
        {
            carInput = new CarModelInput();
            try
            {
                phyiscalModelPort = new SerialPort("COM1");
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }

        void SendModelInput()
        {
            byte[] buffer = new byte[4];

            ShortToBytes(carInput.SteeringAngle, buffer);
            buffer[2] = (byte)carInput.Acceleration;
            buffer[3] = carInput.State;
            if (!phyiscalModelPort.IsOpen)
            {
                phyiscalModelPort.Open();
            }

            int n = phyiscalModelPort.Write(buffer, 0, buffer.Length);

            if (n <= 0)
            {
                Debug.Print("Failure: " + n.ToString());
            }
            else
            {
                Debug.Print("Success, sent: " + n.ToString());
            }
        }


        void ReadModelOutput(object state)
        {
            if (!phyiscalModelPort.IsOpen)
            {
                phyiscalModelPort.Open();
            }
            byte[] readBuffer = new byte[100];
            int bytes_read = phyiscalModelPort.Read(readBuffer, 0, 100);
            if (bytes_read > 0)
            {
                ProcessInput(readBuffer,bytes_read);
            }
        }

        void ProcessInput(byte[] input, int length)
        {
        }

        void Turn(short degrees)
        {
            carInput.SteeringAngle = degrees;
            carInput.State = 0;
            carInput.Acceleration = 0;
            SendModelInput();
        }
        void Start()
        {
            carInput.State = (byte)CarCommands.START;
            SendModelInput();
        }
        void Stop()
        {
            carInput.State = (byte)CarCommands.STOP;
            SendModelInput();
        }
        void TestDrive()
        {
            Start();
            Thread.Sleep(8000); //Drive Straight for 5s

            Turn(20);
            Thread.Sleep(5000); //Slight Left for 3s
            Turn(-20);

            Thread.Sleep(5000); //Drive Straight for 5s

            Turn(-45); //Turn right (almost u-turn) 13s
            Thread.Sleep(7000);
            Turn(45);

            Thread.Sleep(5000); //Drive Straight for 10s

            Turn(45); //Turn left for 5s
            Thread.Sleep(9000);
            Turn(-45);

            Thread.Sleep(4000); //Drive Straight for 5s

            Turn(-45);//Turn right for 10s
            Thread.Sleep(7000);
            Turn(45);

            Thread.Sleep(6000);//Drive Straight for 10s
            Stop();

        }

        byte[] intToBytes(uint x)
        {
            byte[] bytes = new byte[4];
            bytes[3] = (byte)(x >> 24);
            bytes[2] = (byte)(x >> 16);
            bytes[1] = (byte)(x >> 8);
            bytes[0] = (byte)x;
            return bytes;
        }
        void ShortToBytes(short x, byte[] ret)
        {
            //byte[] bytes = new byte[2];
            ret[1] = (byte)(x >> 8);
            ret[0] = (byte)x;
            //return bytes;
        }

        public static void Main()
        {
            Program P = new Program();
            Thread.Sleep(10000);
            P.TestDrive();
            Thread.Sleep(10000); //Sleep for 10 secs then exit.
        }
    }
}
