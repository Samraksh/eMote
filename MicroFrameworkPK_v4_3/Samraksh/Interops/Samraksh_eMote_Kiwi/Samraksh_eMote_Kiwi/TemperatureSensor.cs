using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Collections;
using System.Threading;

namespace Samraksh.eMote.SensorBoard
{    
    /// <summary>
    /// Kiwi Temperature Sensor
    /// </summary>
    public class TemperatureSensor
    {
        private enum DS18B20Commands : int
        {
            ConverT = 0x44,
            ReadScratchPad = 0xBE,
            WriteScratchPad = 0x4E,
            CopyScratchPad = 0x48,
            ReadPowerSupply = 0xB4
        }

        private OutputPort m_pin;
        private OneWire m_oneWire;
        private ArrayList m_devices;
        private Timer m_samplingTimer;
        private double temperature;
        private Object lockTemperature = new Object();

        /// <summary>
        /// Read current temperature
        /// </summary>
        public double Temperature
        {
            get
            {
                lock (lockTemperature)
                {
                    return temperature;
                }
            }
            set
            {
                lock (lockTemperature)
                {
                    temperature = value;
                }
            }
        }

        private void Sense(Object state)
        {

            var temp = 0.0;

            m_oneWire.TouchReset();
            m_oneWire.WriteByte(0xCC);
            m_oneWire.WriteByte((int) DS18B20Commands.ConverT);

            Thread.Sleep(750);

            m_oneWire.TouchReset();
            m_oneWire.WriteByte(0xCC);
            m_oneWire.WriteByte((int)DS18B20Commands.ReadScratchPad);

            var tempLo = m_oneWire.ReadByte();
            var tempHi = m_oneWire.ReadByte();

            if ((tempLo & 0x1) > 0)
            {
                temp += 0.5;
            
            }

            temp += (tempLo >> 1);

            if (tempHi == 0xff)
                temp *= -1;

            Temperature = temp;
            
        }

        /// <summary>
        /// Constructor for Kiwi Temperature Sensor
        /// </summary>
        /// <param name="pin"></param>
        /// <param name="samplingRate"></param>
        /// <exception cref="SystemException"></exception>
        /// <exception cref="InvalidOperationException"></exception>
        public TemperatureSensor(Cpu.Pin pin, int samplingRate)
        {

            Temperature = 0;

            if (samplingRate < 750)
            {
                throw new SystemException("Conversion time is 750ms, can not support this sampling rate");
            }

            try
            {
                m_pin = new OutputPort(pin, false);
            }
            catch (Exception e)
            {
                Debug.Print("Exception from  One Wire Class "  + e.ToString());
            }

            try
            {
                m_oneWire = new OneWire(m_pin);
            }
            catch (Exception e)
            {
                Debug.Print("Exception from One Wire Class " + e.ToString());
            }

            m_devices = m_oneWire.FindAllDevices();

            if (m_devices == null || m_devices.Count < 1)
            {
                throw new InvalidOperationException("No devices found on OneWire bus");
            }

            m_samplingTimer = new Timer(Sense, null, 0, samplingRate);

        }
    }
}
