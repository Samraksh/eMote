using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote
{
    /// <summary>
    /// Options for setting the IQ rejection parameter for the Radar Detection class  
    /// </summary>
    public enum RADAR_NOISE_CONTROL
    {
        /// <summary>The noise rejection parameter is fixed at noiseRejection.</summary>
        FIXED_NOISE_REJECTION,
        /// <summary>The noise rejection parameter will be the sum of the background noise plus the parameter noiseRejection.</summary>
        SCALING_NOISE_REJECTION_ADD,
        /// <summary>The noise rejection parameter will be the product of the background noise times the parameter noiseRejection.</summary>
        SCALING_NOISE_REJECTION_MULTIPLY,
        /// <summary>The noise rejection parameter will adjusted untl we reach the specified background noise of the noiseRejection parameter.</summary>
        SCALING_NOISE_REJECTION_TARGET
    };

    /// <summary>
    /// Options for requesting the IQ rejection parameter for the Radar Detection class  
    /// </summary>
    public enum RADAR_NOISE_REQUEST
    {
        /// <summary>The noise rejection value set to zero</summary>
        IQ_REJECTION_ZERO,
        /// <summary>The noise rejection value set to max</summary>
        IQ_REJECTION_MAX,
        /// <summary>The noise rejection value currently used by the system</summary>
        IQ_REJECTION_CURRENTLY_USED,
        /// <summary>The current radar Q noise.</summary>
        RADAR_Q
    };

    public class Algorithm
    {
        /// <summary>
        /// Radar Detection class  
        /// </summary>
        public class RadarDetection
        {            
            /// <summary>
            /// 
            /// </summary>
            public RadarDetection() {
                Initialize();
                SetDetectionParameters(RADAR_NOISE_CONTROL.SCALING_NOISE_REJECTION_TARGET, 1.5, 1, 2, 3, 0, 1);
            }
            
            //////////////////////////public properties and methods/////////////////////

            /// <summary>
            /// 
            /// </summary>
            ~RadarDetection()
            {
                Uninitialize();
            }

            ///////////////////////////////////Internal methods/////////////////////////

            /// <summary>
            /// Initializes the RadarDetection software
            /// </summary>
            /// <returns>The result of radar detection initialization: Success, Fail</returns>
            [MethodImpl(MethodImplOptions.InternalCall)]
            private extern bool Initialize();

            /// <summary>
            /// Uninitializes the RadarDetection software
            /// </summary>
            /// <returns>The result of radar detection initialization: Success, Fail</returns>
            [MethodImpl(MethodImplOptions.InternalCall)]
            private extern bool Uninitialize();

            /// <summary>
            /// Radar data is given to the unwrap algorithm which looks for displacements above the threshold.
            /// </summary>
            /// <returns>Returns true if a detection was found, false if no detection was found.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool DetectionCalculation(ushort[] bufferI, ushort[] bufferQ, ushort[] bufferUnwrap, Int32 numBytes);

            /// <summary>
            /// Radar data is given to the unwrap algorithm which looks for displacements above the threshold.
            /// </summary>
            /// <returns>Returns true if a detection was found, false if no detection was found.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool DetectionCalculation(ushort[] bufferI, ushort[] bufferQ, Int32 numBytes);

            /// <summary>
            /// Sets detection parameters.
            /// </summary>
            /// <returns>Returns true if detection made, false if there was no detection.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool SetDetectionParameters(RADAR_NOISE_CONTROL radarNoiseCtrl, double thresholdSet, double noiseRejection, ushort M, ushort N, UInt16 debugVal, UInt16 version);

            /// <summary>
            /// Request the background noise level
            /// </summary>
            /// <returns>Return the current background noise level.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public int GetBackgroundNoiseLevel(RADAR_NOISE_REQUEST iqRequestType);

            /// <summary>
            /// Reset background noise tracking
            /// </summary>
            /// <returns>Returns true if reset occured, false if there was no reset.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool ResetBackgroundNoiseTracking();

            /// <summary>
            /// Request the IQ rejection currently used
            /// </summary>
            /// <returns>Return the current IQ rejection level.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public int GetIQRejectionLevel();

            /// <summary>
            /// Request the last unwrap value
            /// </summary>
            /// <returns>Returns the last unwrap value.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public int GetLastUnwrap(RADAR_NOISE_REQUEST iqRequestType);

            /// <summary>
            /// Request whether the last window's displacement was over the threshold
            /// </summary>
            /// <returns>Returns true if last window's displacement was over the threshold and false if not.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool GetWindowOverThreshold();

            /// <summary>
            /// Request the last window's displacement at the mid-point of the window
            /// </summary>
            /// <returns>Returns the last window's displacement at the mid-point of the window.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public int GetMidWindowUnwrap();
        }

        /// <summary>
        /// Classifier class  
        /// </summary>
        public class DecisionFunction
        {
            /// <summary>
            /// Classifier class
            /// </summary>
            public DecisionFunction()
            {
                
            }

            //////////////////////////public properties and methods/////////////////////

            /// <summary>
            /// 
            /// </summary>
            ~DecisionFunction()
            {
            }

            ///////////////////////////////////Internal methods/////////////////////////

            /// <summary>
            /// The feature vector is normailized
            /// </summary>
            /// <returns>A pointer to the nomralized vectors is returned.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public void NormalizeFeatureVector(float[] featureVector, int[] normalizedVector);

            /// <summary>
            /// A decision is made based on the support vecorts
            /// </summary>
            /// <returns>A decision is returned.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public float Decide(int[] normalizedVector);
        }

        /// <summary>
        /// Acoustic Detection class  
        /// </summary>
        public class AcousticDetection
        {
            /// <summary>
            /// 
            /// </summary>
            public AcousticDetection()
            {
                SetDetectionParameters(1, 1);
            }

            //////////////////////////public properties and methods/////////////////////

            /// <summary>
            /// 
            /// </summary>
            ~AcousticDetection()
            {
            }

            ///////////////////////////////////Internal methods/////////////////////////

            /// <summary>
            /// Acoustic data is given to the detection algorithm which looks for detections.
            /// </summary>
            /// <returns>Returns true if a detection was found, false if no detection was found.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool DetectionCalculation(ushort[] bufferAcoustic, Int32 numBytes, double[] processingOutput, bool historyUpdateControl);

            /// <summary>
            /// Sets detection parameters.
            /// </summary>
            /// <returns>Returns true if sucessful, false if failed.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool SetDetectionParameters(Int32 Pa, Int32 Pm);
        }
    }
}
