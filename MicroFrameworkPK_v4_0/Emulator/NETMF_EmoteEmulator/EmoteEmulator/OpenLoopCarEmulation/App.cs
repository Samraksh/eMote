using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Messaging;

using System.IO.Ports;

namespace OpenLoopCarEmulation
{
    public class CarEmulationTest
    {
     
        InterruptPort _button1_InterruptPort, _button2_InterruptPort, _button3_InterruptPort;
        static OutputPort _led1_port, _led2_port, _led3_port;
        static bool button1_state, button2_state, button3_state;
        Timer mainTimer;
        SerialPort mySerialPort;
        CarPMInput carInput;
        public const byte STOP = 255;

        uint X, Y = 0;

        public CarEmulationTest()
        {
           /* _button1_InterruptPort = new InterruptPort((Cpu.Pin)9, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);
            _button2_InterruptPort = new InterruptPort((Cpu.Pin)10, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);
            _button3_InterruptPort = new InterruptPort((Cpu.Pin)22, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeBoth);

            _button1_InterruptPort.OnInterrupt += new NativeEventHandler(button1_OnInterrupt);
            _button2_InterruptPort.OnInterrupt += new NativeEventHandler(button2_OnInterrupt);
            _button3_InterruptPort.OnInterrupt += new NativeEventHandler(button3_OnInterrupt);
            
            //_led1_port = new OutputPort((Cpu.Pin)21, false);
            //_led2_port = new OutputPort((Cpu.Pin)4, false);
            //_led3_port = new OutputPort((Cpu.Pin)38, false);
          
            _led1_port = new OutputPort((Cpu.Pin)1, false);
            _led2_port = new OutputPort((Cpu.Pin)2, false);
            _led3_port = new OutputPort((Cpu.Pin)3, false);*/
           
            
            //mySerialPort = new SerialPort("COM2", 115200, Parity.None);
            try
            {
                mySerialPort = new SerialPort("COM1");
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

           // mainTimer = new Timer(new TimerCallback(readModelParameters), null, 0, 1000);
            carInput = new CarPMInput();

        }

        void readModelOutput(object state)
        {
            //X += 4;
            //Y += 2;
            //(intToBytes(X)).CopyTo(buffer, 0);
            //(intToBytes(Y)).CopyTo(buffer, 4);

            //mySerialPort.Handshake = Handshake.None;
            //mySerialPort.Parity = Parity.None;
            //mySerialPort.BaudRate = 9600;
            //mySerialPort.StopBits = StopBits.One;
            //mySerialPort.DataBits = 8;

            //Debug.Print("PortString: " +mySerialPort.ToString());
            //Debug.Print("PortHash: " +(mySerialPort.GetHashCode()).ToString());
            if (!mySerialPort.IsOpen)
            {
                mySerialPort.Open();
            }
            

        }

        void sendModelInput()
        {
            byte[] buffer = new byte[4];

            ShortToBytes(carInput.SteeringAngle, buffer);
            buffer[2] = (byte)carInput.Accelaration;
            buffer[3] = carInput.State;
            if (!mySerialPort.IsOpen)
            {
                mySerialPort.Open();
            }
       
            int n = mySerialPort.Write(buffer, 0, buffer.Length);

            if (n <= 0)
            {
                Debug.Print("Failure: " + n.ToString());
            }
            else
            {
                Debug.Print("Success, sent: " + n.ToString());
            }
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

        static void button1_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            //if (button1_state) button1_state = false; else button1_state = true;

            //_led1_port.Write(button1_state);
            Debug.Print("Button 1" + ((button1_state) ? "Down" : "Up"));
        }

        static void button2_OnInterrupt(uint data1, uint data2, DateTime time)
        {
           // if (button2_state) button2_state = false; else button2_state = true;
            //_led2_port.Write(button2_state);
            Debug.Print("Button 2: " + "Go Left");
        }

        static void button3_OnInterrupt(uint data1, uint data2, DateTime time)
        {
            //if (button3_state) button3_state = false; else button3_state = true;
            //_led3_port.Write(button3_state);
            Debug.Print("Button 3: " + "No Action");
        }

        void Turn(short degrees)
        {
            carInput.SteeringAngle = degrees;
            carInput.State = 0;
            carInput.Accelaration = 0;
            sendModelInput();
        }
        void Stop()
        {
            carInput.State = STOP;
            sendModelInput();
        }

        void Drive_A_Eight(){

            Thread.Sleep(5000); //Drive Straight for 5s
            
            Turn(70);
            Thread.Sleep(3000); //Slight Left for 3s
            Turn(-70);

            Thread.Sleep(5000); //Drive Straight for 5s

            Turn(-90); //Turn right (almost u-turn) 13s
            Thread.Sleep(13000);
            Turn(90);

            Thread.Sleep(10000); //Drive Straight for 10s

            Turn(90); //Turn left for 5s
            Thread.Sleep(5000); 
            Turn(-90);

            Thread.Sleep(5000); //Drive Straight for 5s

            Turn(-90);//Turn right for 10s
            Thread.Sleep(10000);
            Turn(90);

            Thread.Sleep(7000);//Drive Straight for 10s
            Stop();


        }
       
        
        public static void Main()
        {
            
            CarEmulationTest c = new CarEmulationTest();
            //Debug.Print("Starting a 8 Test");
            c.Drive_A_Eight();
            //Debug.Print("Done with 8 Test");
                        
            //Sleep Indefinitely
            Thread.Sleep(Timeout.Infinite);
        }
        
    }
}
