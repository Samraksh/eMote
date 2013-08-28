using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace Samraks.SPOT.Hardware.SensorBoard
{
    public class Accelerometer
    {
        public const ushort address = 0x53;
        public const int clock = 400;

        private I2CDevice.Configuration config;
        private I2CDevice i2cdevice;

        public Accelerometer()
        {
            config = new I2CDevice.Configuration(address, clock);
            i2cdevice = new I2CDevice(config);
        }
    }
}
