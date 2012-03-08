using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace TestSuite
{
    class SPITest
    {
        public void Run()
        {

            SPI.Configuration config = new SPI.Configuration(Pins.GPIO_PORT_PIN_1, false, 0, 0, false, true, 16, SPI.SPI_module.SPI1);

            SPI testSPI = new SPI(config);

            byte[] tx_data = new byte[1];		
            byte[] rx_data = new byte[1];

            //tx_data[0] = (byte)192;			

            while (true)
            { 
				tx_data[0] = (byte) 1;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				tx_data[0] = (byte) 2;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				tx_data[0] = (byte) 3;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				tx_data[0] = (byte) 4;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				tx_data[0] = (byte) 5;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				tx_data[0] = (byte) 6;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				tx_data[0] = (byte) 7;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				tx_data[0] = (byte) 8;
                testSPI.WriteRead(tx_data, rx_data);
				Thread.Sleep(2000);
				
            }

        }

    }
}
