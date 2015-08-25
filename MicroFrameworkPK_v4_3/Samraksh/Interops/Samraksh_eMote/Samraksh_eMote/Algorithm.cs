using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote
{
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
                SetDetectionParameters(7, 30, 2, 1);
            }
            
            //////////////////////////public properties and methods/////////////////////

            /// <summary>
            /// 
            /// </summary>
            ~RadarDetection()
            {
            }

            ///////////////////////////////////Internal methods/////////////////////////

            /// <summary>
            /// Radar data is given to the unwrap algorithm which looks for displacements above the threshold.
            /// </summary>
            /// <returns>Returns true if a detection was found, false if no detection was found.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public ushort DetectionCalculation(ushort[] bufferI, ushort[] bufferQ, ushort[] bufferUnwrap, Int32 numBytes, short[] acrTan);

            /// <summary>
            /// Radar data is given to the unwrap algorithm which looks for displacements above the threshold.
            /// </summary>
            /// <returns>Returns true if a detection was found, false if no detection was found.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public ushort DetectionCalculation(ushort[] bufferI, ushort[] bufferQ, Int32 numBytes, short[] acrTan);

            /// <summary>
            /// Sets detection parameters.
            /// </summary>
            /// <returns>Returns true if sucessful, false if failed.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool SetDetectionParameters(double thresholdSet, Int32 noiseRejection, UInt16 debugVal,  UInt16 version);
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
            /// Destroys the data in the data store
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
            extern public ushort DetectionCalculation(ushort[] bufferAcoustic, Int32 numBytes, double[] processingOutput, bool historyUpdateControl);

            /// <summary>
            /// Sets detection parameters.
            /// </summary>
            /// <returns>Returns true if sucessful, false if failed.</returns>
            [MethodImplAttribute(MethodImplOptions.InternalCall)]
            extern public bool SetDetectionParameters(Int32 Pa, Int32 Pm);
        }
    }
}
