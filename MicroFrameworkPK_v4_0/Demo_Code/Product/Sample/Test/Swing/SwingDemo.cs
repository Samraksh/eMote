//#define USE_TIMERS

using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
//using System.Runtime.InteropServices;

namespace SwingDemo{

    public class SwingDemo{

        public static class Params
        {
            public static int Sampling_Time_Milli = 1000;
            public static int Sampling_Freq = 1000 / Sampling_Time_Milli;
			public static byte delim = 0xee;
	   }

        public static class State
        {
            public static int Sample_Counter = 0;
        }
		
		
        
        public static void sendSample(ushort I, ushort Q)
        {
			//byte[] buf = new byte[5];
			//buf[0] = Params.delim;
			//buf[1] = (byte)((I >> 8) & 0xff);
			//buf[2] = (byte)(I & 0xff);
		//	buf[3] = (byte)((Q >> 8) & 0xff);
		//	buf[4] = (byte)(Q & 0xff);
			
				
			//Debug.DumpBuffer(buf, false, false);
            //Console.WriteLine(Params.delim);			
			//Microsoft.SPOT.Debug.Print("Hello World!");
            //Debug.Print(Q);
        }

        public static void updateCounter()
        {
            State.Sample_Counter++;
            if (State.Sample_Counter == Params.Sampling_Freq)
            {
                State.Sample_Counter = 0;
                // Toggle some LED
            }
        }

		#if (USE_TIMERS)
		{
        static void fired_sampling()
        {
            newSample = new ushort[2];

            //call ADC get Sample here
            //GetSample(newSample);

            updateCounter();
            sendSample(newSample[0], newSample[1]);
        }
		}
		#endif

        public static void Main()
        {
            // call ADC init function here
            // Turn ON some LED so user knows that the program is live
			ushort x = 0;
			
			Debug.Print("In the main of Swing Demo");
			
			SerialPort sp = new SerialPort(new SerialPort.configuration(COM1,115200,false));
			
            #if (USE_TIMERS)
            {
                Timer t = new Timer(fired_sampling, NULL, 1000, Params.Sampling_Time_Milli);
            }
            #else
            {
                ushort[] newSample = new ushort[2];
                //call ADC get Sample here
                //GetSample(newSample);
				//Debug.Print("Before Update Counter");
                while(true)
				{
					updateCounter();
					//sendSample(newSample[0], newSample[1]);
					//Debug.Print("Before Send Sample");
					sendSample(x,(ushort)(x+50));
					x = (ushort) (x + 100);
					//Debug.Print("Before Thread Sleep");
					Thread.Sleep(Params.Sampling_Time_Milli);
				}
            }
            #endif

        }
    }
}

        