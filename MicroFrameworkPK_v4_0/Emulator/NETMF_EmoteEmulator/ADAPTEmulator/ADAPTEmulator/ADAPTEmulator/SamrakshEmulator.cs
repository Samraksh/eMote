using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Threading;
using System.Windows;
using System.Diagnostics;


using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Emulator.Gpio;
using Microsoft.SPOT.Emulator.Com;


//using Microsoft.SPOT.Emulator.Serial;
using ADAPTEmulator;

namespace ADAPTEmulator
{
    enum Module
    {
        CORE, PHYCOMM, VIZ, DASHBOARD, MFAPP
    }
    class Logger : TraceListener
    {
        public static System.IO.StreamWriter logFile;
        static Form1 _f1;
        static bool Initialized;
        //TextWriter _writer;
        public Logger() {
            if (!Initialized) Initialize();
        }
        public Logger (Form1 f1)
        {
            _f1 = f1;
            if (!Initialized) Initialize();
        }
        public void Initialize()
        {
            DateTime now = DateTime.Now;
            String nowString = now.ToString("yyyy_MM_dd_H_mm");
            logFile = new System.IO.StreamWriter("EmulatorLog_" + nowString + ".txt");
            Initialized = true;
            //_writer = new TextWriter 
        }

        public void Debug(Module m,  String s)
        {
            DateTime now = DateTime.Now;
            String nowString = now.ToString("H:mm:ss.fff");
            logFile.WriteLine(nowString + " :: " + m + " :: " + s );
            logFile.Flush();
            if(_f1!=null)
                _f1.DisplayStatus(nowString + " :: " + m + " :: " + s + "\r\n");
        }
        override public void Close(){
            //logFile.Flush();
            logFile.Close();
        }
        override public void Write(String s)
        {
            Debug(Module.MFAPP, s);
        }
        override public void WriteLine(String s)
        {
            Debug(Module.MFAPP, s);
        }

    }

    class SamrakshEmulator : Emulator
    {
        Logger Log;  
        public override void SetupComponent()
        {
            Log = new Logger();
            //Redirect debug messages to the 
            Debug.Listeners.Add(Log);
            
            Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort esp = new Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort();
            esp.ComponentId = "COM1";
            esp.ComPortHandle = new ComPortHandle(TransportType.Usart, 1);
            
                        
            this.GpioPorts.MaxPorts = 128; //Emote has 8 GPIO pins exposed
            GpioPort[] gpio = new GpioPort[14]; //create 6+8 ports
       
            for (int i = 0; i < 14; i++)
                gpio[i] = new GpioPort();

            //Virtual Led pins
            gpio[0].ComponentId = "led_0";
            gpio[1].ComponentId = "led_1";
            gpio[2].ComponentId = "led_2";

            //Virtual Led pins
            gpio[3].ComponentId = "button_0";
            gpio[4].ComponentId = "button_1";
            gpio[5].ComponentId = "button_2";


            for (int i = 0; i < 8; i++)
            {
                //gpio[i] = new GpioPort();
                string compId = "gpio_" + i.ToString();
                gpio[i+6].ComponentId = compId; //name them
            }
               
            //Configure pins as input or output and
            //Map exposed pins to the chip pins from hardware
            for (int i = 0; i < 14; i++)
            {
                gpio[i].Pin = (Microsoft.SPOT.Hardware.Cpu.Pin)i;
                gpio[i].ModesAllowed = GpioPortMode.InputOutputPort;
                gpio[i].ModesExpected = GpioPortMode.InputOutputPort;
            }

            //Register them with the emulator
            for (int i = 0; i < 14; i++)
            {
                this.RegisterComponent(gpio[i]);  
            }
          
            //register Emulator Serial Port
            this.RegisterComponent(esp);
            base.SetupComponent();
        }

        public override void InitializeComponent()
        {
            base.InitializeComponent();

            // Start the UI in its own thread.
            Thread uiThread = new Thread(StartForm);
            uiThread.SetApartmentState(ApartmentState.STA);
            uiThread.IsBackground = true;
            uiThread.Start();
        }

        public override void UninitializeComponent()
        {
            base.UninitializeComponent();

            // The emulator is stopped. Close the WinForm UI.
            System.Windows.Forms.Application.Exit();
        }

         private void StartForm()
        {
            // Some initial setup for the WinForm UI
            System.Windows.Forms.Application.EnableVisualStyles();
            System.Windows.Forms.Application.SetCompatibleTextRenderingDefault(false);

            Log.Debug(Module.CORE, "Starting Emulation...");
            // Start the WinForm UI. Run() returns when the form is closed.
            System.Windows.Forms.Application.Run(new Form1(this));

            Log.Debug(Module.CORE, "Emulation Ended");
            // When the user closes the WinForm UI, stop the emulator.
            Stop();
            Environment.Exit(-1);
        }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        
        [STAThread]   
        static void Main()
        {
            
            SamrakshEmulator p1=new SamrakshEmulator();
            p1.Start();
            //Thread.Sleep(Timeout.Infinite);
        }
    }
}
