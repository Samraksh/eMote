// Author : Mukundan Sridharan
// Date: Dec-06-2011
// Company : The Samraksh Company
// Email: mukundan.sridharan@samraksh.com
// Description : Test to check the sheduling of policy of NETMF Scheduler
// Expected Results: 




using System;
using System.Threading;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT;
using CSharpTestSuite;

namespace SchedulerTestSuite
{
    public class ThreadPriorityTest
    {
    
    	//int m_CallbackRequired;
	    private static OutputPort Port0 = new OutputPort(EmotePins.GPIO_PIN_0, true); //Emote Pin3 is PA0 on the board
    	
      	public void Run() 
        { 
            Thread thread1 = new Thread(T1Activity); 
            Thread thread2 = new Thread(T2Activity);
			thread1.Priority = ThreadPriority.AboveNormal;
			
            thread1.Start(); 
            thread2.Start(); 
        } 
        static void T1Activity() 
        { 
            //int j;
             
            while (true) 
            { 
                Port0.Write(true);
                /*Debug.Print("T1"); 
                for (int i = 0; i < 200; i++) 
                { 
                    j = i * 3; 
                }*/ 
            } 
        } 
        static void T2Activity() 
        { 
            //int j; 
           
            while (true) 
            { 
            	Port0.Write(false);
                /*Debug.Print(" T2"); 
                for (int i = 0; i < 200; i++) 
                { 
                    j = i * 3; 
                }*/ 
            } 
        } 
    } 
}