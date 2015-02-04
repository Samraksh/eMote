using System;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.Adapt
{
    /// <summary>Gyroscope class</summary>
    public class Gyroscope
    {
        /// <summary>Gyroscope constructor</summary>
        public Gyroscope()
        {
            Initialize();
        }

        /// <summary>Initialize gyroscope hardware</summary>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();

        /// <summary>Reset gyroscope hardware</summary>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Reset();

        /// <summary>Run gyroscope self-test</summary>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SelfTest();

        /// <summary>Get gyroscope X value</summary>
        /// <returns>Gyroscope X value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetX();

        /// <summary>Get gyroscope Y value</summary>
        /// <returns>Gyroscope Y value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetY();

        /// <summary>Get gyroscope Z value</summary>
        /// <returns>Gyroscope Z value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetZ();

        /// <summary>Get gyroscope temperature</summary>
        /// <returns>Gyroscope temperature</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetTemperature();

        /// <summary>Read a register</summary>
        /// <param name="reg">Register read</param>
        /// <returns>Register value</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Byte ReadRegister(Byte reg);

        /// <summary>Write a register</summary>
        /// <param name="reg">Register to write</param>
        /// <param name="data">Value to write</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteRegister(Byte reg, Byte data);

        /// <summary>Configure advanced gyroscope options</summary>
        /// <param name="config">Configuration to use</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool AdvancedConfiguration(UInt32 config);

        /// <summary>Set gyroscope power</summary>
        /// <param name="power">Power to use</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetPowerMode(UInt32 power);

        /// <summary>
        /// Set gyroscope offsets
        /// </summary>
        /// <param name="xOffset">Offset for x-axis</param>
        /// <param name="yOffset">Offset for y-axis</param>
        /// <param name="zOffset">Offset for z-axis</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetOffsets(Int16 xOffset, Int16 yOffset, Int16 zOffset);

        /// <summary>Set gyroscope sample configuration</summary>
        /// <param name="config">Configuration to use</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SampleConfiguration(UInt32 config);

        /// <summary>Get all three axis</summary>
        /// <param name="data">Array of size 3 to fill</param>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool GetAllData(Int16[] data);
    }
}
