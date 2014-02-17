using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Hardware.Adapt
{        
    /// <summary>
    /// Emote LCD Interface class
    /// </summary>
    public class Accelerometer
    {
        public Accelerometer()
        {
        }

        /// <summary>
        /// Initializes the lcd hardware
        /// </summary>
        /// <returns>The result of lcd initialization: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_Init();

        /// <summary>
        /// Uninitializes the lcd hardware
        /// </summary>
        /// <returns>The result of lcd uninitialize: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern int ADAPT_Accel_GetX();

        /// <summary>
        /// Uninitializes the lcd hardware
        /// </summary>
        /// <returns>The result of lcd uninitialize: Success, Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern int ADAPT_Accel_GetY();       
    }
}
