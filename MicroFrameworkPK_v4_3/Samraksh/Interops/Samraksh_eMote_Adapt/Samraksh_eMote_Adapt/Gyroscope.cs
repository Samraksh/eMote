using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.Adapt
{
    class Gyroscope
    {
        public Gyroscope()
        {
            Initialize();
        }

        /// <summary>
        /// Initializes the gyroscope hardware
        /// </summary>
        /// <returns>The result of gyroscope initialization: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Initialize();

        /// <summary>
        /// Resets the gyroscope hardware
        /// </summary>
        /// <returns>The result of gyroscope reset: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool Reset();

        /// <summary>
        /// Runs the gyroscope self-test
        /// </summary>
        /// <returns>The result of gyroscope self-test: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SelfTest();

        /// <summary>
        /// Gets the gyroscope X value
        /// </summary>
        /// <returns>The gyroscope X value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetX();

        /// <summary>
        /// Gets the gyroscope Y value
        /// </summary>
        /// <returns>The gyroscope Y value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetY();

        /// <summary>
        /// Gets the gyroscope Z value
        /// </summary>
        /// <returns>The gyroscope Z value (Int16)</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Int16 GetZ();

        /// <summary>
        /// Gets the gyroscope temperature
        /// </summary>
        /// <returns>The gyroscope temperature (Int16)</returns>
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
        /// <returns>The result of gyroscope register write: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool WriteRegister(Byte reg, Byte data);

        /// <summary>
        /// Gyroscope advanced configuration
        /// </summary>
        /// <param name="config">The configuration to be used.</param>
        /// <returns>The result of gyroscope advanced configuration: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool AdvancedConfiguration(UInt32 config);

        /// <summary>
        /// Gyroscope set power
        /// </summary>
        /// <param name="power">The power to be used.</param>
        /// <returns>The result of gyroscope set power: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetPowerMode(UInt32 power);

        /// <summary>
        /// Gyroscope set offsets
        /// </summary>
        /// <param name="xOffset">The offset for the x-axis.</param>
        /// <param name="yOffset">The offset for the y-axis.</param>
        /// <param name="zOffset">The offset for the z-axis.</param>
        /// <returns>The result of gyroscope set offsets: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetOffsets(Int16 xOffset, Int16 yOffset, Int16 zOffset);

        /// <summary>
        /// Gyroscope sample configuration
        /// </summary>
        /// <param name="config">The configuration to be used.</param>
        /// <returns>The result of gyroscope sample configuration: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SampleConfiguration(UInt32 config);

        /// <summary>
        /// Get all three axis
        /// </summary>
        /// <param name="data">Array of size 3 passed to the driver to be filled.</param>
        /// <returns>The result of gyroscope get all: 0-Success, 1-Fail</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool GetAllData(Int16[] data);
    }
}
