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
        /// Initializes the accelerometer hardware
        /// </summary>
        /// <returns>The result of accelerometer initialization: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_Init();

        /// <summary>
        /// Resets the accelerometer hardware
        /// </summary>
        /// <returns>The result of accelerometer reset: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_Reset();

        /// <summary>
        /// Runs the accelerometer self-test
        /// </summary>
        /// <returns>The result of accelerometer self-test: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_SelfTest();

        /// <summary>
        /// Gets the accelerometer X value
        /// </summary>
        /// <returns>The accelerometer X value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 ADAPT_Accel_GetX();

        /// <summary>
        /// Gets the accelerometer Y value
        /// </summary>
        /// <returns>The accelerometer Y value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 ADAPT_Accel_GetY();

        /// <summary>
        /// Gets the accelerometer Z value
        /// </summary>
        /// <returns>The accelerometer Z value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 ADAPT_Accel_GetZ();

        /// <summary>
        /// Gets the accelerometer temperature
        /// </summary>
        /// <returns>The accelerometer temperature (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 ADAPT_Accel_GetTemperature();
   
        /// <summary>
        /// Reads a register
        /// </summary>
        /// <param name="reg">The register to be read.</param>
        /// <returns>The accelerometer register return value (Byte)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Byte ADAPT_Accel_Raw_Read(Byte reg);

        /// <summary>
        /// Writes a register
        /// </summary>
        /// <param name="reg">The register to be written.</param>
        /// <param name="data">The register value to be written.</param>
        /// <returns>The result of accelerometer register write: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_Raw_Write(Byte reg, Byte data);

        /// <summary>
        /// Accelerometer advanced configuration
        /// </summary>
        /// <param name="configuration">The configuration to be used.</param>
        /// <returns>The result of accelerometer advanced configuration: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_advconfig(UInt32 config);

        /// <summary>
        /// Accelerometer set power
        /// </summary>
        /// <param name="power">The power to be used.</param>
        /// <returns>The result of accelerometer set power: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_SetPowerMode(UInt32 power);

        /// <summary>
        /// Accelerometer set bandwidth
        /// </summary>
        /// <param name="bandwidth">The bandwidth to be used.</param>
        /// <returns>The result of accelerometer set bandwidth: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_SetBandwidth(UInt32 bandwidth);

        /// <summary>
        /// Accelerometer set range
        /// </summary>
        /// <param name="range">The range to be used.</param>
        /// <returns>The result of accelerometer set range: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_SetBandwidth(Byte range);

        /// <summary>
        /// Get all three axis
        /// </summary>
        /// <param name="data">Array of size 3 passed to the driver to be filled.</param>
        /// <returns>The result of accelerometer get all: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ADAPT_Accel_GetAll(Int16[] data);
    }
}