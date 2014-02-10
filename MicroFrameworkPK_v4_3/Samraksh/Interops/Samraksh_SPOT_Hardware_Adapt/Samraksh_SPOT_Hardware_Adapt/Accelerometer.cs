using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Runtime.CompilerServices;
using System.Threading;

namespace Samraksh_SPOT_Hardware_Adapt
{
    /// <summary>
    /// Accelerometer class for Adapt
    /// </summary>
    public class Accelerometer
    {
        static AccelerometerInternal accelInternal;
        //public delegate void AxlCallbackType(EventType eventType, DateTime time);

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
        public enum RegisterMap : byte
        {
            FAKE_REGISTER = 0x00
        }


        private const ushort address = 0x53;
        private const int clock = 100;

        //private const Int32 sensorSamplingTimerDueTime = 1000;

        //private I2CDevice.Configuration config;
        //private I2CDevice i2cdevice;

        private GRange currentRange = GRange.SixteenG;

        private OutputResolution currentResolution = OutputResolution.FullResolution;

        private Data sensorData;

        private Int32 refreshRate;

       // private Timer sensorSamplingTimer;

        //private AxlCallbackType userCallbackFn;

        private Byte[] accelDataBuffer;
        //private I2CDevice.I2CWriteTransaction writeAccelDataBuffer;
        //private I2CDevice.I2CReadTransaction readAccelDataBuffer;
        //private I2CDevice.I2CTransaction[] getAccelDataTransaction;

        /// <summary>
        /// Set the value of refresh rate 
        /// </summary>
        public Int32 RefreshRate
        {
            get { return refreshRate; }
            set { 
                refreshRate = value;
                //sensorSamplingTimer.Change(sensorSamplingTimerDueTime, refreshRate);
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
            /*long currentTime = 0;

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
            }*/
        }

        static public Int16 InitializeAccelerometer()
        {
            Int16 returnValue = accelInternal.ADAPT_Accel_Init();

            return returnValue;
        }

        /// <summary>
        /// Initializes the accelerometer sensor
        /// </summary>
        /// <param name="RefreshRate">controls the rate at which the driver samples the accelerometer</param>
        /// <param name="callbackfunction">user callback function, if null the user is not intimated of changes in sensor values and should use polling</param>
        public Accelerometer(Int32 RefreshRate)
        {
            /*try
            {
                config = new I2CDevice.Configuration(address, clock);
                i2cdevice = new I2CDevice(config);
            }
            catch
            {
                throw new Exception("I2C Bus initialization failed");
            }*/

            accelDataBuffer = new Byte[6];
            accelInternal = new AccelerometerInternal("NULL", 1234);
            //userCallbackFn = callbackfunction;

            /*writeAccelDataBuffer = I2CDevice.CreateWriteTransaction(new Byte[] { (Byte) RegisterMap.DATAX0});
            readAccelDataBuffer = I2CDevice.CreateReadTransaction(accelDataBuffer);
            getAccelDataTransaction = new I2CDevice.I2CTransaction[] { writeAccelDataBuffer, readAccelDataBuffer };*/

            //WriteRegister(RegisterMap.FIFO_CTL, 0x0F);
            //WriteRegister(RegisterMap.DATA_FORMAT, (Byte)OutputResolution.FullResolution | (Byte)GRange.SixteenG);

            // Enable measurement
            //WriteRegister(RegisterMap.POWER_CTL, 0x08);

           /*try
            {

                sensorSamplingTimer = new Timer(SamplingTimerInternalCallback, null, sensorSamplingTimerDueTime, RefreshRate);
            }
            catch
            {
                throw new Exception("Sampling Timer Initialization failed");
            }*/
        }



        private void WriteRegister(RegisterMap register, Byte value)
        {
            Byte[] data = new Byte[] { (Byte)register, value };

            /*I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(data);
            I2CDevice.I2CTransaction[] writeTransaction = new I2CDevice.I2CTransaction[] { write };

            lock (i2cdevice)
            {
                i2cdevice.Config = config;
                i2cdevice.Execute(writeTransaction, 10);
            }*/

        }

        static public Byte ReadRegister(RegisterMap register)
        {
            UInt16 temp = accelInternal.ADAPT_Accel_Raw_Read((UInt16)register);

            /*I2CDevice.I2CWriteTransaction write = I2CDevice.CreateWriteTransaction(new Byte[] { (Byte)register });
            I2CDevice.I2CReadTransaction read = I2CDevice.CreateReadTransaction(data);
            I2CDevice.I2CTransaction[] readTransaction = new I2CDevice.I2CTransaction[] { write, read };

            lock (i2cdevice)
            {
                i2cdevice.Config = config;
                i2cdevice.Execute(readTransaction, 10);
            }*/

            return (Byte)temp;
        }
    }

    /// <summary>
    /// ADCInternal interface to the native driver
    /// </summary>
    public class AccelerometerInternal : NativeEventDispatcher
    {
        /// <summary>
        /// Specifies the driver name for matching with the native eventdispatcher
        /// </summary>
        /// <param name="strDrvName"></param>
        /// <param name="drvData"></param>
        public AccelerometerInternal(string strDrvName, ulong drvData) : base(strDrvName, drvData)
        {

        }

        /// <summary>
        /// Initializes the accelerometer driver
        /// </summary>
        /// <returns>Returns the result of the init function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_Init();

        /// <summary>
        /// Gets X value
        /// </summary>
        /// <returns>Returns the X value</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_GetX();

        /// <summary>
        /// Gets Y value
        /// </summary>
        /// <returns>Returns the Y value</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_GetY();

        /// <summary>
        /// Gets Z value
        /// </summary>
        /// <returns>Returns the Z value</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_GetZ();

        /// <summary>
        /// Gets temperature value
        /// </summary>
        /// <returns>Returns the result of the get temperature function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_GetTemperature();

        /*/// <summary>
        /// Gets all accelerometer values
        /// </summary>
        /// <param name="data">pointer to all three data values</param>
        /// <returns>Returns the result of getting all accelerometer data</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_GetAll(Int16* data);*/

        /// <summary>
        /// reset accelerometer
        /// </summary>
        /// <returns>Returns the result of the reset function</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_reset();

        /// <summary>
        /// advanced configuration for the accelerometer
        /// </summary>
        /// <param name="config">configuration data</param>
        /// <returns>Returns the result of the advanced configuration</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_advconfig(UInt32 config);

        /// <summary>
        /// accelerometer self test
        /// </summary>
        /// <returns>Returns the result of the self test</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_SelfTest();

        /// <summary>
        /// set power to the accelerometer
        /// </summary>
        /// <param name="power">power data</param>
        /// <returns>Returns the result of setting the power</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_SetPowerMode(UInt32 power);

        /// <summary>
        /// set bandwidth of the accelerometer
        /// </summary>
        /// <param name="bandwidth">bandWidth data</param>
        /// <returns>Returns the result of setting the bandwidth</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_SetBandwidth(UInt32 bandwidth);

        /// <summary>
        /// set range of the accelerometer
        /// </summary>
        /// <param name="range">range data</param>
        /// <returns>Returns the result of setting the range</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_SetRange(UInt16 range);

        /// <summary>
        /// read accelerometer register
        /// </summary>
        /// <param name="register">register to read</param>
        /// <returns>Returns the value of 'register'</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public UInt16 ADAPT_Accel_Raw_Read(UInt16 register);

        /// <summary>
        /// write accelerometer register
        /// </summary>
        /// <param name="register">register to write</param>
        /// <param name="data">data to write</param>
        /// <returns>Returns the result of 'register' write</returns>
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern public Int16 ADAPT_Accel_Raw_Write(Byte register, UInt16 data);
    }    
}
