using System.Collections;
using System.Threading;
using Microsoft.SPOT;
using Samraksh.eMote.DotNow;
using Samraksh.eMote.Net;

namespace Samraksh.AppNote.Scarecrow.Radar
{
    public class UnwrapMsg
    {
        public byte[] msg;

        public UnwrapMsg()
        {
            msg = new byte[7];
        }
        public static int Size()
        {
            return 7;
        }
    }

    public struct Comp
    {
        public int I, Q;

        public Comp(int p1, int p2)
        {
            I = p1;
            Q = p2;
        }
    }

    public partial class Program
    {

        // ---------------------------------------------
        // Handle ADC callback and write sample buffers
        // ---------------------------------------------

        /// <summary>
        /// ADC buffer
        /// </summary>
        /// <remarks>Lets a pair of buffers be handled together as a single object</remarks>
        private class IQ
        {
            // ReSharper disable once InconsistentNaming
            public readonly ushort[] IBuff;
            public readonly ushort[] QBuff;
            public IQ(ushort[] iBuff, ushort[] qBuff)
            {
                IBuff = iBuff;
                QBuff = qBuff;
            }
        }

        // A semaphore used by the ADC callback to signal WriteSampleBufferQueue that data is ready for processing
        private static readonly AutoResetEvent SampleBufferSemaphore = new AutoResetEvent(false);

        // The ADC buffers that are populated by the ADC driver
        private static readonly ushort[] ADCBufferI = new ushort[ADCBufferSize];
        private static readonly ushort[] ADCBufferQ = new ushort[ADCBufferSize];

        //public static ushort[] unwrapBuffer = new ushort[ADCBufferSize*50];  

        // The buffer queue and it's maximum length
        private static readonly Queue BufferQueue = new Queue();
        private const int MaxBufferQueueLen = 2;

        // A circular array of pre-allocated buffers that receive the contents of the ADC buffers
        private static readonly ArrayList ADCCopyBuffers = new ArrayList();
        private const int ADCCopyBuffersCnt = MaxBufferQueueLen;
        private static int _adcCopyBuffersPtr;

        public static ushort thresholdRotations = 7;
        public static ushort IQRejection = 30;
        public static ushort thresholdRadians = (ushort) (thresholdRotations * 2 * 3.14159);
        public static bool threshholdMet = false;
        public static int j = 0;

        private static bool buzzerState = false;

        public static Samraksh.eMote.Algorithm.RadarDetection radarDetect = new Samraksh.eMote.Algorithm.RadarDetection();

        private static bool historyUpdateCtrl = false;

        public static LCD detectionDisplay = LCD.CHAR_d;

        private NetOpStatus status;
        private static ushort myAddress;
        /*private static UnwrapMsg sendMsg = new UnwrapMsg();
        private static Samraksh.eMote.Net.MAC.CSMA myCSMA;
        private static ReceiveCallBack myReceiveCB;
        private static NeighborhoodChangeCallBack myNeighborCB;
        private static Samraksh.eMote.Net.MAC.MACConfiguration macConfig = new Samraksh.eMote.Net.MAC.MACConfiguration();

        private static void SendUnwrap(ushort unwrap, byte[] msg, ushort iqUsed)
        {           
            myCSMA.Send((ushort)Samraksh.eMote.Net.MAC.AddressType.BROADCAST, (byte)Samraksh.eMote.Net.MAC.PayloadType.MFM_DATA, msg, 0, (ushort)msg.Length);
        }

        static void NeighborChange(ushort noOfNeigbors)
        {

        }

        static void Receive(ushort noOfPackets)
        {
        }*/

        /// <summary>
        /// Populate the circular queue of ADC buffers
        /// </summary>
        /// <remarks>
        /// Since the buffers are pre-allocated and the references are stored in a static object,
        ///     they won't be de-referenced over the lifetime of the program.
        /// This ensures that allocating and de-referencing buffers won't cause the garbage collector to run
        /// </remarks>
        private static void SetupBuffers()
        {
            for (var i = 0; i < ADCCopyBuffersCnt; i++)
            {
                var iBuff = new ushort[ADCBufferSize];
                var qBuff = new ushort[ADCBufferSize];
                var iqBuff = new IQ(iBuff, qBuff);
                ADCCopyBuffers.Add(iqBuff);
            }
            _adcCopyBuffersPtr = 0;

            // threshold is 7 rotations a second, IQ rejection is 30, debug mode is set to no debug information, software version number is 4
            radarDetect.SetDetectionParameters(Samraksh.eMote.RADAR_NOISE_CONTROL.SCALING_NOISE_REJECTION_TARGET, 1.5, 1, 2, 3, 0, 1);
            //radarDetect.SetDetectionParameters(Samraksh.eMote.RADAR_NOISE_CONTROL.FIXED_NOISE_REJECTION, 7, 100, 2, 3, 6, 1);
            //radarDetect.SetDetectionParameters(Samraksh.eMote.RADAR_NOISE_CONTROL.SCALING_NOISE_REJECTION_MULTIPLY, 7, 1, 3, 4, 0, 1);

            // setup radio
            /*macConfig.NeighborLivenessDelay = 180;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            Debug.Print("Initializing radio");
            macConfig.MACRadioConfig.TxPower = Samraksh.eMote.Net.Radio.TxPowerValue.Power_3dBm;
            macConfig.MACRadioConfig.Channel = Samraksh.eMote.Net.Radio.Channel.Channel_26;
            macConfig.MACRadioConfig.RadioType = Samraksh.eMote.Net.Radio.RadioType.RF231RADIO;
            macConfig.MACRadioConfig.OnReceiveCallback = Receive;
            macConfig.MACRadioConfig.OnNeighborChangeCallback = NeighborChange;

            Debug.Print("Configuring:  CSMA...");
            
                myCSMA = new Samraksh.eMote.Net.MAC.CSMA(macConfig);
                //myReceiveCB = Receive;
                //myNeighborCB = NeighborChange;
                //CSMA.Configure(macConfig, myReceiveCB, myNeighborCB);
                //myCSMA = CSMA.Instance;
            
            

            Debug.Print("CSMA Init done.");
            myAddress = myCSMA.GetRadioAddress();
            Debug.Print("My default address is :  " + myAddress.ToString());*/
        }

        /// <summary>
        /// ADC callback
        /// </summary>
        /// <remarks>Called when the ADC driver has collected a buffer's worth of data</remarks>
        /// <param name="threshhold"></param>
        private static void ADCCallback(long threshhold)
        {
            var bqCnt = BufferQueue.Count;
            _maxBuffersEnqueued = System.Math.Max(_maxBuffersEnqueued, bqCnt + 1);  // Add 1 because we're about to enqueue

            // If queue is full, we can't add another entry
            //  Exit with the _bufferQueueIsFull and _collectIsDone flags set
            if (!_bufferQueueIsFull && bqCnt > MaxBufferQueueLen - 1)
            {
                _bufferQueueIsFull = true;
                _collectIsDone = true;
                AnalogInput.StopSampling();
                return;
            }

            // There's space in the queue: copy the buffers and enqueue them
            //      We need to copy to avoid a race condition. 
            //      Whenever ADC is ready to call back, the new set of samples is stored in the specified buffer.
            //      Hence if we're processing one buffer when the next callback occurs, the values in the current buffer can change.

            // Get an I-Q buffer pair that will receive the ADC buffer data
            //  Note that we're using pre-allocated buffers rather than creating new
            //  This protects against the garbage collector
            var iq = (IQ)ADCCopyBuffers[_adcCopyBuffersPtr];
            // Copy to the pair
            ADCBufferI.CopyTo(iq.IBuff, 0);
            ADCBufferQ.CopyTo(iq.QBuff, 0);
            // Update the circular buffer pointer
            _adcCopyBuffersPtr = (_adcCopyBuffersPtr + 1) % ADCCopyBuffersCnt;

            // Enqueue the pair for processing
            BufferQueue.Enqueue(iq);
            // Signal the processing thread
            SampleBufferSemaphore.Set();
        }

        /// <summary>
        /// Process the sample buffer in a separate thread
        /// </summary>
        /// <remarks>
        /// We do this so that the ADC callback will return quickly.
        /// The main program blocks until this thread ends
        /// </remarks>
        private static void SampleBufferQueue()
        {
            // Run until user indicates end or there is a queue-full error
            while (true)
            {
                // Wait for signal that a buffer is ready for processing
                SampleBufferSemaphore.WaitOne();
                // Process as many buffers as are available
                while (BufferQueue.Count > 0)
                {
                    // Get a buffer to process
                    var iq = (IQ)BufferQueue.Dequeue();
                    ProcessBuffers(iq);
                }
                // Check if the end-sampling flag is set. If so, we're done
                if (!_collectIsDone)
                {
                    continue;
                }
                //Debug.Print("Finished sampling");
                AnalogInput.StopSampling();
                // Terminate the thread and join with the main program
                return;
            }
        }


        /// <summary>
        /// Process AD data
        /// </summary>
        /// <param name="iq">The I-Q-A</param>
        /// <returns>True iff we're done sampling</returns>
        private static void ProcessBuffers(IQ iq)
        {
            /*if (iq.IBuff[0] > 1500 && iq.IBuff[0] < 2500 && iq.QBuff[0] > 1500 && iq.QBuff[0] < 2500)
            {
                lcd.Write(LCD.CHAR_A, detectionDisplay, LCD.CHAR_NULL, codeVersion);
            }
            else
            {
                lcd.Write(LCD.CHAR_UNDERSCORE, detectionDisplay, LCD.CHAR_NULL, codeVersion);
            }*/

            // Pull the members out. Referencing this way seems to be more efficient.
            var iBuff = iq.IBuff;
            var qBuff = iq.QBuff;
            bool detection = radarDetect.DetectionCalculation(iBuff, qBuff, ADCBufferSize);
            int unwrap = radarDetect.GetLastUnwrap(eMote.RADAR_NOISE_REQUEST.IQ_REJECTION_CURRENTLY_USED);
            bool winOverThresh = radarDetect.GetWindowOverThreshold();

            int EntireUnwrap = radarDetect.GetNetDisplacement(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_FULL);
            int FirstHalfUnwrap = radarDetect.GetNetDisplacement(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_FIRST_HALF);
            int SecondHalfUnwrap = radarDetect.GetNetDisplacement(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_SECOND_HALF);

            int AbsEntireUnwrap = radarDetect.GetAbsoluteDisplacement(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_FULL);
            int AbsFirstHalfUnwrap = radarDetect.GetAbsoluteDisplacement(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_FIRST_HALF);
            int AbsSecondHalfUnwrap = radarDetect.GetAbsoluteDisplacement(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_SECOND_HALF);

            int RangeEntireUnwrap = radarDetect.GetDisplacementRange(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_FULL);
            int RangeFirstHalfUnwrap = radarDetect.GetDisplacementRange(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_FIRST_HALF);
            int RangeSecondHalfUnwrap = radarDetect.GetDisplacementRange(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_SECOND_HALF);

            int backgroundNoise = radarDetect.GetBackgroundNoiseLevel(eMote.RADAR_NOISE_REQUEST.IQ_REJECTION_CURRENTLY_USED);
            int backgroundNoiseZero = radarDetect.GetBackgroundNoiseLevel(eMote.RADAR_NOISE_REQUEST.IQ_REJECTION_ZERO);
            int backgroundNoiseMax = radarDetect.GetBackgroundNoiseLevel(eMote.RADAR_NOISE_REQUEST.IQ_REJECTION_MAX);
            int iqUsed = radarDetect.GetIQRejectionLevel();
            //Debug.Print(unwrap.ToString() + " " + backgroundNoiseZero.ToString() + " " + backgroundNoise.ToString() + " " + backgroundNoiseMax.ToString() + " " + iqUsed.ToString());
            /*Debug.Print(unwrap.ToString() + " " + EntireUnwrap.ToString() + " " + FirstHalfUnwrap.ToString() + " " + SecondHalfUnwrap.ToString() + " " + AbsEntireUnwrap.ToString() + " " + AbsFirstHalfUnwrap.ToString() + " " + AbsSecondHalfUnwrap.ToString() + " " + RangeEntireUnwrap.ToString() + " " + RangeFirstHalfUnwrap.ToString() + " " + RangeSecondHalfUnwrap.ToString());
            if (detection == true)
            {
                Debug.Print("****** detection ******\r\n");
            }*/

            /*byte[] sendMsg = new byte[7];
            sendMsg[0] = (byte)radarDetect.GetLastUnwrap(eMote.RADAR_NOISE_REQUEST.IQ_REJECTION_MAX);
            sendMsg[1] = (byte)radarDetect.GetLastUnwrap(eMote.RADAR_NOISE_REQUEST.IQ_REJECTION_CURRENTLY_USED);
            sendMsg[2] = (byte)radarDetect.GetLastUnwrap(eMote.RADAR_NOISE_REQUEST.IQ_REJECTION_ZERO);
            sendMsg[3] = (byte)backgroundNoiseMax;
            sendMsg[4] = (byte)backgroundNoise;
            sendMsg[5] = (byte)backgroundNoiseZero;            
            sendMsg[6] = (byte)iqUsed;
            SendUnwrap((ushort)unwrap, sendMsg, (ushort)iqUsed);*/
        }
        
    }
}
