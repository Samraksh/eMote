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

namespace ADAPTEmulator
{
    public partial class Form1 : Form
    {
        /// <summary>
        /// Generic Emulator Variables
        /// </summary>
    
        ///Declare Samraksh Extensions
        ExternalInput ei;
        PhysicalModelVirtCom vCom;
        Logger Log;
        Thread comPortWatcherThread; 

        private Emulator _emulator; 
        static GpioPort[] _gpio = new GpioPort[8];
        static GpioPort[] _button = new GpioPort[3];
        static GpioPort[] _led = new GpioPort[3];
            
        Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort _esp;
        


        //= new ExternalInput("D:\\Main\\Emote-dev\\Dev\\MicroFrameworkPK_v4_0\\Emulator\\NETMF_EmoteEmulator\\ADAPTEmulator\\EmulatorTestApp\\LedTest_2.txt");
        bool led1_state, led2_state, led3_state;
        bool button1_state, button2_state, button3_state;
        delegate void GpioPortWriteDelegate(bool state);

        /// <summary>
        /// App specific variables
        /// </summary>

        //Form2 f2;
        const byte STOP = 255;
        //CarPMInput carInput;
        //CarPMOutput carOutput;
              
        public Form1(Emulator emulator)
        {
            _emulator = emulator;
            InitializeComponent();
         
            //Initialize the Logger;
            Log = new Logger(this);
                                  
            _led[0] = _emulator.FindComponentById("led_0") as GpioPort;
            _led[1] = _emulator.FindComponentById("led_1") as GpioPort;
            _led[2] = _emulator.FindComponentById("led_2") as GpioPort;

            
            _button[0] = _emulator.FindComponentById("button_0") as GpioPort;
            _button[1] = _emulator.FindComponentById("button_1") as GpioPort;
            _button[2] = _emulator.FindComponentById("button_2") as GpioPort;

            
            _gpio[0] = _emulator.FindComponentById("gpio_0") as GpioPort;
            _gpio[1] = _emulator.FindComponentById("gpio_1") as GpioPort;
            _gpio[2] = _emulator.FindComponentById("gpio_2") as GpioPort;
            _gpio[3] = _emulator.FindComponentById("gpio_3") as GpioPort;
            _gpio[4] = _emulator.FindComponentById("gpio_4") as GpioPort;
            _gpio[5] = _emulator.FindComponentById("gpio_5") as GpioPort;
            _gpio[6] = _emulator.FindComponentById("gpio_6") as GpioPort;
            _gpio[7] = _emulator.FindComponentById("gpio_7") as GpioPort;

            
            _led[0].OnGpioActivity += new GpioActivity(_led1_port_OnGpioActivity);
            _led[1].OnGpioActivity += new GpioActivity(_led2_port_OnGpioActivity);
            _led[2].OnGpioActivity += new GpioActivity(_led3_port_OnGpioActivity);

            
            //carInput = new CarPMInput();

            _esp = _emulator.FindComponentById("COM1") as Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort;
            if (_esp != null)
            {
                Log.Debug(Module.CORE,  "Found Emulator COM port 1: " + _esp.ComPortHandle.ToString());
                Log.Debug(Module.CORE,  _esp.ToString());


                comPortWatcherThread = new Thread(new ThreadStart(ComPortWatchThreadMethod));
                comPortWatcherThread.IsBackground = true;
                comPortWatcherThread.Start();
            }
            else
            {
                Log.Debug(Module.CORE,  "Could not find COM Port");
            }
            //Start the Virtual Communication Server to the Physical Model.
            vCom = new PhysicalModelVirtCom();
            vCom.StartVirtComServer();

          
          
        }
        private void ComPortWatchThreadMethod()
        {
            byte[] buffer = new byte[1024];
            Log.Debug(Module.CORE, "Watching for communication from Model..");
            while (true)
            {
                //Log.Debug(Module.CORE, "checking com..");
                int espCount = 0;
                _esp.Invoke(new MethodInvoker(delegate 
                    {
                        espCount = _esp.StreamOut.Read(buffer, 0, buffer.Length);
                    }
                ));
                if (espCount > 0)
                {
                    Log.Debug(Module.CORE, "Received bytes: " + espCount.ToString());
                    Invoke(new MethodInvoker(delegate
                        {
                           //Send message to the Model
                            byte[] packet = new byte[espCount+1];
                            packet[0] = (byte)VirtEmulatorInterface.USART;
                            for (int i=0; i< espCount; i++)
                            packet[i+1] = buffer[i];
                            if (vCom.Connected)
                            {
                                int s_bytes = vCom.SendModelMessage(packet);
                                Log.Debug(Module.CORE, "Sent to model packet of length : " + s_bytes.ToString());
                            }
                            else
                            {
                                Log.Debug(Module.CORE, "Warning: Looks like the Physical Model has not connected.");
                                Log.Debug(Module.CORE, "Warning: Please start the Physical Model for correct execution.");
                            }
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

        public void DisplayStatus(String s){
            statusBox.Text += s + "\n";
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
            if (button1_state)
            {
                button1_state = false;
                GpioPortSafeWrite(_button[0], false);
            }
            else
            {
                button1_state = true;
                GpioPortSafeWrite(_button[0], true);
            }
        }
        

        private void button2_Click(object sender, EventArgs e)
        {
            if (button2_state)
            {
                button2_state = false;
                GpioPortSafeWrite(_button[1], false);
            }
            else
            {
                button2_state = true;
                GpioPortSafeWrite(_button[1], true);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (button3_state)
            {
                button3_state = false;
                GpioPortSafeWrite(_button[2], false);
            }
            else
            {
                button3_state = true;
                GpioPortSafeWrite(_button[2], true);
            }
        }

        public static void signalGpio(int cpu_pin, Boolean value)
        {
            if (cpu_pin < 3)
                GpioPortSafeWrite(_led[cpu_pin], value);
            else if (cpu_pin < 6)
                GpioPortSafeWrite(_button[cpu_pin - 3], value);
            else
                GpioPortSafeWrite(_gpio[cpu_pin -6], value);
        }

        private void inputEmulation_Click(object sender, EventArgs e)
        {
            
            ei.Run();
        }

        private void button_carGUI_Click(object sender, EventArgs e)
        {
            //vCom = new PhysicalModelVirtCom();
            //vCom.StartVirtComServer();
            
            /*f2 = new Form2();
            f2.Height = 800;
            f2.Width = 700;
            f2.Show();        */
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            Log.Debug(Module.DASHBOARD, "The file is:" + openFileDialog1.FileName);
            inputEmulationFileTextBox.Text = openFileDialog1.FileName;
            inputEmulationFileTextBox.Enabled = false;
            if(openFileDialog1.CheckFileExists){
                ei =new ExternalInput(openFileDialog1.FileName);
                inputEmulation.Enabled = true;
            }
            

        }

        private void InputAutmationFileBrowseButton_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
        }

        ~Form1()
        {
            comPortWatcherThread.Abort();
        }

       

      

       
    }
}
