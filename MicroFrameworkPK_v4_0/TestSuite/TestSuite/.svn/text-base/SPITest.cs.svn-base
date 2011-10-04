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
			
			int i = 0;

            byte[] tx_data = new byte[1];		
            byte[] rx_data = new byte[1];

            tx_data[0] = (byte)192;			

            //while (true)
            //{                
                testSPI.WriteRead(tx_data, rx_data);
				while(i <400)
				{ i++; }
				tx_data[0] = (byte)157;
				testSPI.WriteRead(tx_data, rx_data);
                //tx_data[0] = (byte) (tx_data[0]+ 1);				
                Debug.Print("" + (int)(rx_data[0]));
                //Thread.Sleep(2);
            //}


        }

    }
}
