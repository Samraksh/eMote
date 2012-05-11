// Name : Nived Sivadas
// Company : The Samraksh Company
// Description : Simple test designed to write data to flash storage using datastore api
// 
//


using System;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;
using TestSuite;
using Samraksh;


namespace TestSuite
{
	public class DataStoreTest
	{
		private int data;
		
		public DataStoreTest()
		{
			data = 0;
		}
	
		public void Run()
		{
			try
			{
				Samraksh.Memory store = new Memory(4);
				
				Timer tmr = new Timer(new TimerCallback(Callback), null, 0, 100);
				
			}
			catch(Exception e)
			{
			}
		}
		private void Callback(Object state)
        {
			byte[] buffer = new buffer[4];
			store.write(data++);
			
			buffer = store.read();
			
			USBPipe.Write(buffer,0,4);
        }
	}
}