using System;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.Adapt
{
    /// <summary>Adapt Accelerometer class</summary>
    public class Accelerometer
    {
        /// <summary>Accelerometer constructor</summary>
        public Accelerometer()
        {
            Initialize();
        }

        /// <summary>Initialize accelerometer hardware</summary>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();

        /// <summary>Reset accelerometer hardware</summary>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Reset();

        /// <summary>Run accelerometer self-test</summary>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SelfTest();

        /// <summary>Get accelerometer X value</summary>
        /// <returns>Accelerometer X value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetX();

        /// <summary>Get accelerometer Y value</summary>
        /// <returns>Accelerometer Y value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetY();

        /// <summary>Get accelerometer Z value</summary>
        /// <returns>Accelerometer Z value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetZ();

        /// <summary>Get accelerometer temperature</summary>
        /// <returns>Accelerometer temperature</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetTemperature();

        /// <summary>Read a register</summary>
        /// <param name="reg">Register to read</param>
        /// <returns>Register value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Byte ReadRegister(Byte reg);

        /// <summary>Write a register</summary>
        /// <param name="reg">Register to write</param>
        /// <param name="data">Value to write</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteRegister(Byte reg, Byte data);

        /// <summary>
        /// Configure accelerometer advanced options.
        /// </summary>
        /// <param name="config">Configuration to use</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool AdvancedConfiguration(UInt32 config);

        /// <summary>Set accelerometer power</summary>
        /// <param name="power">Power to use</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetPowerMode(UInt32 power);

        /// <summary>Set accelerometer bandwidth</summary>
        /// <param name="bandwidth">Bandwidth to use</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetBandwidth(UInt32 bandwidth);

        /// <summary>Set accelerometer range</summary>
        /// <param name="range">Range to use</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetRange(Byte range);

        /// <summary>Get all three axis values</summary>
        /// <param name="data">Array of size 3 to fill</param>
        /// <returns>Success/fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool GetAllData(Int16[] data);
    }
}