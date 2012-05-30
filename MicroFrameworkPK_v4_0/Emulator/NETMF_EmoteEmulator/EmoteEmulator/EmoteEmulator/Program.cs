using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Threading;
using System.Windows;

using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Emulator.Gpio;
using Microsoft.SPOT.Emulator.Com;


//using Microsoft.SPOT.Emulator.Serial;
using EmoteEmulator;

namespace EmoteEmulator
{
    class Program : Emulator
    {
        
        Thread carThread;
        //Window guiWindow;
        CarGUI cg;
        //public delegate void com2Delegate();
        

        public override void SetupComponent()
        {


            Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort esp = new Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort();
            esp.ComponentId = "COM1";
            esp.ComPortHandle = new ComPortHandle(TransportType.Usart, 1);
            //ComPortToMemoryStream com2 = new ComPortToMemoryStream();
            //com2.ComponentId = "COM2";
            //com2.ComPortHandle = new ComPortHandle(TransportType.Usart,2);
            //com2.MaxBaudrateHz = 9600;
            //com2.MinBaudrateHz = 9600;
            

            //com2.BeginInvoke(new com2Delegate(com2BeginInvoke));
            
            this.GpioPorts.MaxPorts = 128; //Emote has 8 GPIO pins exposed
            GpioPort[] gpio = new GpioPort[11]; //create 8 ports
            gpio[0] = new GpioPort();
            gpio[1] = new GpioPort();
            gpio[2] = new GpioPort();
            gpio[3] = new GpioPort();
            gpio[4] = new GpioPort();
            gpio[5] = new GpioPort();
            gpio[6] = new GpioPort();
            gpio[7] = new GpioPort();
            gpio[8] = new GpioPort();
            gpio[9] = new GpioPort();
            gpio[10] = new GpioPort(); 
            

            for (int i = 0; i < 8; i++)
            {
                //gpio[i] = new GpioPort();
                string compId = "gpio_" + i.ToString();
                gpio[i].ComponentId = compId; //name them
            }
            //Virtual Led pins
            gpio[8].ComponentId = "led_1";
            gpio[9].ComponentId = "led_2";
            gpio[10].ComponentId = "led_3";
   
            //Map exposed pins to the chip pins from hardware
          /*  gpio[0].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)9;	    //PA9
            gpio[1].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)10;	//PA10
            gpio[2].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)22;	//PB6
            gpio[3].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)0;	    //PA0
            gpio[4].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)8;	    //PA8
            gpio[5].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)21;	//PB7
            gpio[6].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)4;	    //PA4
            gpio[7].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)38;	//PC6

            gpio[8].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)1;	//Virtual Pins for led
            gpio[9].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)2;
            gpio[10].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)3;

            //TODO: Configure pins as input or output
            for (int i = 0; i < 11; i++)
            {
                gpio[i].ModesAllowed = GpioPortMode.InputOutputPort;
                gpio[i].ModesExpected = GpioPortMode.InputOutputPort;
            }

            //Register them with the emulator
            for (int i = 0; i < 11; i++)
            {
                this.RegisterComponent(gpio[i]);  
            }
            */
            this.RegisterComponent(esp);
            //this.RegisterComponent(com2);
            base.SetupComponent();
        }

        public override void InitializeComponent()
        {
            base.InitializeComponent();

            // Start the UI in its own thread.

            /*if (FindComponentById("COM2") == null)
            {
                throw new Exception(("Com port not found"));
                
            }*/

            Thread uiThread = new Thread(StartForm);
            uiThread.SetApartmentState(ApartmentState.STA);
            uiThread.IsBackground = true;
            uiThread.Start();

            /*carThread = new Thread(StartCarEmulation);
            carThread.SetApartmentState(ApartmentState.STA);
            //carThread.IsBackground = true;
            carThread.Start();
            */

            //StartCarEmulation();
            
      
        }

        public override void UninitializeComponent()
        {
            base.UninitializeComponent();

            // The emulator is stopped. Close the WinForm UI.
            System.Windows.Forms.Application.Exit();
        }
       
        void StartCarEmulation()
        {
             
            
            Console.WriteLine("Creating Car GUI");
           
            Window guiWindow = new Window();
            guiWindow.Height = 650;
            guiWindow.Width = 650;
            guiWindow.Title = "Open-loop Car Emulation";
            //guiWindow.Visibility = Visibility.Visible;
            Console.WriteLine("Done 1");
            
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
            Thread.Sleep(Timeout.Infinite);
            
        }
        

        private void StartForm()
        {
            //StartCarEmulation();

            // Some initial setup for the WinForm UI
            System.Windows.Forms.Application.EnableVisualStyles();
            System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(false);
            
            //StartCarEmulation();

            // Start the WinForm UI. Run() returns when the form is closed.
            System.Windows.Forms.Application.Run(new Form1(this));
            
            // When the user closes the WinForm UI, stop the emulator.
            Stop();
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        
        [STAThread]   
        static void Main()
        {
            
            Program p1=new Program();
            p1.Start();
            
            //Thread.Sleep(Timeout.Infinite);
        }
    }
}
