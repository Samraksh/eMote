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
			int adcValue = 0;
			//int x = 0;
			//int y = 2000;			
			
			ADC myADC = new ADC();		
			Leds LED = new Leds();
						
			myADC.initialize();	
				
			//Debug.Print("ADC Initialized..... \n");			
			//Debug.Print("##########################\n");			
			
			while (true)
			{			
			  Debug.Print("");			
			  
			  LED.redOn();     
			  for(int i=0; i<10000; i++);
			  LED.redOff();
			  
			  LED.blueOn();     
			  for(int i=0; i<10000; i++);
			  LED.blueOff();
			  
			  LED.orangeOn();     
			  for(int i=0; i<10000; i++);
			  LED.orangeOff();
			  
			  LED.greenOn();     
			  for(int i=0; i<10000; i++);
			  LED.greenOff();
			  
			  adcValue = myADC.getData();
			  //Debug.Print("Sample " + adcValue);
			  //System.Threading.Thread.Sleep(10);			  
			  
			}  
			
        }
    }
}

