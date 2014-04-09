using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Samraksh.eMote.SensorBoard
{


    public class Accelerometer
    {

        public delegate void AxlCallbackType(EventType eventType, DateTime time);

        public enum EventType
        {
            DataUpdate,
            FreeFall
        }

        public struct Data
        {
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

            public override string ToString()
            {
                return "X=" + X.ToString() + "g, Y=" + Y.ToString() + "g, Z=" + Z.ToString() + "g";
            }

        }

        // Enum describing the GRange of the accelerometer
        public enum GRange : byte
        {
            TwoG = 0x00,
            FourG = 0x01,
            EightG = 0x02,
            SixteenG = 0x03
        }

        // Enum describing the output resolution of the accelerometer
        public enum OutputResolution : byte
        {
            FixedResolution = 0x00,
            FullResolution = 0x08
        }

        // Enum describing the register map on the accelerometer 
        private enum RegisterMap : byte
        {
            DEVID = 0x00,
            THRESH_TAP = 0x1d,
            OFSX = 0x1e,
            OFSY = 0x1f,
            OFSZ = 0x20,
            DUR = 0x21,
            LATENT = 0x22,
            WINDOW = 0x23,
            THRESH_ACT = 0x24,
            THRESH_INACT = 0x25,
            TIME_INACT = 0x26,
            ACT_INACT_CTL = 0x27,
            THRESH_FF = 0x28,
            TIME_FF = 0x29,
            TAP_AXES = 0x2a,
            ACT_TAP_STATUS = 0x2b,
            BW_RATE = 0x2c,
            POWER_CTL = 0x2d,
            INT_ENABLE = 0x2e,
            INT_MAP = 0x2f,
            INT_SOURCE = 0x30,
            DATA_FORMAT = 0x31,
            DATAX0 = 0x32,
            DATAX1 = 0x33,
            DATAY0 = 0x34,
            DATAY1 = 0x35,
            DATAZ0 = 0x36,
            DATAZ1 = 0x37,
            FIFO_CTL = 0x38,
            FIFO_STATUS = 0x39
        }


        private const ushort address = 0x53;
        private const int clock = 100;

        private const Int32 sensorSamplingTimerDueTime = 1000;

        private I2CDevice.Configuration config;
        private I2CDevice i2cdevice;

        private GRange currentRange = GRange.SixteenG;

        private OutputResolution currentResolution = OutputResolution.FullResolution;

        private Data sensorData;

        private Int32 refreshRate;

        private Timer sensorSamplingTimer;

        private AxlCallbackType userCallbackFn;

        private Byte[] accelDataBuffer;
        private I2CDevice.I2CWriteTransaction writeAccelDataBuffer;
        private I2CDevice.I2CReadTransaction readAccelDataBuffer;
        private I2CDevice.I2CTransaction[] getAccelDataTransaction;

        /// <summary>
        /// Set the value of refresh rate 
        /// </summary>
        public Int32 RefreshRate
        {
            get { return refreshRate; }
            set { 
                refreshRate = value;
                sensorSamplingTimer.Change(sensorSamplingTimerDueTime, refreshRate);
            }
        }

        /// <summary>
        /// Get the current sampling value of the accelerometer
        /// </summary>
        public Data CurrentData
        {
            get
            {
                return sensorData;
            }
        }

        private void SamplingTimerInternalCallback(object state)
        {
            long currentTime = 0;

            lock (i2cdevice)
            {
                i2cdevice.Config = config;
                currentTime = DateTime.Now.Ticks;
                i2cdevice.Execute(getAccelDataTransaction, 50);
            }

            sensorData.RawX = (Int16)(accelDataBuffer[1] << 8 | accelDataBuffer[0]);
            sensorData.RawY = (Int16)(accelDataBuffer[3] << 8 | accelDataBuffer[2]);
            sensorData.RawZ = (Int16)(accelDataBuffer[5] << 8 | accelDataBuffer[4]);

            if (currentResolution == OutputResolution.FullResolution)
            {
                sensorData.X = 0.004f * sensorData.RawX;
                sensorData.Y = 0.004f * sensorData.RawY;
                sensorData.Z = 0.004f * sensorData.RawZ;
            }
            else
            {
                Single res = 0.0f;

                switch (currentRange)
                {
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
            if (userCallbackFn != null)
            {
                userCallbackFn(EventType.DataUpdate, new DateTime(currentTime));
            }
        }

        /// <summary>
        /// Initializes the i2c and the accelerometer sensor
        /// </summary>
        /// <param name="RefreshRate">controls the rate at which the driver samples the accelerometer</param>
        /// <param name="callbackfunction">user callback function, if null the user is not intimated of changes in sensor values and should use polling</param>
        public Accelerometer(Int32 RefreshRate, AxlCallbackType callbackfunction)
        {
            try
            {
                config = new I2CDevice.Configuration(address, clock);
                i2cdevice = new I2CDevice(config);
            }
            catch
            {
                throw new Exception("I2C Bus initialization failed");
            }

            accelDataBuffer = new Byte[6];

            userCallbackFn = callbackfunction;

            writeAccelDataBuffer = I2CDevice.CreateWriteTransaction(new Byte[] { (Byte) RegisterMap.DATAX0});
            readAccelDataBuffer = I2CDevice.CreateReadTransaction(accelDataBuffer);
            getAccelDataTransaction = new I2CDevice.I2CTransaction[] { writeAccelDataBuffer, readAccelDataBuffer };

            WriteRegister(RegisterMap.FIFO_CTL, 0x0F);
            WriteRegister(RegisterMap.DATA_FORMAT, (Byte)OutputResolution.FullResolution | (Byte)GRange.SixteenG);

            // Enable measurement
            WriteRegister(RegisterMap.POWER_CTL, 0x08);

            try
            {

                sensorSamplingTimer = new Timer(SamplingTimerInternalCallback, null, sensorSamplingTimerDueTime, RefreshRate);
            }
            catch
            {
                throw new Exception("Sampling Timer Initialization failed");
            }
        }

        private void WriteRegister(RegisterMap register, Byte value)
        {
            Byte[] data = new Byte[] { (Byte)register, value };

            I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(data);
            I2CDevice.I2CTransaction[] writeTransaction = new I2CDevice.I2CTransaction[] { write };

            lock (i2cdevice)
            {
                i2cdevice.Config = config;
                i2cdevice.Execute(writeTransaction, 10);
            }

        }

        private Byte ReadRegister(RegisterMap register)
        {
            Byte[] data = new Byte[1];

            I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(new Byte[] { (Byte)register });
            I2CDevice.I2CReadTransaction read = I2CDevice.CreateReadTransaction(data);
            I2CDevice.I2CTransaction[] readTransaction = new I2CDevice.I2CTransaction[] { write, read };

            lock (i2cdevice)
            {
                i2cdevice.Config = config;
                i2cdevice.Execute(readTransaction, 10);
            }

            return data[0];
        }
    }
}
