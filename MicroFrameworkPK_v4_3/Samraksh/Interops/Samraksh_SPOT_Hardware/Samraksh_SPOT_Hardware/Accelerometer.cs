
using System;
using System.Threading;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Hardware {
    /// <summary>
    /// Adapt Accelerometer Interface class
    /// </summary>
    public class Accelerometer {
        /// <summary>
        /// Accelerometer call-back
        /// </summary>
        /// <param name="eventType"></param>
        /// <param name="time"></param>
        public delegate void AxlCallbackType(EventType eventType, DateTime time);

        private const Int32 sensorSamplingTimerDueTime = 1000;
        private GRange currentRange = GRange.SixteenG;
        private OutputResolution currentResolution = OutputResolution.FullResolution;
        private Data sensorData;
        private Int32 refreshRate;
        private Timer sensorSamplingTimer;
        private AxlCallbackType userCallbackFn;
        private Int16[] accelerometerDataBuffer = new Int16[3];

        public enum EventType {
            DataUpdate,
            FreeFall
        }

        public struct Data {
            /// <summary>
            /// The raw x axis acceleration data 
            /// </summary>
            public Int16 RawX;
            /// <summary>
            /// The raw y axis acceleration data 
            /// </summary>
            public Int16 RawY;
            /// <summary>
            /// The raw z axis acceleration data 
            /// </summary>
            public Int16 RawZ;
            public Single X;
            public Single Y;
            public Single Z;
            public override string ToString() {
                return "X=" + X.ToString() + "g, Y=" + Y.ToString() + "g, Z=" + Z.ToString() + "g";
            }
        }

        /// <summary>
        /// Enum describing the GRange of the accelerometer   
        /// </summary>
        public enum GRange : byte {
            TwoG = 0x00,
            FourG = 0x01,
            EightG = 0x02,
            SixteenG = 0x03
        }

        /// <summary>
        /// Enum describing the output resolution of the accelerometer    
        /// </summary>
        public enum OutputResolution : byte {
            FixedResolution = 0x00,
            FullResolution = 0x08
        }

        /// <summary>
        /// Set the value of refresh rate 
        /// </summary>
        public Int32 RefreshRate {
            get { return refreshRate; }
            set {
                refreshRate = value;
                sensorSamplingTimer.Change(sensorSamplingTimerDueTime, refreshRate);
            }
        }

        /// <summary>
        /// Get the current sampling value of the accelerometer
        /// </summary>
        public Data CurrentData {
            get {
                return sensorData;
            }
        }

        private void SamplingTimerInternalCallback(object state) {
            long currentTime = 0;

            //lock (i2cdevice)
            //{
            //    i2cdevice.Config = config;
            currentTime = DateTime.Now.Ticks;
            //    i2cdevice.Execute(getAccelDataTransaction, 50);
            //}
            GetAllData(accelerometerDataBuffer);

            sensorData.RawX = accelerometerDataBuffer[0];
            sensorData.RawY = accelerometerDataBuffer[1];
            sensorData.RawZ = accelerometerDataBuffer[2];

            if (currentResolution == OutputResolution.FullResolution) {
                sensorData.X = 0.004f * sensorData.RawX;
                sensorData.Y = 0.004f * sensorData.RawY;
                sensorData.Z = 0.004f * sensorData.RawZ;
            }
            else {
                Single res = 0.0f;

                switch (currentRange) {
                    case GRange.TwoG:
                        res = 2.0f / 512.0f;
                        break;
                    case GRange.FourG:
                        res = 4.0f / 512.0f;
                        break;

                    case GRange.EightG:
                        res = 8.0f / 512.0f;
                        break;

                    case GRange.SixteenG:
                        res = 16.0f / 512.0f;
                        break;

                }

                sensorData.X = res * sensorData.RawX;
                sensorData.Y = res * sensorData.RawY;
                sensorData.Z = res * sensorData.RawZ;

            }
            if (userCallbackFn != null) {
                userCallbackFn(EventType.DataUpdate, new DateTime(currentTime));
            }
        }

        /// <summary>
        /// Accelerometer constructor
        /// </summary>
        public Accelerometer() {
            Initialize();
        }

        /// <summary>
        /// Initializes the i2c and the accelerometer sensor
        /// </summary>
        /// <param name="RefreshRate">controls the rate at which the driver samples the accelerometer</param>
        /// <param name="callbackfunction">user callback function, if null the user is not intimated of changes in sensor values and should use polling</param>
        public Accelerometer(Int32 RefreshRate, AxlCallbackType callbackfunction) {
            Initialize();

            userCallbackFn = callbackfunction;

            //writeAccelDataBuffer = I2CDevice.CreateWriteTransaction(new Byte[] { (Byte)RegisterMap.DATAX0 });
            //readAccelDataBuffer = I2CDevice.CreateReadTransaction(accelDataBuffer);
            //getAccelDataTransaction = new I2CDevice.I2CTransaction[] { writeAccelDataBuffer, readAccelDataBuffer };

            //WriteRegister(RegisterMap.FIFO_CTL, 0x0F);
            //WriteRegister(RegisterMap.DATA_FORMAT, (Byte)OutputResolution.FullResolution | (Byte)GRange.SixteenG);

            // Enable measurement
            //WriteRegister(RegisterMap.POWER_CTL, 0x08);

            try {

                sensorSamplingTimer = new Timer(SamplingTimerInternalCallback, null, sensorSamplingTimerDueTime, RefreshRate);
            }
            catch {
                throw new Exception("Accelerometer Sampling Timer Initialization failed");
            }
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
        public extern bool AdvancedConfig(UInt32 config);

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
