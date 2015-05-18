using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.eMote.DotNow
{
    /// <summary>
    /// Delegate for ADC continuous mode mode callback method
    /// </summary>
    public delegate void AdcCallBack(long thresholdTime);

    /// <summary>
    /// Samraksh AnalogInput
    /// </summary>
    /// <remarks>Similar to Microsoft AnalogInput but with additional features</remarks>
    public class AnalogInput
    {
        /// <summary>
        /// ADC internal class 
        /// </summary>
        static ADCInternal _adcInternal;

        /// <summary>
        /// Callback method
        /// </summary>
        static AdcCallBack _myCallback;

        /// <summary>
        /// Initialize ADC
        /// </summary>
        /// 
        static public bool InitializeADC()
        {
            _adcInternal = new ADCInternal("ADCCallback", 1234, 0);
            return true;
        }

        private AnalogInput()
        {
            //AdcInternal = new ADCInternal("ADCCallback", 1234, 0);
        }

        /// <summary>
        /// Initialize ADC for specified channel
        /// </summary>
        /// <param name="channel">Channel</param>
        /// <returns>Result of operation</returns>
        static public int InitChannel(Cpu.AnalogChannel channel)
        {
            return ADCInternal.Init((int) channel);
        }

        /// <summary>
        /// Read two ADC channels synchronously
        /// </summary>
        /// <param name="data">Two-ushort array for return values</param>
        /// <returns>True iff read succeeds</returns>
        static public bool DualChannelRead(ushort[] data)
        {
            return ADCInternal.DualChannelRead(data);
        }

        /// <summary>
        /// Read a single ADC channel synchronously
        /// </summary>
        /// <param name="channel">Channel</param>
        /// <returns>Value read</returns>
        static public double Read(Cpu.AnalogChannel channel)
        {
            return ADCInternal.Read((int) channel);
        }

        /// <summary>
        /// Configure ADC batch mode with threshold
        /// </summary>
        /// <param name="sampleBuff">Buffer for samples</param>
        /// <param name="channel">Channel to use</param>
        /// <param name="numSamples">Number of samples to read before callback</param>
        /// <param name="samplingTime">Sampling time interval in microseconds</param>
        /// <param name="threshold">Threshold to apply</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <returns>True iff operation success</returns>
        static public bool ConfigureBatchModeWithThresholding(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint numSamples, uint samplingTime, uint threshold, AdcCallBack callback)
        {
            _myCallback = callback;
            var eventHandler = new NativeEventHandler(InternalCallback);
            _adcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureBatchModeWithThresholding(sampleBuff, (int) channel, numSamples, samplingTime, threshold) == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Configure ADC continouous mode with threshold
        /// </summary>
        /// <param name="sampleBuff">Buffer for samples</param>
        /// <param name="channel">Channel to use</param>
        /// <param name="numSamples">Number of samples to read before callback</param>
        /// <param name="samplingTime">Sampling time interval in microseconds</param>
        /// <param name="threshold">Threshold to apply</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <returns>True iff operation success</returns>
        static public bool ConfigureContinuousModeWithThresholding(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint numSamples, uint samplingTime, uint threshold, AdcCallBack callback)
        {
            _myCallback = callback;
            var eventHandler = new NativeEventHandler(InternalCallback);
            _adcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureContinuousModeWithThresholding(sampleBuff, (int)channel, numSamples, samplingTime, threshold) == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Configure ADC to read channels 1 and 2 in batch mode
        /// </summary>
        /// <param name="sampleBuff1">Sample buffer 1</param>
        /// <param name="sampleBuff2">Sample buffer 2</param>
        /// <param name="numSamples">Number of samples to read before callback</param>
        /// <param name="samplingTime">Sampling time interval in microseconds</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <returns>True if operation success</returns>
        static public bool ConfigureBatchModeDualChannel(ushort[] sampleBuff1, ushort[]  sampleBuff2, uint numSamples, uint samplingTime, AdcCallBack callback)
        {
            _myCallback = callback;
            var eventHandler = new NativeEventHandler(InternalCallback);
            _adcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureBatchModeDualChannel(sampleBuff1, sampleBuff2, numSamples, samplingTime) != DeviceStatus.Success)
            {
                return false;
            }
            else
                return true;
        }

        /// <summary>
        /// Configure ADC to read channels 1 and 2 in continuous mode
        /// </summary>
        /// <param name="sampleBuff1">Sample buffer 1</param>
        /// <param name="sampleBuff2">Sample buffer 2</param>
        /// <param name="numSamples">Number of samples to read before callback</param>
        /// <param name="samplingTime">Sampling time interval in microseconds</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <param name="debugMode">0 for normal operation, 1 for sending data to COM2 directly</param>
        /// <returns>True iff operation success</returns>
        static public bool ConfigureContinuousModeDualChannel(ushort[] sampleBuff1, ushort[] sampleBuff2, uint numSamples, uint samplingTime, AdcCallBack callback, uint debugMode)
        {
            _myCallback = callback;
            var eventHandler = new NativeEventHandler(InternalCallback);
            _adcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureContinuousModeDualChannel(sampleBuff1, sampleBuff2, numSamples, samplingTime, debugMode) != DeviceStatus.Success)
            {
                return false;
            }
            else
                return true;
        }



        /// <summary>
        /// Configure ADC batch mode
        /// </summary>
        /// <param name="sampleBuff">Sample buffer</param>
        /// <param name="channel">Channel to read</param>
        /// <param name="numSamples">Number of samples to read before callback</param>
        /// <param name="samplingTime">Sampling time interval in microseconds</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <returns>True iff operation success</returns>
        static public int ConfigureBatchMode(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint numSamples, uint samplingTime, AdcCallBack callback)
        {
            _myCallback = callback;
            var eventHandler = new NativeEventHandler(InternalCallback);
            _adcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureBatchMode(sampleBuff, (int)channel, numSamples, samplingTime) != DeviceStatus.Success)
                return 0;
            else
                return 1;
            
        }

        /// <summary>
        /// Configure ADC continuous mode
        /// </summary>
        /// <param name="sampleBuff">Sample buffer</param>
        /// <param name="channel">Channel to read</param>
        /// <param name="numSamples">Number of samples to read before callback</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <param name="samplingTime">Sampling time interval in microseconds</param>
        /// <returns>True iff operation success</returns>
        static public int ConfigureContinuousMode(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint numSamples, uint samplingTime, AdcCallBack callback)
        {
            _myCallback = callback;
            var eventHandler = new NativeEventHandler(InternalCallback);
            _adcInternal.OnInterrupt += eventHandler;

            ADCInternal.ConfigureContinuousMode(sampleBuff,(int) channel, numSamples, samplingTime);
            return 1;
        }

        /// <summary>
        /// Stop ADC sampling for continuous or batch mode
        /// </summary>
        /// <returns>Operation success</returns>
        static public int StopSampling()
        {
            return ADCInternal.StopSampling();
        }

        /// <summary>
        /// Native ADC driver callback
        /// </summary>
        /// <param name="data1">Parameter passed from native</param>
        /// <param name="data2">Parameter passed from native</param>
        /// <param name="time">Time of callback</param>
        static public void InternalCallback(uint data1, uint data2, DateTime time)
        {
            _myCallback((long)(((long)data1 << 32) | (long)data2));
        }
    }

    /// <summary>
    /// Time taken to sample in the adc driver
    /// </summary>
    public enum AdcSampleTime
    {
        // ReSharper disable CSharpWarnings::CS1591
        ADC_SampleTime_1_5_Cycles = 0,
        ADC_SampleTime_7_5_Cycles = 1,
        ADC_SampleTime_13_5_Cycles = 2,
        ADC_SampleTime_28_5_Cycles = 3,
        ADC_SampleTime_41_5_Cycles = 4,
        ADC_SampleTime_55_5_Cycles = 5,
        ADC_SampleTime_71_5_Cycles = 6,
        ADC_SampleTime_239_5_Cycles = 7,
// ReSharper restore CSharpWarnings::CS1591
    }

    /// <summary>
    /// ADCInternal interface to the native driver
    /// </summary>
    public class ADCInternal : NativeEventDispatcher
    {
        /// <summary>
        /// Specify the driver name for matching with the native eventdispatcher
        /// </summary>
        /// <param name="strDrvName"></param>
        /// <param name="drvData"></param>
        /// <param name="callbackCount"></param>
        public ADCInternal(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {

        }

        /// <summary>
        /// Initialize the ADC native driver
        /// </summary>
        /// <param name="channel">Specify the channel to be sampled</param>
        /// <returns>Returns the result of the init function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Init(int channel);

        /// <summary>
        /// Read the channel
        /// </summary>
        /// <param name="channel">Channel to read</param>
        /// <returns>Return value read</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public double Read(int channel);

        /// <summary>
        /// Configure batch mode
        /// </summary>
        /// <param name="sampleBuff">Pass the buffer to be filled</param>
        /// <param name="channel">Define the channel to be read</param>
        /// <param name="NumSamples">Defines the number of samples to be read before callback</param>
        /// <param name="SamplingTime">Defines the rate at which the adc should read channels</param>
        /// <returns>Return the result of the operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureBatchMode(ushort[] sampleBuff, int channel, uint NumSamples, uint SamplingTime);

        /// <summary>
        /// Configure the adc in continuous mode 
        /// </summary>
        /// <param name="SampleBuff">pass the buffer to be filled</param>
        /// <param name="channel">pass the channel to be sampled</param>
        /// <param name="NumSamples">specify the number of samples to be collected before callback</param>
        /// <param name="SamplingTime">specify the rate of sampling</param>
        /// <returns>Returns the result of this operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousMode(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousModeDualChannel(ushort[] SampleBuff1, ushort[] SampleBuff2, uint NumSamples, uint SamplingTime, uint debugMode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureBatchModeDualChannel(ushort[] SampleBuff1, ushort[] SampleBuff2, uint NumSamples, uint SamplingTime);


        /// <summary>
        /// Configure continuous mode adc sampling with a threshold timestamp 
        /// </summary>
        /// <param name="SampleBuff"></param>
        /// <param name="channel"></param>
        /// <param name="NumSamples"></param>
        /// <param name="SamplingTime"></param>
        /// <param name="threshold"></param>
        /// <returns></returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousModeWithThresholding(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime, uint threshold);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureBatchModeWithThresholding(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime, uint threshold);


        /// <summary>
        /// Read both the channel simulateously Channel 1 followed by Channel 2
        /// </summary>
        /// <param name="sample">specify the buffer to be filled</param>
        /// <returns>Return the result of this operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool DualChannelRead(ushort[] sample);

        /// <summary>
        /// Stop batch mode and continous mode sampling of the adc 
        /// </summary>
        /// <returns>Returns the result of this operation</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int StopSampling();


    }

}
