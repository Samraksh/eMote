using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.Adapt
{        
    /// <summary>
    /// Adapt Accelerometer Interface class
    /// </summary>
    public class Accelerometer
    {
        public Accelerometer()
        {
            Initialize();
        }

        /// <summary>
        /// Initializes the accelerometer hardware
        /// </summary>
        /// <returns>The result of accelerometer initialization: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();

        /// <summary>
        /// Resets the accelerometer hardware
        /// </summary>
        /// <returns>The result of accelerometer reset: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Reset();

        /// <summary>
        /// Runs the accelerometer self-test
        /// </summary>
        /// <returns>The result of accelerometer self-test: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SelfTest();

        /// <summary>
        /// Gets the accelerometer X value
        /// </summary>
        /// <returns>The accelerometer X value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetX();

        /// <summary>
        /// Gets the accelerometer Y value
        /// </summary>
        /// <returns>The accelerometer Y value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetY();

        /// <summary>
        /// Gets the accelerometer Z value
        /// </summary>
        /// <returns>The accelerometer Z value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetZ();

        /// <summary>
        /// Gets the accelerometer temperature
        /// </summary>
        /// <returns>The accelerometer temperature (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetTemperature();

        /// <summary>
        /// Reads a register
        /// </summary>
        /// <param name="reg">The register to be read.</param>
        /// <returns>The  register return value (Byte)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Byte ReadRegister(Byte reg);

        /// <summary>
        /// Writes a register
        /// </summary>
        /// <param name="reg">The register to be written.</param>
        /// <param name="data">The register value to be written.</param>
        /// <returns>The result of accelerometer register write: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteRegister(Byte reg, Byte data);

        /// <summary>
        /// Accelerometer advanced configuration
        /// </summary>
        /// <param name="config">The configuration to be used.</param>
        /// <returns>The result of accelerometer advanced configuration: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool AdvancedConfiguration(UInt32 config);

        /// <summary>
        /// Accelerometer set power
        /// </summary>
        /// <param name="power">The power to be used.</param>
        /// <returns>The result of accelerometer set power: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetPowerMode(UInt32 power);

        /// <summary>
        /// Accelerometer set bandwidth
        /// </summary>
        /// <param name="bandwidth">The bandwidth to be used.</param>
        /// <returns>The result of accelerometer set bandwidth: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetBandwidth(UInt32 bandwidth);

        /// <summary>
        /// Accelerometer set range
        /// </summary>
        /// <param name="range">The range to be used.</param>
        /// <returns>The result of accelerometer set range: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetRange(Byte range);

        /// <summary>
        /// Get all three axis
        /// </summary>
        /// <param name="data">Array of size 3 passed to the driver to be filled.</param>
        /// <returns>The result of accelerometer get all: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool GetAllData(Int16[] data);
    }
}