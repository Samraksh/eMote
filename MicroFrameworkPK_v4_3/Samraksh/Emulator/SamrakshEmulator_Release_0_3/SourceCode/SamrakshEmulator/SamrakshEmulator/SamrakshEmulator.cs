using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Threading;
using System.Windows;
using System.Diagnostics;

//using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Emulator;
using Microsoft.SPOT.Emulator.Gpio;
using Microsoft.SPOT.Emulator.Com;
using Microsoft.SPOT.Emulator.Spi;

//using Microsoft.SPOT.Emulator.Serial;
using SamrakshEmulator;
using Samraksh.SPOT.Emulator;

namespace SamrakshEmulator
{
    enum Module
    {
        CORE, PHYCOMM, VIZ, DASHBOARD, MFAPP
    }
    class SamrakshEmulator : Emulator
    {
        Logger Log;  
        public override void SetupComponent()
        {
            /*******************Initialize Logger********************/
            Log = new Logger();
            Debug.Listeners.Add(Log); //Redirect the debug messages to the log
            
            /****************Create and initialize hardware**********/

            /*************ADC*****************/
            Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort ADCPort = new Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort();
            //ComPortToMemoryStream ADCPort = new ComPortToMemoryStream();
            ADCPort.ComponentId = "COM1";
            ADCPort.ComPortHandle = new ComPortHandle(TransportType.Usart, 1);
            this.RegisterComponent(ADCPort);
            /*************SPI*****************/
            //int chipSelectPin = 15;
            /*SPI.Configuration config = new SPI.Configuration(
                chipSelectPin, //chip select port
                false, //IC is accessed when chip select is low
                1, //setup time 1 ms
                1, //hold chip select 1 ms after transfer
                true, //clock line is high if device is not selected
                false, //data is sampled at falling edge of clock
                15000, //clockrate is 15 MHz
                SPI.SPI_module //use first SPI bus
            );*/
            //SPI spi = new SPI(config);

            /******************************Serial*********************/
            
            Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort esp = new Microsoft.SPOT.Emulator.Sample.EmulatorSerialPort();
            //Microsoft.SPOT.Emulator.Serial.SerialPortCollection esp = new SerialPortCollection();
            //ComPortToMemoryStream esp = new ComPortToMemoryStream();
            esp.ComponentId = "COM2";
            esp.ComPortHandle = new ComPortHandle(TransportType.Usart, 2);
            //register Emulator Serial Port
            this.RegisterComponent(esp);
            
            /*************************GPIO*****************************/                        
            this.GpioPorts.MaxPorts = 128; //Emote has 8 GPIO pins exposed
            GpioPort[] gpio = new GpioPort[14]; //create 6+8 ports
       
            for (int i = 0; i < 14; i++)
                gpio[i] = new GpioPort();

            /**********************LED**************************/
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
            Log.Debug(Module.CORE, "/******Samraksh NETMF4.3 Emulator, verion 0.3******/");

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
