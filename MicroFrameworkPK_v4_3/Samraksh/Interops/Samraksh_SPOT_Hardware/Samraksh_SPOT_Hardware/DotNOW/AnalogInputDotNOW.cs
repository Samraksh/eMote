/**********************************************************************************
 * This is a duplicate of AnalogInput_Base.cs. 
 * This is in namespace Samraksh.SPOT.Hardware.EmoteDotNow.
 * Is included to retain backwards compatibility.
**********************************************************************************/
#if DotNOW

using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Hardware.DotNow
{
    /// <summary>
    /// Defines a delegate for the continuous mode of the callback function
    /// </summary>
    /// <param name="thresholdTime"></param>
    public delegate void AdcCallBack(long thresholdTime);

    /// <summary>
    /// AnalogInput class similar to Microsoft AnalogInput but with additional features
    /// </summary>
    public class AnalogInput
    {
        /// <summary>
        /// Represents the adc internal class 
        /// </summary>
        static ADCInternal AdcInternal;

        /// <summary>
        /// Represents the callback function
        /// </summary>
        static AdcCallBack MyCallback;

        /// <summary>
        /// Initializes the adc dll 
        /// </summary>
        /// 
        static public bool InitializeADC()
        {
            AdcInternal = new ADCInternal("ADCCallback", 1234, 0);

            return true;
        }

        private AnalogInput()
        {
            //AdcInternal = new ADCInternal("ADCCallback", 1234, 0);
        }

        /// <summary>
        /// Initializes the adc hardware based on channel passed
        /// </summary>
        /// <param name="channel">channel 1 or 2</param>
        /// <returns>Returns the result of the initialization operation</returns>
        static public int InitChannel(Cpu.AnalogChannel channel)
        {
            return ADCInternal.Init((int) channel);
        }

        /// <summary>
        /// Read both channel synchrnously
        /// </summary>
        /// <param name="data">array of size 2 bytes passed to the adc driver to be filled</param>
        /// <returns>Returns the result of this operation</returns>
        static public bool DualChannelRead(ushort[] data)
        {
            return ADCInternal.DualChannelRead(data);
        }

        /// <summary>
        /// Read a single channel
        /// </summary>
        /// <param name="channel">Channel to be read</param>
        /// <returns>Returns the result of the read operation</returns>
        static public double Read(Cpu.AnalogChannel channel)
        {
            return ADCInternal.Read((int) channel);
        }


        static public bool ConfigureBatchModeWithThresholding(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint NumSamples, uint SamplingTime, uint threshold, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureBatchModeWithThresholding(sampleBuff, (int) channel, NumSamples, SamplingTime, threshold) == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        static public bool ConfigureContinuousModeWithThresholding(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint NumSamples, uint SamplingTime, uint threshold, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureContinuousModeWithThresholding(sampleBuff, (int)channel, NumSamples, SamplingTime, threshold) == DeviceStatus.Success)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Read the adc channels 1 and 2 in batch mode
        /// </summary>
        /// <param name="sampleBuff">Sample buffer to be filled</param>
        /// <param name="NumSamples">Number of samples before callback</param>
        /// <param name="SamplingTime">Sampling frequency</param>
        /// <param name="Callback">Callback funtion to be called</param>
        /// <returns></returns>
        static public bool ConfigureBatchModeDualChannel(ushort[] sampleBuff1, ushort[]  sampleBuff2, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureBatchModeDualChannel(sampleBuff1, sampleBuff2, NumSamples, SamplingTime) != DeviceStatus.Success)
            {
                return false;
            }
            else
                return true;
        }

        static public bool ConfigureContinuousModeDualChannel(ushort[] sampleBuff1, ushort[] sampleBuff2, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureContinuousModeDualChannel(sampleBuff1, sampleBuff2, NumSamples, SamplingTime) != DeviceStatus.Success)
            {
                return false;
            }
            else
                return true;
        }



        /// <summary>
        /// Read the ADC in batch mode and collect the specified number of samples before stopping
        /// </summary>
        /// <param name="sampleBuff">Buffer passed to the driver to be filled</param>
        /// <param name="channel">Defne the channel to be read in batch mode</param>
        /// <param name="NumSamples">Defines the number of samples to be read</param>
        /// <param name="SamplingTime">Defines the sample time</param>
        /// <param name="Callback">Defines the callback to be called when the samples are ready</param>
        /// <returns></returns>
        static public int ConfigureBatchMode(ushort[] sampleBuff, Cpu.AnalogChannel channel, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            if (ADCInternal.ConfigureBatchMode(sampleBuff, (int)channel, NumSamples, SamplingTime) != DeviceStatus.Success)
                return 0;
            else
                return 1;
            
        }

        /// <summary>
        /// Read the adc in continous mode and keep generating samples of adc forever
        /// </summary>
        /// <param name="SampleBuff">Buffer passed to the driver</param>
        /// <param name="channel">Specify the channel to be scanned</param>
        /// <param name="NumSamples">Specify the number of samples to be read before callback</param>
        /// <param name="SamplingTime">Specifies the sampling time</param>
        /// <param name="Callback">Specify the callback function</param>
        /// <returns></returns>
        static public int ConfigureContinuousMode(ushort[] SampleBuff, Cpu.AnalogChannel channel, uint NumSamples, uint SamplingTime, AdcCallBack Callback)
        {
            MyCallback = Callback;
            NativeEventHandler eventHandler = new NativeEventHandler(InternalCallback);
            AdcInternal.OnInterrupt += eventHandler;

            ADCInternal.ConfigureContinuousMode(SampleBuff,(int) channel, NumSamples, SamplingTime);
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
        /// Internal Callback called by the adc driver from native
        /// </summary>
        /// <param name="data1">Parameter passed from native</param>
        /// <param name="data2"></param>
        /// <param name="time"></param>
        static public void InternalCallback(uint data1, uint data2, DateTime time)
        {
            MyCallback((long)(((long)data1 << 32) | (long)data2));
        }
    }

    /// <summary>
    /// Time taken to sample in the adc driver
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
        /// Initializes the adc native driver
        /// </summary>
        /// <param name="channel">Specify the channel to be sampled</param>
        /// <returns>Returns the result of the init function</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public int Init(int channel);

        /// <summary>
        /// Read the channel
        /// </summary>
        /// <param name="channel">Specify the channel to be read</param>
        /// <returns>Return the read value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public double Read(int channel);

        /// <summary>
        /// Configure the batch mode
        /// </summary>
        /// <param name="sampleBuff">Pass the buffer to be filled</param>
        /// <param name="channel">Define the channel to be read</param>
        /// <param name="NumSamples">Defines the number of samples to be read before callback</param>
        /// <param name="SamplingTime">Defines the rate at which the adc should read channels</param>
        /// <returns>Return the result of the operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureBatchMode(ushort[] sampleBuff, int channel, uint NumSamples, uint SamplingTime);

        /// <summary>
        /// Configure the adc in continuous mode 
        /// </summary>
        /// <param name="SampleBuff">pass the buffer to be filled</param>
        /// <param name="channel">pass the channel to be sampled</param>
        /// <param name="NumSamples">specify the number of samples to be collected before callback</param>
        /// <param name="SamplingTime">specify the rate of sampling</param>
        /// <returns>Returns the result of this operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousMode(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousModeDualChannel(ushort[] SampleBuff1, ushort[] SampleBuff2, uint NumSamples, uint SamplingTime);

        [MethodImpl(MethodImplOptions.InternalCall)]
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
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureContinuousModeWithThresholding(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime, uint threshold);

        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public DeviceStatus ConfigureBatchModeWithThresholding(ushort[] SampleBuff, int channel, uint NumSamples, uint SamplingTime, uint threshold);


        /// <summary>
        /// Read both the channel simulateously Channel 1 followed by Channel 2
        /// </summary>
        /// <param name="sample">specify the buffer to be filled</param>
        /// <returns>Return the result of this operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public bool DualChannelRead(ushort[] sample);

        /// <summary>
        /// Stop batch mode and continous mode sampling of the adc 
        /// </summary>
        /// <returns>Returns the result of this operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        extern static public int StopSampling();
    }
}

#endif