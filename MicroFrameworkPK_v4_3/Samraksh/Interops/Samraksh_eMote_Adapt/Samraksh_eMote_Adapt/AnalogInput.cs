using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.eMote.Adapt
{
    /// <summary>
    /// Defines a delegate for the continuous mode of the ADC callback function
    /// </summary>
    /// <param name="time"></param>
    public delegate void AdcCallBack(long time);

    /// <summary>
    /// AnalogInput class similar to Microsoft AnalogInput
    /// </summary>
    public class AnalogInput
    {
        /// <summary>
        /// Represents the ADC internal class 
        /// </summary>
        static ADCInternal AdcInternal;

        /// <summary>
        /// Represents the callback function
        /// </summary>
        static AdcCallBack MyCallback;

        private AnalogInput()
        {
        }

        /// <summary>
        /// Initializes the ADC hardware
        /// </summary>
        /// <returns>The result of ADC initialization: 0-Success, 1-Fail.</returns>
        static public int Initialize()
        {
            AdcInternal = new ADCInternal("ADCCallback", 1234, 0);

            return ADCInternal.Initialize();
        }

        /// <summary>
        /// Read a single channel
        /// </summary>
        /// <param name="channel">Channel to be read</param>
        /// <returns>Returns the result of the read operation</returns>
        static public double Read(AdcChannel channel)
        {
            return ADCInternal.Read(channel);
        }

        /*static public bool ConfigureContinuousModeWithThresholding(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint NumSamples, uint SamplingTime, uint threshold, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureContinuousModeWithThresholding(sampleBuff, (int)channel, NumSamples, SamplingTime, threshold) == DeviceStatus.Success)
                return true;
            else
                return false;
        }*/

        /// <summary>
        /// Read the ADC in continous mode and keep generating samples of ADC forever
        /// </summary>
        /// <param name="SampleBuff">Buffer passed to the driver</param>
        /// <param name="channels">Bitmask of the channels to be read.</param>
        /// <param name="NumSamples">Specify the number of samples to be read before callback</param>
        /// <param name="SamplingTime">Specifies the sampling time</param>
        /// <param name="Callback">Specify the callback function</param>
        /// <returns></returns>
        static public int ConfigureContinuousMode(ushort[] SampleBuff, AdcChannel channels, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            ADCInternal.ConfigureContinuousMode(SampleBuff, channels, NumSamples, SamplingTime);
            return 1;
        }

        /// <summary>
        /// Used to stop sampling in either continuous or batch mode 
        /// </summary>
        /// <returns>Returns the result of this operation</returns>
        static public int StopSampling()
        {
            return ADCInternal.StopSampling();
        }

        /// <summary>
        /// Get the sample rate maximum of the ADC
        /// </summary>
        /// <returns>Returns the maximum sample rate of the ADC.</returns>
        static public UInt32 GetMaxSampleRate()
        {
            return ADCInternal.GetMaxSampleRate();
        }

        /// <summary>
        /// Uninitializes the ADC hardware
        /// </summary>
        /// <returns>The result of ADC uninitialization: 0-Success, 1-Fail.</returns>
        static public int Uninitialize()
        {
            return ADCInternal.Uninitialize();
        }

        /// <summary>
        /// Internal Callback called by the ADC driver from native
        /// </summary>
        /// <param name="data1">Upper 32-bits of return data</param>
        /// <param name="data2">Lower 32-bits of return data</param>
        /// <param name="time">Time of the callback.</param>
        static public void InternalCallback(uint data1, uint data2, DateTime time)
        {
            MyCallback((long)(((long)data1 << 32) | (long)data2));
        }
    }

    /// <summary>
    /// Time taken to sample in the ADC driver
    /// </summary>
    public enum AdcChannel
    {
        ADC_Channel1 = 0x01,
        ADC_Channel2 = 0x02,
        ADC_Channel3 = 0x04,
        ADC_Channel4 = 0x08,
        ADC_Channel5 = 0x10,
        ADC_Channel6 = 0x20,
        ADC_Channel7 = 0x40,
        ADC_Channel8 = 0x80
    }

    /// <summary>
    /// Time taken to sample in the ADC driver
    /// </summary>
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

    /// <summary>
    /// ADCInternal interface to the native driver
    /// </summary>
    public class ADCInternal : NativeEventDispatcher
    {
        /// <summary>
        /// Specifies the driver name for matching with the native eventdispatcher
        /// </summary>
        /// <param name="strDrvName"></param>
        /// <param name="drvData"></param>
        /// <param name="callbackCount"></param>
        public ADCInternal(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {
        }

        /// <summary>
        /// Initializes the ADC native driver
        /// </summary>
        /// <param name="channel">Specify the channel to be sampled</param>
        /// <returns>Returns the result of the init function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Initialize();

        /// <summary>
        /// Read the channel
        /// </summary>
        /// <param name="channel">Specify the channel to be read</param>
        /// <returns>Return the read value</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public UInt32 Read(AdcChannel channel);

        /// <summary>
        /// Configure the ADC in continuous mode 
        /// </summary>
        /// <param name="SampleBuff">The buffer to be filled</param>
        /// <param name="channels">Bitmask of the channels to be read.</param>
        /// <param name="NumSamples">Specify the number of samples to be collected before callback</param>
        /// <param name="SamplingTime">Specify the rate of sampling</param>
        /// <returns>Returns the result of this operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousMode(ushort[] SampleBuff, AdcChannel channels, uint NumSamples, uint SamplingTime);

        /*/// <summary>
        /// Configure continuous mode ADC sampling with a threshold timestamp 
        /// </summary>
        /// <param name="SampleBuff"></param>
        /// <param name="channels">Bitmask of the channels to be read.</param>
        /// <param name="NumSamples"></param>
        /// <param name="SamplingTime"></param>
        /// <param name="threshold"></param>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousModeWithThresholding(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime, uint threshold);*/

        /// <summary>
        /// Stop batch mode and continous mode sampling of the ADC 
        /// </summary>
        /// <returns>Returns the result of this operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int StopSampling();

        /// <summary>
        /// Get the sample rate maximum of the ADC
        /// </summary>
        /// <returns>Returns the maximum sample rate of the ADC.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public UInt32 GetMaxSampleRate();

        /// <summary>
        /// Uninitializes the ADC native driver
        /// </summary>
        /// <returns>Returns the result of the uninitialization function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Uninitialize();
    }
}
