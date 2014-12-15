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
using System.IO.Ports;
using Samraksh.eMote.DotNow;
using Samraksh.eMote.NonVolatileMemory;
using AnalogInput = Samraksh.eMote.DotNow.AnalogInput;

namespace Samraksh.AppNote.DataCollector.Radar {
    /// <summary>
    /// Main program
    /// </summary>
    public partial class Program {
        public static LCD codeVersion = LCD.CHAR_6;
        public static EmoteLCD lcd = new EmoteLCD();        
        // Set up parameters for collection and for DataStore
        //  DataStore can only track 256 data references. 
        //      If each ADC buffer's work were a separate data reference, the limit would be quickly reached.
        //  LargeDataStore lets multiple buffers be stored in one data reference
        //  For efficiency, ADCBufferSize should divide LargeDataStoreReferenceSize
        private const int DataStoreBlockSize = 250 * 1024; // 128k bytes/block
        // ReSharper disable once UnusedMember.Local
        private const int DataStoreNumBlocks = 125;
        private const int AudioBufferSize = 2000;
        private const int ADCBufferSize = 250; // Number of ushorts per ADC buffer
        private const int SampleIntervalMicroSec = 4000;

        public static SerialPort serialPort = new SerialPort("COM2");
        
        //private const int ADCBufferSize = 256; // Number of ushorts per ADC buffer
        //private const int SampleIntervalMicroSec = 3906;

        //private const int LargeDataStoreReferenceSize = (DataStoreBlockSize / 4) / sizeof(ushort);

        // End of file value
        //  This must be the same value across all Data Collector programs 
        private const byte Eof = 0xF0;  // A ushort of F0F0 (2 bytes of Eof) is larger than a 12-bit sample (max 0FFF)

        // Misc definitions
        //private static readonly DataStore DataStore = DataStore.Instance(StorageType.NOR, true);
        //private static readonly LargeDataReference LargeDataRef = new LargeDataReference(DataStore, LargeDataStoreReferenceSize);
        //private static DataStoreReturnStatus _retVal;
        private static bool _debuggerIsAttached;

        // The sampling interval in micro seconds. Sample rate per sec = 1,000,000 / SampleIntervalMicroSec

        public static OutputPort buzzerGPIO = new OutputPort((Cpu.Pin)24, true);
        //public static OutputPort radarEnableGPIO = new OutputPort((Cpu.Pin)25, true);
        private static InputPort testPort = new InputPort((Cpu.Pin)25, false, Port.ResistorMode.Disabled);
        //public static OutputPort timeMeasure = new OutputPort((Cpu.Pin)29, true);

        public static OutputPort radarInterrupt = new OutputPort((Cpu.Pin)30, true);
        //public static OutputPort audioInterrupt = new OutputPort((Cpu.Pin)31, true);

        // Define the GPIO pins used
        //private static class GpioPins {
        //    public static readonly InterruptPort EndCollect = new InterruptPort(Pins.GPIO_J11_PIN5, false, Port.ResistorMode.PullUp, Port.InterruptMode.InterruptEdgeBoth);
        //}

        // Flag that's set when the user enables the EndCollect GPIO pin
        //  This stops the data collection after the current buffer is processed
        private static bool _collectIsDone;

        // Flag that the sample data buffer queue is full
        //  This means that the time to process data was larger than the time required to collect the data
        //  This means the data collection failed
        private static bool _bufferQueueIsFull;

        // The maximum number of buffers used. Reported at the end.
        private static int _maxBuffersEnqueued;
        private static string commandStr = "";

        static void SerialPortHandler(object sender, SerialDataReceivedEventArgs e)
        {            
            byte[] m_recvBuffer = new byte[100];
            string proccesCmd = "";
            int i;
            byte[] msgBuff = new byte[6];
            
            try {
                int numBytes = serialPort.BytesToRead;
                serialPort.Read(m_recvBuffer, 0, numBytes);
                char[] rxStr = new char[numBytes];
                for (i = 0; i < numBytes; i++)
                {
                    commandStr = string.Concat(commandStr, ((char)m_recvBuffer[i]).ToString());        
                }

                //commandStr = "aba?c\nNumber Two";
                int tempPlace = commandStr.IndexOf('\r');
                if ((tempPlace != -1))
                {
                    proccesCmd = commandStr.Substring(0, tempPlace + 1);
                    commandStr = commandStr.Substring(tempPlace + 1, commandStr.Length - tempPlace - 1);

                    int checksum = 0;
                    for (i = 0; i < proccesCmd.Length - 2; i++)
                    {
                        checksum = checksum + (int)proccesCmd[i];
                    }
                    if ((byte)checksum != proccesCmd[proccesCmd.Length - 2])
                    {
                        //Debug.Print("Checksum failed");
                    }
                    else
                    {
                        //Debug.Print("Checksum the same");
                        if (proccesCmd.Length == 6)
                        {
                            if (proccesCmd[0] == 'T' && proccesCmd[1] == 'S')
                            {
                                int threshold = (int)proccesCmd[2];
                                int IQRej = (int)proccesCmd[3];
                                if ((threshold > 10) && (threshold < 256) && (IQRej >= 0) && (IQRej < 256))
                                {
                                    //Debug.Print("Threshold set: " + threshold.ToString() + " " + IQRej.ToString());
                                    radarDetect.SetDetectionParameters(threshold, IQRej);
                                    msgBuff[0] = (byte)'T';
                                    msgBuff[1] = (byte)'S';
                                    msgBuff[2] = (byte)threshold;
                                    msgBuff[3] = (byte)IQRej;
                                    msgBuff[4] = (byte)checksum;
                                    msgBuff[5] = (byte)'\r';
                                    serialPort.Write(msgBuff, 0, 6);
                                }
                                else
                                {
                                    msgBuff[0] = (byte)'X';
                                    msgBuff[1] = (byte)'X';
                                    msgBuff[2] = (byte)threshold;
                                    msgBuff[3] = (byte)IQRej;
                                    msgBuff[4] = (byte)checksum;
                                    msgBuff[5] = (byte)'\r';
                                    serialPort.Write(msgBuff, 0, 6);
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                //finalMsg = ex.Message;
                //EnhancedLcd.Display(LCDMessages.Error);
            }
            
        }


        /// <summary>
        /// The main program
        /// </summary>
        /// <exception cref="InvalidOperationException"></exception>
        /// <exception cref="IOException"></exception>
        public static void Main() {
            var finalMsg = string.Empty;

            lcd.Initialize();
            radarDetect.SetDetectionParameters(60, 30);
            acousticDetect.SetDetectionParameters(1, 1);

            lcd.Write(LCD.CHAR_NULL, LCD.CHAR_NULL, LCD.CHAR_NULL, codeVersion);

            serialPort.BaudRate = 57600;
            serialPort.Parity = Parity.None;
            serialPort.StopBits = StopBits.One;
            serialPort.DataBits = 8;
            serialPort.Handshake = Handshake.None;
            serialPort.DataReceived += new SerialDataReceivedEventHandler(SerialPortHandler);
            serialPort.Open();

            Thread.Sleep(60000);
            try {
                Debug.EnableGCMessages(false);

                // Print the version and build info
                //VersionInfo.Init(Assembly.GetExecutingAssembly());

                // Note whether the debugger is attached
                //  Used to suppress print messages during buffer processing if not debugging
                //  Otherwise, print statements can create and discard strings and trigger a garbage collection
                _debuggerIsAttached = System.Diagnostics.Debugger.IsAttached;

                // Start sampling
                //EnhancedLcd.Display(LCDMessages.Collecting);

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

                /* This overlaps the init in ConfigureScanModeThreeChannels
                AnalogInput.InitChannel(ADCChannel.ADC_Channel1);
                AnalogInput.InitChannel(ADCChannel.ADC_Channel2);
                */

                // Start the continuous mode dual channel sampling
                //  SampleIntervalMicroSec gives the interval between samples, in micro seconds
                //  ADCCallback is called when ADCBufferSize number of samples has been collected
                //  On callback, ADCBufferI and ADCBufferQ contain the data
                
                //if (!AnalogInput.ConfigureContinuousModeDualChannel(ADCBufferI, ADCBufferQ, ADCBufferSize, SampleIntervalMicroSec, ADCCallback))
                if (!AnalogInput.ConfigureScanModeThreeChannels(ADCBufferI, ADCBufferQ, ADCBufferAudio, AudioBufferSize, SampleIntervalMicroSec, ADCCallback))
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

                finalMsg = "Finished";
                //EnhancedLcd.Display(LCDMessages.Done);
            }
            catch (Exception ex) {
                finalMsg = ex.Message;
                //EnhancedLcd.Display(LCDMessages.Error);
            }
            finally {

            }
        }
    }
}
