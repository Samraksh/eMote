using System;
using Microsoft.SPOT;
using System.Threading;  //Needed for threads 
using Microsoft.SPOT.Hardware; //Needed for accessing hardware
using Microsoft.SPOT.Messaging; // 

using Samraksh.SPOT.Emulator;

namespace ADCTestApp
{
    public class ADCTest
    {
        AdcSampleTime SampleTime = AdcSampleTime.ADC_SampleTime_1_5_Cycles;
        ADC Adc;
        const int NumberOfChannels =2;
        const int NumberOfSamples = 1000;
        static ushort SamplingTime = 1000; //micro seconds. 1KHz frequency.
        bool BatchMode = false;
        int BatchNumber = 0;
        ushort[] Adc_buffer= new ushort[NumberOfSamples*2];
        
        Timer SamplingTimer;
        int SampleNumber = 0;

        ADCTest()
        {
            Adc = new ADC(); // Create a ADC with 2 channels
        }

        void ProcessSingleValue(Object o)
        {
           // Debug.Print("Timer fired");
            ushort[] dataBuffer = new ushort[2];
            int bytes_read=0;
            try
            {
                bytes_read = Adc.GetData(dataBuffer, 0, 2);
            }
            catch (Exception e)
            {
                Debug.Print("Exception reading ADC: " +e.ToString());
            }
            if (bytes_read > 0 && (dataBuffer[0] !=0 || dataBuffer[1]!=0) )
            {
                //Debug.Print("I: " + dataBuffer[0].ToString() + ", Q: " + dataBuffer[1].ToString());
                //Debug.Print(SampleNumber + ", " + dataBuffer[0] + ", " + dataBuffer[1]);
                Adc_buffer[SampleNumber*2] = dataBuffer[0];
                Adc_buffer[SampleNumber*2+1] = dataBuffer[1];
                SampleNumber++;
                if (SampleNumber >= NumberOfSamples)
                {
                    StopADC();
                }
            }
            else
                Debug.Print("Error Reading ADC!!");
            
        }
        void StopADC()
        {
            if(! BatchMode ) SamplingTimer.Dispose();
            for (int i = 0; i < NumberOfSamples; i++)
            {
                Debug.Print(i + ", " + Adc_buffer[i*2] + ", " + Adc_buffer[i*2+1]);
            }
            Adc.Stop();
            Thread.CurrentThread.Abort();   
        }

        void StartSingleShotTest()
        {
            Debug.Print("Start Single Value Test");
            try
            {
                Adc.Init(SampleTime, NumberOfChannels);
                SamplingTimer = new Timer(ProcessSingleValue, null, SamplingTime, SamplingTime);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
                Thread.Sleep(5000);
            }
        }

        void StartBatchModeTest()
        {
            Debug.Print("Starting Batch Mode Test");
            try
            {
                BatchMode = true;
                Adc.Init(SampleTime, NumberOfChannels);
                Thread.Sleep(2000); //Let the ADC system initialize
                Adc.ConfigureBatchMode(Adc_buffer, 0, 2, NumberOfSamples, SamplingTime, BatchModeCallback);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }
        public void BatchModeCallback(bool Status)
        {
            if (Status) StopADC();
            else { Debug.Print("Error in ADC Batch Mode"); }
        }

        void StartContinuousModeTest()
        {
            Debug.Print("Starting Continuous Mode Test");
            try
            {
                BatchMode = true;
                Adc.Init(SampleTime, NumberOfChannels);
                Thread.Sleep(2000); //Let the ADC system initialize
                Adc.ConfigureContinuousMode(Adc_buffer, 0, 2, NumberOfSamples, SamplingTime, ContinuousModeCallback);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }
        public void ContinuousModeCallback(bool Status)
        {
            if (Status) {
                double[] Mean=new double[NumberOfChannels];
                for(int i=0; i< Adc_buffer.Length; i++){
                    Mean[i % NumberOfChannels]+=Adc_buffer[i];
                }
                for (int i=0; i< NumberOfChannels; i++){
                    Mean[i]/= (Adc_buffer.Length/NumberOfChannels);
                    Debug.Print("BatchNumber: "+ BatchNumber+ ", ADC[" + i + "] Mean:" + Mean[i]);
                    Mean[i] = 0;
                }
                BatchNumber++;
                if (BatchNumber == 10) Adc.Stop();
            }
            else { Debug.Print("Error in ADC Continous Mode"); }
        }

        public static void Main()
        {  
            ADCTest _test = new ADCTest();

            //Start only ONE of the following tests
            //_test.StartSingleShotTest();//Start a single value test 
           // _test.StartBatchModeTest();//Start a batch mode test
            _test.StartContinuousModeTest();//Start a Continuous mode test

            Thread.Sleep(Timeout.Infinite);
        }
    }
}
