////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using System;
using System.Threading;

using Microsoft.SPOT;
using Microsoft.SPOT.TestFramework;
using Microsoft.SPOT.Hardware;
using Crossbow.platform.imote2;
using ADC_library;


namespace Microsoft.SPOT.Test.Threads
{
    public class ThreadJoin :            Microsoft.SPOT.TestFramework.Test
    {
        static int s_ThreadReleaseCount = 0;
        Thread m_th;

        public ThreadJoin( string comment ):base( comment ){}

        public override void Run()
        {
            Thread []ths = new Thread[3];
            m_th = new Thread(new ThreadStart(ThreadProc));
            m_th.Start();

            for(int i=0; i<ths.Length; i++)
            {
                ths[i] = new Thread(new ThreadStart(ThreadProcWait));
                ths[i].Start();
            }

            Thread.Sleep( 5000 );
            if( !m_th.IsAlive )
                Pass = true; // Thread should be dead

            if( 3 != s_ThreadReleaseCount )
                Pass = true; // Threads returned from Join
            
            foreach(Thread th in ths)
            {
                if(th.IsAlive)
                {
                    th.Abort();
                }
            }
            if(m_th.IsAlive)
            {
                m_th.Abort();
            }
            
        }

        void ThreadProc()
        {
            Thread.Sleep(3000);
        }

        void ThreadProcWait()
        {
            m_th.Join();
            Interlocked.Increment(ref s_ThreadReleaseCount);
        }
    }
    public class MainEntryPoint
    {
        public static void Main()
        {
			System.Int32 adcValue = 0;
            //System.Int16 adc1Value = 0;
 //           System.Int16 adc2Value = 0;

			//int x = 0;
			//int y = 2000;	

            Leds myLed = new Leds();

            //OutputPort LightGreen = new OutputPort((Cpu.Pin)90, true);
			
			ADC myADC = new ADC();		
						
			myADC.initialize();	
				
			Debug.Print("ADC Initialized..... \n");			
			Debug.Print("##########################\n");			
			
			while (true)
			{
              //myLed.greenOn();
              //  myLed.blueOn();
              //LightGreen.Write(false);
			  //Debug.Print("Sample,1000,1000\n");
              //myLed.blueOff();
              //myLed.greenOff();
              //LightGreen.Write(true);

             // myLed.blueOn();
			  adcValue = myADC.getData();
            //  myLed.blueOff();

              //adc2Value = (System.Int16) (adcValue & 0x0000ffff);

              //adc1Value = (System.Int16) (adcValue >> 16);

              
              //adc1Value++;
              //adc2Value++;

              //Debug.Print("Sample," + adc1Value + "," + adc2Value); 

			  //x++;
			  //y++;
			  //Debug.Print("Sample," + adc1Value + "," + adc2Value);
			  //System.Threading.Thread.Sleep(10);
			  //Debug.Print("From C#: The ADC Value is\n" + adcValue);
			  
			}  
			
        }
    }
}

