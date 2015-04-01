using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;

namespace Samraksh.eMote.Kiwi
{

    public class I2CInitializationException : Exception
    {
        public I2CInitializationException()
        {
        }

        public I2CInitializationException(string message)
            : base(message)
        {
        }

        public I2CInitializationException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }

    public class TimerException : Exception
    {
        public TimerException()
        {
        }

        public TimerException(string message)
            : base(message)
        {
        }

        public TimerException(string message, Exception inner)
            : base(message, inner)
        {
        }
    }

    /// <summary>
    /// Kiwi Accelerometer
    /// </summary>
    public class Accelerometer
    {

        /// <summary>
        /// Kiwi Accelerometer callback signature
        /// </summary>
        /// <param name="eventType">Event type</param>
        /// <param name="time">Time of the event</param>
        public delegate void AxlCallbackType(EventType eventType, DateTime time);

        /// <summary>
        /// Types of Kiwi Accelerometer events
        /// </summary>
        public enum EventType
        {
            /// <summary>Updating data</summary>
            DataUpdate,
            /// <summary>Freefall detected </summary>
            FreeFall
        }

        /// <summary>Kiwi Accelerometer data class</summary>
        public class Data
        {
            /// <summary>Raw x axis acceleration data</summary>
            public Int16 RawX;

            /// <summary>Raw y axis acceleration data</summary>
            public Int16 RawY;

            /// <summary>Raw z axis acceleration data</summary>
            public Int16 RawZ;

            /// <summary>Processed x axis acceleration data</summary>
            public Single X;

            /// <summary>Processed y axis accelerometer data</summary>
            public Single Y;

            /// <summary>Processed z azis accelerometer data</summary>
            public Single Z;

            public override string ToString()
            {
                return "X=" + X.ToString() + "g, Y=" + Y.ToString() + "g, Z=" + Z.ToString() + "g";
            }

        }

        /// <summary>
        /// Range of accelerometer
        /// </summary>
        /// <remarks>
        /// "Range" refers to the range of values returned. 
        /// <para>The user can choose the units in which the resulting data should be interpreted. </para>
        /// <para>Lower G gives better precision but limited range of values.</para>
        /// <para>Higher G gives less precision but larger range of values.</para>
        /// <para>Hence a value of 13 for FourG would be 26 for TwoG.</para>
        /// </remarks>
        public enum GRange : byte
        {
            /// <summary>2 G range</summary>
            TwoG = 0x00,
            /// <summary>4 G range</summary>
            FourG = 0x01,
            /// <summary>8 G range</summary>
            EightG = 0x02,
            /// <summary>16 G range</summary>
            SixteenG = 0x03
        }

        /// <summary>
        /// Output resolution for Kiwi Accelerometer
        /// </summary>
        public enum OutputResolution : byte
        {
            /// <summary>Fixed resolution</summary>
            FixedResolution = 0x00,
            /// <summary>Full resolution</summary>
            FullResolution = 0x08
        }

        /// <summary>
        /// Registers on the Kiwi Accelerometer
        /// </summary>
        private enum RegisterMap : byte
        {
            // ReSharper disable UnusedMember.Local
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
            // ReSharper restore UnusedMember.Local
        }


        private const ushort address = 0x53;
        private const int clock = 100;
        private const Int32 sensorSamplingTimerDueTime = 1000;
        private Byte[] accelDataBuffer;
        private Int32 refreshRate;

        private I2CDevice.Configuration config;
        private I2CDevice i2cdevice;
        private GRange currentRange = GRange.SixteenG;
        private OutputResolution currentResolution = OutputResolution.FullResolution;
        private Data sensorData;
        private Timer sensorSamplingTimer;
        private AxlCallbackType userCallbackFn;

        private I2CDevice.I2CWriteTransaction writeAccelDataBuffer;
        private I2CDevice.I2CReadTransaction readAccelDataBuffer;
        private I2CDevice.I2CTransaction[] getAccelDataTransaction;

        /// <summary>
        /// Set the value of refresh rate for the Kiwi Accelerometer
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
                var res = 0.0f;

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
        /// Initialize the i2c and the Kiwi Accelerometer sensor
        /// </summary>
        /// <param name="RefreshRate">Sample rate</param>
        /// <param name="callbackfunction">Callback method. Can be null.</param>
        /// <remarks>If the callback method is null, the user is not notified of sensor events. User should poll instead.</remarks>
        public Accelerometer(Int32 RefreshRate, AxlCallbackType callbackfunction)
        {
            try
            {
                config = new I2CDevice.Configuration(address, clock);
                i2cdevice = new I2CDevice(config);
            }
            catch
            {
                throw new I2CInitializationException("I2C Bus initialization failed");
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
                throw new TimerException("Sampling Timer Initialization failed");
            }
        }

        private void WriteRegister(RegisterMap register, Byte value)
        {
            var data = new[] { (Byte)register, value };

            var write = I2CDevice.CreateWriteTransaction(data);
            var writeTransaction = new I2CDevice.I2CTransaction[] { write };

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
