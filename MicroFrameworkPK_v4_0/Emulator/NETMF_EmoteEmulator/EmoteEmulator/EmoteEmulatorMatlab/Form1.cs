using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows;
using System.Windows.Forms;
using System.Threading;
using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Emulator.Com;
using System.IO.Ports;
using Microsoft.SPOT.Emulator.Gpio;

namespace EmoteEmulator
{
    public partial class Form1 : Form
    {
        /// <summary>
        /// Generic Emulator Variables
        /// </summary>
        private Emulator _emulator; 
        static GpioPort[] _gpio = new GpioPort[8];
        Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort _esp;
        GpioPort _led1_port, _led2_port, _led3_port;
        ExternalInput ei = new ExternalInput("D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\Emulator\\NETMF_EmoteEmulator\\EmoteEmulator\\EmoteEmulator\\LedTest_2.txt");
        bool led1_state, led2_state, led3_state;
        delegate void GpioPortWriteDelegate(bool state);

        /// Create Matlab object
        object matlab;
        


        /// <summary>
        /// App specific variables
        /// </summary>

        Form2 f2;
        const byte STOP = 255;
        CarPMInput carInput;
        CarPMOutput carOutput;

        void ConnectToMatlab()
        {
           
            Type matlabType;
            matlabType = Type.GetTypeFromProgID("matlab.application");
            matlab = Activator.CreateInstance(matlabType);

        }

        public Form1(Emulator emulator)
        {
            
            _emulator = emulator;
            //_cg = cg;
            InitializeComponent();

            for (int i = 0; i < 8; i++)
            {
                _gpio[i] = new GpioPort();
            }
           

            /* _gpio[0] = _emulator.FindComponentById("gpio_0") as GpioPort;
            _gpio[1] = _emulator.FindComponentById("gpio_1") as GpioPort;
            _gpio[2] = _emulator.FindComponentById("gpio_2") as GpioPort;
            _gpio[3] = _emulator.FindComponentById("gpio_3") as GpioPort;
            _gpio[4] = _emulator.FindComponentById("gpio_4") as GpioPort;
            _gpio[5] = _emulator.FindComponentById("gpio_5") as GpioPort;
            _gpio[6] = _emulator.FindComponentById("gpio_6") as GpioPort;
            _gpio[7] = _emulator.FindComponentById("gpio_7") as GpioPort;

            _led1_port = _emulator.FindComponentById("led_1") as GpioPort;
            _led2_port = _emulator.FindComponentById("led_2") as GpioPort;
            _led3_port = _emulator.FindComponentById("led_3") as GpioPort;
            
            _led1_port.OnGpioActivity += new GpioActivity(_led1_port_OnGpioActivity);
            _led2_port.OnGpioActivity += new GpioActivity(_led2_port_OnGpioActivity);
            _led3_port.OnGpioActivity += new GpioActivity(_led3_port_OnGpioActivity);
            */

            carInput = new CarPMInput();

            _esp = _emulator.FindComponentById("COM1") as Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort;
            if (_esp != null)
            {
                Console.WriteLine("Found com port 1: " + _esp.ComPortHandle.ToString());
                Console.WriteLine(_esp.ToString());
                

                Thread comPortWatcherThread = new Thread(new ThreadStart(ComPortWatchThreadMethod));
                comPortWatcherThread.IsBackground = true;
                comPortWatcherThread.Start();
            }
            else
            {
                Console.WriteLine("Could not find COM Port");
            }

            //StartCarEmulation();
        }

        /*void StartCarEmulation()
        {
            //cg;

            Console.WriteLine("Creating Car GUI");

            Window guiWindow = new Window();
            guiWindow.Height = 650;
            guiWindow.Width = 650;
            guiWindow.Title = "Open-loop Car Emulation";
            guiWindow.Visibility = Visibility.Visible;
            //Console.WriteLine("Done 1");

            cg = new CarGUI();
            //cg.Show();
            //Console.WriteLine("Done 2");
            guiWindow.Content = cg;

            //guiWindow.ShowDialog();
            //new Thread(() => guiWindow.ShowDialog()).Start();
            guiWindow.Show();
            //System.Windows.Window.A

            //Console.WriteLine("Done 2.1");

            //OpenLoopCar oCar = new OpenLoopCar(cg);

            //Console.WriteLine("Done 3");
            //Thread.Sleep(Timeout.Infinite);

        }
        */
        private void ComPortWatchThreadMethod()
        {
           

            byte[] buffer = new byte[1024];
            Console.WriteLine("Starting com watch");
            while (true)
            {
                //Console.WriteLine("checking com..");
                int espCount = 0;
                _esp.Invoke(new MethodInvoker(delegate 
                    {
                        espCount = _esp.StreamOut.Read(buffer, 0, buffer.Length);
                    }
                ));
                if (espCount > 0)
                {                    
                    carInput.SteeringAgle = BytesToShort(buffer);
                    carInput.Accelaration = (sbyte) buffer[2];
                    carInput.State = buffer[3];
                    //Console.WriteLine("Received somthing");
                    Invoke(new MethodInvoker(delegate
                        {
                            Console.WriteLine("Read from app: " + espCount.ToString() + ", Wheel Angle:" + (carInput.SteeringAgle).ToString() + " , Accl:" + carInput.Accelaration.ToString());
                            if (carInput.State == STOP)
                            {
                                f2.olc.Stop();
                            }
                            else
                            {
                                f2.olc.Turn(carInput.SteeringAgle);
                            }
                            //cg.moveCar(location[0], location[1], 750);
                        }
                     ));
                }
                Thread.Sleep(500);
            }
        }

        int[] BytesToInt(byte[] x)
        {
            int[] ret=new int[2];
            //f (BitConverter.IsLittleEndian)
                //Array.Reverse(x);
            ret[0]=BitConverter.ToInt32(x, 0);
            ret[1] = BitConverter.ToInt32(x, 4);
            
            return ret;
        }
        short BytesToShort(byte[] x)
        {
            short ret;
            //f (BitConverter.IsLittleEndian)
            //Array.Reverse(x);
            ret = BitConverter.ToInt16 (x, 0);
            //ret[1] = BitConverter.ToInt32(x, 4);

            return ret;
        }


        void _led3_port_OnGpioActivity(GpioPort sender, bool edge)
        {
            led3_state = sender.Read();
            if (led3_state) led3_on(); else led3_off();
        }

        void _led2_port_OnGpioActivity(GpioPort sender, bool edge)
        {
            led2_state = sender.Read();
            if (led2_state) led2_on(); else led2_off();
        }

        void _led1_port_OnGpioActivity(GpioPort sender, bool edge)
        {
            led1_state = sender.Read();
            if(led1_state)  led1_on(); else led1_off();
            //throw new NotImplementedException();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
                
        private static void GpioPortSafeWrite(GpioPort port, bool value)
        {
            port.Invoke(new GpioPortWriteDelegate(port.Write), value);
        }

        private bool GpioPortSafeRead(GpioPort port)
        {
            bool value = port.Read();
            return value;
        }

        public void led1_on()
        {
            led1.BackColor = Color.FromName("Red");
        }

        public void led1_off()
        {
            led1.BackColor = Color.FromName("MistyRose");
        }

        public void led2_on()
        {
            led2.BackColor = Color.FromName("Blue");
        }

        public void led2_off()
        {
            led2.BackColor = Color.FromName("LightSteelBlue");
        }

        public void led3_on()
        {
            led3.BackColor = Color.FromName("Green");
        }

        public void led3_off()
        {
            led3.BackColor = Color.FromName("Honeydew");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (led1_state)
            {
                led1_state = false;
                GpioPortSafeWrite(_gpio[0], false);                
            }
            else
            {
                led1_state = true;
                GpioPortSafeWrite(_gpio[0], true);        
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (led2_state)
            {
                led2_state = false;
                GpioPortSafeWrite(_gpio[1], false);
            }
            else
            {
                led2_state = true;
                GpioPortSafeWrite(_gpio[1], true);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (led3_state)
            {
                led3_state = false;
                GpioPortSafeWrite(_gpio[2], false);
            }
            else
            {
                led3_state = true;
                GpioPortSafeWrite(_gpio[2], true);
            }
        }

        public static void signalGpio(int port_id, Boolean value)
        {
            GpioPortSafeWrite(_gpio[port_id], value);
        }

        private void inputEmulation_Click(object sender, EventArgs e)
        {
            
            ei.Run();
        }

        private void button_carGUI_Click(object sender, EventArgs e)
        {
            f2 = new Form2();
            
            f2.Height = 800;
            f2.Width = 700;
            f2.Show();        
        }

      
    }
}
