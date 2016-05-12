/*******************************************
 * Data Collector Radar SD
 *      Collects BumbleBee radar data and stores it on an SD card for later exfiltration
 * Version
 *  1.1 Initial version (1.0 was a very early version and was not released)
 *  1.2 
 *      -   Fixed minor issues
 *      -   Improved reporting on bytes received at end
*******************************************/

using System;
using System.IO;
using System.Reflection;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.eMote.DotNow;
using System.IO.Ports;
using Samraksh.eMote.NonVolatileMemory;
using AnalogInput = Samraksh.eMote.DotNow.AnalogInput;

namespace Samraksh.AppNote.Scarecrow.Radar
{
    /// <summary>
    /// Main program
    /// </summary>
    public partial class Program {
        
        public static SerialPort serialPort2;

        public static LCD codeVersion = LCD.CHAR_7;
        public static EmoteLCD lcd = new EmoteLCD();
        // Set up parameters for collection and for DataStore
        //  DataStore can only track 256 data references. 
        //      If each ADC buffer's work were a separate data reference, the limit would be quickly reached.
        //  LargeDataStore lets multiple buffers be stored in one data reference
        //  For efficiency, ADCBufferSize should divide LargeDataStoreReferenceSize
        private const int DataStoreBlockSize = 125 * 1024; // 128k bytes/block
        // ReSharper disable once UnusedMember.Local
        private const int DataStoreNumBlocks = 125;

        // sampling 125 radar I and 125 radar Q samples every 0.5 seconds
        private const int ADCBufferSize = 125; // Number of ushorts per ADC buffer
        private const int SampleIntervalMicroSec = 4001;    // 4000 gives 3.999ms using logic analyzer (1 second windows of 250 raw data bytes)        

        // End of file value
        //  This must be the same value across all Data Collector programs 
        private const byte Eof = 0xF0;  // A ushort of F0F0 (2 bytes of Eof) is larger than a 12-bit sample (max 0FFF)

        // Misc definitions
        private static bool _debuggerIsAttached;


        public static OutputPort buzzerGPIO = new OutputPort((Cpu.Pin)24, false);

        // Flag that's set when the user enables the EndCollect GPIO pin
        //  This stops the data collection after the current buffer is processed
        private static bool _collectIsDone;

        // Flag that the sample data buffer queue is full
        //  This means that the time to process data was larger than the time required to collect the data
        //  This means the data collection failed
        private static bool _bufferQueueIsFull;

        // The maximum number of buffers used. Reported at the end.
        private static int _maxBuffersEnqueued;


        /// <summary>
        /// The main program
        /// </summary>
        /// <exception cref="InvalidOperationException"></exception>
        /// <exception cref="IOException"></exception>
        public static void Main() {
            var finalMsg = string.Empty;

            lcd.Initialize();
            lcd.Write(LCD.CHAR_NULL, LCD.CHAR_NULL, LCD.CHAR_NULL, codeVersion);
            Thread.Sleep(1000);
            try {
                Debug.EnableGCMessages(false);


                // Note whether the debugger is attached
                //  Used to suppress print messages during buffer processing if not debugging
                //  Otherwise, print statements can create and discard strings and trigger a garbage collection
                _debuggerIsAttached = System.Diagnostics.Debugger.IsAttached;


                // Start the ADC buffer processing thread
                //  This is done first since the ADC sampling below starts right away, so better to start WriteSampleBufferQueue thread first
                //      Note that since we have ADC buffer queues, this isn't strictly necessary
                var bufferThread = new Thread(SampleBufferQueue);
                bufferThread.Start();

                // Set up ADC
                // Pre-allocate an array of sample buffers
                SetupBuffers();
                // Initialize the ADC and the channels
                AnalogInput.InitializeADC();

                // initializing the COM2 port. radar data will be streamed out COM2 for debugging purposes
                serialPort2 = new SerialPort("COM2");
                serialPort2.BaudRate = 115200;
                serialPort2.Parity = Parity.None;
                serialPort2.StopBits = StopBits.One;
                serialPort2.DataBits = 8;
                serialPort2.Handshake = Handshake.None;
                serialPort2.Open();

                // Start the continuous mode dual channel sampling
                //  SampleIntervalMicroSec gives the interval between samples, in micro seconds
                //  ADCCallback is called when ADCBufferSize number of samples has been collected
                //  On callback, ADCBufferI and ADCBufferQ contain the data

                //Debug.Print("Attempting to initialize ADC in C#");
                if (!AnalogInput.ConfigureContinuousModeDualChannel(ADCBufferI, ADCBufferQ, ADCBufferSize, SampleIntervalMicroSec, ADCCallback))
                {
                    //EnhancedLcd.Display(LCDMessages.Error);
                    throw new InvalidOperationException("Could not initialize ADC");
                }

                // Wait till we're finished sampling
                //  When the user enagles the EndCollect pin, bufferThread will terminate
                bufferThread.Join();

                // If the buffer filled up, give error
                if (_bufferQueueIsFull) {
                    //EnhancedLcd.Display(LCDMessages.BufferQueueFull);
                    throw new IOException("Buffer queue is full");
                }

                //finalMsg = "Finished";
                //EnhancedLcd.Display(LCDMessages.Done);
            }
            catch (Exception ex) {
                //finalMsg = ex.Message;
                //EnhancedLcd.Display(LCDMessages.Error);
            }
            finally {

            }
        }
    }
}
