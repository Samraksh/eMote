using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT
{
    public delegate void AdcCallBack(Object state);
    public class ADC
    {
        static ADCInternal AdcInternal;
        static AdcCallBack MyCallback;
        public ADC(){
            AdcInternal = new ADCInternal("Samraksh_ADC", 1234, 0);      
        }

        static public int Init(AdcSampleTime sampleTime, int num_channels)
        {
            return ADCInternal.Init((int)sampleTime);
        }

        static public int GetData(ushort[] currSample, uint startChannel, uint numChannels)
        {
            return ADCInternal.GetData(currSample, startChannel, numChannels);
        }

        static public int ConfigureBatchMode(ushort[] sampleBuff, uint StartChannel, uint NumChannels, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            ADCInternal.ConfigureBatchMode(sampleBuff, StartChannel, NumChannels, NumSamples, SamplingTime);
            return 1;
        }

        static public int ConfigureContinuousMode(ushort[] SampleBuff, uint StartChannel, uint NumChannels, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            ADCInternal.ConfigureContinuousMode(SampleBuff, StartChannel, NumChannels, NumSamples, SamplingTime);
            return 1;
        }
        static public void InternalCallback(uint data1, uint data2, DateTime time)
        {
        }
    }

    public enum AdcSampleTime
    {
    	ADC_SampleTime_1_5_Cycles = 0,
    	ADC_SampleTime_7_5_Cycles = 1,
    	ADC_SampleTime_13_5_Cycles = 2,
    	ADC_SampleTime_28_5_Cycles = 3,
    	ADC_SampleTime_41_5_Cycles = 4,
    	ADC_SampleTime_55_5_Cycles = 5,
    	ADC_SampleTime_71_5_Cycles = 6,
    	ADC_SampleTime_239_5_Cycles = 7,	
    }

    public class ADCInternal: NativeEventDispatcher
    {
         public ADCInternal(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {

        }              

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Init(int adc_num_cycles);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int GetData(ushort[] currSample, uint startChannel, uint numChannels);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int ConfigureBatchMode(ushort[] sampleBuff, uint StartChannel, uint NumChannels, uint NumSamples, uint SamplingTime);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int ConfigureContinuousMode(ushort[] SampleBuff, uint StartChannel, uint NumChannels, uint NumSamples, uint SamplingTime );
 
    }
}
