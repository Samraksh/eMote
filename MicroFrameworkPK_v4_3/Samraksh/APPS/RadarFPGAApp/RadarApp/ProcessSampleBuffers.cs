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
        public static ushort thresholdRadians = (ushort)(thresholdRotations * 2 * 3.14159);
        public static bool threshholdMet = false;
        public static int j = 0;

        private static bool buzzerState = false;

        //public static Samraksh.eMote.Algorithm.RadarDetection radarDetect = new Samraksh.eMote.Algorithm.RadarDetection();

        private static bool historyUpdateCtrl = false;

        private static int num_humans = 0;
        private static int num_nonhumans = 0;
        private static bool ongoingTimer = false;
        private static bool newDisplacement = false;

        private static int _detectNum = 0;

        private const ushort nSV = 607;
        private const ushort nFeature = 12;
        private static int numCountAboveTarget = 0;

        private static int hitCount = 0;
        private static int distance = 0;

        //Countdown do decision
        private static int remStepsToDecision = 0;

        private static HumanCarClassifier _humanCarClassifier = new HumanCarClassifier();
        public static Samraksh.eMote.Algorithm.DecisionFunction decisionFunc = new Samraksh.eMote.Algorithm.DecisionFunction();
        private static Timer _aggregateTimer = new Timer(Send_Decision, null, Timeout.Infinite, Timeout.Infinite);
        private static DecisionSendState decState = DecisionSendState.TEMP_DEC;

        private static DetectionStatus ongoingDisp = DetectionStatus.FALSE;
        enum DetectionStatus
        {
            FALSE = 0,
            SPECULATIVE = 1,   //3.1415*4096 = 12868  4096pi
            TRUE = 2
        }

        enum DecisionSendState
        {
            TEMP_DEC = 0,
            CONF_DEC = 1,
            DEFERRED_DEC = 2
        }

        private static bool[] slidingWindowEndDetect = new bool[3]; // N -- hardcoded for now


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

            // Initialize parameters
            float rho = -0.015521f;
            float gamma = 5f;
            float[] weight = new float[nSV] { 2.4655f, 0.56519f, 0.066342f, 2.4234f, 10f, 0.10739f, 2.3261f, 0.3757f, 0.11023f, 0.25108f, 1.4208f, 0.13423f, 0.66217f, 3.7894f, 10f, 0.061114f, 4.2763f, 1.1484f, 0.83438f, 4.3804f, 0.96626f, 1.0947f, 0.41161f, 0.29151f, 2.6183f, 0.45351f, 0.72376f, 1.0939f, 3.6758f, 0.17707f, 1.0332f, 2.5567f, 6.6983f, 0.081226f, 1.4979f, 10f, 1.0745f, 0.42088f, 0.29071f, 1.5086f, 1.4484f, 0.34084f, 2.0417f, 0.38889f, 1.0702f, 0.46337f, 0.42726f, 0.22934f, 0.15193f, 0.20832f, 0.84323f, 0.35475f, 0.15841f, 5.4484f, 0.34442f, 1.1547f, 0.20782f, 0.56018f, 1.2295f, 0.53584f, 3.4939f, 0.12937f, 0.33949f, 0.035723f, 0.98478f, 0.72014f, 0.96718f, 2.2524f, 0.35574f, 0.61718f, 0.54368f, 0.22227f, 0.73862f, 1.2045f, 6.1422f, 0.31585f, 0.11541f, 0.99586f, 0.64951f, 10f, 0.88383f, 0.62376f, 10f, 2.4971f, 3.7906f, 1.3386f, 0.90917f, 0.97148f, 0.69492f, 10f, 0.21963f, 0.99509f, 0.66275f, 0.96316f, 1.946f, 1.0631f, 0.59153f, 0.10519f, 0.34037f, 0.13813f, 0.061092f, 0.018645f, 0.43699f, 4.7134f, 8.4653f, 0.72588f, 1.7483f, 1.819f, 1.4207f, 0.20747f, 1.0504f, 2.3407f, 0.48409f, 0.95822f, 0.72146f, 2.1425f, 10f, 1.228f, 0.69048f, 1.3247f, 3.7727f, 0.90761f, 4.6354f, 10f, 1.1172f, 0.75531f, 0.96153f, 4.4651f, 0.28742f, 3.0933f, 0.88977f, 7.8376f, 0.97624f, 7.7972f, 3.2037f, 10f, 0.17942f, 6.9942f, 0.49903f, 0.2925f, 2.3435f, 0.058079f, 10f, 0.49824f, 9.6491f, 1.6043f, 7.0502f, 0.0016595f, 1.2197f, 1.3902f, 0.23836f, 0.97073f, 0.17763f, 0.10476f, 10f, 5.7114f, 2.0413f, 3.0518f, 1.1918f, 4.002f, 0.41516f, 10f, 9.0714f, 3.7823f, 3.593f, 0.55268f, 0.4433f, 7.0591f, 10f, 1.4825f, 0.045363f, 1.1991f, 3.2607f, 10f, 2.4711f, 0.22839f, 0.054836f, 0.50443f, 0.32565f, 10f, 0.2391f, 0.48944f, 0.51725f, 0.20639f, 0.1931f, 0.015731f, 2.3972f, 0.040962f, 0.93259f, 10f, 0.28029f, 6.8274f, 0.81652f, 10f, 1.2009f, 0.075604f, 1.0286f, 0.96956f, 1.1673f, 0.97602f, 0.23703f, 1.8787f, 0.8901f, 0.84465f, 0.77809f, 0.80899f, 0.74462f, 0.069914f, 0.46274f, 0.97297f, 0.95588f, 0.045044f, 0.020171f, 0.092662f, 0.76974f, 0.44644f, 0.88776f, 0.026415f, 2.9763f, 0.87523f, 0.23553f, 0.093051f, 2.39f, 0.58188f, 6.4f, 0.42185f, 0.8412f, 0.58135f, 0.82877f, 3.6581f, 10f, 0.10156f, 0.20217f, 0.5104f, 1.0795f, 0.31938f, 1.3987f, 5.4528f, 0.47523f, 3.0876f, 1.5958f, 1.2939f, 2.0647f, 0.98522f, 2.2241f, 0.78141f, 0.34488f, 0.60074f, 0.95228f, 0.74293f, 0.070702f, 0.74939f, 0.060669f, 0.98618f, 0.4349f, 0.98438f, 0.8631f, 0.15578f, 0.304f, 2.8905f, 0.61954f, 0.44195f, 0.84742f, 0.018714f, 0.098547f, 1.0311f, 0.86489f, 0.80631f, 0.40176f, 1.21f, 8.9735f, 0.63373f, 3.3633f, 6.1607f, 0.71381f, 2.2149f, 0.12054f, 0.17899f, 2.0046f, 2.5288f, 3.249f, 0.51861f, 4.4648f, 0.26421f, 0.84348f, 0.18037f, 10f, 1.7197f, 4.3159f, 0.84894f, 0.66036f, 0.29494f, 5.6104f, 10f, 1.5217f, 1.8989f, 1.4386f, 6.7061f, 0.37689f, 0.13067f, 6.5879f, 10f, 0.15206f, 0.41263f, 3.5407f, 0.092072f, 2.3731f, 6.1936f, 4.2717f, 0.67459f, 2.2106f, 1.1979f, 1.4372f, 1.4042f, 0.67212f, 1.5796f, 2.4114f, 0.054878f, 0.52832f, 0.98698f, 0.33972f, 0.17933f, 0.89074f, 3.2875f, 1.8486f, 0.10849f, 0.7903f, 0.16063f, 0.57145f, 8.1314f, 0.0044505f, -7.8057f, -0.71579f, -0.12777f, -0.33126f, -0.31708f, -0.12583f, -4.8134f, -0.2789f, -0.60702f, -1.9742f, -10f, -2.9519f, -0.71768f, -5.6248f, -10f, -0.64837f, -3.1498f, -9.2016f, -10f, -1.8338f, -0.91015f, -4.6391f, -0.83783f, -3.7572f, -4.448f, -0.11173f, -2.7584f, -3.3196f, -3.1414f, -0.32228f, -10f, -5.232f, -10f, -0.1528f, -3.3426f, -0.50104f, -0.34406f, -1.1164f, -3.5265f, -5.0367f, -6.9423f, -0.008177f, -1.0296f, -1.2815f, -3.5279f, -0.68782f, -0.42516f, -2.3689f, -0.84228f, -1.2538f, -4.1642f, -1.0705f, -1.6675f, -0.98475f, -1.5179f, -0.41953f, -0.27806f, -9.223f, -0.4879f, -10f, -5.6608f, -0.48248f, -8.7454f, -1.1581f, -0.41006f, -1.2612f, -2.5866f, -0.83066f, -1.1942f, -0.12094f, -0.58942f, -5.2706f, -0.23939f, -0.59281f, -1.3405f, -1.0842f, -0.60562f, -0.179f, -0.48144f, -0.23124f, -0.76662f, -5.7598f, -0.58268f, -0.22828f, -0.8004f, -0.52897f, -0.09037f, -2.9136f, -3.9179f, -1.6641f, -1.8636f, -0.59796f, -6.8402f, -2.2468f, -0.043981f, -0.56866f, -2.5024f, -2.9606f, -1.4445f, -2.7788f, -1.0473f, -0.6478f, -1.9586f, -0.70873f, -0.078035f, -0.34439f, -5.8885f, -1.509f, -0.92708f, -1.9713f, -0.028325f, -2.0896f, -0.53331f, -0.35367f, -2.1429f, -0.71122f, -0.65832f, -0.53179f, -0.49813f, -0.13118f, -6.5789f, -6.5422f, -1.6799f, -0.35191f, -1.132f, -0.12211f, -3.11f, -4.1583f, -1.0718f, -0.25299f, -10f, -0.22179f, -0.90624f, -0.33946f, -0.50173f, -0.69229f, -4.8105f, -10f, -0.48385f, -0.31228f, -2.1055f, -0.59774f, -0.40566f, -1.8222f, -1.1912f, -9.3711f, -0.18009f, -0.19872f, -1.1775f, -10f, -1.2184f, -0.94019f, -0.22926f, -5.3604f, -0.83287f, -7.1275f, -0.053934f, -1.0967f, -4.1684f, -0.39227f, -0.54992f, -0.7235f, -0.66992f, -4.5856f, -1.0252f, -3.4577f, -4.9401f, -0.44635f, -0.85299f, -8.244f, -1.2697f, -0.92848f, -0.57155f, -0.62912f, -0.33102f, -2.1573f, -0.56264f, -0.20174f, -2.3818f, -2.4113f, -0.12715f, -0.57991f, -0.87521f, -7.6069f, -8.68f, -0.20342f, -3.1253f, -1.4345f, -0.12305f, -4.4883f, -1.8159f, -8.209f, -2.4108f, -0.78102f, -1.5725f, -0.5009f, -1.0008f, -0.15816f, -4.5611f, -0.60801f, -6.0309f, -0.48849f, -7.1916f, -2.8549f, -1.2195f, -2.8856f, -10f, -2.6756f, -7.1961f, -0.73805f, -0.40126f, -2.6068f, -1.7121f, -0.44684f, -6.5132f, -10f, -1.5596f, -1.3861f, -10f, -0.67989f, -0.016002f, -0.98094f, -0.61957f, -2.3295f, -7.1585f, -2.3368f, -0.13129f, -10f, -2.0713f, -2.0909f, -8.0141f, -0.12857f, -3.8943f, -0.52714f, -0.546f, -0.22881f, -0.59902f, -0.3023f, -4.9368f, -7.805f, -2.4448f, -1.5284f, -0.62619f, -0.06526f, -3.0008f, -1.235f, -0.33422f, -0.64955f, -1.2231f, -0.95376f, -10f, -1.4285f, -0.81851f, -1.4527f, -0.94634f, -0.095761f, -3.3804f, -0.11441f, -7.5315f, -0.23599f, -3.2555f, -1.2677f, -1.691f, -10f, -3.9218f, -0.50678f, -0.59335f, -8.8889f, -3.5045f, -0.66005f, -0.55127f, -1.0169f, -2.8864f, -0.77529f, -1.5076f, -0.7586f };
            float[] feature_min = new float[nFeature] { 487071.4276f, 1365.3935f, 80f, 8f, 1.7564f, 58f, -148f, 32f, 8f, 3f, -1f, -41f };
            float[] scalingFactors = new float[nFeature] { 2.0944e-14f, 2.1925e-06f, 5.5047e-07f, 0.0040323f, 7.0137e-05f, 0.014286f, 0.0043353f, 0.010417f, 0.0083333f, 2.8774e-06f, 0.0077519f, 0.0096774f };

            decisionFunc.Initialize(nSV, nFeature, rho, gamma, weight, feature_min, scalingFactors);
        }

        /// <summary>
        /// ADC callback
        /// </summary>
        /// <remarks>Called when the ADC driver has collected a buffer's worth of data</remarks>
        /// <param name="FPGAIQRejection"></param>
        private static void ADCCallback(long FPGAIQRejection)
        {
            radar.SetProcessingInProgress(true);
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

        private static void Send_Decision(object state)
        {

            switch (decState)
            {
                case DecisionSendState.TEMP_DEC:
                    //lock (_thisLock)
                    {
                        if (num_humans == num_nonhumans)
                        {
                            Debug.Print("\tAttempting to send temporary decision AMBIGUOUS.");
                        }
                        else if (num_humans > num_nonhumans)
                        {

                            Debug.Print("\tAttempting to send temporary decision HUMAN.");
                        }
                        else
                        {

                            Debug.Print("\tAttempting to send temporary decision NON-HUMAN.");
                        }

                        Debug.Print("Second Timer: memory usage (before) " + Debug.GC(true));

                        _aggregateTimer.Change(45000, Timeout.Infinite);
                        decState = DecisionSendState.CONF_DEC;

                        Debug.Print("Second Timer: memory usage (after) " + Debug.GC(true));

                    }
                    break;

                case DecisionSendState.CONF_DEC:
                    //lock (_thisLock)
                    {

                        if (!newDisplacement) // If not in an ongoing displacement, send result immediately
                        {
                            if (num_humans == num_nonhumans)
                            {
                                Debug.Print("\tAttempting to send final decision AMBIGUOUS.");
                            }
                            else if (num_humans > num_nonhumans)
                            {
                                Debug.Print("\tAttempting to send final decision HUMAN.");
                            }
                            else
                            {
                                Debug.Print("\tAttempting to send final decision NON-HUMAN.");
                            }
                            _detectNum++;

                            //Reset variables
                            //_aggregateTimer = null;
                            _aggregateTimer.Change(Timeout.Infinite, Timeout.Infinite);
                            decState = DecisionSendState.TEMP_DEC;
                            ongoingTimer = false;
                            num_humans = 0;
                            num_nonhumans = 0;
                        }
                        else // Defer sending the decision
                        {
                            _aggregateTimer.Change(Timeout.Infinite, Timeout.Infinite);
                            decState = DecisionSendState.DEFERRED_DEC;
                        }
                    }
                    break;

                default:
                    break;

            }
        }

        private static float getDecision()
        {
            float[] featureVector = new float[nFeature]; // Array length hardcoded for now
            int time = HumanCarClassifier.detectionStepIndex + 1;

            // if (time <= 8 || ((double)numCountAboveTarget)/(time*ADCBufferSize/2) <= 0.05) // Taking care of small walks and noise bumps
            if (time <= 8 || HumanCarClassifier.Spectrogram.numHitBins_max_99 > 168 || ((double)numCountAboveTarget) / (time * ADCBufferSize / 2) <= 0.05) // Taking care of small walks and metastability
            {
                return 999;
            }

            // Initialize feature vector
            featureVector[0] = distance * time;
            featureVector[1] = distance / time;
            featureVector[2] = HumanCarClassifier.Spectrogram.numHitBins_sum_9;
            featureVector[3] = HumanCarClassifier.Statistics.median(HumanCarClassifier.Spectrogram.numHitBinsArr_9, HumanCarClassifier.Spectrogram.spectArrIndex);
            featureVector[4] = HumanCarClassifier.Statistics.var(HumanCarClassifier.Spectrogram.numHitBinsArr_9, HumanCarClassifier.Spectrogram.spectArrIndex);
            featureVector[5] = HumanCarClassifier.Spectrogram.maxFreqThroughWindows_9;
            featureVector[6] = (float)HumanCarClassifier.Spectrogram.numHitBins_max_9 / (time - 8);
            featureVector[7] = HumanCarClassifier.Spectrogram.maxFreqThroughWindows_19;
            featureVector[8] = HumanCarClassifier.Spectrogram.maxFreqThroughWindows_30;
            featureVector[9] = HumanCarClassifier.Spectrogram.numHitBins_sum_99;
            featureVector[10] = HumanCarClassifier.Spectrogram.maxFreqThroughWindows_99;
            featureVector[11] = (float)HumanCarClassifier.Spectrogram.numHitBins_max_99 / (time - 8);

            int[] normalized_data = new int[nFeature];
            decisionFunc.NormalizeFeatureVector(featureVector, normalized_data);
            return decisionFunc.Decide(normalized_data);
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
        /// Process the data in a separate thread
        /// </summary>
        /// <remarks>
        /// We do this so that the ADC callback will return quickly.
        /// The main program blocks until this thread ends
        /// </remarks>
        private static void ProcessBuffers(IQ iq)
        {
            bool walkendflag = false;
            bool detection = true;


            var iBuff = iq.IBuff;
            var qBuff = iq.QBuff;
            if (!newDisplacement)
            {
                newDisplacement = true;
                Debug.Print("new");
            }
            if (!ongoingTimer)
            {
                Debug.Print("Detect " + _detectNum);
            }
            var windowOverThresh = radar.GetWindowOverThreshold();
            // Implement sliding window to detect end of detection
            slidingWindowEndDetect[0] = slidingWindowEndDetect[1];
            slidingWindowEndDetect[1] = slidingWindowEndDetect[2];
            slidingWindowEndDetect[2] = windowOverThresh;
            if (ongoingDisp == DetectionStatus.FALSE && windowOverThresh)
            {
                // Speculative walk; to be confirmed in N-1 steps
                Debug.Print("Spec");
                ongoingDisp = DetectionStatus.SPECULATIVE;
                remStepsToDecision = 2; // N-1 for us
            }
            else if (ongoingDisp == DetectionStatus.SPECULATIVE && !detection)
            {
                if (--remStepsToDecision == 0)
                {
                    Debug.Print("false");
                    //M out of N failed, this was a false alarm. Clear features computed so far
                    ongoingDisp = DetectionStatus.FALSE;
                    _humanCarClassifier.ClearAll();
                    distance = 0;
                    numCountAboveTarget = 0;
                }
            }
            else if (ongoingDisp == DetectionStatus.SPECULATIVE && detection)
            {
                //Legitimate walk
                ongoingDisp = DetectionStatus.TRUE;
                Debug.Print("ongoing");
                // Test -- send detection to base -- PEASE COMMENT LATER
                //routing.Send_Data('D');
            }
            else if (ongoingDisp == DetectionStatus.TRUE && (!windowOverThresh || (HumanCarClassifier.detectionStepIndex + 1) == HumanCarClassifier.nStepMax))
            {
                int numLows = 0;

                // Check the last 3 sliding windows to decide if the present walk has ended
                for (int j = 0; j < 3; j++)
                {
                    if (!slidingWindowEndDetect[j])
                        numLows++;
                }

                //Ongoing legitimate walk has ended - perform classification
                // Checking if numlows >=M (hard-coded for now), or if the max allowed time for a walk has elapsed
                if (numLows > 1 || (HumanCarClassifier.detectionStepIndex + 1) >= HumanCarClassifier.nStepMax)
                {
                    ongoingDisp = DetectionStatus.FALSE;
                    walkendflag = true;
                    float dec = getDecision();

                    //lock (_thisLock)
                    {
                        if (newDisplacement)
                        {
                            newDisplacement = false;
                        }

                        //Routing Send stuff goes here
                        if (dec < 0)
                        {
                            Debug.Print("Non-human, decision=" + dec);
                            //routing.Send_Data('N');
                            num_nonhumans++;
                        }
                        else if (dec < 999) // Added to avoid no-classify case
                        {
                            Debug.Print("Human, decision=" + dec);
                            //routing.Send_Data('H');
                            num_humans++;
                        }

                        if (!ongoingTimer)
                        {

                            Debug.Print("First Timer: memory usage (before) " + Debug.GC(true));

                            //_aggregateTimer = new Timer(Send_Confirmation, null, 60000, Timeout.Infinite); // Create one-shot timer for 15 seconds
                            _aggregateTimer.Change(15000, Timeout.Infinite);
                            ongoingTimer = true;

                            Debug.Print("First Timer: memory usage (after) " + Debug.GC(true));

                        }
                        else if (decState == DecisionSendState.DEFERRED_DEC) // Send final message now that ongoing detection is over
                        {
                            if (num_humans == num_nonhumans)
                            {
                                Debug.Print("\tAttempting to send final decision AMBIGUOUS.");
                            }
                            else if (num_humans > num_nonhumans)
                            {
                                Debug.Print("\tAttempting to send final decision HUMAN.");
                            }
                            else
                            {
                                Debug.Print("\tAttempting to send final decision NON-HUMAN.");
                            }
                            _detectNum++;

                            //Reset variables
                            //_aggregateTimer = null;
                            _aggregateTimer.Change(Timeout.Infinite, Timeout.Infinite);
                            decState = DecisionSendState.TEMP_DEC;
                            ongoingTimer = false;
                            num_humans = 0;
                            num_nonhumans = 0;
                        }
                    }

                    _humanCarClassifier.ClearAll();
                    distance = 0;
                    numCountAboveTarget = 0;
                }
            }

            if (ongoingDisp != DetectionStatus.FALSE)
            {
                distance += radar.GetAbsoluteDisplacement(eMote.SAMPLE_WINDOW_PORTION.SAMPLE_WINDOW_FULL);
                Debug.Print("distance: " + distance.ToString());
                numCountAboveTarget += radar.GetCountOverTarget();
                Debug.Print("count over target: " + numCountAboveTarget.ToString());
                _humanCarClassifier.ComputePhaseFeaturesLongBuffers(iBuff, qBuff);
            }
            radar.SetProcessingInProgress(false);
        }



    }
}
