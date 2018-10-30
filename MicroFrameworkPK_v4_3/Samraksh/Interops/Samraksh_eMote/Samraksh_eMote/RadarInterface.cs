using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.eMote
{
    /// <summary>
    /// Delegate for Radar callback method
    /// </summary>
    public delegate void RadarCallBack(long time);



    public class RadarInterface
    {
        /// <summary>
        /// ADC internal class 
        /// </summary>
        static RadarInternal _radarInternal;

        /// <summary>
        /// Callback method
        /// </summary>
        static RadarCallBack _myCallback;
        /// <summary>
        /// 
        /// </summary>
        public RadarInterface()
        {
            _radarInternal = new RadarInternal("RadarCallback", 1234, 0);
        }

        //////////////////////////public properties and methods/////////////////////

        /// <summary>
        /// 
        /// </summary>
        ~RadarInterface()
        {
            _radarInternal = null;
            TurnOff();
            RadarInternal.Uninit();
        }

        /// <summary> Turns on the Radar </summary>
        /// <param name="sampleBuffI">Buffer for samples</param>
        /// <param name="sampleBuff1">Buffer for samples</param>
        /// <param name="numSamples">Number of samples</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <returns>True if operation success</returns>
        /// <returns>The result of turning on the radar: Success, Fail</returns>
        public bool ConfigureFPGADetection(ushort[] sampleBuffI, ushort[] sampleBuffQ, uint numSamples, RadarCallBack callback)
        {
            _myCallback = callback;
            var eventHandler = new NativeEventHandler(InternalCallback);
            _radarInternal.OnInterrupt += eventHandler;

            if (_radarInternal.ConfigureFPGADetectionPrivate(sampleBuffI, sampleBuffQ, numSamples))
                return true;
            else
                return false;
        }

        /// <summary>
        /// Request whether the last window's displacement was over the threshold
        /// </summary>
        /// <returns>Returns true if last window's displacement was over the threshold and false if not.</returns>
        public bool GetWindowOverThreshold()
        {
            return _radarInternal.GetWindowOverThreshold();
        }

        /// <summary>
        /// Request whether the current detection has finished
        /// </summary>
        /// <returns>Returns true if the current detection is finished and false if not.</returns>
        public bool CurrentDetectionFinished()
        {
            return _radarInternal.CurrentDetectionFinished();
        }

        /// <summary>
        /// Request the window's displacement.
        /// </summary>
        /// <returns>Returns the window's displacement.</returns>
        public int GetDisplacement()
        {
            return _radarInternal.GetDisplacement();
        }


        /*/// <summary>
        /// Request the window's net displacement
        /// </summary>
        /// <returns>Returns the window's net displacement.</returns>
        public int GetNetDisplacement(SAMPLE_WINDOW_PORTION portion)
        {
            return _radarInternal.GetNetDisplacement(portion);
        }
        /// <summary>
        /// Request the window's absolute displacement.
        /// </summary>
        /// <returns>Returns the window's absolute displacement.</returns>
        public int GetAbsoluteDisplacement(SAMPLE_WINDOW_PORTION portion)
        {
            return _radarInternal.GetAbsoluteDisplacement(portion);
        }
        /// <summary>
        /// Request the window's displacement range.
        /// </summary>
        /// <returns>Returns the window's displacement range.</returns>
        public int GetDisplacementRange(SAMPLE_WINDOW_PORTION portion)
        {
            return _radarInternal.GetDisplacementRange(portion);
        }*/

        /// <summary>
        /// Inform firmware of status of data processing
        /// </summary>
        /// <returns>Nothing returned</returns>
        public void SetProcessingInProgress(bool status)
        {
            _radarInternal.SetProcessingInProgress(status);
        }
        


       /* /// <summary>
        /// Request the window's count of samples over classifierTargetFilter parameter.
        /// </summary>
        /// <returns>Returns the window's count of samples over target.</returns>
        public int GetCountOverTarget()
        {
            //var x = _radarInternal.GetCountOverTarget();
            //Debug.Print("C#RadarInterface::GetCountOverTarget()" + x);
            return _radarInternal.GetCountOverTarget();
        }*/

        /// <summary>
        /// Native Radar driver callback
        /// </summary>
        /// <param name="data1">Parameter passed from native</param>
        /// <param name="data2">Parameter passed from native</param>
        /// <param name="time">Time of callback</param>
        static public void InternalCallback(uint data1, uint data2, DateTime time)
        {
            _myCallback((long)(((long)data1 << 32) | (long)data2));
        }
        ///////////////////////////////////Internal methods/////////////////////////



        /// <summary>
        /// Turns on the Radar
        /// </summary>
        /// <returns>The result of turning on the radar: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool TurnOn();

        /// <summary>
        /// Turns off the Radar
        /// </summary>
        /// <returns>The result of turning off the radar: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool TurnOff();

        /// <summary>
        /// Blinds the Radar
        /// </summary>
        /// <returns>The result of blinding the radar: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Blind();
    }

    /// <summary>
    /// ADCInternal interface to the native driver
    /// </summary>
    class RadarInternal : NativeEventDispatcher
    {
        /// <summary>
        /// Specify the driver name for matching with the native eventdispatcher
        /// </summary>
        /// <param name="strDrvName"></param>
        /// <param name="drvData"></param>
        /// <param name="callbackCount"></param>
        public RadarInternal(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {

        }

        /// <summary>
        /// Initialize the Radar native driver
        /// </summary>
        /// <param name="channel">Specify the channel to be sampled</param>
        /// <returns>Returns the result of the init function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Init(int channel);

        /// <summary>
        /// Uninitialize the Radar native driver
        /// </summary>
        /// <returns>Returns the result of the uninit function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public bool Uninit();

        /// <summary>
        /// Turns on the Radar
        /// </summary>
        /// <param name="sampleBuffI">Buffer for samples</param>
        /// <param name="sampleBuff1">Buffer for samples</param>
        /// <param name="numSamples">Number of samples</param>
        /// <param name="callback">Method to call when numSamples collected</param>
        /// <returns>True if operation success</returns>
        /// <returns>The result of turning on the radar: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ConfigureFPGADetectionPrivate(ushort[] sampleBuffI, ushort[] sampleBuffQ, uint numSamples);

        /// <summary>
        /// Request whether the last window's displacement was over the threshold
        /// </summary>
        /// <returns>Returns true if last window's displacement was over the threshold and false if not.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool GetWindowOverThreshold();

        /// <summary>
        /// Request whether the current detection has finished
        /// </summary>
        /// <returns>Returns true if the current detection is finished and false if not.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern bool CurrentDetectionFinished();


        /// <summary>
        /// Request the total displacement over the window
        /// </summary>
        /// <returns>Returns the window's displacement.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetDisplacement();
        /// <summary>
        /// Request the window's displacement range.
        /// </summary>
        /// <returns>Returns the window's displacement range.</returns>
        /// 
       /*/// <summary>
        /// Request the window's net displacement
        /// </summary>
        /// <returns>Returns the window's net displacement.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetNetDisplacement(SAMPLE_WINDOW_PORTION portion);


        /// <summary>
        /// Request the window's absolute displacement.
        /// </summary>
        /// <returns>Returns the window's absolute displacement.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetAbsoluteDisplacement(SAMPLE_WINDOW_PORTION portion);
        /// <summary>
        /// Request the window's displacement range.
        /// </summary>
        /// <returns>Returns the window's displacement range.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetDisplacementRange(SAMPLE_WINDOW_PORTION portion);

        /// <summary>
        /// Request the window's count of samples over classifierTargetFilter parameter.
        /// </summary>
        /// <returns>Returns the window's count of samples over target.</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern int GetCountOverTarget();
        */
        /// <summary>
        /// Inform firmware of status of data processing
        /// </summary>
        /// <returns>Nothing returned</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern void SetProcessingInProgress(bool status);
    }


}
