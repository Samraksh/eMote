using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Hardware.EmoteDotNow
{
    public delegate void AdcCallBack(bool state);
    public class AnalogInput
    {
        static ADCInternal AdcInternal;
        static AdcCallBack MyCallback;
        public AnalogInput()
        {
            AdcInternal = new ADCInternal("ADCCallback", 1234, 0);
        }

        static public int Init(Cpu.AnalogChannel channel)
        {
            return ADCInternal.Init((int) channel);
        }

        static public double Read(Cpu.AnalogChannel channel)
        {
            return ADCInternal.Read((int) channel);
        }

        static public int ConfigureBatchMode(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            ADCInternal.ConfigureBatchMode(sampleBuff,(int) channel, NumSamples, SamplingTime);
            return 1;
        }

        static public int ConfigureContinuousMode(ushort[] SampleBuff, Cpu.AnalogChannel channel, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            ADCInternal.ConfigureContinuousMode(SampleBuff,(int) channel, NumSamples, SamplingTime);
            return 1;
        }
        static public void InternalCallback(uint data1, uint data2, DateTime time)
        {
            MyCallback(true);
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

    public class ADCInternal : NativeEventDispatcher
    {
        public ADCInternal(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {

        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Init(int channel);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public double Read(int channel);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int ConfigureBatchMode(ushort[] sampleBuff, int channel, uint NumSamples, uint SamplingTime);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int ConfigureContinuousMode(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime);

    }

}
