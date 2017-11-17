//#define DBG_VERBOSE
//#define DBG_SIMPLE
#define DBG_LOGIC

// Ensure exactly one defined
#if !DBG_VERBOSE && !DBG_SIMPLE && !DBG_LOGIC
#error Exactly one of DBG_VERBOSE, DBG_SIMPLE, DBG_LOGIC must be defined.
#endif
#if  DBG_VERBOSE && (DBG_SIMPLE || DBG_LOGIC) || DBG_SIMPLE && (DBG_VERBOSE || DBG_LOGIC) || DBG_LOGIC && (DBG_VERBOSE || DBG_SIMPLE)
#error Exactly one of DBG_VERBOSE, DBG_SIMPLE, DBG_LOGIC must be defined.
#endif

using System;
using System.Reflection;
using System.Threading;
using Microsoft.SPOT;
//using Microsoft.SPOT.Hardware;
using Samraksh.eMote;
using Samraksh.eMote.NonVolatileMemory;
using Samraksh.eMote.DotNow;
using Samraksh.eMote.DSP;
//using Samraksh.eMote.Net;
//using Samraksh.eMote.Net.Radio;
//using Samraksh.eMote.Net.Mac;
//using Samraksh.SPOT.Hardware;
//using Samraksh.SPOT.DSP;
//using Samraksh.SPOT.NonVolatileMemory;
//using Samraksh_eMote_Net_Routing;
using System.Collections;

// relation between real length and the big int (VALUE) in the program:
// lambda = 3e8/5.8e9 = 0.0517
// lambda/2=0.0258  corresponding to 2pi * 4096= 25736
// length = 0.0258 * VALUE/25736 = VALUE * 1.0025e-06 ~= VALUE *1e-6;
/*enum PI
{
    HALF = 6434,
    FULL = 12868,   //3.1415*4096 = 12868  4096pi
    NEG = -12868,
    TWO = 25736,
}*/

namespace Samraksh.AppNote.Scarecrow.Radar
{
    public class HumanCarClassifier
    {
        // Microsoft.SPOT.Hardware.OutputPort testPort_PIN1 = new Microsoft.SPOT.Hardware.OutputPort(Samraksh.eMote.DotNow.Pins.GPIO_J12_PIN1, true);

        //public static bool waiting = true;
        public static int c = 0;

        public const int fftWindowSize = 256;
        public const int step = 64;   // in fact it is fftWindowSize - overlap, which is the proceed points
        public const int nStepMax = 60; // Added by Dhrubo - variable extracted from now-nonexistent dstate

        public static ushort[] channelIBuffer;
        public static ushort[] channelQBuffer;

        public static short[] fftInput = new short[2 * fftWindowSize];
        public static short[] fftOutput = new short[2 * fftWindowSize];

        public static int meanI = 1960; //<-For internal ADC. Old values: Jin - 2044;// 2053 new radar; // 2044 old radar;//2047; // 2837;    // dummy test change
        public static int meanQ = 1960; //<-For internal ADC. Old values: Jin - 2048;//2055 new radar; ////1924 old radar; // 2852;

        public static int detectionStepIndex; // Added by Dhrubo

        public static Transforms transforms = new Transforms();

        // Paste remaining stuff here
        public static class Spectrogram
        {
            //public static int[] freq;
            public static int[] numHitBinsArr_9;
            public static int spectArrIndex;

            //public static int[] numHitBinsArr_high;
            //public static double[] thr_sqr_Csharp;

            public static int thr_sqr_Csharp_9;
            public static int thr_sqr_Csharp_19;
            public static int thr_sqr_Csharp_30;
            public static int thr_sqr_Csharp_99;

            public static int numHitBins_9;
            public static int numHitBins_19;
            public static int numHitBins_30;
            public static int numHitBins_99;

            public static int numHitBins_sum_9;
            public static int numHitBins_sum_99;

            public static int numHitBins_max_9;
            public static int numHitBins_max_99;

            public static int maxFreqInWindow_9;
            public static int maxFreqInWindow_19;
            public static int maxFreqInWindow_30;
            public static int maxFreqInWindow_99;

            public static int maxFreqThroughWindows_9;
            public static int maxFreqThroughWindows_19;
            public static int maxFreqThroughWindows_30;
            public static int maxFreqThroughWindows_99;

            // public static int maxFreqThroughWindows_high;
            // public static int minFreqInWindow; //useless
            // public static int minFreqThroughWindows; //useless
            // public static int MaxRunLenInWindow;  //useless
            // public static int maxRunLenThroughWindows; //useless

            public static void init()
            {
                numHitBins_9 = 0;
                numHitBins_19 = 0;
                numHitBins_30 = 0;
                numHitBins_99 = 0;

                numHitBins_sum_9 = 0;
                numHitBins_sum_99 = 0;

                numHitBins_max_9 = 0;
                numHitBins_max_99 = 0;

                detectionStepIndex = -1; // Added by Dhrubo
                spectArrIndex = 0;

                maxFreqInWindow_9 = -1;
                maxFreqInWindow_19 = -1;
                maxFreqInWindow_30 = -1;
                maxFreqInWindow_99 = -1;

                maxFreqThroughWindows_9 = -1;
                maxFreqThroughWindows_19 = -1;
                maxFreqThroughWindows_30 = -1;
                maxFreqThroughWindows_99 = -1;

                /////////////////////////////////// parameters to change according to matlab code - File2Feature.m
                // thr_sqr_Csharp = 30;//10; //25->122, 30->385, 20->38.5(39), 15->12, 10->3.85(4), 17->19.3 18->24.3 19->30.6 16->15.3
                // thr_sqr_Csharp_high = 99;// 17;
                thr_sqr_Csharp_9 = 9;
                thr_sqr_Csharp_19 = 19;
                thr_sqr_Csharp_30 = 30;
                thr_sqr_Csharp_99 = 99;

                ///////////////////////////////////////////////////////////////////////////////////////////////
                numHitBinsArr_9 = new int[nStepMax];
                // numHitBinsArr = new int[60];  //dState.nWindow-3
                // numHitBinsArr_high = new int[60];
            }

            public static void resetOneCol()
            {
                //numHitBins = 0;
                //numHitBins_high = 0;

                numHitBins_9 = 0;
                numHitBins_19 = 0;
                numHitBins_30 = 0;
                numHitBins_99 = 0;

                //maxFreqInWindow = -1;
                //maxFreqInWindow_high = -1;

                maxFreqInWindow_9 = -1;
                maxFreqInWindow_19 = -1;
                maxFreqInWindow_30 = -1;
                maxFreqInWindow_99 = -1;

                //useless
                // MaxRunLenInWindow = 0;
            }

            public static void reset()
            {
                numHitBins_9 = 0;
                numHitBins_19 = 0;
                numHitBins_30 = 0;
                numHitBins_99 = 0;

                numHitBins_sum_9 = 0;
                numHitBins_sum_99 = 0;

                numHitBins_max_9 = 0;
                numHitBins_max_99 = 0;

                maxFreqInWindow_9 = -1;
                maxFreqInWindow_19 = -1;
                maxFreqInWindow_30 = -1;
                maxFreqInWindow_99 = -1;

                maxFreqThroughWindows_9 = -1;
                maxFreqThroughWindows_19 = -1;
                maxFreqThroughWindows_30 = -1;
                maxFreqThroughWindows_99 = -1;

                spectArrIndex = 0;
            }

            public static int freq(int i)
            {
                return i < 128 ? i : (256 - i);
            }

        }

        public static class Sorter
        {
            public static void Sort(int[] a, int size)
            {
                QuickSort(a, 0, size - 1);
            }
            public static void QuickSort(int[] a, int start, int end)
            {
                if (start >= end) return;
                int q = partition(a, start, end);
                QuickSort(a, start, q - 1);
                QuickSort(a, q + 1, end);
            }
            public static int partition(int[] a, int start, int end)
            {
                int pivotIndex = start;//start+(int)(Math.random()*(end-start));
                int pivot = a[pivotIndex];
                swap(a, pivotIndex, end);
                int storeIndex = start;
                for (int i = start; i <= end - 1; i++)
                {
                    if (a[i] <= pivot)
                    {
                        swap(a, i, storeIndex++);
                    }
                }
                swap(a, storeIndex, end);
                return storeIndex;
            }

            public static void swap(int[] a, int i, int j)
            {
                int tmp = a[i];
                a[i] = a[j];
                a[j] = tmp;
            }
        }

        public static class Statistics
        {
            public static float mean(ushort[] arr)
            {
                float res = 0;
                for (int i = 0; i < arr.Length; i++)
                    res += arr[i];
                res /= arr.Length;
                return res;
            }
            public static float mean(int[] arr, int size)
            {
                float res = 0;
                for (int i = 0; i < size; i++)
                    res += arr[i];
                res /= size;
                return res;
            }
            public static float var(int[] arr, int size)
            {
                float res = 0;
                float m = mean(arr, size);

                for (int i = 0; i < size; i++)
                    res += (arr[i] - m) * (arr[i] - m);
                res /= size - 1;
                return res;
            }

            public static float std(int[] arr, int size)
            {
                float res = 0;
                float v = var(arr, size);
                res = (float)System.Math.Sqrt((float)v);
                return res;
            }

            public static float median(int[] arr, int size)
            {
                Sorter.Sort(arr, size);
                if (size % 2 == 1) { return arr[size / 2]; }
                else { return (arr[size / 2 - 1] + arr[size / 2]) / 2.0f; }
            }
        }


        ////////////////////////////////////////////////

        public HumanCarClassifier()
        {
            /*Debug.Print("Initializing LCD ....");
            lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Clear();
            lcd.Write(LCD.CHAR_S, LCD.CHAR_T, LCD.CHAR_A, LCD.CHAR_R);*/

#if !DBG_LOGIC
            Debug.Print("Initializing fft buffer arr ...");
#endif
            channelIBuffer = new ushort[fftWindowSize];  //fftWindowSize, 
            channelQBuffer = new ushort[fftWindowSize];  //fftWindowSize,

            Spectrogram.init();
#if !DBG_LOGIC
            Debug.Print("Successfully initialized spectrogram.");
#endif

            // DecisionFunction.init();
            // Debug.Print("Successfully initialized decision function.");
        }

        public void ClearAll()
        {
            //Debug.Print("dState.phaseChangeInOneStep=" + dState.phaseChangeInOneStep.ToString());
            // feature calculation are from here at 0->1, if later 1->2. If 1->0, the feature computation should reset at next 0->1
            // may need to add more. All intermediate variables to compute features need to be clear to 0 here.
            Spectrogram.reset();
            // Changed detectionStepIndex reset to -1 instead of 0 to maintain parity with initialization -- Dhrubo
            detectionStepIndex = -1; // 0; // can be any small value like 1 2 3, just to avoid the outofrange error in array
            // dState.noiseParameter.clear(); --Commented by Dhrubo
            // dState.maxAbsPhaseChangeInOneStep = 0; --Commented by Dhrubo
            // dState.detectionState = 1; --Commented by Dhrubo
            // Debug.Print("detectionState 0->1"); --Commented by Dhrubo
            // Debug.Print("dState.allStepIndex=" + dState.allStepIndex.ToString());
        }

        /* Added by Dhrubo to reset spectrogram features alone - typically at the start+3rd hit before starting
         * computation of spectrogram features*/
        public void ResetSpectrogram()
        {
            Spectrogram.reset();
        }

        /* Added by Dhrubo -- Meant to compute features on the entire buffer of step*2 = 128 samples at the start of
         * detection, so as not to lose the first 1/4 second of data.
         * Leverages the fact that spectrogram features are not computed in the beginning, so there's time to process
         * phase features on a longer array */
        public void ComputePhaseFeaturesLongBuffers(ushort[] longBuffer1, ushort[] longBuffer2)
        {
            ushort[] buf1 = new ushort[step];
            ushort[] buf2 = new ushort[step];

            if ((detectionStepIndex + 1) < (fftWindowSize / longBuffer1.Length - 1) * longBuffer1.Length / step)
            {
                for (int i = 0; i < longBuffer1.Length / step; i++)
                {
                    Array.Copy(longBuffer1, i * step, buf1, 0, step);
                    Array.Copy(longBuffer2, i * step, buf2, 0, step);

                    //testPort_PIN1.Write(true);
                    ComputeIncrementalFeatures(buf1, buf2, false);
                    //testPort_PIN1.Write(false);
                }
            }
            else if ((detectionStepIndex + 1) == (fftWindowSize / longBuffer1.Length - 1) * longBuffer1.Length / step)
            {
                for (int i = 0; i < longBuffer1.Length / step; i++)
                {
                    Array.Copy(longBuffer1, i * step, buf1, 0, step);
                    Array.Copy(longBuffer2, i * step, buf2, 0, step);

                    bool b = (i == (longBuffer1.Length / step - 1)) ? true : false;
                    //testPort_PIN1.Write(true);
                    ComputeIncrementalFeatures(buf1, buf2, b);
                    //testPort_PIN1.Write(false);
                }
            }
            else
            {
                for (int i = 0; i < longBuffer1.Length / step; i++)
                {
                    Array.Copy(longBuffer1, i * step, buf1, 0, step);
                    Array.Copy(longBuffer2, i * step, buf2, 0, step);
                    //testPort_PIN1.Write(true);
                    ComputeIncrementalFeatures(buf1, buf2, true);
                    //testPort_PIN1.Write(false);
                }
            }
        }

        /* Added by Dhrubo -- Meant to operate on channelIBuffer and channelQBuffer as in Jin's implementation.
         * 1. sampleBuffer1[] and sampleBuffer2[] are step-size (64) samples passed to the function on a per-ADC callback basis.
         * 2. computeSpectrogramFlag indicates if a spectrogram needs to be computed - not needed for first three hits because
         * we need sliding window to be full*/
        public void ComputeIncrementalFeatures(ushort[] sampleBuffer1, ushort[] sampleBuffer2, bool computeSpectrogramFlag)
        {

            //////////////////// update and rotate the sample array
            //Debug.Print("d1 " + step.ToString() + " " + (fftWindowSize - step).ToString());
            //Array.Copy(channelIBuffer, step, channelIBuffer, 0, fftWindowSize - step);
            //Array.Copy(channelQBuffer, step, channelQBuffer, 0, fftWindowSize - step);
            Debug.Print("d2");
            sampleBuffer1.CopyTo(channelIBuffer, fftWindowSize - step);
            sampleBuffer2.CopyTo(channelQBuffer, fftWindowSize - step);
            Debug.Print("d3");
            if (detectionStepIndex < nStepMax - 1) { detectionStepIndex++; }
            ///////////////////// rotate and unwrap the phase
            //TODO: Replace phase calculation with lower layer stuff
            //dState.rotatePhase();
            //dState.clearMaxMinPhaseInStep();

            //dState.IsumInStep = 0;
            //dState.QsumInStep = 0;

            //dState.I2sumInStep = 0;
            //dState.Q2sumInStep = 0;

            /* Commented by Dhrubo
            for (int i = 0; i < step; i++)
            {
                dState.IsumInStep += channelIBuffer[256 - step + i];
                dState.QsumInStep += channelQBuffer[256 - step + i];

                dState.I2sumInStep += channelIBuffer[256 - step + i] * channelIBuffer[256 - step + i];
                dState.Q2sumInStep += channelQBuffer[256 - step + i] * channelQBuffer[256 - step + i];

                //Debug.Print("I2sumInStep=" + dState.I2sumInStep);

                //short acI = (short)(channelIBuffer.buffer[256 - step + i] - meanI);
                //short acQ = (short)(channelQBuffer.buffer[256 - step + i] - meanQ);
                //Debug.Print("acI=" + acI.ToString());
                //Debug.Print("acQ=" + acQ.ToString());

                \* Commented by Dhrubo*\
                //dState.noiseParameter.sumIsqr += acI * acI;
                //dState.noiseParameter.sumQsqr += acQ * acQ;
                \*End comment*\

                // TimeDomain. - PLEASE UNCOMMENT!
                \*int currPhase = Phase.unwrap(acI, acQ);//>> 12; // /4096
                if (currPhase > dState.maxPhaseInStep) dState.maxPhaseInStep = currPhase;
                if (currPhase < dState.minPhaseInStep) dState.minPhaseInStep = currPhase;

                dState.phase[128 - step + i] = currPhase;*\

                //Debug.Print("currPhase=" + currPhase.ToString());

            }*/
            //dState.updateMaxMinPhaseInStepArr();


            /*dState.phaseChangeInOneStep = dState.phase[127] - dState.phase[63]; -- Commented by Dhrubo */
            // dState.maxAbsPhaseChangeInOneStep = System.Math.Max(dState.maxAbsPhaseChangeInOneStep, System.Math.Abs(phaseChangeInOneStep));

            /*Commented by Dhrubo - unused elsewhere in the computation*/
            /*---------------------------------------------------------*/
            /*//Filling out the phaseChangeArr
            Array.Copy(dState.phaseChangeArr, 1, dState.phaseChangeArr, 0, dState.phaseChangeArr.Length - 1);
            dState.phaseChangeArr[dState.N - 1] = dState.phaseChangeInOneStep;*/

            ///////////////////////////////////////////////////////////////////////////////////////////////////////

            // if (detectionStepIndex < nStepMax - 1) { detectionStepIndex++; }

            // phase related feature update
            //dState.cumAbsPhaseChange += System.Math.Abs(dState.phaseChangeInOneStep); --Commented by Dhrubo
            //dState.cumPhaseChange += dState.phaseChangeInOneStep; --Commented by Dhrubo

            //////////////////////////////////////////////// features computation //////////////////////////////////////////////

            /////////////////////// use phase to compute feature class 1, 

            // Commented by Dhrubo
            /*dState.clearMaxMinPhaseInWindow();
            for (int i = 0; i < dState.maxPhaseInStepArr.Length; i++)
            {
                if (dState.maxPhaseInStepArr[i] > dState.maxPhaseInWindow) dState.maxPhaseInWindow = dState.maxPhaseInStepArr[i];
                if (dState.minPhaseInStepArr[i] < dState.minPhaseInWindow) dState.minPhaseInWindow = dState.minPhaseInStepArr[i];
            }

            dState.diffPhaseInWindow = dState.maxPhaseInWindow - dState.minPhaseInWindow;  //to Change*/

            //Debug.Print("dState.diffPhaseInWindow=" + dState.diffPhaseInWindow);



            //////////////////////////////////////////////// compute fft ////////////////////////////////

            //callbackTime.Write(true);
            //callbackTime.Write(false);

            if (computeSpectrogramFlag) // compute spectrogram only when the flag is set - start+3rd hit onwards
            {

                for (int i = 0; i < fftWindowSize; i++)
                {
                    //fftInput[i*2] = (short)(((channelIBuffer.buffer[i] - meanI)>>4) * hammingWindow[i]); 
                    //fftInput[i*2+1] = (short)(((channelQBuffer.buffer[i] - meanQ)>>4) * hammingWindow[i]); 
                    fftInput[i * 2] = (short)(channelIBuffer[i] - meanI);
                    fftInput[i * 2 + 1] = (short)(channelQBuffer[i] - meanQ);
                }

                transforms.FFT(fftInput, fftOutput, (ushort)(2 * fftWindowSize));

                // after testing, we know that the power scaling factor is: 256^2 = 65536
                // for small power this scaling factor is not accurate. but for large power, this is more accurate
                // some example in testing are 63149, 67612, 66133
                /*
                if (dState.allStepIndex == 3)
                {
                    for (int i = 0; i < 256; i++)
                    {
                        Debug.Print("i=" + i.ToString()+",Iin=" + fftInput[i * 2].ToString()+",Qin=" + fftInput[i * 2+1].ToString());
                    }
                    for (int i = 0; i < 256; i++)
                    {
                        Debug.Print("i=" + i.ToString() + ",Iout=" + fftOutput[i * 2].ToString() + ",Qout=" + fftOutput[i * 2 + 1].ToString() + ",power=" + (fftOutput[i * 2] * fftOutput[i * 2] + fftOutput[i * 2+1] * fftOutput[i * 2+1]).ToString());
                    }
                }*/


                ///////////////////////////// spectrogram related //////////////////////////////////////////////////////

                Spectrogram.resetOneCol();


                //callbackTime.Write(true);
                //callbackTime.Write(false);
                //Debug.Print("fftInput[0]="+fftInput[0].ToString()+" fftInput[1]="+fftInput[1].ToString()+" fftInput[2]="+fftInput[2].ToString() );
                //Debug.Print("fftOutput[0]="+fftOutput[0].ToString()+" fftOutput[1]="+fftOutput[1].ToString()+" fftOutput[2]="+fftOutput[2].ToString() );
                //testPort_PIN1.Write(true);
                for (int i = 0; i < 256; i++)
                {
                    //if (i==0||i==1||i==2||i==254||i==255) continue; //dont include low frequency staff - 5 frequencies 


                    int power = fftOutput[2 * i] * fftOutput[2 * i] + fftOutput[2 * i + 1] * fftOutput[2 * i + 1];
                    //if (dState.detectionStepIndex==0) Debug.Print("i=" + i.ToString() + " power=" + power.ToString());


                    //dState.noiseParameter.meanPowerOnFreq[0][i] = dState.noiseParameter.meanPowerOnFreq[1][i];
                    //dState.noiseParameter.meanPowerOnFreq[1][i] += power;

                    /*
                    dState.noiseParameter.maxPowerInSpect = System.Math.Max(dState.noiseParameter.maxPowerInSpect, power);
                    dState.noiseParameter.sumPowerInSpect += power;
                     * */

                    // power -= (dState.noiseParameter.meanPowerOnFreq[0][i]+dState.noiseParameter.meanPowerOnFreq[1][i])/2;



                    //if (power> Spectrogram.thr_sqr_Csharp[i])    // threshold on different frequencies 

                    if (power > Spectrogram.thr_sqr_Csharp_9)
                    {
                        Spectrogram.numHitBins_9++;
                        if (Spectrogram.freq(i) > Spectrogram.maxFreqInWindow_9)
                        {
                            Spectrogram.maxFreqInWindow_9 = Spectrogram.freq(i);
                        }
                    }
                    if (power > Spectrogram.thr_sqr_Csharp_19)
                    {
                        Spectrogram.numHitBins_19++;
                        if (Spectrogram.freq(i) > Spectrogram.maxFreqInWindow_19)
                        {
                            Spectrogram.maxFreqInWindow_19 = Spectrogram.freq(i);
                        }
                    }
                    if (power > Spectrogram.thr_sqr_Csharp_30)
                    {
                        Spectrogram.numHitBins_30++;
                        if (Spectrogram.freq(i) > Spectrogram.maxFreqInWindow_30)
                        {
                            Spectrogram.maxFreqInWindow_30 = Spectrogram.freq(i);
                        }
                    }
                    if (power > Spectrogram.thr_sqr_Csharp_99)
                    {
                        Spectrogram.numHitBins_99++;
                        if (Spectrogram.freq(i) > Spectrogram.maxFreqInWindow_99)
                        {
                            Spectrogram.maxFreqInWindow_99 = Spectrogram.freq(i);
                        }
                    }
                }

                //if (Spectrogram.MaxRunLenInWindow > Spectrogram.maxRunLenThroughWindows) Spectrogram.maxRunLenThroughWindows = Spectrogram.MaxRunLenInWindow;

                Spectrogram.numHitBinsArr_9[Spectrogram.spectArrIndex++] = Spectrogram.numHitBins_9;  //dState.allStepIndex-3
                // Spectrogram.numHitBinsArr_high[detectionStepIndex] = Spectrogram.numHitBins_high;
                //Debug.Print("detectionStepIndex=" + dState.detectionStepIndex + " numHitBins=" + Spectrogram.numHitBins + " numHitBins_high=" + Spectrogram.numHitBins_high);

                Spectrogram.numHitBins_sum_9 += Spectrogram.numHitBins_9;
                Spectrogram.numHitBins_sum_99 += Spectrogram.numHitBins_99;
                Spectrogram.numHitBins_max_9 = System.Math.Max(Spectrogram.numHitBins_max_9, Spectrogram.numHitBins_9);
                Spectrogram.numHitBins_max_99 = System.Math.Max(Spectrogram.numHitBins_max_99, Spectrogram.numHitBins_99);

                Spectrogram.maxFreqThroughWindows_9 = System.Math.Max(Spectrogram.maxFreqThroughWindows_9, Spectrogram.maxFreqInWindow_9);
                Spectrogram.maxFreqThroughWindows_19 = System.Math.Max(Spectrogram.maxFreqThroughWindows_19, Spectrogram.maxFreqInWindow_19);
                Spectrogram.maxFreqThroughWindows_30 = System.Math.Max(Spectrogram.maxFreqThroughWindows_30, Spectrogram.maxFreqInWindow_30);
                Spectrogram.maxFreqThroughWindows_99 = System.Math.Max(Spectrogram.maxFreqThroughWindows_99, Spectrogram.maxFreqInWindow_99);
                // testPort_PIN1.Write(false);
                // Spectrogram.maxFreqThroughWindows = System.Math.Max(Spectrogram.maxFreqThroughWindows, Spectrogram.maxFreqInWindow);
                // Spectrogram.maxFreqThroughWindows_high = System.Math.Max(Spectrogram.maxFreqThroughWindows_high, Spectrogram.maxFreqInWindow_high);
            }

            ///////////////////////////////////////// insertion sort
            /*int diffPhaseFlag = 0;

            // Next two lines commented by Dhrubo - features not used
            for (int i = 0; i < dState.detectionStepIndex; i++)  // because window 3 is window 0, the first 3 is thrown
            {
                if (diffPhaseFlag == 0 && dState.diffPhaseInWindowArr[i] > dState.diffPhaseInWindow)
                {
                    Array.Copy(dState.diffPhaseInWindowArr, i, dState.diffPhaseInWindowArr, i + 1, dState.detectionStepIndex - i);
                    dState.diffPhaseInWindowArr[i] = dState.diffPhaseInWindow;
                    diffPhaseFlag = 1;
                }
            }

            if (diffPhaseFlag == 0) dState.diffPhaseInWindowArr[dState.detectionStepIndex] = dState.diffPhaseInWindow;*/
        }
    }
}
