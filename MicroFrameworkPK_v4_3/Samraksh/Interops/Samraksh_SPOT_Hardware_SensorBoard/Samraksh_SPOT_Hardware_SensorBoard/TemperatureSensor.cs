using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Collections;

namespace Samraksh.SPOT.Hardware.SensorBoard
{
    public class TemperatureSensor
    {
        private OutputPort m_pin;
        private OneWire m_oneWire;
        private ArrayList m_devices;

        public TemperatureSensor(Cpu.Pin pin)
        {
            m_pin = new OutputPort(pin, false);
            m_oneWire = new OneWire(m_pin);

            ArrayList devices = m_oneWire.FindAllDevices();

            if (devices == null || devices.Count < 1)
            {
                throw new InvalidOperationException("No devices found on OneWire bus");
            }

            foreach (UInt32 device in devices)
            {
                Debug.Print(device.ToString());
            }

        }

    }
}
